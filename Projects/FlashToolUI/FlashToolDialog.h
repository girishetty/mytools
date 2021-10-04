#ifndef __FLASH_TOOL_DIALOG_H__
#define __FLASH_TOOL_DIALOG_H__

#include "stdafx.h"
#include "Resource.h"
#include <string>
#include "UsbHubPortTable.h"
using namespace std;

#define NUM_OF_USB 7
#define DEVICE_ID_MAX_LENGTH 16
#define BUS_PORT_MAX_LENGTH 20

class UsbMonitor;
class CFlashToolDialog;

struct USBDeviceInfo {
	CHAR iDeviceID[DEVICE_ID_MAX_LENGTH + 1];
	CHAR iBusPortInfo[BUS_PORT_MAX_LENGTH];
	UINT iBusNum;
	UINT iPortNum;
	UINT iSubPortNum;
};

struct USBPortInfo {
	USBDeviceInfo iDeviceInfo;
	BOOL iSelected;
};

struct ThreadData {
	CFlashToolDialog* iFlashToolDialog;
	INT iIntVal;
	HWND iHandle;
	DWORD iThreadId;

	ThreadData() : iFlashToolDialog(NULL), iIntVal(-1), iHandle(NULL), iThreadId(0) {}
};

class CFlashToolDialog
{
public:
	static CFlashToolDialog* CreateFlashTool(UsbHubType anUSBHUBType = D_LINK);
	~CFlashToolDialog();

	INT HandleMessages(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	inline INT TriggerADB(BOOL isAttached) {
#ifdef __DEBUG
		if (isAttached) {
			MessageBox(NULL, L"USB attached", L"Notification", MB_OK);
		} else {
			MessageBox(NULL, L"USB detached", L"Notification", MB_OK);
		}
#endif
		return HandleUpdateADBDeviceList(iHandleDlg, isAttached);
	}

	inline void SetDialogHandle(HWND hDlg) {
		iHandleDlg = hDlg;
	}

	void NotifyChangeUsbTopology();

	inline void SetUsbTopology(string usbTopology) {
		iCurrentTopology = usbTopology;
	}

	inline const string GetUsbTopology() const {
		return iCurrentTopology;
	}

	INT CreateUsbHubPortTable(UsbHubType anUSBHUBType);

private:
	CFlashToolDialog();
	INT DoInitialization(UsbHubType anUSBHUBType);

	INT HandleImageFileSelection(HWND hDlg);
	INT UpdateImageFileName(HWND hDlg, LPWSTR pFileName);
	void EnableAllUSBPort(HWND hDlg);
	void EnableUSBPort(HWND hDlg, INT index);
	INT UpdateDeviceID(HWND hDlg, INT portNum);
	INT HandleUpdateADBDeviceList(HWND hDlg, BOOL isAttached);
	INT UpdateADBDeviceList(HWND hDlg, BOOL isAttached);

	INT SetUSBDeviceInfo(CHAR* deviceInfo);
	INT GetADBDeviceList(BOOL isAttached);
	INT LaunchADB();
	INT UpdateDeviceList();
	void ResetDeviceList();
	INT TriggerADBShell(INT aPortNum);
	string GetAdbDeviceName(INT aPortNum) const;
	INT UpdateCPUInfo(INT aPortNum, string minFreq, string maxFreq, string currFreq,
		string currGovernors, string tranLatency);
	INT GetCPUInfo(const wstring& processName, const string& deviceName, const string& srcFile,
		const string& dstLocation, const CHAR* outputFileName, string* valOne, string* valTwo = NULL);

	inline INT GetUsbDeviceCount() const {
		return iUsbDeviceCount;
	}

	INT ProgramFCT1();
	INT ProgramUsbBootReset();
	INT MonitorCPUDetails();
	INT HandleAdbPush(INT aPortNum);
	INT HandleUsbBootReset(INT aPortNum);
	INT HandleCPUDetails(INT aPortNum);
	INT StartUpdateADBDeviceListThread(HWND hDlg, BOOL isAttached);
	static DWORD AdbPushThread(LPVOID pThreadArg);
	static DWORD UpdateADBDeviceListThread(LPVOID pThreadArg);
	static DWORD UsbBootResetThread(LPVOID pThreadArg);
	static DWORD HandleCPUDetailsThread(LPVOID pThreadArg);

private:
	USBPortInfo iUsbPortInfo[NUM_OF_USB];
	INT iUsbDeviceCount;
	string iCurrentTopology;
	string iImageFileName;
	string iDeviceListFileName;
	wstring iADBExeFullName;
	HWND iHandleDlg;
	UsbMonitor* iUsbMonitor;
	UsbHubPortTable* iUsbHubPortTable;
};

#endif
