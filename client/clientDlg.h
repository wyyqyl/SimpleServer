
// clientDlg.h : header file
//

#pragma once
#include "MyButton.h"
#include "afxwin.h"

// CclientDlg dialog
class CclientDlg : public CDialogEx
{
// Construction
public:
	CclientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CLIENT_DIALOG };

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
	MyButton m_btn_down;
	MyButton m_btn_left;
	MyButton m_btn_right;
	MyButton m_btn_up;
	CStatic m_static_buffer;
};

BOOL initConn();
void EndConn();