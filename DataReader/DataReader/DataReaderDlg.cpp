
// DataReaderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataReader.h"
#include "DataReaderDlg.h"
#include "afxdialogex.h"
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SUCCESS 1

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

LARGE_INTEGER GetFileSize(wstring fileName)
{
	LARGE_INTEGER nLargeInteger = { 0 };
	HANDLE hFile = CreateFile(fileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, NULL);
	if (hFile != INVALID_HANDLE_VALUE) {
		GetFileSizeEx(hFile, &nLargeInteger);
	}

	return nLargeInteger;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataReaderDlg dialog

CDataReaderDlg::CDataReaderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDataReaderDlg::IDD, pParent), iReadThreadData(this), iWriteThreadData(this)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataReaderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDataReaderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_STN_CLICKED(IDC_FILENAME, &CDataReaderDlg::OnClickedFileName)
	ON_STN_CLICKED(IDC_BROWSE, &CDataReaderDlg::OnClickedBrowseFileName)
	ON_STN_CLICKED(IDC_PREPARE, &CDataReaderDlg::OnClickedPrepareRecord)
END_MESSAGE_MAP()


// CDataReaderDlg message handlers

BOOL CDataReaderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataReaderDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataReaderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataReaderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CDataReaderDlg::OnClickedFileName()
{

}

void CDataReaderDlg::OnClickedBrowseFileName()
{
	//Setup the FileOpen Dialog Box
	OPENFILENAME fileOpen;
	WCHAR fileName[_MAX_DIR + _MAX_FNAME] = L"\0";

	memset(fileName, 0, sizeof(WCHAR) * _MAX_DIR + _MAX_FNAME);
	memset(&fileOpen, 0, sizeof(fileOpen));

	fileOpen.lStructSize = sizeof(fileOpen);
	fileOpen.hwndOwner = m_hWnd;
	fileOpen.hInstance = NULL;
	fileOpen.lpstrFile = fileName;
	fileOpen.nFilterIndex = 0;
	fileOpen.nMaxFile = 256;
	fileOpen.lpstrInitialDir = NULL;
	fileOpen.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&fileOpen)) {
		UpdateDataFileName(fileOpen.lpstrFile);
	}
}

void CDataReaderDlg::OnClickedPrepareRecord()
{
	wstring fileName = GetDataFileName();
	int expectedWaitTime = CreateDataProcessingThread(fileName);
}

void CDataReaderDlg::UpdateDataFileName(LPWSTR pFileName)
{
	CWnd* wFileName = GetDlgItem(IDC_FILENAME);
	if (wFileName) {
		wFileName->SetWindowText(pFileName);
		//SetDlgItemText(IDC_FILENAME, pFileName);
	}
}

wstring CDataReaderDlg::GetDataFileName()
{
	wstring sFileName;
	WCHAR fileName[_MAX_FNAME];
	CWnd* wFileName = GetDlgItem(IDC_FILENAME);
	if (wFileName) {
		int ret = wFileName->GetWindowText(fileName, _MAX_FNAME);

		if (ret) {
			sFileName = fileName;
			char temp[_MAX_FNAME];
			size_t length = 0;
			wcstombs_s(&length, temp, _MAX_FNAME, sFileName.c_str(), _MAX_FNAME);
			iFileName = temp;
		}
	}

	return sFileName;
}

int CDataReaderDlg::CreateDataProcessingThread(wstring& fileName)
{
	int retVal = SUCCESS;
	int expectedTime = 100;
	HANDLE readThreadID = NULL;
	HANDLE writeThreadID = NULL;

#ifdef PROFILING
	Profiling Profile;
	__int64 counterStart = Profile.StartCounter();
#endif 

	readThreadID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ReadRecordThread, &iReadThreadData, 0, &iReadThreadData.iThreadId);
	if (NULL == readThreadID) {
		//Creation of thread failed, terminate the loop
		retVal = -1;
	} else {
		writeThreadID = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&ProcessRecordThread, &iWriteThreadData, 0, &iWriteThreadData.iThreadId);
		if (NULL == writeThreadID) {
			//Creation of thread failed, terminate the loop
			retVal = -1;
		}
	}

	if (retVal == SUCCESS) {
		LARGE_INTEGER fileSize = GetFileSize(fileName);
		expectedTime = fileSize.QuadPart / 410000;
		WCHAR infoMsg[_MAX_FNAME];
		wsprintf(infoMsg, L"Started Processing %s\nIt may take %d seconds", fileName.c_str(), expectedTime);
		MessageBox(infoMsg, L"Info", MB_OK);

		//Now wait for the threads to complete
		WaitForSingleObject(readThreadID, INFINITE);
		CloseHandle(readThreadID);
		WaitForSingleObject(writeThreadID, INFINITE);
		CloseHandle(writeThreadID);

#ifdef PROFILING
		int timeTaken = Profile.GetCounter(counterStart);
		wsprintf(infoMsg, L"Took %d milliseconds", timeTaken);
		MessageBox(infoMsg, L"Notification", MB_OK);
#endif
	}
	else {
		expectedTime = -1;
		MessageBox(L"Thread Creation Failed!", L"ERROR", ERROR);
	}

	return expectedTime;
}

DWORD CDataReaderDlg::ReadRecordThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData && pThreadData->iDataReader) {
		retVal = pThreadData->iDataReader->HandleReadRecord();
	}

	return retVal;
}

DWORD CDataReaderDlg::ProcessRecordThread(LPVOID pThreadArg)
{
	DWORD retVal = -1;
	ThreadData* pThreadData = (ThreadData*)pThreadArg;
	if (pThreadData && pThreadData->iDataReader) {
		retVal = pThreadData->iDataReader->HandleProcessRecord();
	}

	return retVal;
}

DWORD CDataReaderDlg::HandleReadRecord()
{
	INT retVal = SUCCESS;
	string entry;
	ifstream recordFile(iFileName);

	//Reset Record List
	iUserDataRecord.Reset();

	if (recordFile.is_open()) {

		while (getline(recordFile, entry)) {
			iUserDataRecord.ProcessUserData(entry);
		}
		recordFile.close();
	} else {
		retVal = -1;
	}

	iUserDataRecord.FinishedWriting();
	return retVal;
}

void UserDataRecord::ProcessUserData(string& anEntry)
{
	int index = 0;
	UserData userData;
	Tokenizer tokenizer(anEntry, " \t,;");
	string price;

	userData.iName = tokenizer.GetToken();

	if (!userData.iName.empty()) {
		userData.iLocation = tokenizer.GetNextToken();
		userData.iItem = tokenizer.GetNextToken();
		price = tokenizer.GetNextToken();
		userData.iPrice = atof(price.c_str());

		Write(userData);
	}
}

DWORD CDataReaderDlg::HandleProcessRecord()
{
	INT retVal = SUCCESS;
	string entry;
	int found = iFileName.find_last_of("/\\");
	string outputFileName = iFileName.substr(0, found + 1);
	outputFileName += "user_record_data.txt";
	ofstream outputFile(outputFileName);
	UserData userData;

	if (outputFile.is_open()) {
		while (!iUserDataRecord.ReadDone()) {
			userData = iUserDataRecord.Read();
			if (!userData.iName.empty()) {
				outputFile << userData.iName << "\t" << userData.iItem << "\t" << userData.iPrice << endl;
			}
		}
		outputFile.close();
	}
	return retVal;
}
