
// clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "clientDlg.h"
#include "MyButton.h"
#include "afxdialogex.h"
#include <WinSock2.h>

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEFAULT_PORT 27015

SOCKET m_socket = INVALID_SOCKET;

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


// CclientDlg dialog



CclientDlg::CclientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CclientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_btn_down.setName("m_btn_down");
	m_btn_left.setName("m_btn_left");
	m_btn_right.setName("m_btn_right");
	m_btn_up.setName("m_btn_up");
}

void CclientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_DOWN, m_btn_down);
	DDX_Control(pDX, IDC_BTN_LEFT, m_btn_left);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btn_right);
	DDX_Control(pDX, IDC_BTN_UP, m_btn_up);
	DDX_Control(pDX, IDC_STATIC_BUFFER, m_static_buffer);
}

BEGIN_MESSAGE_MAP(CclientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CclientDlg message handlers

BOOL CclientDlg::OnInitDialog()
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
	if (!initConn())
	{
		MessageBox(TEXT("Can't connect to the server\n"), TEXT("Error"), MB_OK);
		EndDialog(-1);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CclientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CclientDlg::OnPaint()
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
HCURSOR CclientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL initConn()
{
	//initialize Winsock
	WSADATA wsaData;
	TCHAR tzTmp[64];

	int iRet = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if(iRet != NO_ERROR)
	{
		MessageBox(NULL, TEXT("Error at WSAStartup()\n"), TEXT("Error"), MB_OK);
		return FALSE;
	}

	//create a socket

	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_socket == INVALID_SOCKET)
	{
		wsprintf(tzTmp, TEXT("Error at socket(): %ld\n"), WSAGetLastError());
		MessageBox(NULL, tzTmp, TEXT("Error"), MB_OK);
		WSACleanup();
		return FALSE;
	}

	//----------------------
	// The sockaddr_in structure specifies the address family,
	// IP address, and port of the server to be connected to.
	struct sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	clientService.sin_port = htons( DEFAULT_PORT );

	//----------------------
	// Connect to server.
	iRet = connect(m_socket, (SOCKADDR *)&clientService, sizeof(clientService));
	if (iRet == SOCKET_ERROR) {
		wsprintf(tzTmp, TEXT("connect failed with error: %d\n"), WSAGetLastError());
		closesocket(m_socket);
		WSACleanup();
		return FALSE;
	}

	return TRUE;
}

void EndConn()
{
	TCHAR tzTmp[32];

	// shutdown the connection since no more data will be sent
	int iResult = shutdown(m_socket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		wsprintf(tzTmp, TEXT("shutdown failed with error: %d\n"), WSAGetLastError());
		MessageBox(NULL, tzTmp, TEXT("Error"), MB_OK);
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	// close the socket
	iResult = closesocket(m_socket);
	if (iResult == SOCKET_ERROR) {
		wsprintf(tzTmp, TEXT("close failed with error: %d\n"), WSAGetLastError());
		MessageBox(NULL, tzTmp, TEXT("Error"), MB_OK);
		WSACleanup();
		return;
	}

	WSACleanup();
}