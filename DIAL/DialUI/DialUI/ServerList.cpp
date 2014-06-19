// ServerList.cpp : implementation file
//

#include "stdafx.h"
#include "DialUI.h"
#include "ServerList.h"
#include "MainFrm.h"
#include "afxdialogex.h"


// CServerList dialog

IMPLEMENT_DYNAMIC(CServerList, CDialogEx)

CServerList::CServerList(CMainFrame* pMainFrame, CWnd* pParent)
	: CDialogEx(CServerList::IDD, pParent),
	  m_pMainFrame(pMainFrame),
	  m_listIndex(0), m_wndList(NULL) {
}

CServerList::~CServerList()
{
}

void CServerList::DoDataExchange(CDataExchange* pDX)
{
	//CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_LIST_NAME, mListBox);
}

BEGIN_MESSAGE_MAP(CServerList, CDialogEx)
	ON_BN_CLICKED(IDOK, &CServerList::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CServerList::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_NAME, &CServerList::OnLbnSelchangeListName)
	ON_BN_CLICKED(IDC_BUTTON1, &CServerList::OnBnClickedRefresh)
END_MESSAGE_MAP()


// CServerList message handlers

BOOL CServerList::OnInitDialog() {
	m_wndList = GetDlgItem(IDC_LIST_NAME);
	return CDialog::OnInitDialog();
}

void CServerList::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


void CServerList::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
	
}


void CServerList::OnLbnSelchangeListName() {
	// TODO: Add your control notification handler code here
	CListBox* pLB = (CListBox*) m_wndList;
	int index = pLB->GetCurSel();
	m_pMainFrame->UpdateSeverNum(index);
}

#define SERVER_INFO_LEN 100

void CServerList::UpdateServerList(const char* pName, const char* pIP) {
	char serverInfo[SERVER_INFO_LEN];
	wchar_t w_serverInfo[SERVER_INFO_LEN];

	strcpy(serverInfo, "Name: ");
	strcat(serverInfo, pName);
	strcat(serverInfo, " - IP [");
	strcat(serverInfo, pIP);
	strcat(serverInfo, "]");

	int len = MultiByteToWideChar(CP_ACP, 0, serverInfo, -1, w_serverInfo, SERVER_INFO_LEN);
	if (len < 0) {
		len = 0;
	}else if(len >= SERVER_INFO_LEN) {
		//truncate it
		len = SERVER_INFO_LEN - 1;
	}

	//Make sure we are NULL terminating it
    w_serverInfo[len] = 0;

	int pos = m_wndList->SendMessageW(LB_ADDSTRING, 0, (LPARAM)w_serverInfo);
	m_wndList->SendMessage(LB_SETITEMDATA, pos, (LPARAM) m_listIndex++); 
}

void CServerList::OnBnClickedRefresh()
{
	// TODO: Add your control notification handler code here
	m_pMainFrame->UpdateSeverList(this);
    m_wndList->SetFocus(); 
}
