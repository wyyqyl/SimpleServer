// MyButton.cpp : implementation file
//

#include "stdafx.h"
#include "client.h"
#include "MyButton.h"


// MyButton

IMPLEMENT_DYNAMIC(MyButton, CButton)

MyButton::MyButton()
{

}

MyButton::~MyButton()
{
}


BEGIN_MESSAGE_MAP(MyButton, CButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// MyButton message handlers



extern SOCKET m_socket;

void MyButton::handleClick(char *action, UINT nFlags, CPoint point)
{
	char sendbuf[64] = { 0 };
	char recvBuf[1024] = { 0 };
	TCHAR tzTmp[32];

	strcpy_s(sendbuf, this->name);
	strcat_s(sendbuf, action);
	int iResult = send(m_socket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		wsprintf(tzTmp, TEXT("send failed with error: %d\n"), WSAGetLastError());
		MessageBox(tzTmp, TEXT("Error"), MB_OK);
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	int bytesReceived = SOCKET_ERROR;
	while (bytesReceived == SOCKET_ERROR)
		bytesReceived = recv(m_socket, recvBuf, sizeof(recvBuf), 0);

	GetParent()->GetDlgItem(IDC_STATIC_BUFFER)->SetWindowText(recvBuf);
}

void MyButton::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MyButton::handleClick(" OnLButtonDown", nFlags, point);
	CButton::OnLButtonDown(nFlags, point);
}


void MyButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MyButton::handleClick(" OnLButtonUp", nFlags, point);
	CButton::OnLButtonUp(nFlags, point);
}

//
void MyButton::setName(char *name)
{
	memcpy_s(this->name, sizeof(this->name), name, sizeof(this->name));
}