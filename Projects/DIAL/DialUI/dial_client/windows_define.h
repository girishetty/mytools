
#ifndef WINDOWS_DEFINE_H
#define WINDOWS_DEFINE_H

/*
 * This header is specific for windows that defines missing Windows specific functions, that we use in UNIX/LINUX system
 *
 */

#if (defined(__WIN32__) || defined(_WIN32))
#include <winsock2.h>

#define MILLISEC 1000
#define sleep(TIME) Sleep(TIME*MILLISEC);
#define INET_ADDRSTRLEN 16
typedef int socklen_t;

#define START_WINSOCK()		/* Structure for WinSock setup communication */ 	\
							WSADATA wsaData;                 					\
																				\
							/* Load Winsock 2.0 DLL */							\
							if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)  {	\
								printf("WSAStartup() failed!!!!");				\
								exit(1);										\
							}													\

#define CLOSE_WINSOCK()		/* Cleanup Winsock */  								\
							WSACleanup();  
#endif

#endif



