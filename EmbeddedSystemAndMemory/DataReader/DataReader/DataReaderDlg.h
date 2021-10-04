
// DataReaderDlg.h : header file
//

#pragma once

#include "UserData.h"
using namespace std;

class CDataReaderDlg;

struct ThreadData {
	CDataReaderDlg* iDataReader;
	INT iIntVal;
	DWORD iThreadId;

	ThreadData(CDataReaderDlg* pDataReader = NULL, INT val = -1) : iDataReader(pDataReader), iIntVal(val), iThreadId(0) {}
};

// CDataReaderDlg dialog
class CDataReaderDlg : public CDialogEx
{
// Construction
public:
	CDataReaderDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DATAREADER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedFileName();
	afx_msg void OnClickedBrowseFileName();
	afx_msg void OnClickedPrepareRecord();

private:
	void UpdateDataFileName(LPWSTR pFileName);
	wstring GetDataFileName();
	int CreateDataProcessingThread(wstring& fileName);

	static DWORD ReadRecordThread(LPVOID pThreadArg);
	static DWORD ProcessRecordThread(LPVOID pThreadArg);
	DWORD HandleReadRecord();
	DWORD HandleProcessRecord();
	
private:
	string iFileName;
	UserDataRecord iUserDataRecord;
	ThreadData iReadThreadData;
	ThreadData iWriteThreadData;
 };
