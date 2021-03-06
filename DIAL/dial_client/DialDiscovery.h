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

#ifndef DIALDISCOVERY_H
#define DIALDISCOVERY_H

#include <pthread.h>
#include <map>
#include "DialServer.h"

using namespace std;

#define SSDP_REQUEST_INTERVAL 5
#define SSDP_RESPONSE_TIMEOUT 3

typedef struct {
    struct sockaddr_in saddr;
    int sock;
    socklen_t addrlen;
}search_conn;

typedef void (*DialDiscoveryCallback) (vector<DialServer*> list);

class DialDiscovery
{
public:
    /**
     * Create a singleton
     */
    static DialDiscovery* create(void);

    /**
     * Get a pointer to the singleton
     */
    static inline DialDiscovery* instance(void);

    ~DialDiscovery();

    /**
     * Initialize the discover object.  This will kick off a periodic
     * worker thread that will poll for DIAL servers.
     *
     */
    void init(DialDiscoveryCallback aDialListCallback);
	void stop();
	
	inline bool RunMcastThread() const { return m_bRunMcastThread; }
	inline bool RunResponseThread() const { return m_bRunResponseThread; }
	inline void SetRunResponseThread(bool val) { m_bRunResponseThread = val; }

    /**
     * Get the list of servers that have been discovered.
     *
     * @param[out] list List of DIAL servers.  Returns an empty list if there
     * are no servers.
     */
    void getServerList(vector<DialServer*>& list);

    /**
     * Get a DIAL server based on friendly name
     *
     * @param[in] friendlyName Friendly name of DIAL server
     * @param[out] server Server object (if successful)
     *
     * @return true if successful, false otherwise
     */
    bool getServer( 
        const string& friendlyName,
        DialServer &server );

private:
    DialDiscovery();
    void updateServerList(string& server);
    static void* receiveResponses(void *p);
    static void* send_mcast(void *p);
    void receive_discovery_response();
    void send_discovery_request();

    void processServer(char *pResponse);
    void cleanServerList();
    void resetDiscovery();

    pthread_t _mcastThread;
    pthread_t _responseThread;

    typedef map<string, DialServer*> ServerMap;
    ServerMap mServerMap;

    static DialDiscovery* sDiscovery;
	bool m_bRunMcastThread;
	bool m_bRunResponseThread;
	int m_RequestCount;
	search_conn connection;
	DialDiscoveryCallback pDialListCallback;
};

inline DialDiscovery* DialDiscovery::instance() {
    return DialDiscovery::sDiscovery;
}

#endif // DIALDISCOVERY_H
