/*
 * Apache client
 * This Apache Client lists all the available Apche Servers n the visible range 
 * and then connects to the one that user wishes to
 * After connection it starts exchanging information with the server, such as
 * - WiFi AP Name
 * - WiFi AP security code, if there is one
 * - resolution
 * - 
 */

#include <stdlib.h>
#include "apr.h"
#include "apr_network_io.h"


apr_status_t get_apache_server_info(char* server, int* port) {
    //List all the Apache Servers running around
    //Ask user to select one
}

apr_status_t start_communicating(apr_pool_t* pool, const char* server, int port) {
    apr_status_t ret = APR_SUCCESS;
    apr_size_t len = 0;
    apr_size_t total = 0;
    apr_sockaddr_t* saddr = NULL;
    apr_socket_t* skt = NULL;
    char ap_name[250] = { 0 };

    ret = apr_socket_create(&skt, APR_INET, SOCK_STREAM, APR_PROTO_TCP, pool);
    if (ret)
       return ret;

    ret = apr_sockaddr_info_get(&saddr, server, APR_UNSPEC, port, 0, pool);
    if (ret)
        return ret;

    ret = apr_socket_connect(skt, saddr);
    if (ret)
        return ret;

    //Configure the WiFi AP now
    //Get the AP to connect to
    len = strlen(ap_name);
    ret = apr_socket_send(skt, str, &len);
    if (ret)
        return ret;
    //Successfully sent WiFi AP name to the Server
    //Do the same if we have to send something else

    //Ger some kind of ACK from Server to see everything is fine
    ret = apr_socket_recv(skt, &c, &len);
    if (ret)
        return ret;
    if(len) {
        //We hve received something from the Apache Server
        //Process it if needed
    }

    return ret;
}
    
int main(int argc, const char* const argv[]) {
    char buffer[256] = { 0 };
    char server[50] = { 0 };
    int port = 0;
    apr_pool_t* pool = NULL;
    apr_status_t ret = 0;

    apr_initialize();
    atexit(apr_terminate);

    //get the info server to be connected
    get_apache_server_info(server, &port);

    //now start communicating with the apache server
    apr_pool_create(&pool, NULL);
    ret = start_communicating(pool, server, port);
    if (ret) {
        fprintf(stderr, "error: %s\n", apr_strerror(ret, buffer, sizeof(buffer)));
        return -1;
    }

    return 0;
}

