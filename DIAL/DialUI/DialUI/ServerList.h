#pragma once
#include "afxwin.h"

// CServerList dialog
class CMainFrame;

class CServerList : public CDialogEx
{
	DECLARE_DYNAMIC(CServerList)

public:
	CServerList(CMainFrame* pMainFrame, CWnd* pParent = NULL);
	virtual ~CServerList();
	virtual BOOL OnInitDialog();
	void UpdateServerList(const char* pName, const char* pIP);

// Dialog Data
	enum { IDD = ID_DLG_LIST_SERVER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

private:
	CMainFrame* m_pMainFrame;
	int m_listIndex;
	CWnd* m_wndList;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLbnSelchangeListName();
	afx_msg void OnBnClickedRefresh();
};
