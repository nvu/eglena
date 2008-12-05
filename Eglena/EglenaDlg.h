// EglenaDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlg 대화 상자
class CDlg : public CDialog
{
// 생성입니다.
public:
	CDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EGLENA_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
