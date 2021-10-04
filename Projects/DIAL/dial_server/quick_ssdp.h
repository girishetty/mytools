#ifndef _QUICK_SSDP_H
#define _QUICK_SSDP_H

// TODO: Partners should define this port
#define SSDP_PORT (56790)

/*
 * API that starts ssdp multicasting listen and response thread
 */
int run_ssdp(int port, const char *pFriendlyName, const char * pModelName, const char *pUuid);

/*
 * API that stops the thread
 */
int stop_ssdp();

#endif

