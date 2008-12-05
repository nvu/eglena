// EglenaDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg ��ȭ ����
class CDlg : public CDialog
{
// �����Դϴ�.
public:
	CDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_EGLENA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CHttpClient		client;
	CListBox lstMessage;
	CProgressCtrl progressBar;
	afx_msg void OnBnClickedOk();
	HANDLE m_hThread;
	int m_nOperationStat ;
	CStringW m_sID;
	CStringW m_sPWD;
	CStringW m_sURL;
	CProgressCtrl progressSub;
	CStatic m_sStatus;
	afx_msg void OnEnChangeEdit1();
	CStatic m_sLeftTime;
	CButton m_sOK;
	afx_msg void OnClose();
};
