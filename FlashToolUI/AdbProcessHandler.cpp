#include "stdafx.h"
#include <Tlhelp32.h>
#include <fstream>
#include "AdbProcessHandler.h"

const WCHAR* const gADBProcess = L"adb.exe";
const wstring gADBDevices(L"devices");
const WCHAR* const gADBTerminate = L"kill-server";
const CHAR* const gADBDeviceListFile = "devicelist.txt";
const CHAR* const gADBLookup = "List of devices attached";
#if 0
const CHAR* const gADBError = "error: protocol fault";
#else
const CHAR* const gADBError = "error";
#endif
const INT BUFSIZE = 4096;

//ADB error codes
const INT READ_FAILED_ADB_ERROR = -1001;

AdbProcessHandler::AdbProcessHandler(const wstring& processName, const wstring& arguments, const CHAR* outputFileName) : 
ProcessHandler(processName, arguments, outputFileName)
{

}

AdbProcessHandler::~AdbProcessHandler()
{
}

INT AdbProcessHandler::HandleReadError(INT readError)
{
	INT retVal = 0;
	if (readError == READ_FAILED_ADB_ERROR) {
		//adb failed, do adb kill-server and try adb again
		CreateChildProcess(iProcessName.c_str(), gADBTerminate);
		Sleep(1000);
		retVal = LaunchDetachedProcess();
	}

	return retVal;
}

AdbDevicesProcessHandler::AdbDevicesProcessHandler(const wstring& processName, const CHAR* outputFileName) : AdbProcessHandler(processName, gADBDevices, outputFileName)
{

}

AdbDevicesProcessHandler::~AdbDevicesProcessHandler()
{
}

// Read output from the child process's pipe for STDOUT
// and write it to the Output File
// In this Process (adb devices), we know that we are going to read
// list of adb devices connected
INT AdbDevicesProcessHandler::ReadFromChildProcess()
{
	INT retVal = READ_FROM_CHILD_SUCCESS;
	DWORD dwRead = 0;
	CHAR chBuf[BUFSIZE] = "";
	BOOL bSuccess = FALSE;
	string readBuffer;
	ofstream deviceFile(iOutputFileName);
	INT index = -1;

	if (deviceFile.is_open()) {
		while (TRUE) {
			bSuccess = ReadFile(iHandleChildStdOutRd, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead <= 0) {
				//adb process terminated, so we can break the loop
				break;
			}
			chBuf[dwRead] = '\0';
			readBuffer = chBuf;
			index = readBuffer.find(gADBLookup);
			if (-1 != index) {
				//adb device list is available, copy it to the file and then end the loop
				deviceFile << (readBuffer.c_str() + index);
				break;
			}
			else if (-1 != readBuffer.find(gADBError)) {
				//adb failed, do adb kill-server and try adb again
				deviceFile.close();
				retVal = READ_FAILED_ADB_ERROR;
				break;
			}
			else {
				//We haven't got the list of devices yet. Wait for 200 milli seconds before trying to read again
				Sleep(200);
			}
		}
	}
	deviceFile.close();
	return retVal;
}

AdbPushProcessHandler::AdbPushProcessHandler(const wstring& processName, const string& deviceName, const string& srcFile,
	const string& dstLocation, const CHAR* outputFileName) : AdbProcessHandler(processName, L"", outputFileName)
{
	//Construct the push command
	string pushCommand("-s ");
	pushCommand += deviceName;
	pushCommand += " push ";
	pushCommand += srcFile;
	pushCommand += " ";
	pushCommand += dstLocation;
	WCHAR tempArg[512] = L"";
	mbstowcs(tempArg, pushCommand.c_str(), 512);
	wstring arguments(tempArg);

	SetAgruments(arguments);
}

AdbPushProcessHandler::~AdbPushProcessHandler()
{
}

// Read output from the child process's pipe for STDOUT
// and write it to the Output File
// In this Process (adb push), we know that we are going to read
// the adb push result
INT AdbPushProcessHandler::ReadFromChildProcess()
{
	INT retVal = READ_FROM_CHILD_SUCCESS;
	DWORD dwRead = 0;
	CHAR chBuf[BUFSIZE] = "";
	BOOL bSuccess = FALSE;
	ofstream pushResult(iOutputFileName);

	if (pushResult.is_open()) {
		while (TRUE) {
			bSuccess = ReadFile(iHandleChildStdOutRd, chBuf, BUFSIZE, &dwRead, NULL);
			if (!bSuccess || dwRead <= 0) {
				//adb process terminated, so we can break the loop
				break;
			}
			chBuf[dwRead] = '\0';
			//adb push is done, copy it to the file and then end the loop
			pushResult << chBuf;
			break;
		}
	}
	pushResult.close();
	return retVal;
}
