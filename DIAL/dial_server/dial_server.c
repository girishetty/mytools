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

#include "dial_server.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "mongoose.h"

// TODO: Partners should define this port
#define DIAL_PORT (56789)
extern const char* gsGenericApp;

struct DIALApp_ {
    struct DIALApp_ *next;
    struct DIALAppCallbacks callbacks;
    void *callback_data;
    DIAL_run_t run_id;
    DIALStatus state;
    char *name;
    char payload[DIAL_MAX_PAYLOAD];
};

typedef struct DIALApp_ DIALApp;

struct DIALServer_ {
    struct mg_context *ctx;
    struct DIALApp_ *apps;
    pthread_mutex_t mux;
};

static void ds_lock(DIALServer *ds) {
    pthread_mutex_lock(&ds->mux);
}

static void ds_unlock(DIALServer *ds) {
    pthread_mutex_unlock(&ds->mux);
}

// finds an app and returns a pointer to the previous element's next pointer
// if not found, return a pointer to the last element's next pointer
static DIALApp **find_app(DIALServer *ds, const char *app_name) {
    DIALApp *app;
    DIALApp **ret = &ds->apps;
  
    for (app = ds->apps; app != NULL; ret = &app->next, app = app->next) {
        if (!strcmp(app_name, app->name)) {
            break;
        }
    }
    return ret;
}

/*
 * A bad payload is defined to be an unprintable character or a
 * non-ascii character.
 */
static int isBadPayload( const char* pPayload, int numBytes )
{
    int i = 0;
    fprintf( stderr, "Checking %d bytes\n", numBytes );
    for( ; i < numBytes; i++)
    {
        // High order bit should not be set
        // 0x7F is DEL (non-printable)
        // Anything under 32 is non-printable
        if( ((pPayload[i] & 0x80) == 0x80)  || 
            (pPayload[i] == 0x7F)           || 
            (pPayload[i] <= 0x1F) )
            return 1;
    }
    return 0;
}

static void handle_app_start(struct mg_connection *conn,
                             const struct mg_request_info *request_info,
                             const char *app_name) {
    char body[DIAL_MAX_PAYLOAD+2] = {0,};
    DIALApp *app;
    DIALServer *ds = request_info->user_data;
    int nread;
  
    ds_lock(ds);
#if 0
    app = *find_app(ds, app_name);
#else
    app = *find_app(ds, gsGenericApp);
#endif
    if (!app) {
        mg_send_http_error(conn, 404, "Not Found", "Not Found");
    } else {
        nread = mg_read(conn, body, sizeof(body));
        // NUL-terminate it just in case
        if( nread > DIAL_MAX_PAYLOAD ) {
            mg_send_http_error(conn, 413, "413 Request Entity Too Large",
                             "413 Request Entity Too Large");
        }
        else if( isBadPayload( body, nread ) ){
            mg_send_http_error(conn, 400, "400 Bad Request", "400 Bad Request");
        }
        else {
            
            app->state = app->callbacks.start_cb(ds, app_name, body, nread,
                                                 &app->run_id, app->callback_data);
            if (app->state == kDIALStatusRunning) {
                char laddr[INET6_ADDRSTRLEN];
                const struct sockaddr_in *addr =
                    (struct sockaddr_in *)&request_info->local_addr;
                inet_ntop(addr->sin_family, &addr->sin_addr, laddr, sizeof(laddr));
                mg_printf(conn, "HTTP/1.1 201 Created\r\n"
                                "Content-Type: text/plain\r\n"
                                "Location: http://%s:%d/apps/%s/run\r\n"
                                "\r\n", laddr, DIAL_get_port(ds), app_name);
                // copy the payload into the application struct
                memset( app->payload, 0, DIAL_MAX_PAYLOAD );
                memcpy( app->payload, body, nread );
            } else {
                mg_send_http_error(conn, 503, "Service Unavailable",
                                 "Service Unavailable");
            }
        }
    }
    ds_unlock(ds);
}

static void handle_app_status(struct mg_connection *conn,
                              const struct mg_request_info *request_info,
                              const char *app_name) {
    DIALApp *app;
    int canStop = 0;
    DIALServer *ds = request_info->user_data;
  
    ds_lock(ds);
#if 0
    app = *find_app(ds, app_name);
#else
    app = *find_app(ds, gsGenericApp);
#endif
    if (!app) {
        mg_send_http_error(conn, 404, "Not Found", "Not Found");
    } else {
        app->state = app->callbacks.status_cb(ds, app_name, app->run_id,
                                              &canStop, app->callback_data);
        mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                      "Content-Type: application/xml\r\n"
                      "\r\n"
                      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
                      "<service xmlns=\"urn:dial-multiscreen-org:schemas:dial\">\r\n"
                      "  <name>%s</name>\r\n"
                      "  <options allowStop=\"%s\"/>\r\n"
                      "  <state>%s</state>\r\n"
                      "%s"
                      "</service>\r\n", app->name, canStop ? "true":"false",
                      app->state ? "running" : "stopped",
                      app->state ==  kDIALStatusStopped ?
                      "" : "  <link rel=\"run\" href=\"run\"/>\r\n" );
    }
    ds_unlock(ds);
}

static void handle_app_stop(struct mg_connection *conn,
                            const struct mg_request_info *request_info,
                            const char *app_name) {
    DIALApp *app;
    DIALServer *ds = request_info->user_data;
    int canStop = 0;
  
    ds_lock(ds);
    app = *find_app(ds, app_name);
#if 0
    app = *find_app(ds, app_name);
#else
    app = *find_app(ds, gsGenericApp);
#endif

    // update the application state
    if( !app ) {
        app->state = app->callbacks.status_cb(ds, app_name, app->run_id,
                                              &canStop, app->callback_data);
    }

    if (!app || app->state != kDIALStatusRunning) {
        mg_send_http_error(conn, 404, "Not Found", "Not Found");
    } else {
        app->callbacks.stop_cb(ds, app_name, app->run_id, app->callback_data);
        app->state = kDIALStatusStopped;
        mg_printf(conn, "HTTP/1.1 200 OK\r\n"
                        "Content-Type: text/plain\r\n"
                        "\r\n");
    }
    ds_unlock(ds);
}

#define APPS_URI "/apps/"
#define RUN_URI "/run"

static void *request_handler(enum mg_event event,
                             struct mg_connection *conn,
                             const struct mg_request_info *request_info) {
    if (event == MG_NEW_REQUEST) {
        // URL ends with run
        if (!strncmp(request_info->uri + strlen(request_info->uri)-4, RUN_URI, strlen(RUN_URI))) {
            char app_name[256] = {0,}; // assuming the application name is not over 256 chars.
            strncpy( app_name, request_info->uri + strlen(APPS_URI),
              ((strlen(request_info->uri)-4) - (sizeof(APPS_URI)-1)) );
      
            // DELETE non-empty app name
            if (app_name[0] != '\0' &&
                !strcmp(request_info->request_method, "DELETE")) {
                handle_app_stop(conn, request_info, app_name);
            } else {
                mg_send_http_error(conn, 501, "Not Implemented", "Not Implemented");
            }
        }
        // URI starts with "/apps/"
        else if (!strncmp(request_info->uri, APPS_URI, sizeof(APPS_URI) - 1)) {
            const char *app_name;
            app_name = request_info->uri + sizeof(APPS_URI) - 1;
            // start app
            if (!strcmp(request_info->request_method, "POST")) {
                handle_app_start(conn, request_info, app_name);
            // get app status
            } else if (!strcmp(request_info->request_method, "GET")) {
                handle_app_status(conn, request_info, app_name);
            } else {
                mg_send_http_error(conn, 501, "Not Implemented", "Not Implemented");
            }
        } else {
            mg_send_http_error(conn, 404, "Not Found", "Not Found");
        }
        return "done";
    } else if (event == MG_EVENT_LOG) {
        fprintf( stderr, "MG: %s\n", request_info->log_message);
        return "done";
    }
    return NULL;
}

DIALServer *DIAL_start() {
    DIALServer *ds = calloc(1, sizeof(DIALServer));
    struct mg_context *ctx;
  
    pthread_mutex_init(&ds->mux, NULL);
    ctx = mg_start(&request_handler, ds, DIAL_PORT);
    if (!ctx) {
        free(ds);
        return NULL;
    }
    ds->ctx = ctx;
    return ds;
}

void DIAL_stop(DIALServer *ds) {
    mg_stop(ds->ctx);
    pthread_mutex_destroy(&ds->mux);
}

in_port_t DIAL_get_port(DIALServer *ds) {
    struct sockaddr sa;
    socklen_t len = sizeof(sa);
    if (!mg_get_listen_addr(ds->ctx, &sa, &len)) {
        return 0;
    }
    return ntohs(((struct sockaddr_in *)&sa)->sin_port);
}

int DIAL_register_app(DIALServer *ds, const char *app_name,
                      struct DIALAppCallbacks *callbacks,
                      void *user_data) {
    DIALApp **ptr, *app;
    int ret;
  
    ds_lock(ds);
    ptr = find_app(ds, app_name);
    if (*ptr != NULL) { // app already registered
        ds_unlock(ds);
        ret = 0;
    } else {
        app = malloc(sizeof(DIALApp));
        app->callbacks = *callbacks;
        app->name = strdup(app_name);
        app->next = *ptr;
        app->state = kDIALStatusStopped;
        app->callback_data = user_data;
        *ptr = app;
        ret = 1;
    }
  
    ds_unlock(ds);
    return ret;
}

int DIAL_unregister_app(DIALServer *ds, const char *app_name) {
    DIALApp **ptr, *app;
    int ret;
  
    ds_lock(ds);
    ptr = find_app(ds, app_name);
    if (*ptr == NULL) { // no such app
        ret = 0;
    } else {
        app = *ptr;
        *ptr = app->next;
        free(app->name);
        free(app);
        ret = 1;
    }
  
    ds_unlock(ds);
    return ret;
}

const char * DIAL_get_payload(DIALServer *ds, const char *app_name)
{
    const char * pPayload = NULL;
    DIALApp **ptr, *app;
  
    // NOTE: Don't grab the mutex as we are calling this function from 
    // inside the application callback which already has the lock.
    //ds_lock(ds);
    ptr = find_app(ds, app_name);
    if (*ptr != NULL)
    {
        app = *ptr;
        pPayload = app->payload;
    }
    //ds_unlock(ds);
    return pPayload;
}

