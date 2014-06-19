/*
 * Copyright (c) 2012 Netflix, Inc.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NETFLIX, INC. AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 * DISCLAIMED. IN NO EVENT SHALL NETFLIX OR CONTRIBUTORS BE LIABLE FOR ANY 
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND 
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF 
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#if (defined(__WIN32__) || defined(_WIN32))
#include "windows_define.h"
#else
#include <arpa/inet.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <assert.h>
#include <curl/curl.h>
#include "DialDiscovery.h"

using namespace std;
const int DialMaxMSearchRequests = 1;

DialDiscovery *DialDiscovery::sDiscovery = 0;

static pthread_mutex_t list_locker = PTHREAD_MUTEX_INITIALIZER;
class ScopeLocker {
public:
    ScopeLocker(pthread_mutex_t &m) : mLock(m) { pthread_mutex_lock(&mLock); }
    virtual ~ScopeLocker() { pthread_mutex_unlock(&mLock); }
private:
    pthread_mutex_t mLock;
};

static const char ssdp_msearch[] = 
    "M-SEARCH * HTTP/1.1\r\n"
    "HOST: 239.255.255.250:1900\r\n"
    "MAN: \"ssdp:discover\"\r\n"
    "MX: 2\r\n"
    "ST: urn:dial-multiscreen-org:service:dial:1\r\n"
	"USER-AGENT: Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)\r\n"
	"\r\n";

static string getLocation(char *pResponse) {
    string loc(pResponse);
    size_t prev = 0, index = loc.find("\r\n");
    string locUrl, retUrl;
    while( index != string::npos ) {
        locUrl = loc.substr(prev, index-prev);
        ATRACE("locUrl: ##%s## prev: %d index %d\n", locUrl.c_str(), (int)prev, (int)index );
        if( locUrl.find("LOCATION: ") != string::npos ) {
            index = locUrl.find("\r\n");
            retUrl = locUrl.substr( 10 );
            break;
        }

        prev = index+2; // move past the "\r\n" token
        index = loc.find("\r\n", prev);
    }
    return retUrl;
}

static size_t header_cb(void* ptr, size_t size, size_t nmemb, void* userdata) {
    if ((size * nmemb) != 0) {
        string parse((char*)ptr);
        if( parse.find("Application-URL: ") != string::npos ) {
            size_t index_start = parse.find(":");
            index_start += 2;
            size_t index_end = parse.find("\r\n");
            string *header = static_cast<string*>(userdata);
            header->append(parse.substr(index_start, index_end-index_start));
            ATRACE("Apps URL set to %s\n", header->c_str());
#ifndef DEBUG
        }
#else
        } else {
            ATRACE("%s: Dropping %s\n", __FUNCTION__, (char*)ptr);
        }
#endif
    }
    return (size * nmemb);
}

static size_t receiveData(void *ptr, size_t size, size_t nmemb, void *userdata) {
    if ((size * nmemb) != 0) {
        string *url = static_cast<string*>(userdata);
        url->append((char*)ptr);
    }
    return (size * nmemb);
}

static void getServerInfo( const string &server, string& appsUrl, string& ddxml ) {
    CURL* curl = NULL;

    if (curl_global_init(CURL_GLOBAL_ALL) != CURLE_OK) {
        fprintf(stderr, "curl_global_init() failed\n");
        return; 
    }

    if ((curl = curl_easy_init()) == NULL) {
        fprintf(stderr, "curl_easy_init() failed\n");
        curl_global_cleanup();
        return;
    }

    ATRACE("Sending ##%s##\n", server.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, server.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, header_cb);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &appsUrl);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, receiveData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ddxml);
    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    //curl_global_cleanup();
}

void DialDiscovery::updateServerList(string& server) {
    ServerMap::const_iterator it;
    it = mServerMap.find(server);
    if( it == mServerMap.end() ) {
        // not found, add it
        string appsUrl, ddxml;
        getServerInfo(server, appsUrl, ddxml);
        mServerMap[server] = new DialServer(server, appsUrl, ddxml);
    } else {
        // just mark that we found it
        (*it).second->setFound(true);
    }
}

void DialDiscovery::processServer(char *pResponse) {
	if (strstr(pResponse, "ST: urn:dial-multiscreen-org:service:dial:1")) {
        string server;
    
        ScopeLocker s(list_locker);
        // parse for LOCATION header
        server = getLocation(pResponse);
        ATRACE("FOUND server: %s\n", server.c_str());
    
        // save the appURL in the server
        updateServerList(server);
    }
    else {
       ATRACE("Dropping Server\n");
    }
}

void* DialDiscovery::receiveResponses(void* p) {
    DialDiscovery* self = (DialDiscovery*) p;
	if(self) {
        self->receive_discovery_response();
    }
	return NULL;
}

void DialDiscovery::receive_discovery_response() {
    int bytes;
    char buf[4096] = {0,};
	printf("DialDiscovery::receiveResponses Thread Started[%p]:\n");
    while (m_bRunResponseThread) {
        if (-1 == (bytes = recvfrom(connection.sock, buf, sizeof(buf) - 1, 0,
                                  (struct sockaddr *)&connection.saddr, &connection.addrlen))) {
            perror("recvfrom");
            break;
        }
        buf[bytes] = 0;
		printf("DialDiscovery::receiveResponses Received[%s]:\n", buf);
        //ATRACE("Received [%s:%d] %s\n", __FUNCTION__, __LINE__, buf);
        processServer(buf);
    }
	printf("DialDiscovery::receiveResponses Thread Ended:\n");
}

void DialDiscovery::cleanServerList(void) {
    ScopeLocker s(list_locker);

    ServerMap::const_iterator it;
    vector<string> removal;
    for( it = mServerMap.begin(); it != mServerMap.end(); it++ )     {
        if( !(*it).second->isFound() ) {
            removal.push_back((*it).second->getLocation());
        }
    }

    // now remove and delete
    vector<string>::iterator iter;
    for( iter = removal.begin(); iter != removal.end(); iter ++) {
        ATRACE("Removing Server: %s\n", (*iter).c_str());
        DialServer *p = mServerMap[(*iter)];
        delete (p);
        mServerMap.erase((*iter));
    }
}

void DialDiscovery::resetDiscovery(void) {
    ScopeLocker s(list_locker);
    ServerMap::const_iterator it;
    for( it = mServerMap.begin(); it != mServerMap.end(); it++ ) {
        (*it).second->setFound(false);
    }
}

void* DialDiscovery::send_mcast(void* p) {
    DialDiscovery* self = (DialDiscovery*) p;
	if(self) {
        self->send_discovery_request();
    }
	
	return NULL;
}

void DialDiscovery::send_discovery_request() {
    int one = 1;
	int ttl = 10;
	int my_sock = -1;
    socklen_t addrlen;
    char send_buf[strlen((char*)ssdp_msearch) + INET_ADDRSTRLEN + 256] = {0,};
    int send_size;
    pthread_attr_t attr;
	struct sockaddr_in self_saddr;
	struct sockaddr_in mcast_saddr;
	struct ip_mreq mreq;
	unsigned long addr = inet_addr("239.255.255.250");
	vector<DialServer*> list;

	send_size = snprintf(send_buf, sizeof(send_buf), ssdp_msearch);
	send_buf[send_size]='\0';
    ATRACE("[%s:%d] %s\n", __FUNCTION__, __LINE__, send_buf);
	
#if (defined(__WIN32__) || defined(_WIN32))
	START_WINSOCK();
#endif
    //Create a socket	
    if (-1 == (my_sock = socket(AF_INET, SOCK_DGRAM, 0))) {
        perror("socket");
        exit(1);
    }
    //Make it reusable
    if (-1 == setsockopt(my_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof(one))) {
        perror("reuseaddr");
        exit(1);
    }
    //Set Multicast TTL	
	if(-1 == setsockopt(my_sock, IPPROTO_IP, IP_MULTICAST_TTL, (const char*)&ttl, sizeof(ttl))) {
        perror("ttl");
        exit(1);
    }
    //Set Multicast Interface		
	if(-1 == setsockopt(my_sock, IPPROTO_IP, IP_MULTICAST_IF, (const char*)&addr, sizeof(addr))) {
        perror("multicast");
        exit(1);
    }

    self_saddr.sin_family = AF_INET;
    self_saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    self_saddr.sin_port = htons(8899);
	if(-1 == bind(my_sock, (sockaddr*)&self_saddr, sizeof(self_saddr))){
        perror("bind");
        exit(1);
	}
	
    //Set IP groudp membership
    mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
    if(-1 == setsockopt(my_sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq))) {
        perror("add_membership");
        exit(1);
	}
	addrlen = sizeof(mcast_saddr);
    mcast_saddr.sin_family = AF_INET;
    mcast_saddr.sin_addr.s_addr = inet_addr("239.255.255.250");
    mcast_saddr.sin_port = htons(1900);

    connection.saddr = mcast_saddr;
    connection.sock = my_sock;
    connection.addrlen = addrlen;
		
    while (m_bRunMcastThread) {
        ATRACE("Sending SSDP M-SEARCH to %s:%d\n", inet_ntoa(mcast_saddr.sin_addr), ntohs(mcast_saddr.sin_port));
        if (-1 == sendto(my_sock, send_buf, send_size, 0, (struct sockaddr *)&mcast_saddr, addrlen)) {
            perror("sendto");
            continue;
        }
		printf("DialDiscovery::send_mcast: Sent: %s\n", send_buf);
        m_RequestCount += 1;
		if(m_RequestCount == DialMaxMSearchRequests) {
            //Once after sending DialMaxMSearchRequests number of SSDP M-SEARCH request, lets stop doing this
            m_bRunMcastThread = false;
        }

        // set all servers to not found
        resetDiscovery();

        // spawn a response thread
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
		SetRunResponseThread(true);
        pthread_create(&_responseThread, &attr, DialDiscovery::receiveResponses, this);

        // sleep SSDP_RESPONSE_TIMEOUT seconds to allow clients to response
        sleep(SSDP_RESPONSE_TIMEOUT);
        cleanServerList();

		sleep(SSDP_REQUEST_INTERVAL-SSDP_RESPONSE_TIMEOUT);
		//Issue the callback now
		getServerList(list);
		pDialListCallback(list);
		
		SetRunResponseThread(false);
        pthread_cancel(_responseThread);
    }
	printf("DialDiscovery::send_mcast: Ending\n");
#if (defined(__WIN32__) || defined(_WIN32))
	CLOSE_WINSOCK();
#endif
}

void DialDiscovery::init(DialDiscoveryCallback aDialListCallback) {
    pDialListCallback = aDialListCallback;
    m_RequestCount = 0;
	m_bRunMcastThread = true;
    m_bRunResponseThread = true;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&_mcastThread, &attr, DialDiscovery::send_mcast, this );
}

void DialDiscovery::stop() {
	m_bRunMcastThread = false;
	m_bRunResponseThread = false;
	m_RequestCount = 0;
	pthread_cancel(_mcastThread);
}


void DialDiscovery::getServerList( vector<DialServer*>& list ) {
    for( ServerMap::iterator it = mServerMap.begin(); it != mServerMap.end(); ++it ) {
        list.push_back( it->second );
    }
}

bool DialDiscovery::getServer( const string& friendlyName, DialServer &server ) {
    return true;
}

DialDiscovery::DialDiscovery() : m_bRunMcastThread(true), m_bRunResponseThread(true), m_RequestCount(0) {
    assert( DialDiscovery::sDiscovery == 0 );
    DialDiscovery::sDiscovery = this;
}

DialDiscovery::~DialDiscovery() {
    assert( sDiscovery == this );
	cleanServerList();
    sDiscovery = 0;
}

DialDiscovery * DialDiscovery::create() {
    assert( sDiscovery == 0 );
    return new DialDiscovery();
}

