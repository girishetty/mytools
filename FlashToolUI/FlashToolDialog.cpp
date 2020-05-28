#include "stdafx.h"
#include <psapi.h>
#include <Commdlg.h>
#include <fstream>
#include <sys/stat.h>
#include "FlashToolDialog.h"
#include "UsbMonitor.h"
#include "AdbProcessHandler.h"
#include "UsbHubPortTable.h"

//Enable this for console based ADB launching
#define PROFILING
#ifdef PROFILING

class Profiling {

public:
	Profiling() {
		//Initialize the Frequency once
		if (iPCFreq == 0) {
			LARGE_INTEGER frequency;
			QueryPerformanceFrequency(&frequency);
			iPCFreq = double(frequency.QuadPart) / 1000.0;
		}
	}

	__int64 StartCounter() {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return counter.QuadPart;
	}

	double GetCounter(__int64 aCounterStart) {
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);
		return double(counter.QuadPart - aCounterStart) / iPCFreq;
	}

private:
	static double iPCFreq;
};

double Profiling::iPCFreq = 0.0;

#endif

//Enable this for console based ADB launching
//#define CONSOLE_BASED_ADB

extern HINSTANCE hInst;
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#ifdef CONSOLE_BASED_ADB
const CHAR* const gADBBatch = "launchadb.bat";
#endif

const char* gAvailFreq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies";
const char* gCurFreq = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_cur_freq";
const char* gAvailGov = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_available_governors";
const char* gCurGov = "/sys/devices/system/cpu/cpu0/cpufreq/scaling_governor";
const char* gLatency = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_transition_latency";

const INT ADB_RETRY_COUNT = 6;
const INT ADB_RETRY_WAIT_TIME = 3000;  //in milli seconds
//const INT ADB_RETRY_WAIT_TIME = 1000;  //in milli seconds

CFlashToolDialog* CFlashToolDialog::CreateFlashTool(UsbHubType anUSBHUBType)
{
	CFlashToolDialog* self = new CFlashToolDialog();
	if (self) {
		if (0 >= self->DoInitialization(anUSBHUBType)) {
			//Initialization Faied, we need to kill the APP
			MessageBox(NULL, L"Failed to Initialize USB Device Manager", L"Error!", ERROR);
			delete self;
			exit(0);
		}
	}

	return self;
}

CFlashToolDialog::CFlashToolDialog() : iUsbDeviceCount(0), iHandleDlg(0), iUsbMonitor(NULL), iUsbHubPortTable(NULL)
{
	memset(iUsbPortInfo, 0, NUM_OF_USB * sizeof(USBPortInfo));
}


CFlashToolDialog::~CFlashToolDialog()
{
	if (iUsbMonitor) {
		iUsbMonitor->EndUsbMonitor();
	}
	delete iUsbHubPortTable;
}

INT CFlashToolDialog::DoInitialization(UsbHubType anUSBHUBType)
{
	INT retVal = 0;

	iUsbMonitor = UsbMonitor::CreateUsbMonitor(this);

	if (iUsbMonitor) {
		// Get the process name and update the PWD.
		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
		if (NULL != hProcess) {
			HMODULE hMod = 0;
			WCHAR wProcessFilename[_MAX_DIR + _MAX_FNAME];
			CHAR cPWD[_MAX_DIR + _MAX_FNAME];
			DWORD nLen = _MAX_DIR + _MAX_FNAME;

			if (0 != GetModuleFileNameEx(hProcess, NULL, wProcessFilename, nLen)) {
				wstring fileName(wProcessFilename);
				unsigned found = fileName.find_last_of(L"/\\");
				wstring path = fileName.substr(0, found);
				found = path.find_last_of(L"/\\");
				wstring pwd = path.substr(0, found);
				//Set the PWD now and launch ADB
				retVal = SetCurrentDirectory(pwd.c_str());
				//Save the PWD for future reference
				wcstombs(cPWD, pwd.c_str(), _MAX_DIR + _MAX_FNAME);
				iDeviceListFileName = cPWD;
				iDeviceListFileName += "\\";
				iDeviceListFileName += gADBDeviceListFile;
				iADBExeFullName = pwd;
				iADBExeFullName += L"\\";
				iADBExeFullName += gADBProcess;
				retVal = 0;

				iUsbHubPortTable = UsbHubPortTable::CreateUsbHubPortTable(anUSBHUBType);
				if (iUsbHubPortTable) {
					retVal = 1; //SUCCESS
				}
			}
		}
	}

	return retVal;
}

INT CFlashToolDialog::CreateUsbHubPortTable(UsbHubType anUSBHUBType)
{
	INT retVal = 1;

	if (iUsbHubPortTable) {
		delete iUsbHubPortTable;
	}
	iUsbHubPortTable = UsbHubPortTable::CreateUsbHubPortTable(anUSBHUBType);
	if (!iUsbHubPortTable) {
		MessageBox(NULL, L"Failed to Initialize USB Device Manager", L"Error!", ERROR);
		retVal = 0;
	} else if (iHandleDlg) {
		TriggerADB(FALSE);
	}

	return retVal;
}

INT CFlashToolDialog::HandleMessages(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	INT retVal = FALSE;
	switch (wParam) {
	//Update the Serial Port Device List
	case IDC_BUTTON_REFRESH:
		retVal = HandleUpdateADBDeviceList(hDlg, FALSE);
		break;

	//Handle Image File Selection
	case ID_IMAGE_NAME:
		break;
	case ID_BROWSE_IMAGE:
		retVal = HandleImageFileSelection(hDlg);
		break;

	//Handle Imaging Steps
	case ID_IMAGE_FCT1:
		ProgramFCT1();
		break;
	case ID_IMAGE_FCT2:
		//ProgramFCT2();
		ProgramUsbBootReset();
		break;
	case ID_IMAGE_FCT3:
		MonitorCPUDetails();
		break;
	case ID_IMAGE_FCT4:
		break;

	case ID_PORT_SELECT_ALL:
		EnableAllUSBPort(hDlg);
		break;
	
	//Handle Device Selection
	case ID_PORT_1:
		EnableUSBPort(hDlg, 0);
		break;
	case ID_PORT_2:
		EnableUSBPort(hDlg, 1);
		break;
	case ID_PORT_3:
		EnableUSBPort(hDlg, 2);
		break;
	case ID_PORT_4:
		EnableUSBPort(hDlg, 3);
		break;
	case ID_PORT_5:
		EnableUSBPort(hDlg, 4);
		break;
	case ID_PORT_6:
		EnableUSBPort(hDlg, 5);
		break;
	case ID_PORT_7:
		EnableUSBPort(hDlg, 6);
		break;

	//Update Device IDs
	case ID_DEVICE_ID_1:
		break;
	case ID_DEVICE_ID_2:
		break;
	case ID_DEVICE_ID_3:
		break;
	case ID_DEVICE_ID_4:
		break;
	case ID_DEVICE_ID_5:
		break;
	case ID_DEVICE_ID_6:
		break;
	case ID_DEVICE_ID_7:
		break;

	//Update Progress bar
	case ID_PROGRESS_PORT_1:
		break;
	case ID_PROGRESS_PORT_2:
		break;
	case ID_PROGRESS_PORT_3:
		break;
	case ID_PROGRESS_PORT_4:
		break;
	case ID_PROGRESS_PORT_5:
		break;
	case ID_PROGRESS_PORT_6:
		break;
	case ID_PROGRESS_PORT_7:
		break;

	//Handle Log button clicks
	case ID_LOG_PORT_1:
		TriggerADBShell(0);
		break;
	case ID_LOG_PORT_2:
		TriggerADBShell(1);
		break;
	case ID_LOG_PORT_3:
		TriggerADBShell(2);
		break;
	case ID_LOG_PORT_4:
		TriggerADBShell(3);
		break;
	case ID_LOG_PORT_5:
		TriggerADBShell(1);
		break;
	case ID_LOG_PORT_6:
		TriggerADBShell(2);
		break;
	case ID_LOG_PORT_7:
		TriggerADBShell(3);
		break;

	//Handle Log update
	case ID_STATUS_LOG:
		break;

	//About the Tool
	case IDC_ABOUT_BUTTON:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), iHandleDlg, About);
		break;
	}
	return TRUE;
}

INT CFlashToolDialog::HandleUpdateADBDeviceList(HWND hDlg, BOOL isAttached)
{
#if 1
	return UpdateADBDeviceList(hDlg, isAttached);
#else
	return StartUpdateADBDeviceListThread(hDlg, isAttached);
#endif
}

INT CFlashToolDialog::UpdateADBDeviceList(HWND hDlg, BOOL isAttached)
{
	//Get the ADB device list
	INT retVal = GetADBDeviceList(isAttached);

	//Update the devices
	for (INT portNum = 0; portNum < NUM_OF_USB; portNum++) {
		retVal = UpdateDeviceID(hDlg, portNum);
	}

	return retVal;
}

INT CFlashToolDialog::StartUpdateADBDeviceListThread(HWND hDlg, BOOL isAttached)
{
	INT retVal = 0;
	HANDLE adbDeviceThreadID = 0;
	ThreadData threadData;

	threadData.iFlashToolDialog = this;
	threadData.iHandle = hDlg;
	threadData.iIntVal = isAttached;

	adbDeviceThreadID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&UpdateADBDeviceListThread, &threadData, 0, &threadData.iThreadId);

	if (adbDeviceThreadID) {
		//Creation of thread failed, terminate the loop
		WaitForSingleObject(adbDeviceThreadID, INFINITE);
		CloseHandle(adbDeviceThreadID);
	}
	else {
		retVal = -1;
	}

	return retVal;
}

DWORD CFlashToolDialog::UpdateADBDeviceListThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData) {
		retVal = pThreadData->iFlashToolDialog->UpdateADBDeviceList(pThreadData->iHandle, pThreadData->iIntVal);
	}

	return retVal;
}

INT CFlashToolDialog::HandleImageFileSelection(HWND hDlg)
{
	//Setup the FileOpen Dialog Box
	OPENFILENAME fileOpen;
	WCHAR fileName[_MAX_DIR + _MAX_FNAME] = L"\0";

	memset(fileName, 0, sizeof(WCHAR) * _MAX_DIR + _MAX_FNAME);
	memset(&fileOpen, 0, sizeof(fileOpen));

	fileOpen.lStructSize = sizeof(fileOpen);
	fileOpen.hwndOwner = hDlg;
	fileOpen.hInstance = hInst;
	fileOpen.lpstrFile = fileName;
	fileOpen.nFilterIndex = 0;
	fileOpen.nMaxFile = 256;
	fileOpen.lpstrInitialDir = NULL;
	fileOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&fileOpen)) {
		UpdateImageFileName(hDlg, fileOpen.lpstrFile);
	}

	return TRUE;
}

INT CFlashToolDialog::UpdateImageFileName(HWND hDlg, LPWSTR pFileName)
{
	INT retVal = -1;
	HWND hEdit = GetDlgItem(hDlg, ID_IMAGE_NAME);
	if (hEdit) {
		retVal = SetDlgItemText(hDlg, ID_IMAGE_NAME, pFileName);
		if (retVal) {
			CHAR fileName[_MAX_DIR + _MAX_FNAME];
			wcstombs(fileName, pFileName, _MAX_DIR + _MAX_FNAME);
			iImageFileName = fileName;
		}
	}
	return retVal;
}

void CFlashToolDialog::EnableAllUSBPort(HWND hDlg)
{
	int index = 0;
	int editID = -1;
	int retVal = -1;
	BOOL val = FALSE;

	HWND hAllListBox = GetDlgItem(hDlg, ID_PORT_SELECT_ALL);
	if (hAllListBox) {
		val = IsDlgButtonChecked(hDlg, ID_PORT_SELECT_ALL);
	}

	//Set all the Port Checkbox accordingly
	for (; index < NUM_OF_USB; index++) {
		iUsbPortInfo[index].iSelected = val;
		switch (index) {
		case 0:
			editID = ID_PORT_1;
			break;
		case 1:
			editID = ID_PORT_2;
			break;
		case 2:
			editID = ID_PORT_3;
			break;
		case 3:
			editID = ID_PORT_4;
			break;
		case 4:
			editID = ID_PORT_5;
			break;
		case 5:
			editID = ID_PORT_6;
			break;
		case 6:
			editID = ID_PORT_7;
			break;
		}
		HWND hListBox = GetDlgItem(hDlg, editID);
		if (hListBox) {
			CheckDlgButton(hDlg, editID, val);
		}
	}
}

void CFlashToolDialog::EnableUSBPort(HWND hDlg, INT index)
{
	if (index >= 0 && index < NUM_OF_USB) {
		//Toggle/Flip the Port selection
		iUsbPortInfo[index].iSelected = !iUsbPortInfo[index].iSelected;
	}
}

INT CFlashToolDialog::UpdateDeviceID(HWND hDlg, INT portNum)
{
	INT retVal = -1;
	int nIDDlgItem = 0;

	switch (portNum) {
	case 0:
		nIDDlgItem = ID_DEVICE_ID_1;
		break;
	case 1:
		nIDDlgItem = ID_DEVICE_ID_2;
		break;
	case 2:
		nIDDlgItem = ID_DEVICE_ID_3;
		break;
	case 3:
		nIDDlgItem = ID_DEVICE_ID_4;
		break;
	case 4:
		nIDDlgItem = ID_DEVICE_ID_5;
		break;
	case 5:
		nIDDlgItem = ID_DEVICE_ID_6;
		break;
	case 6:
		nIDDlgItem = ID_DEVICE_ID_7;
		break;
	}

	HWND hDevice = GetDlgItem(hDlg, nIDDlgItem);
	if (hDevice) {
		WCHAR wDeviceID[DEVICE_ID_MAX_LENGTH + 2];
		mbstowcs(wDeviceID, iUsbPortInfo[portNum].iDeviceInfo.iDeviceID, DEVICE_ID_MAX_LENGTH + 1);
		retVal = SetDlgItemText(hDlg, nIDDlgItem, wDeviceID);
	}
	return retVal;
}

void CFlashToolDialog::NotifyChangeUsbTopology()
{
	MessageBox(NULL, L"USB Topology Changed, Please POWER RESET the USB HUB", L"Notification", MB_OK);
}

INT CFlashToolDialog::GetADBDeviceList(BOOL isAttached)
{
	INT retVal = -1;
	INT retryCount = 1;
	BOOL done = FALSE;
	INT lastDeviceCount = GetUsbDeviceCount();
	INT updatedDeviceCount = 0;

	if (isAttached) {
		retryCount = ADB_RETRY_COUNT;
	}

	do{
		retVal = LaunchADB();
		updatedDeviceCount = GetUsbDeviceCount();
		//Retry again if adb devices fails to update the devices
		if (lastDeviceCount == updatedDeviceCount) {
			retryCount--;
			if (retryCount == 0) {
				//We have tried enough, now its time to give up
				done = TRUE;
			} else {
				//We failed to get the device list. So, wait some time before trying again
				Sleep(ADB_RETRY_WAIT_TIME);
			}
		} else {
			done = TRUE;
		}
	} while (!done);

	return retVal;
}

INT CFlashToolDialog::LaunchADB()
{
	INT retVal = -1;
	struct stat st = { 0 };

	ProcessHandler* processHandler = new AdbDevicesProcessHandler(iADBExeFullName, iDeviceListFileName.c_str());
	if (processHandler) {
		//Kill all the adb processes, if exists one
		//processHandler->KillProcessByName(gADBProcess);

		//Remove the device list file, if it exists
		remove(iDeviceListFileName.c_str());

		//Launch ADB
#ifdef CONSOLE_BASED_ADB
		retVal = system(gADBBatch);
#else
		retVal = processHandler->LaunchDetachedProcess();
#endif

		if (retVal == 0) {
			//Successfully issued ADB
			if (stat(iDeviceListFileName.c_str(), &st) != -1) {
				//We have the device list file ready and available. So check is there any entries in the file
				if (0 < UpdateDeviceList()) {
					//We have atleast one entry in the file
					retVal = 1;
				}
				else {
					retVal = -1;
				}
			}
		}
		else {
			retVal = -1;
		}
		delete processHandler;
	}
	
	return retVal;
}

INT CFlashToolDialog::UpdateDeviceList()
{
	INT readCount = 0;
	string entry;
	INT portNum = -1;
	INT found = -1;
	ifstream deviceFile(iDeviceListFileName);

	if (deviceFile.is_open()) {
		//Reset the current device list
		ResetDeviceList();

		//We can skip all the entries until we find "List of devices attached"
		while (getline(deviceFile, entry)) {
			if (0 == entry.find(gADBLookup)) {
				break;
			}
		}

		while (getline(deviceFile, entry)) {
			/* 
			 * Each entry is of the format:
			 *     "<DeviceID> SPACE <PortInfo> SPACE/TAB [device][offline]"
			 * Example: 532b97ec985c307a 3-2	device
             */
			//Look for the 2nd occurance of SPACE/TAB after <DeviceID>, truncate the entry there and parse it
#if 0
			found = entry.find(' ', DEVICE_ID_MAX_LENGTH + 1);
#else
			found = entry.find(" ");
#endif
			if (found == -1) {
				//If SPACE is not found look to TAB
#if 0
				found = entry.find('\t', DEVICE_ID_MAX_LENGTH + 1);
#else
				found = entry.find('\t');
#endif
			}

			//Don't bother if its an offline device
			if (found != -1 && -1 != entry.find("device", found + 1)) {
				entry.resize(found);
				//Fetch Device ID and Port Info now
				portNum = SetUSBDeviceInfo((CHAR*)entry.c_str());
				if (portNum != -1) {
					iUsbDeviceCount++;
				}
			}
		}
		deviceFile.close();
	}
	return iUsbDeviceCount;
}

void CFlashToolDialog::ResetDeviceList()
{
	//Reset all the device entries
	for (int index = 0; index < NUM_OF_USB; index++) {
		iUsbPortInfo[index].iDeviceInfo.iDeviceID[0] = '\0';
		iUsbPortInfo[index].iDeviceInfo.iBusPortInfo[0] = '\0';
		iUsbPortInfo[index].iDeviceInfo.iBusNum = 0;
		iUsbPortInfo[index].iDeviceInfo.iPortNum = 0;
		iUsbPortInfo[index].iDeviceInfo.iSubPortNum = 0;
	}
	iCurrentTopology.resize(0);
	iUsbDeviceCount = 0;
}

INT CFlashToolDialog::SetUSBDeviceInfo(CHAR* deviceInfo)
{
#if 0
	CHAR tempBuf[100];
	CHAR busPortInfo[20];
	CHAR* deviceID = NULL;
	CHAR* busNum = NULL;
	CHAR* portNum = NULL;
	CHAR* subPortNum = NULL;
	CHAR* nextItem = NULL;
	INT port = -1;
	INT subPort = -1;
	/*
	* Device Info String that we receive is of the format: <16-HEX-Digit-Device-ID> SPACE <BusNo>-<Port>.[<SubPort>]
	* Example: 532b97ec985c307a 3-2.1
	* So, lets parse the string to get Deto get ip_address and the port */
	strcpy(tempBuf, deviceInfo);
	deviceID = strtok(deviceInfo, " ");
	strcpy(busPortInfo, tempBuf + strlen(deviceID) + 1);

	busNum = strtok(NULL, "-");
	portNum = strtok(NULL, ".");
	nextItem = portNum;

	// USB "portNum" may be connected to a USB HUB (which in turn can connect to another USB HUB)
	// So, go deep inside until we find the last SubPort Number
	do {
		subPortNum = nextItem;
		nextItem = strtok(NULL, ".");
	} while (nextItem != NULL);

	if (subPortNum != NULL) {
		subPort = atoi(subPortNum);
	}

#if 1
	//Get the Port Number from the UsbHubPortTable
	subPort = iUsbHubPortTable->GetPortNum(busPortInfo);
#endif

	if (subPort > 0 && subPort <= NUM_OF_USB) {
		subPort -= 1;
		//Now we can MAP our USB Port Table
		iUsbPortInfo[subPort].iDeviceInfo.iSubPortNum = subPort + 1;
		iUsbPortInfo[subPort].iDeviceInfo.iPortNum = atoi(portNum);
		iUsbPortInfo[subPort].iDeviceInfo.iBusNum = atoi(busNum);
		strcpy(iUsbPortInfo[subPort].iDeviceInfo.iDeviceID, deviceID);
		strcpy(iUsbPortInfo[subPort].iDeviceInfo.iBusPortInfo, busPortInfo);
		iCurrentTopology = iUsbPortInfo[subPort].iDeviceInfo.iBusPortInfo;
		//We don't need the last port number in the USB topology.
		iCurrentTopology.resize(iCurrentTopology.size() - 2);
	}
	else {
		subPort = -1;
	}
#else
	INT subPort = 0;
	iUsbPortInfo[subPort].iDeviceInfo.iSubPortNum = 1;
	iUsbPortInfo[subPort].iDeviceInfo.iPortNum = 1;
	iUsbPortInfo[subPort].iDeviceInfo.iBusNum = 1;
	strcpy(iUsbPortInfo[subPort].iDeviceInfo.iDeviceID, deviceInfo);
	strcpy(iUsbPortInfo[subPort].iDeviceInfo.iBusPortInfo, deviceInfo);
	iCurrentTopology = iUsbPortInfo[subPort].iDeviceInfo.iBusPortInfo;
	//We don't need the last port number in the USB topology.
	iCurrentTopology.resize(iCurrentTopology.size() - 2);
#endif
	return subPort;
}

string CFlashToolDialog::GetAdbDeviceName(INT aPortNum) const
{
	string adbDevice;
	if (aPortNum >= 0 && aPortNum < NUM_OF_USB) {
		const CHAR* deviceID = iUsbPortInfo[aPortNum].iDeviceInfo.iDeviceID;
		const CHAR* portInfo = iUsbPortInfo[aPortNum].iDeviceInfo.iBusPortInfo;
		//Proceed if we have a valid adb device attached to this Port
		if (deviceID[0] != '\0' && portInfo[0] != '\0') {
#if 0
			adbDevice = "\"";
			adbDevice += deviceID;
			adbDevice += " ";
			adbDevice += portInfo;
			adbDevice += '\"';
#else
			adbDevice = deviceID;
#endif
		}
	}

	return adbDevice;
}


INT CFlashToolDialog::TriggerADBShell(INT aPortNum)
{
	INT retVal = -1;

	string adbDevice = GetAdbDeviceName(aPortNum);

	if (adbDevice.size() >  0) {
		//Construct adb shell arguments
		string adbCommand("adb -s ");
		adbCommand += adbDevice;
		adbCommand += " shell";

		//Launch ADB shell now
		INT retVal = system(adbCommand.c_str());

		if (retVal == 0) {
			//Successfully issued ADB
		}
		else {
			retVal = -1;
		}
	}
	return retVal;
}

INT CFlashToolDialog::ProgramFCT1()
{
	INT retVal = 0;
	HANDLE flashingThreadID[NUM_OF_USB] = { 0 };
	ThreadData threadData[NUM_OF_USB];
	int index = 0;

#ifdef PROFILING
	CHAR info[200];
	CHAR fileSize[50] = "";
	WCHAR wInfo[200];
	WCHAR wFileName[_MAX_DIR + _MAX_FNAME];
	INT deviceCount = 0;
	Profiling Profile;
	__int64 counterStart = Profile.StartCounter();
#endif 

	//Set all the Port Checkbox accordingly
	for (; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected) {
#ifdef PROFILING
			deviceCount++;
#endif 
			threadData[index].iFlashToolDialog = this;
			threadData[index].iIntVal = index;

			flashingThreadID[index] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&AdbPushThread, &threadData[index], 0, &threadData[index].iThreadId);

			if (NULL == flashingThreadID[index]) {
				//Creation of thread failed, terminate the loop
				retVal = -1;
				break;
			}
		}
	}

	//Now wait for the threads to complete
	for (index = 0; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected && NULL != flashingThreadID[index]) {
			WaitForSingleObject(flashingThreadID[index], INFINITE);
			CloseHandle(flashingThreadID[index]);
		}
	}

#ifdef PROFILING
	double timeTaken = Profile.GetCounter(counterStart);

	LARGE_INTEGER nLargeInteger = { 0 };
	mbstowcs(wFileName, iImageFileName.c_str(), _MAX_DIR + _MAX_FNAME);

	HANDLE hFile = CreateFile(wFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		GetFileSizeEx(hFile, &nLargeInteger);
	}

	if (nLargeInteger.HighPart != 0) {
		sprintf(fileSize, "%d", nLargeInteger.HighPart);
	}
	INT len = strlen(fileSize);
	sprintf(fileSize + len, "%d", nLargeInteger.LowPart);

	sprintf(info, "ADB Push took %f milliseconds to transfer %s bytes of data to %d devices", timeTaken, fileSize, deviceCount);
	mbstowcs(wInfo, info, 200);

	MessageBox(NULL, wInfo, L"Notification", MB_OK);
#endif
	return retVal;
}

DWORD CFlashToolDialog::AdbPushThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData) {
		retVal = pThreadData->iFlashToolDialog->HandleAdbPush(pThreadData->iIntVal);
	}

	return retVal;
}

INT CFlashToolDialog::HandleAdbPush(INT aPortNum)
{
	INT retVal = -1;
	string adbDevice = GetAdbDeviceName(aPortNum);
	string destPath("/home/galois/lib/");

	if (adbDevice.size() >  0) {
#if 0
		//Construct adb shell arguments
		string adbCommand("adb -s ");
		adbCommand += adbDevice;
		adbCommand += " push ";
		adbCommand += iImageFileName;
		adbCommand += " ";
		adbCommand += destPath;

		//Launch ADB shell now
		INT retVal = system(adbCommand.c_str());
#else
		char fName[50];
		sprintf(fName, "adbpush_result_%d.txt", aPortNum);
		unsigned found = iDeviceListFileName.find_last_of("/\\");
		string outputFile = iDeviceListFileName.substr(0, found+1);
		outputFile += fName;
		ProcessHandler* pProcessHandler = new AdbPushProcessHandler(iADBExeFullName, adbDevice, iImageFileName, destPath, outputFile.c_str());
		if (pProcessHandler) {
			retVal = pProcessHandler->LaunchDetachedProcess();
			delete pProcessHandler;
		}
#endif

		if (retVal == 0) {
			//Successfully issued ADB
		}
		else {
			retVal = -1;
		}
	}
	return retVal;
}

INT CFlashToolDialog::ProgramUsbBootReset()
{
	INT retVal = 0;
	HANDLE resetThreadID[NUM_OF_USB] = { 0 };
	ThreadData threadData[NUM_OF_USB];
	int index = 0;

	//Set all the Port Checkbox accordingly
	for (; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected) {
			threadData[index].iFlashToolDialog = this;
			threadData[index].iIntVal = index;

			resetThreadID[index] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&UsbBootResetThread, &threadData[index], 0, &threadData[index].iThreadId);

			if (NULL == resetThreadID[index]) {
				//Creation of thread failed, terminate the loop
				retVal = -1;
				break;
			}
		}
	}

	//Now wait for the threads to complete
	for (index = 0; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected && NULL != resetThreadID[index]) {
			WaitForSingleObject(resetThreadID[index], INFINITE);
			CloseHandle(resetThreadID[index]);
		}
	}

	return retVal;
}

DWORD CFlashToolDialog::UsbBootResetThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData) {
		retVal = pThreadData->iFlashToolDialog->HandleUsbBootReset(pThreadData->iIntVal);
	}

	return retVal;
}

INT CFlashToolDialog::HandleUsbBootReset(INT aPortNum)
{
	INT retVal = -1;
	string adbDevice = GetAdbDeviceName(aPortNum);

	if (adbDevice.size() >  0) {
		char fName[50];
		sprintf(fName, "adbreboot_result_%d.txt", aPortNum);
		unsigned found = iDeviceListFileName.find_last_of("/\\");
		string outputFile = iDeviceListFileName.substr(0, found + 1);
		outputFile += fName;
		ProcessHandler* pProcessHandler = new AdbResetTrackerProcessHandler(iADBExeFullName, adbDevice, outputFile.c_str());
		if (pProcessHandler) {
			retVal = pProcessHandler->LaunchDetachedProcess();
			delete pProcessHandler;
			//Once after successfully setting Reset Tracker Register, trigger warm reboot now
			if (retVal == 0) {
				pProcessHandler = new AdbWarmBootProcessHandler(iADBExeFullName, adbDevice, outputFile.c_str());
				if (pProcessHandler) {
					retVal = pProcessHandler->LaunchDetachedProcess();
					delete pProcessHandler;
				}
			}
		}
		if (retVal == 0) {
			//Successfully rebooted the device on usb mode
		}
		else {
			retVal = -1;
		}
	}
	return retVal;
}


INT CFlashToolDialog::MonitorCPUDetails()
{
	INT retVal = 0;
	HANDLE monitorThreadID[NUM_OF_USB] = { 0 };
	ThreadData threadData[NUM_OF_USB];
	int index = 0;

	//Set all the Port Checkbox accordingly
	for (; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected) {
			threadData[index].iFlashToolDialog = this;
			threadData[index].iIntVal = index;

			monitorThreadID[index] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&HandleCPUDetailsThread, &threadData[index], 0, &threadData[index].iThreadId);

			if (NULL == monitorThreadID[index]) {
				//Creation of thread failed, terminate the loop
				retVal = -1;
				break;
			}
		}
	}

	//Now wait for the threads to complete
	for (index = 0; index < NUM_OF_USB; index++) {
		if (iUsbPortInfo[index].iSelected && NULL != monitorThreadID[index]) {
			WaitForSingleObject(monitorThreadID[index], INFINITE);
			CloseHandle(monitorThreadID[index]);
		}
	}

	return retVal;
}

DWORD CFlashToolDialog::HandleCPUDetailsThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData) {
		retVal = pThreadData->iFlashToolDialog->HandleCPUDetails(pThreadData->iIntVal);
	}

	return retVal;
}

INT CFlashToolDialog::HandleCPUDetails(INT aPortNum)
{
	INT retVal = -1;
	string adbDevice = GetAdbDeviceName(aPortNum);
	string minFreq;
	string maxFreq;
	string currFreq;
	string currGovernors;
	string tranLatency;

	if (adbDevice.size() >  0) {
		string deviceSrcPath("/etc/proc/");
		char fName[50];
		unsigned found = iDeviceListFileName.find_last_of("/\\");
		string outputFile = iDeviceListFileName.substr(0, found + 1);
		string destPath(outputFile);
		sprintf(fName, "adbpull_result_%d.txt", aPortNum);
		outputFile += fName;
		sprintf(fName, "cpu_details_%d.txt", aPortNum);
		destPath += fName;

		retVal = GetCPUInfo(iADBExeFullName, adbDevice, gAvailFreq, destPath, outputFile.c_str(), &minFreq, &maxFreq);
		retVal = GetCPUInfo(iADBExeFullName, adbDevice, gCurFreq, destPath, outputFile.c_str(), &currFreq);
		retVal = GetCPUInfo(iADBExeFullName, adbDevice, gCurGov, destPath, outputFile.c_str(), &currGovernors);
		retVal = GetCPUInfo(iADBExeFullName, adbDevice, gLatency, destPath, outputFile.c_str(), &tranLatency);

		if (retVal == 0) {
			//Successfully pulled the proc file that has CPU information
			retVal = UpdateCPUInfo(aPortNum, minFreq, maxFreq, currFreq, currGovernors, tranLatency);
		} else {
			retVal = -1;
		}
	}
	return retVal;
}

INT CFlashToolDialog::GetCPUInfo(const wstring& processName, const string& deviceName, const string& srcFile,
	const string& dstLocation, const CHAR* outputFileName, string* valOne, string* valTwo)
{
	INT retVal = -1;
	INT foundIndex = -1;
	ProcessHandler* pProcessHandler = new AdbPullProcessHandler(processName, deviceName, srcFile, dstLocation, outputFileName);
	if (pProcessHandler) {
		retVal = pProcessHandler->LaunchDetachedProcess();
		if (retVal == 0) {
			//Update the min and max frequencies
			ifstream cpuInfoFile(dstLocation);
			string entry;

			if (cpuInfoFile.is_open()) {
				if (getline(cpuInfoFile, entry)) {
					if (entry[entry.size() - 1] == ' ' || entry[entry.size() - 1] == '\t') {
						entry.resize(entry.size() - 1);
					}
					foundIndex = 0;
					//Read the first entry
					if (valOne) {
						foundIndex = entry.find_first_of(" \t");
						if (foundIndex == -1) {
							foundIndex = entry.size();
						}
						*valOne = entry.substr(0, foundIndex);
					}
					//Read the last entry, if expected to read
					if (valTwo && foundIndex != -1) {
						foundIndex = entry.find_last_of(" \t");
						if (foundIndex != -1) {
							*valTwo = entry.substr(foundIndex+1);
						}
					}
				}
			}
			if (foundIndex == -1) {
				//Failed to read the CPU Info
				retVal = -1;
			}
		}
		delete pProcessHandler;
		remove(dstLocation.c_str());
	}

	return retVal;
}

INT CFlashToolDialog::UpdateCPUInfo(INT aPortNum, string minFreq, string maxFreq, string currFreq,
	string currGovernors, string tranLatency)
{
	INT retVal = -1;
	CHAR cpuInfo[500];
	WCHAR wCPUInfo[500];

	sprintf(cpuInfo, "CPU Information:\nMin-Frequency: %s\nMax-Frequency: %s\nCurrent-Frequency: %s\nGovernor: %s\nLatency: %s",
		minFreq.c_str(), maxFreq.c_str(), currFreq.c_str(), currGovernors.c_str(), tranLatency.c_str());
	mbstowcs(wCPUInfo, cpuInfo, 500);

	MessageBox(NULL, wCPUInfo, L"Info", MB_OK);
	return retVal;
}
