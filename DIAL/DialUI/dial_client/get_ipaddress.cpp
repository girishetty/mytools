
#include <stdio.h>

#if (defined(__WIN32__) || defined(_WIN32))
#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define inet_pton InetPton
#else
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <string.h> 
#include <arpa/inet.h>
#endif
#include "get_ipaddress.h"


bool IsValidIpAddr(const char* pIP) {
    struct sockaddr_in sa;
#if 0
    int result = inet_pton(AF_INET, pIP, &(sa.sin_addr));
    return result != 0;
#else
	if(strcmp(pIP, "0.0.0.0") == 0) {
		return false;
	}
#endif
}

#if (defined(__WIN32__) || defined(_WIN32))
bool GetSelfIpAddr(char* pIpAddr, USHORT interfaceType) {
    /* Declare and initialize variables */
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    const char* pIP_Addr = NULL;
    bool found = false;

    ULONG ulOutBufLen = sizeof (IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(sizeof (IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL) {
        printf("Error allocating memory needed to call GetAdaptersinfo\n");
        return found;
    }

    // Make an initial call to GetAdaptersInfo to get the necessary size into the ulOutBufLen variable
    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO *) MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL) {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
            return found;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
        pAdapter = pAdapterInfo;
        while (pAdapter) {
			printf("GetSelfIpAddr: %s Adapter: IP Address: %s\n", pAdapter->Description, pAdapter->IpAddressList.IpAddress.String);
			pIP_Addr = NULL;
			if(pAdapter->Type == IF_TYPE_IEEE80211 && interfaceType == WIRELESS_INTERFACE) {
				//Wireless Adapter. Right not we are not using this, 
				//Just to confirm if its Wireless Adapter
				if(strstr(pAdapter->Description, "Wireless") != NULL || strstr(pAdapter->Description, "WiFi") != NULL) {
				    pIP_Addr = pAdapter->IpAddressList.IpAddress.String;
					printf("Wireless Adapter: IP Address: %s\n", pIP_Addr);
					if(IsValidIpAddr(pIP_Addr)) {
						found = true;
						break;
					}
			    }
			}
			else if(pAdapter->Type == MIB_IF_TYPE_ETHERNET && interfaceType == ETHERNET_INTERFACE) {
				if (pAdapter->HaveWins) {
					pIP_Addr = pAdapter->IpAddressList.IpAddress.String;
					printf("Ethernet Adapter: IP Address: %s\n", pIP_Addr);
					if(IsValidIpAddr(pIP_Addr)) {
						found = true;
						break;
					}
				}
			}
			else if(pAdapter->Type == MIB_IF_TYPE_PPP) {
					//PPP Adapter. Right not we are not using this
			}
			else if(pAdapter->Type == MIB_IF_TYPE_LOOPBACK) {
					//LOOPBACK Adapter. Right not we are not using this
			}
			else {
				//discard it
			}

			pAdapter = pAdapter->Next;
		}
    	if(pIP_Addr) {
			//Copy the IP Address before returning
			strcpy(pIpAddr, pIP_Addr);
        }
    } else {
        printf("GetAdaptersInfo failed with error: %ld\n", dwRetVal);
    }
    if (pAdapterInfo) {
        FREE(pAdapterInfo);
    }

    return found;
}
#else
bool GetSelfIpAddr(char* pIpAddr, USHORT interfaceType) {
    struct ifaddrs * ifAddrStruct=NULL;
    struct ifaddrs * ifa=NULL;
    void * tmpAddrPtr=NULL;
    char addressBuffer[INET_ADDRSTRLEN];
    char addressBuffer6[INET6_ADDRSTRLEN];
	bool found = false;

    if(getifaddrs(&ifAddrStruct) == -1) {
	    return false;
	}

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
			printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
			if(interfaceType == WIRELESS_INTERFACE && strcasestr(pAdapter->Description, "wlan")  != NULL) {
			    found = true;
				break;
			}
			else if(interfaceType == ETHERNET_INTERFACE && strcasestr(pAdapter->Description, "eth")  != NULL) {
			    found = true;
				break;
			}
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6 *)ifa->ifa_addr)->sin6_addr;
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer6, INET6_ADDRSTRLEN);
            printf("%s IPv6 Address %s\n", ifa->ifa_name, addressBuffer6); 
        } 
    }
	if(found) {
	    //Copy the IP Address for return
	    strcpy(pIpAddr, addressBuffer);
	}
    if (ifAddrStruct!=NULL) {
        freeifaddrs(ifAddrStruct);
    }
	
	return found;
}
#endif


