
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "DialUI.h"
#include "ServerList.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI dial_client_comms_thread(LPVOID pParam) {
	CMainFrame* pMainFrame = (CMainFrame*)pParam;
	return pMainFrame->DialCommsSetup();
}

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_LIST_SERVERS, &CMainFrame::OnListServers)
	ON_COMMAND(ID_ACTIONS, &CMainFrame::OnListActions)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_bConnected(false) {
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
	CloseHandle(m_threadHndl);
	CleanWinSock();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	//Start a communication Channel with the Dial Client
	StartDialClientComms();

	return 0;
}

int CMainFrame::StartDialClientComms() {
	int ret = 0;

	m_threadHndl = CreateThread(NULL, 0, dial_client_comms_thread, this, 0, &m_threadId);
	if(m_threadHndl == NULL) {
		printf("Error creating the thread!");
		ret = -1;
	}

    return ret;
}

int CMainFrame::DialCommsSetup() {
	//Lets act as a Server for this communication
	struct sockaddr_in service_addr;
	struct sockaddr_in dial_client_addr;
	int namelength;

	int dial_client_port_no = 7789;
	int opt=1;
	int retval = 0;
	printf("Socket creation FAILED\n");
	StartWinSock();

	/* Create a reliable, stream socket using TCP */
	if ((m_sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		printf("Socket creation FAILED\n");
		return m_sock;
	}

	/* Construct the server address structure */
	memset(&service_addr, 0, sizeof(service_addr));				/* Zero out structure */
	service_addr.sin_family      = AF_INET;						/* Internet address family */
	service_addr.sin_addr.s_addr = inet_addr("127.0.0.1");		/* Loopback address */
	service_addr.sin_port        = htons(dial_client_port_no);  /* Server port */

	if (setsockopt(m_sock, SOL_SOCKET, SO_REUSEADDR,(const char *)&opt,sizeof opt)<0){
		printf ("setsockopt (SO_RESUSEADDR): %s\r\n",strerror(errno));
		exit(EXIT_FAILURE);
	}

	retval = bind (m_sock, (struct sockaddr*)&service_addr, sizeof(service_addr));
	if (retval) 	{
		perror("bind");
		exit(EXIT_FAILURE);
	}

	/*
	* Listen on socket 5 for connections.
	*/
	retval = listen (m_sock, 5);
	if (retval) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	/* Establish the connection to the server */
	namelength = sizeof(dial_client_addr);
	m_dial_client_sock = accept (m_sock, (struct sockaddr*)&dial_client_addr, &namelength);

	if(m_dial_client_sock == -1) {
		printf("accept() failed with error[%d]\n", errno);
		perror("accept error");
		retval = -1;
	} else {
		m_bConnected = true;
	}
	
	return retval; 
}

void CMainFrame::UpdateSeverList(CServerList* pServerList) {
	//Now start receiving Name:IP pair from the server until we receive "end"
	char* name = NULL;
	char* ipAddr = NULL;
	char serverInfo[70];
	int len=0;

	//Make sure we have established the connection with the Dial Client
	while(!m_bConnected) {
		Sleep(0);
	}

	do{
		len = recv(m_dial_client_sock, serverInfo, 70, 0);
		send(m_dial_client_sock, "ok", 2, 0);
		serverInfo[len]='\0';
		name = strtok(serverInfo, ":");
		if(!name || strlen(name) > 50){
			printf("CMainFrame::UpdateSeverList: Invalid Name!!\n");
			break;
		}else if(strcmp(serverInfo, "end") == 0) {
			//Loop exit condition
			break;
		}

		ipAddr = strtok(NULL, ":");
		if(!ipAddr || strlen(ipAddr) > 16){
			printf("CMainFrame::UpdateSeverList: Invalid IP!!\n");
			break;
		}
		pServerList->UpdateServerList(name, ipAddr);

	}while(true);
}

void CMainFrame::UpdateSeverNum(int index) {
	char server_num[5];
	_itoa(index, server_num, 10);
	server_num[4]='\0';
	send(m_dial_client_sock, server_num, 5, 0);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// App command to run the dialog
void CMainFrame::OnListServers()
{
	CServerList servDlg(this);
	servDlg.DoModal();
}

// App command to run the dialog
void CMainFrame::OnListActions()
{
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

