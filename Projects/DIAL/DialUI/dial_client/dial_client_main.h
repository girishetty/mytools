
#ifndef DIAL_CLIENT_MAIN_H
#define DIAL_CLIENT_MAIN_H

#include "get_ipaddress.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * start_dial_client: client interface to start the Dial Client thread
 *
 * returns 0 on success and -1 on failure
 *
 */
int start_dial_client();
/*
 * stop_dial_client: client interface to stop the Dial Client thread
 *
 * returns 0 on success and -1 on failure
 *
 */
 int stop_dial_client();

#ifdef __cplusplus
}
#endif

#endif


