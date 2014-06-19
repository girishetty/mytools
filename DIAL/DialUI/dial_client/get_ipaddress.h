
#ifndef GET_IPADDRESS_H
#define GET_IPADDRESS_H

typedef unsigned short USHORT;

#define ETHERNET_INTERFACE 1
#define WIRELESS_INTERFACE 2
#define UNDEFINED_INTERFACE 3

/*
 * Gets the IP Address of the machine calling this API (Self IP)
 * and copies it to pIpAddr, which should be big enough (16 bytes atleast) to copy an IP Address 
 * interfaceType referst to what type of interface we are interested in (Ethernet/Wireless
 *
 * returns true on success or flase
 *
 */
bool GetSelfIpAddr(char* pIpAddr, USHORT interfaceType);

#endif


