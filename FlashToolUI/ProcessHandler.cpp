
#include "stdafx.h"
#include <Tlhelp32.h>
#include <fstream>
#include "ProcessHandler.h"

const INT READ_FROM_CHILD_SUCCESS = 0;

ProcessHandler::ProcessHandler(const wstring& processName, const wstring& arguments, const CHAR* outputFileName) :
iProcessName(processName),
iArguments(arguments),
iOutputFileName(outputFileName),
iHandleChildStdOutRd(NULL),
iHandleChildStdOutWr(NULL),
iReadProgress(-1)
{
}

ProcessHandler::~ProcessHandler()
{
	CloseHandle(iHandleChildStdOutRd);
	CloseHandle(iHandleChildStdOutWr);
}

void ProcessHandler::KillProcess()
{
	unsigned found = iProcessName.find_last_of(L"/\\");
	wstring wExeName = iProcessName.substr(found + 1);
	KillProcessByName(wExeName.c_str());
}

void ProcessHandler::KillProcessByName(const WCHAR* pProcessName)
{
	//Fetch list of all "running" process, locate the one which we want to kill and kill it using Process Handle
	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
	PROCESSENTRY32 pEntry;
	pEntry.dwSize = sizeof(pEntry);
	BOOL hRes = Process32First(hSnapShot, &pEntry);

	while (hRes) {
		if (lstrcmpW(pEntry.szExeFile, pProcessName) == 0) {
			HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, 0, (DWORD)pEntry.th32ProcessID);
			if (hProcess != NULL) {
				TerminateProcess(hProcess, 9);
				CloseHandle(hProcess);
			}
		}
		hRes = Process32Next(hSnapShot, &pEntry);
	}
	CloseHandle(hSnapShot);
}

INT ProcessHandler::LaunchDetachedProcess()
{
	INT retVal = -1;
	// Set the bInheritHandle flag so pipe handles are inherited. 
	SECURITY_ATTRIBUTES saAttr;
	ZeroMemory(&saAttr, sizeof(saAttr));
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;

	// Create a pipe for the child process's STDOUT. 
	if (!CreatePipe(&iHandleChildStdOutRd, &iHandleChildStdOutWr, &saAttr, 0))
		return -1;

	// Ensure the read handle to the pipe for STDOUT is not inherited.
	if (!SetHandleInformation(iHandleChildStdOutRd, HANDLE_FLAG_INHERIT, 0))
		return -1;

	// Create the child process.
	retVal = CreateChildProcess(iProcessName.c_str(), iArguments.c_str());

	if (retVal != -1) {
		// Read from pipe that is the standard output for child process. 
		iReadProgress = ReadFromChildProcess();
		if (iReadProgress != READ_FROM_CHILD_SUCCESS) {
			retVal = HandleReadError(iReadProgress);
		}
	}

	CloseHandle(iHandleChildStdOutRd);
	iHandleChildStdOutRd = NULL;
	CloseHandle(iHandleChildStdOutWr);
	iHandleChildStdOutWr = NULL;

	return retVal;
}

INT ProcessHandler::CreateChildProcess(const WCHAR* pProcessName, const WCHAR* pArgList)
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
	PROCESS_INFORMATION procInfo;
	STARTUPINFO startInfo;
	BOOL bSuccess = FALSE;
	INT retVal = -1;
	WCHAR wCmdArgs[_MAX_FNAME];

	wstring wFillPath(pProcessName);
	wstring wExeName;
	unsigned found = wFillPath.find_last_of(L"/\\");
	wExeName = wFillPath.substr(found + 1);
	lstrcpyW(wCmdArgs, wExeName.c_str());
	lstrcatW(wCmdArgs, L" ");
	lstrcatW(wCmdArgs, pArgList);

	// Set up members of the PROCESS_INFORMATION structure. 
	ZeroMemory(&procInfo, sizeof(PROCESS_INFORMATION));

	// Set up members of the STARTUPINFO structure. 
	// This structure specifies the STDIN and STDOUT handles for redirection.
	ZeroMemory(&startInfo, sizeof(STARTUPINFO));
	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.hStdError = iHandleChildStdOutWr;
	startInfo.hStdOutput = iHandleChildStdOutWr;
	startInfo.dwFlags |= STARTF_USESTDHANDLES;

	// Create the child process. 
	bSuccess = CreateProcess(pProcessName,
		wCmdArgs,          // command line 
		NULL,              // process security attributes 
		NULL,              // primary thread security attributes 
		TRUE,              // handles are inherited 
		DETACHED_PROCESS,  // creation flags 
		NULL,              // use parent's environment 
		NULL,              // use parent's current directory 
		&startInfo,      // STARTUPINFO pointer 
		&procInfo);      // receives PROCESS_INFORMATION 

	if (bSuccess) {
		// Close handles to the child process and its primary thread.
		// Some applications might keep these handles to monitor the status
		// of the child process, for example. 

		CloseHandle(procInfo.hProcess);
		CloseHandle(procInfo.hThread);
		retVal = 0;
	}

	return retVal;
}
