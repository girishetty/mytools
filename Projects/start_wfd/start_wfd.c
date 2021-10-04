#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h> 
#include <errno.h>

const int default_listen_port = 5599;
const char* default_rtsp_port = "9000";

int start_wfd(const char* server_ip, const char* rtsp_port) {
    int ret = 0;
    pid_t pid;
    int status = 0;
    printf("We are here to start the wfd_sink with [%s:%s]\n", server_ip, rtsp_port);

    pid = fork();
    if (-1 == pid) {
        printf("start_wfd: Failed to fork!!\n");
        return -1;
    }

    if (0 == pid) {
        execl("/system/bin/wfd_sink", "wfd_sink", "-s", server_ip, "-p", rtsp_port, NULL);
        printf("start_wfd: We should not Reach here! Error[%d]\n", errno);
    } else {
        printf("start_wfd: Launched wfd_sink with pid[%d]\n", pid);
        //Now lets wait till the child complets
        printf("start_wfd: waiting on wfd_sink to complete...\n");
        sleep(1);
        waitpid(pid, &status, 0);
        printf("start_wfd: wfd_sink ended...\n");
    }

    return ret;
}

int main(int argc, char* argv[]) {
    int fromlen;
    int s, ns;
    struct sockaddr_in saun, fsaun;
    char source_ip_addr[22];
    int ret = 0;
    int port_no = default_listen_port;
    char* ip = NULL;
    char* port = NULL;

    printf("WFD Starter service is ON...\n");

    /*
     * Get a socket to work with.  This socket will
     * be in the UNIX domain, and will be a
     * stream socket.
     */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("server: socket");
        exit(1);
    }
    printf("created socket\n");
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
    printf("established connection\n");

    // recv IP:Port 
    ret = recv(ns, source_ip_addr, 21, 0);
    if(ret == -1) {
        perror("server: recv");
        exit(1);
    }
    source_ip_addr[ret] = '\0';
    printf("Source Addr is: %s\n", source_ip_addr);

    /* Parse the message to get ip_address and the port */
    ip = strtok(source_ip_addr, ":");
    port = strtok(NULL, ":");

    if(port == NULL){
        port = default_rtsp_port;
    }
    printf("ip[%s] port[%s]\n", ip, port);

    /* Now start the wfd_sink App */
    start_wfd(ip, port);

    /*
     * We can simply use close() to terminate the
     * connection, since we're done with both sides.
     */
    close(s);
    close(ns);
    return 0;
}

