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

#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <regex.h>

#ifdef ANDROID
#include <utils/Log.h>
#define printf LOGI
#endif

#include "dial_server.h"
#include "quick_ssdp.h"
#include "dial_main.h"

const DialServerProperty gDefaultProp = { "Android DIAL Server", "NOT A VALID MODEL NAME", "deadbeef-dead-beef-dead-beefdeadbeef", NULL };
DialServerProperty* gDialProperty = NULL;
DIALServer* pDialServ = NULL;
int gDialPort = 0;
const char* gsGenericApp = "GenericApp";
DialAppInfo gAppInfo = { NULL, NULL, 0 };


void AllocAndCopy(char** dest, const char* const src) {
    *dest = (char*) malloc(strlen(src));
    strcpy(*dest, src);
}

void CopyAppInfo(const char* pAppName, const char* pArgs, size_t argLen) {
    if(gAppInfo.pAppName) {
        free(gAppInfo.pAppName);
    }
    AllocAndCopy(&gAppInfo.pAppName, pAppName);
    if(gAppInfo.pArgs) {
        free(gAppInfo.pArgs);
    }
    if(pArgs) {
        gAppInfo.pArgs = (char*) malloc(argLen+1);
        memcpy(gAppInfo.pArgs, pArgs, argLen);
        gAppInfo.pArgs[argLen]='\0';
        gAppInfo.argLen = argLen;
        LOGI("\n %s: Dial: gAppInfo.pAppName=  %s gAppInfo.pArg = %s   ", __FUNCTION__, gAppInfo.pAppName, gAppInfo.pArgs);
    }
}
/*
 *  Callbacks to pass the info back to the JAVA layer of Dial Server
 */
static DIALStatus generic_app_start(DIALServer* pDS, const char* pAppName, const char* pArgs, size_t argLen, DIAL_run_t* run_id, void* callback_data) {
    /* Setup the arguments in the global list and issue the callback with the APP_LAUNCH event */
    CopyAppInfo(pAppName, pArgs, argLen);
    gDialProperty->pCallback(APP_LAUNCH, 0, 0);
    LOGI("\n %s: DialServer Callback: %s %s   ", __FUNCTION__, pAppName, pArgs);
    return kDIALStatusRunning;
}

static DIALStatus generic_app_status(DIALServer* pDS, const char* pAppName, DIAL_run_t run_id, int* pCanStop, void* callback_data) {
    CopyAppInfo(pAppName, NULL, 0);
    gDialProperty->pCallback(APP_GET_STATUS, 0, 0);
    LOGI("\n %s: DialServer Callback   ", __FUNCTION__);
    return kDIALStatusRunning;
}

static void generic_app_stop(DIALServer* pDS, const char* pAppName, DIAL_run_t run_id, void* callback_data) {
    CopyAppInfo(pAppName, NULL, 0);
    gDialProperty->pCallback(APP_STOP, 0, 0);
    LOGI("\n %s: DialServer Callback   ", __FUNCTION__);
    return;
}

/*
 * Internal funcitons
 */

int RunDial() {
    int ret = -1;
    struct DIALAppCallbacks dial_callback = {generic_app_start, generic_app_stop, generic_app_status};
    printf("DialServer: runDial: before DIAL_start()\n");
    pDialServ = DIAL_start();
    if(pDialServ) {
        DIAL_register_app(pDialServ, gsGenericApp, &dial_callback, NULL);
        gDialPort = DIAL_get_port(pDialServ);
        printf("DialServer: runDial: launcher listening on gDialPort %d\n", gDialPort);
        ret = run_ssdp(gDialPort, gDialProperty->pFriendlyName, gDialProperty->pModelName, gDialProperty->pUuid);
    }
    return ret;
}

void SetDialProperty(const DialServerProperty* pPro) {
    char* temp = NULL;
    LOGI("\nDialServer: %s", __FUNCTION__);
    gDialProperty = (DialServerProperty*) malloc(sizeof(DialServerProperty));

    temp = gDefaultProp.pFriendlyName;
    if(pPro && pPro->pFriendlyName) {
        temp = pPro->pFriendlyName;
    }
    AllocAndCopy(&gDialProperty->pFriendlyName, temp);

    temp = gDefaultProp.pModelName;
    if(pPro && pPro->pModelName) {
        temp = pPro->pModelName;
    }
    AllocAndCopy(&gDialProperty->pModelName, temp);

    temp = gDefaultProp.pUuid;
    if(pPro && pPro->pUuid) {
        temp = pPro->pUuid;
    }
    AllocAndCopy(&gDialProperty->pUuid, temp);
}


/**********************************************************************************************
 * ALL EXPORTED APIs
 **********************************************************************************************/

/*
 * Application will call this API to initialize and register callback function
 */
int DialServerInit(void* pCbFunc) {
    LOGI("\nDialServer: %s", __FUNCTION__);
    SetDialProperty(&gDefaultProp);
    gDialProperty->pCallback = (DialCallback)pCbFunc;

    RunDial();

    return 0;
}

/*
 * API to terminate the DIAL Server
 */
int DialServerStop() {
    int ret = -1;
    LOGI("\nDialServer: %s", __FUNCTION__);
    if(pDialServ) {
        DIAL_stop(pDialServ);
        stop_ssdp();
        pDialServ = NULL;
        printf("DialServer: DialServerStop: after DIAL_stop()\n");
    }

    return ret;
}

/*
 * API to get any info from dial library for application
 * prop_id will be a macro which we can decide later
 */
int DialServerGet(int propID, void* pParam1, void* pParam2) {
    int ret = 0;
    LOGI("\nDialServer: %s", __FUNCTION__);
    if(propID == PROP_GET_APP_NAME) {
        strcpy(pParam1, gAppInfo.pAppName);
        LOGI("\nDialServer: %s: APPNAME =  %s ", __FUNCTION__, (char*)pParam1);
    }
    else if(propID == PROP_GET_APP_ARGS) {
        /* We are NULL terminating it */
        memcpy(pParam1, gAppInfo.pArgs, gAppInfo.argLen);
        ((char*)pParam1)[gAppInfo.argLen]='\0';
        *(int*)pParam2 = gAppInfo.argLen;
        LOGI("\nDialServer: %s: APPARGS =  %s ", __FUNCTION__, (char*)pParam1);
    }
    else {
        ret = -1;
    }

    return ret;
}
/*
 * API to set any info from dial library for application
 * prop_id will be a macro which we can decide later
 */
int DialServerSet(int prop_id, unsigned int param1, unsigned int param2) {
    LOGI("\nDialServer: %s", __FUNCTION__);
    return 0;
}

