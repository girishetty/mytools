#ifndef __ADB_PROCESS_HANDLER_H__
#define __ADB_PROCESS_HANDLER_H__

#include "ProcessHandler.h"

extern const WCHAR* const gADBProcess;
extern const CHAR* const gADBDeviceListFile;
extern const CHAR* const gADBLookup;
extern const INT BUFSIZE;

class AdbProcessHandler : public ProcessHandler {
public:
	AdbProcessHandler(const wstring& processName, const wstring& arguments, const CHAR* outputFileName);
	virtual ~AdbProcessHandler();

private:
	virtual INT HandleReadError(INT readError);
};


class AdbDevicesProcessHandler : public AdbProcessHandler {
public:
	AdbDevicesProcessHandler(const wstring& processName, const CHAR* outputFileName);
	virtual ~AdbDevicesProcessHandler();

private:
	virtual INT ReadFromChildProcess();
};


class AdbPushProcessHandler : public AdbProcessHandler {
public:
	AdbPushProcessHandler(const wstring& processName, const string& deviceName, const string& srcFile, const string& dstLocation, const CHAR* outputFileName);
	virtual ~AdbPushProcessHandler();

private:
	virtual INT ReadFromChildProcess();
};

class AdbPullProcessHandler : public AdbProcessHandler {
public:
	AdbPullProcessHandler(const wstring& processName, const string& deviceName, const string& srcFile, const string& dstLocation, const CHAR* outputFileName);
	virtual ~AdbPullProcessHandler();

private:
	virtual INT ReadFromChildProcess();
};

class AdbShellProcessHandler : public AdbProcessHandler {
public:
	AdbShellProcessHandler(const wstring& processName, const string& deviceName, const wstring& command, const CHAR* outputFileName);
	virtual ~AdbShellProcessHandler();

private:
	virtual INT ReadFromChildProcess();
};

class AdbResetTrackerProcessHandler : public AdbShellProcessHandler {
public:
	AdbResetTrackerProcessHandler(const wstring& processName, const string& deviceName, const CHAR* outputFileName);
	virtual ~AdbResetTrackerProcessHandler();

};

class AdbWarmBootProcessHandler : public AdbShellProcessHandler {
public:
	AdbWarmBootProcessHandler(const wstring& processName, const string& deviceName, const CHAR* outputFileName);
	virtual ~AdbWarmBootProcessHandler();

};

#endif //__ADB_PROCESS_HANDLER_H__
