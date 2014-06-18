#ifndef __PROCESS_HANDLER_H__
#define __PROCESS_HANDLER_H__

#include "stdafx.h"
#include <string>
using namespace std;

extern const INT READ_FROM_CHILD_SUCCESS;

class ProcessHandler {
public:
	ProcessHandler(const wstring& processName, const wstring& arguments, const CHAR* outputFileName);
	virtual ~ProcessHandler();

	//Utility Functions
	void KillProcess();
	void KillProcessByName(const WCHAR* pProcessName);
	INT LaunchDetachedProcess();

	inline INT GetReadStatus() const {
		return iReadProgress;
	}

protected:
	INT CreateChildProcess(const WCHAR* pProcessName, const WCHAR* pArgList);
	inline void SetAgruments(const wstring& arguments) {
		iArguments = arguments;
	}

private:
	virtual INT ReadFromChildProcess() = 0;
	virtual INT HandleReadError(INT readError) = 0;

protected:
	wstring iProcessName;
	wstring iArguments;
	string iOutputFileName;
	HANDLE iHandleChildStdOutRd;
	HANDLE iHandleChildStdOutWr;
	INT iReadProgress;
};

#endif // __PROCESS_HANDLER_H__