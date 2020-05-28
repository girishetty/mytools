#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <string.h> 
#include <arpa/inet.h>

void getIpAddr() {
    struct ifaddrs* ifAddrStruct = NULL;
    struct ifaddrs* ifa = NULL;
    void* tmpAddrPtr = NULL;
    char addressBuffer[INET_ADDRSTRLEN];
    char addressBuffer6[INET6_ADDRSTRLEN];

    getifaddrs(&ifAddrStruct);

    for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa ->ifa_addr->sa_family==AF_INET) { // check it is IP4
            // is a valid IP4 Address
            tmpAddrPtr=&((struct sockaddr_in*)ifa->ifa_addr)->sin_addr;
            inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
            printf("%s IP Address %s\n", ifa->ifa_name, addressBuffer); 
        } else if (ifa->ifa_addr->sa_family==AF_INET6) { // check it is IP6
            // is a valid IP6 Address
            tmpAddrPtr=&((struct sockaddr_in6*)ifa->ifa_addr)->sin6_addr;
            inet_ntop(AF_INET6, tmpAddrPtr, addressBuffer6, INET6_ADDRSTRLEN);
            printf("%s IPv6 Address %s\n", ifa->ifa_name, addressBuffer6); 
        } 
    }
    if (ifAddrStruct != NULL) {
        freeifaddrs(ifAddrStruct);
    }
}

void tokenizeString(int argc, char* argv[]) {
    int index=0;
    char* ip;
    char* port;
    for(index=0;index<argc;index++) {
        printf("argv[%d]: %s\n", index, argv[index]);
    }
    ip = strtok(argv[index-1], ":");
    port = strtok(NULL, ":");
    if (ip && port) {
        printf("ip: %s, port[%d]\n", ip, atoi(port));
    } else {
        printf("Invalid Arguments\n");
    }
}

int main(int argc, char* argv[]) {
    int fromlen;
    int s, ns;
    struct sockaddr_in saun, fsaun;
    char source_ip_addr[22];
    int ret = 0;
    int port_no = 7788;

#if 1
    getIpAddr();
    tokenizeString(argc, argv);
    return 0;
#endif

    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }
    printf("create socket\n");
    /*
     * Create the address we will be binding to.
     */
    saun.sin_family = AF_INET;
    saun.sin_addr.s_addr = INADDR_ANY;
    saun.sin_port = htons(port_no);

    if (bind(s, (struct sockaddr *)&saun, sizeof(saun)) < 0) {
        perror("server: bind");
        exit(1);
    }

    /*
     * Listen on the socket.
     */
    if (listen(s, 5) < 0) {
        perror("server: listen");
        exit(1);
    }
    printf("listening...\n");
    /*
     * Accept connections.  When we accept one, ns
     * will be connected to the client.  fsaun will
     * contain the address of the client.
     */
    if ((ns = accept(s, (struct sockaddr *)&fsaun, &fromlen)) < 0) {
        perror("server: accept");
        exit(1);
    }
    printf("establish connection\n");

    // recv IP:Port 
    ret = recv(ns, source_ip_addr, 21, 0);
    if(ret == -1) {
        perror("server: recv");
        exit(1);
    }
    source_ip_addr[ret] = '\0';
    printf("Source Addr is: %s ret[%d]\n", source_ip_addr, ret);

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(s);

    return 0;
}

