#ifndef _DIAL_MAIN_H
#define _DIAL_MAIN_H

/*
 * Events associated with DIAL Server
 * These events are used by the JNI Dial Server App callback to notify of the events
 */
#define APP_LAUNCH 		2000
#define APP_GET_STATUS 		2001
#define APP_STOP 		2002

/*
 * Properties associated with DIAL Server
 * JNI Dial Server App uses these properties to ask DIAL Server to send back properties
 */
#define PROP_GET_APP_NAME 	1001
#define PROP_GET_APP_ARGS 	1002

/*
 * Structure that defines the information about a DIAL enabled Application
 */
typedef struct Dial_App_Info {
    char* 	pAppName;
    char* 	pArgs;
    size_t 	argLen;
} DialAppInfo;

/*
 * Callback function for Dial Server
 */
typedef void (*DialCallback) (int, unsigned int, unsigned int);

/*
 * Structure that defines the properties for the Dial Server
 * This info is passed while initializing the dial server
 */
typedef struct Dial_Server_Property {
    char* 		pFriendlyName;
    char* 		pModelName;
    char* 		pUuid;
    DialCallback 	pCallback;
} DialServerProperty;

/*
 * Application will call this API to initialize and register callback function
 */
int DialServerInit(void* pCbFunc);

/*
 * API to terminate the DIAL Server
 */
int DialServerDestroy();

/*
 * API to get any info from dial library for application
 * prop_id should be one of the PROPERTY as defined above
 */
int DialServerGet(int propID, void* pParam1, void* pParam2);

/*
 * API to set any info from dial library for application
 * prop_id should be one of the PROPERTY as defined above
 */
int DialServerSet(int propID, unsigned int param1, unsigned int param2);

#endif

