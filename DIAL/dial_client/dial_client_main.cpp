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

#include <string>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#if (defined(__WIN32__) || defined(_WIN32))
#include <TlHelp32.h>
#include "windows_define.h"
#endif
#include "DialDiscovery.h"
#include "get_ipaddress.h"

//Define this FLAG if we want to work along with the DIAL CLIENT UI APP
//#define READ_FROM_SOCKET
#ifdef READ_FROM_SOCKET
int dial_client_ui_sock = -1;
#endif

const char* const AppYoutube = "AP_YOUTUBE";
const char* const AppNetflix = "AP_NETFLIX";
const char* const AppMiracast = "AP_MIRACAST";
const int DIAL_RESPONSE_WAIT_TIME = 5;
const char* RTSP_SERV_PORT = ":554";
char* gWFDServerExeFullPath = "C:\\Users\\gshetty\\work\\codebase\\rt-rk\\wfd_snapshot_win\\wifi_display\\wfd_core\\server\\MarvellWFDServer.exe";
char* gWFDServerArg = "-p 554 --screen --force-video 1280x720";
const char* const gWFDServerExe = "MarvellWFDServer.exe";
pthread_t gThreadID;
using namespace std;

int stop_dial_client(DialServer* pServer=NULL);
static DialDiscovery* gpDiscovery;

// TODO: Make it possible to pass applications from the command line
//static vector<string> gAppList;
static string gOutputFile;
static string gInputFile;

// IP address of the DIAL server
static string gIpAddress;

PROCESS_INFORMATION gWFDServerInfo;
bool gWFDServerRunning = false;
bool gNetflixRunning = false;
bool gYoutubeRunning = false;
bool gMiracastRunning = false;

int endProcess(PROCESS_INFORMATION& processInformation) {
    // Terminate the process and close the handles
    int ret = 0;
    if(gWFDServerRunning) {
        if(FALSE == TerminateProcess( processInformation.hProcess, 0)) {
            ret = -1;
        }
        CloseHandle( processInformation.hProcess );
        CloseHandle( processInformation.hThread );
        gWFDServerRunning = false;
	}
	
	return ret;
}

int createAndRunProcess(PROCESS_INFORMATION& processInformation, char* pAppName, char* pArguments) {
    int ret = 0;
    STARTUPINFO startupInfo;
    memset(&processInformation, 0, sizeof(processInformation));
    memset(&startupInfo, 0, sizeof(startupInfo));
    startupInfo.cb = sizeof(startupInfo);
	
	char commandLineArgs[MAX_PATH *2];
	int len1 = strlen(pAppName);
	memcpy(commandLineArgs, pAppName, len1);
	commandLineArgs[len1] = ' ';
	int len2 = strlen(pArguments);
	memcpy((commandLineArgs+len1+1), pArguments, len2);
	commandLineArgs[len1+len2+1] = '\0';

    if(FALSE == CreateProcess(pAppName, commandLineArgs, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &startupInfo, &processInformation)) {
	    ret = -1;
		printf("\n\n\n********WFD SERVER START FAILED..\\n\n\n");
	}
	else {
	    gWFDServerRunning = true;
		printf("\n\n\n********WFD SERVER STARTED..\\n\n\n");
	}
	return ret;
}

bool isProcessRunning(const char* const pProcess) {
    //Else use Win32 APIs to find whether the process is still running or not
    bool running = false;
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE) {
        while (Process32Next(snapshot, &entry) == TRUE) {
            if (strcmp(entry.szExeFile, pProcess) == 0) { 
                running = true;
                printf("********[%s] Already running.. So not Launching the Process again..\n", pProcess);
                break;
            }
        }
    }

    CloseHandle(snapshot);
    return running;
}	

void launchAppIfNeeded(const char* pApp) {
    if(strcmp(pApp, AppMiracast) == 0 && gWFDServerRunning == false) {
        //We have to launch WFD Server now, but before that whether its really not running.
		printf("launchAppIfNeeded: Lets see if %s is running already\n", pApp);
        if(isProcessRunning(gWFDServerExe) == false) {
			printf("launchAppIfNeeded: %s is not running, so lets start it\n", pApp);
            createAndRunProcess(gWFDServerInfo, gWFDServerExeFullPath, gWFDServerArg);
        }
    }
}

void launchNetflix(DialServer* pServer) {
	//Get the movie name from the user
    char movie_name[255];  
    printf("Enter Movie Name: ");
	scanf("%s", movie_name);

    string responseHeaders, responseBody, payload;
    string ap_netflix = AppNetflix;	
    string ap_netflix_payload = movie_name;
    pServer->launchApplication(ap_netflix, ap_netflix_payload, responseHeaders, responseBody);
	gNetflixRunning = true;
}

void stopNetflix(DialServer* pServer) {
    if(gNetflixRunning) {
        string responseHeaders;
        string ap_netflix = AppNetflix;	
        string endpoint = "/run";

        pServer->stopApplication(ap_netflix, endpoint, responseHeaders);
        gNetflixRunning = false;
	}
}

void queryNetflix(DialServer* pServer) {
    string responseHeaders, responseBody;
    string ap_netflix = AppNetflix;	

    pServer->getStatus(ap_netflix, responseHeaders, responseBody);
}

void launchYoutube(DialServer* pServer) {
	//Get the youtube url from the user before passing it on
    char youtube_url[255] = "https://www.youtube.com/watch?v=XSGBVzeBUbk";  
    printf("Enter Youtube link: ");
	//scanf("%s", youtube_url);

    string responseHeaders, responseBody, payload;
    string ap_youtube = AppYoutube;	
    string ap_youtube_payload = youtube_url;
    pServer->launchApplication(ap_youtube, ap_youtube_payload, responseHeaders, responseBody);
	gYoutubeRunning = true;
}

void stopYoutube(DialServer* pServer) {
    if(gYoutubeRunning) {
        string responseHeaders;
        string ap_yotube = AppYoutube;	
	    string endpoint = "/run";

        pServer->stopApplication(ap_yotube, endpoint, responseHeaders);
        gYoutubeRunning = false;
    }
}

void queryYoutube(DialServer* pServer) {
    string responseHeaders, responseBody;
    string ap_yotube = AppYoutube;	

    pServer->getStatus(ap_yotube, responseHeaders, responseBody);
}

void launchMiracast(DialServer* pServer) {
    //Start the WFD Server first
	launchAppIfNeeded(AppMiracast);
	
	//Get the IP Address of self as we need to post the RTSP_IP:PORT to the DIAL Server
	USHORT interfaceType = ETHERNET_INTERFACE;
	printf("Enter interface type 1. Etherner\t 2. Wireless: ");
	scanf("%hu", &interfaceType);
	if(interfaceType >= UNDEFINED_INTERFACE) {
	    //fallback to the default one
	    interfaceType = ETHERNET_INTERFACE;
	}
    char rtsp_serv_ip_addr[22];  
    if(GetSelfIpAddr(rtsp_serv_ip_addr, interfaceType)) {
        //Got the IP Address of Self, Append the RTSP Server Port#
        strcat(rtsp_serv_ip_addr, RTSP_SERV_PORT);
    }
    printf("launchMiracast: Self Address: %s\n", rtsp_serv_ip_addr);

    string responseHeaders, responseBody, payload;
    string ap_miracast = AppMiracast;	
    string ap_miracast_payload = rtsp_serv_ip_addr;
    pServer->launchApplication(ap_miracast, ap_miracast_payload, responseHeaders, responseBody);
	gMiracastRunning = true;
}

void stopMiracast(DialServer* pServer) {
    if(gMiracastRunning) {
	    printf("stopMiracast\n");
        string responseHeaders;
        string ap_miracast = AppMiracast;	
	    string endpoint = "/run";

        pServer->stopApplication(ap_miracast, endpoint, responseHeaders);
        gMiracastRunning = false;
		//TODO: Kill WFD Source as well
    }
}

void queryMiracast(DialServer* pServer) {
    string responseHeaders, responseBody;
    string ap_miracast = AppMiracast;	

    pServer->getStatus(ap_miracast, responseHeaders, responseBody);
}

#ifdef READ_FROM_SOCKET
static int setupDialClientSock() {
	int dial_client_port_no = 7789;
	struct sockaddr_in service_addr;

	/* Create a reliable, stream socket using TCP */
	if ((dial_client_ui_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("Socket creation FAILED\n");
		return dial_client_ui_sock;
	}
	
	int flag = 1;
	int ret = setsockopt(dial_client_ui_sock, IPPROTO_TCP, TCP_NODELAY, (char *) &flag, sizeof(int)); 
	if ( ret < 0) {
		printf("setsockopt() failed with ret[%d] error[%d]\n", ret, errno);
		perror("setsockopt error");
		return -1;
	}
	/* Construct the server address structure */
	memset(&service_addr, 0, sizeof(service_addr));     		/* Zero out structure */
	service_addr.sin_family      = AF_INET;             		/* Internet address family */
	service_addr.sin_addr.s_addr = inet_addr("127.0.0.1");   		/* Server IP address */
	service_addr.sin_port        = htons(dial_client_port_no);  /* Server port */

	/* Establish the connection to the server */
	ret = connect(dial_client_ui_sock, (struct sockaddr*) &service_addr, sizeof(service_addr));
	if ( ret < 0) {
		printf("connect() failed with ret[%d] error[%d]\n", ret, errno);
		perror("connect error");
		return -1;
	}
	return 0;
}

static void cleanupDialClientSock() {
	closesocket(dial_client_ui_sock);
	dial_client_ui_sock = -1;
}

static void printServerList(vector<DialServer*> list) {
	//Now Pass the list of Dial Server to the Dial Client UI App
    int i = 0;
    vector<DialServer*>::iterator it;
	string name_ip;
	char ack[2];
	int len=0;
    for(it = list.begin(); it < list.end(); it++, i++) {
        string uuid, name, serv_ip;
        (*it)->getFriendlyName(name);
        (*it)->getUuid(uuid);
		serv_ip = (*it)->getIpAddress();
		//Now we need to pass Name:serv_ip to Dial Client UI
		name_ip = name;
		name_ip += ":";
		name_ip += serv_ip;
		send(dial_client_ui_sock, name_ip.c_str(), name_ip.size(), 0);
		//wait for ACK and ignore it
		recv(dial_client_ui_sock, ack, 2, 0);
    }
#if 0	
	send(dial_client_ui_sock, "Girish:10.83.33.17", 18, 0);
	//wait for ACK
	recv(dial_client_ui_sock, ack, 2, 0);
	send(dial_client_ui_sock, "Shetty:10.83.33.27", 18, 0);
	//wait for ACK
	recv(dial_client_ui_sock, ack, 2, 0);
#endif
	//Indicate the end to Dial Client UI App
	send(dial_client_ui_sock, "end", 3, 0);
	recv(dial_client_ui_sock, ack, 2, 0);
}
#else
static void printServerList(vector<DialServer*> list) {
    int i = 0;
    vector<DialServer*>::iterator it;
	printf("=============================\n");
    for(it = list.begin(); it < list.end(); it++, i++) {
        string uuid, name, serv_ip;
        (*it)->getFriendlyName(name);
        (*it)->getUuid(uuid);
		serv_ip = (*it)->getIpAddress();
		printf("%d. Server IP[%s] FriendlyName[%s] \n", i+1, serv_ip.c_str(), name.c_str());
    }
	printf("=============================\n");
}
#endif

static DialServer* getServerFromUser(vector<DialServer*> list) {
    DialServer* pServer = NULL;

#ifdef READ_FROM_SOCKET
	if(dial_client_ui_sock == -1 && setupDialClientSock() != 0) {
		printf("Failed to establish connection with the Dial Client UI App\n");
		return NULL;
	}
	//Now Pass the list of Dial Server to the Dial Client UI App
	printServerList(list);
	//And read the one UI Selected
	unsigned int server_no = 1;
	char buffer[10];
	int size = recv(dial_client_ui_sock, buffer, sizeof(server_no), 0);
	buffer[size] = '\0';
	server_no = atoi(buffer);
	printf("server no: %d\n", server_no);
	assert(server_no <= list.size());
	pServer = list[server_no-1];
#else		
    if(list.size() > 1) {
        // show a list to the user
        printf("Found Multiple[%d] servers\n", list.size());
        printServerList(list);
        printf("Enter server [1-%d]: ", list.size());
		unsigned int server_no = 1;
        scanf("%u", &server_no);
        assert(server_no <= list.size() );
        pServer = list[server_no-1];
    }
    else {
	    //There is only one DIAL Server, connect to that
        pServer = list.front();
    }
#endif
    return pServer;
}

DialServer* getServerFromList(DialDiscovery* pDial, vector<DialServer*>& rList) {
    pDial->getServerList(rList);
	do {
		if( rList.size() == 0 ) {
			printf("No servers available.. will try again after %d secs\n", SSDP_REQUEST_INTERVAL);
			//If we haven't discovered any Dial based devices, try sending SSDP M-SEARCH M-CAST again
			pDial->init(printServerList);
			sleep(SSDP_REQUEST_INTERVAL);
			pDial->getServerList(rList);
			continue;
		}
		break;
	}while(1);
	
    return getServerFromUser( rList );
}

int getUserChoice() {
    int input = 0;
#ifdef READ_FROM_SOCKET
	//Now read the input from DialClient UI App
	char buffer[10];
	int size = recv(dial_client_ui_sock, buffer, sizeof(input), 0);
	buffer[size] = '\0';
	input = atoi(buffer);
#else
	printf("Enter what you want to Do:\n");
	printf("0. Print List of Dial Servers\n");
    printf("1. Launch Netflix\n2. Launch Youtube\n3. Launch Miracast\n");
	printf("4. Stop Netflix\n5. Stop Youtube\n6. Stop Miracast\n");
	printf("7. Query Status of Netflix\n8. Query Status of Youtube\n9. Query Status of Miracast\n");
	printf("To exit this App, press any other key:.....\nWaiting for your Input: ");

    scanf("%d", &input);
#endif
	return input;
}

int communicateWithDialServer(DialDiscovery* pDial) {
    vector<DialServer*> list;
	DialServer* pServer = NULL;
	pServer = getServerFromList(pDial, list);
	do{
	    //pServer = getServerFromList(pDial, list);
		int input = getUserChoice();
		if(input == 0) {
			printServerList(list);
		}else if(input == 1) {
			launchNetflix(pServer);
		} else if(input == 2) {
			launchYoutube(pServer);
		} else if(input == 3) {
			launchMiracast(pServer);
		} else if(input == 4) {
			stopNetflix(pServer);
		} else if(input == 5) {
			stopYoutube(pServer);
		} else if(input == 6) {
			stopMiracast(pServer);
		} else if(input == 7) {
			queryNetflix(pServer);
		} else if(input == 8) {
			queryYoutube(pServer);
		} else if(input == 9) {
			queryMiracast(pServer);
		} else {
		    break;
		}
		//pDial->stop();
	}while(1);

	//Now we can terminate the DIAL Client
	
	stop_dial_client(pServer);

    return 0;
}

void* (dial_client_entry) (void* ) {
    gpDiscovery = DialDiscovery::create();
	printf("dial_client_main: created DialDiscovery\n");
    gpDiscovery->init(printServerList);
    // Sleep for DIAL_RESPONSE_WAIT_TIME seconds to allow DIAL servers to response to MSEARCH.
    sleep(DIAL_RESPONSE_WAIT_TIME);

    communicateWithDialServer(gpDiscovery);
	return NULL;
}

int start_dial_client() {
#ifdef RUN_DIAL_CLIENT_ON_THREAD
	int ret = -1;
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	ret = pthread_create(&gThreadID, &attr, dial_client_entry, NULL);
	
    return ret;
#else
	dial_client_entry(NULL);
	return 0;
#endif
}

int stop_dial_client(DialServer *pDial) {
    endProcess(gWFDServerInfo);
    if(pDial) {
	    stopNetflix(pDial);
        stopYoutube(pDial);
        stopMiracast(pDial);
    }
	gpDiscovery->stop();
#ifdef READ_FROM_SOCKET
	cleanupDialClientSock();
#endif

#ifdef RUN_DIAL_CLIENT_ON_THREAD
    int ret = -1;
	void* res = 0;

	ret = pthread_cancel(gThreadID);
	if (ret == 0) {
		/* Join with thread to see what its exit status was */
		ret = pthread_join(gThreadID, &res);
		printf("stop_dial_client: DIAL Client exited with %d\n", ret);
	}
	else {
		printf("stop_dial_client: pthread_cancel FAILED\n");
	}

    return ret;
#else
	return 0;
#endif
}

int main(int argc, char* argv[]) {
    //Start the WFD Server first
	//launchAppIfNeeded(AppMiracast);

    int ret = start_dial_client();
	if( ret!= 0) {
		printf("FAILED to start Dial Client App");
	}
	return ret;
}
