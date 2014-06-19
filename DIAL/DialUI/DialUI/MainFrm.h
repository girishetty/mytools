
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include <winsock2.h>

class CServerList;

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:
	int DialCommsSetup();
	void UpdateSeverList(CServerList* pServerList);
	void UpdateSeverNum(int index);

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

private:
	int m_sock; 
	int m_dial_client_sock;
	HANDLE  m_threadHndl;
	DWORD m_threadId;
	bool m_bConnected;

private:
	inline void StartWinSock() {
		/* Structure for WinSock setup communication */
		WSADATA wsaData;

		/* Load Winsock 2.0 DLL */
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)  {
			printf("WSAStartup() failed!!!!");
			exit(1);
		}
	}

	inline void CleanWinSock() {
		/* Cleanup Winsock */
		closesocket(m_sock);
		closesocket(m_dial_client_sock);
		WSACleanup();
	}

protected:
	int StartDialClientComms();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnListServers();
	afx_msg void OnListActions();
	DECLARE_MESSAGE_MAP()

};


