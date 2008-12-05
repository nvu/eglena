// EglenaDlg.cpp : 구현 파일
//

#include "stdafx.h"

#include "Eglena.h"
#include "EglenaDlg.h"
#include <MMSystem.h>
#include <vector>
#include "../json_spirit_v2.06/json_spirit/json_spirit.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI HttpProc (LPVOID lpParameter);

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CDlg 대화 상자




CDlg::CDlg(CWnd* pParent /*=NULL*/)
: CDialog(CDlg::IDD, pParent)
, m_sID(_T(""))
, m_sPWD(_T(""))
, m_sURL(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, lstMessage);
	DDX_Control(pDX, IDC_TOTALPROGRESS, progressBar);
	DDX_Text(pDX, IDC_EDIT1, m_sID);
	DDX_Text(pDX, IDC_EDIT2, m_sPWD);
	DDX_Control(pDX, IDC_TOTALPROGRESS2, progressSub);
	DDX_Control(pDX, IDC_STATUS, m_sStatus);
	DDX_Control(pDX, IDC_TIME, m_sLeftTime);
	DDX_Control(pDX, IDOK, m_sOK);
}

BEGIN_MESSAGE_MAP(CDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &CDlg::OnEnChangeEdit1)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDlg 메시지 처리기
CDlg *pDlg = NULL;
CHttpClient* pExtraClient = NULL;
map<CStringW, CStringW> mapTitle;
map<CStringW, CStringW> mapCate;
map<CStringW, CStringW> mapDate;
vector<CStringW> vecPermal;

BOOL CDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	m_nOperationStat = 0;
	m_hThread = NULL;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDlg::OnBnClickedOk()
{
	UpdateData();
	if (m_sID.IsEmpty()) {
		MessageBox(_T("ID를 입력하세요!"));
		return;
	}

	if (m_sPWD.IsEmpty()) {
		MessageBox(_T("Password를 입력하세요!"));
		return;
	}
	m_sOK.EnableWindow(false);

	m_hThread = (HANDLE) ::_beginthreadex ( NULL, 0, (unsigned (_stdcall *) (void *)) HttpProc, (void *) this, 0, NULL);
}


DWORD HttpRead(bool isPost, CString sURL, CHttpClient* pClient, BYTE **pData)
{
	CHttpResponse*	pobjRes = NULL ;
	CHttpPostStat	objPostStat ;
	const DWORD					cbStep = 1024 ;

	if (*pData != NULL) delete [] *pData;

//	Sleep(500);
	try
	{
		if (isPost) {
			pClient->BeginPost(sURL);
		}
		else {
			pobjRes = pClient->RequestGet (sURL) ;
		}

		while ( pobjRes == NULL ) {
			pClient->Query (objPostStat) ;
			pobjRes = pClient->Proceed(cbStep);
		}

		CString		str ;
		str.Format (_T ("%u %s\r\n"), pobjRes->GetStatus (), pobjRes->GetStatusText ()) ;

		static LPCTSTR		szHeaders[] =
		{ _T ("Server"), _T ("Date"), _T ("X-Powered-By"), _T ("Content-Length"), _T ("Set-Cookie")
		, _T ("Expires"), _T ("Cache-control"), _T ("Connection"), _T ("Transfer-Encoding")
		, _T ("Content-Type"), _T ("") } ;

		/*
		LPCTSTR		szHeader ;
		for (size_t i = 0; i < sizeof (szHeaders) / sizeof (LPCTSTR); i++) {
			if ( szHeader = pobjRes->GetHeader (szHeaders[i]) ) {
				str.Format (_T ("%s: %s\r\n"), szHeaders[i], szHeader) ;
			}
		}
		pDlg->lstMessage.AddString (_T ("\r\n")) ;
		BOOL		bIsText = FALSE ;
		if ( szHeader = pobjRes->GetHeader (_T ("Content-Type")) )
			bIsText = (0 == ::_tcsncicmp (szHeader, _T ("text/"), 5)) ;

			*/
		DWORD		dwContSize ;
		if ( !pobjRes->GetContentLength (dwContSize) ) {
			dwContSize = 0 ;
		}
		else {
			pDlg->progressSub.SetRange(0, dwContSize);
		}

		const LONG dwSize = 10240;
		LONG dwBuffer = dwSize;

		BYTE pBuffer[dwSize];
		BYTE* pContainer = new BYTE[dwBuffer];
		BYTE* pPointer = pContainer;

		DWORD			dwRead ;
		size_t			cbSize = 0 ;

		while ( dwRead = pobjRes->ReadContent (pBuffer, dwSize) ) {
			pPointer += cbSize;
			CopyMemory(pPointer, pBuffer, dwRead);
			dwBuffer += dwSize;
			cbSize += dwRead;
			BYTE* pNewContainer = new BYTE[dwBuffer + 1];
			CopyMemory(pNewContainer, pContainer, cbSize);
			delete [] pContainer;
			pContainer = pNewContainer; pPointer = pContainer;
			if (dwContSize != 0) pDlg->progressSub.SetPos(cbSize);
		}
		pContainer[cbSize ] = 0;
		pContainer[cbSize + 1 ] = 0;
		//delete [] pBuffer ;
		*pData = pContainer;
		delete pobjRes ;
		return cbSize;
	}
	catch (DWORD & nOpStat) {
		delete pobjRes ;
		pClient->Cancel();

		//If the program is going to terminate,
		//if ( nOpStat & CCHttpClientDemoDlg::TERMINATING ) return 0 ;

		// canceled by user
		pDlg->lstMessage.AddString (_T ("Canceled by user.")) ;
		//::PostMessage (pobjDlg->GetSafeHwnd (), WM_THREADFINISHED, 0, 0) ;
		return 0 ;

	} catch (CHttpClient::Exception & e) {
		delete pobjRes ;
		pClient->Cancel();
		pDlg->lstMessage.AddString(e.errmsg());
		return 0 ;
	}
	delete pobjRes ;

}

void ParseAttach(CStringW permal, CStringW& contents)
{

	static DWORD ltime = timeGetTime();

	CStringW result;
	CStringW data = contents;

	int head;
	while ((head = data.Find(L"[#")) > -1) {
		bool isFound = false;
		int bottom = data.Find(L"#]");
		if (bottom > -1) {
			CStringW sFileInfo = data.Mid(head, bottom - head + 2);
			vector<CStringW> vecToken;
			int curpos = 0;
			CStringW sToken = sFileInfo.Tokenize(L"|", curpos);
			while (sToken != L"") {
				vecToken.push_back(sToken);
				sToken = sFileInfo.Tokenize(L"|", curpos);
			}
			if (vecToken[0] == L"[#IMAGE" || vecToken[1] == L"[#FILE") {
				if (vecToken.size() == 7 || vecToken.size() == 6)  {
					isFound = true;
					if (vecToken.size() == 6) vecToken.push_back(L"pds#]");
					if (vecToken[2].Mid(6,1) == L"/" && vecToken[2].Mid(9,1) == L"/" && vecToken[2].Mid(12,1) == L"/" ) {
						CStringW path = L"pds/1/" + vecToken[2];
						vecToken[2] = path;
					}
					//[#IMAGE|a0004222_4924b34133769.png|pds/200811/20/22/|mid|982|988|pds10#]
					//http://pds12.egloos.com/pds/200811/20/22/a0004222_4924b0ba20c4d.png
					CStringW sURL, sFormat;
					sURL.Format(L"http://%s.egloos.com/%s%s", vecToken[6].Left(vecToken[6].GetLength() - 2), vecToken[2], vecToken[1]);

					CStringW spath = L"attach\\" + permal;
					CreateDirectory(spath, NULL);
					BYTE* pData = NULL;
					if (!pExtraClient) {
						pExtraClient = new CHttpClient;
						pExtraClient->SetInternetUserAgent(L"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
						pExtraClient->SetUseUtf8(TRUE);
						pExtraClient->AddHeader(L"Referer", L"http://" + pDlg->m_sURL + L"/");
					}
					DWORD size = HttpRead(false, sURL, pExtraClient, &pData);

					if (size != 0) {
						CStringW sFile;
						sFile.Format(L"attach\\%s\\%s", permal, vecToken[1]);
						HANDLE hFile = CreateFile(sFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
						DWORD dw;
						WriteFile(hFile, pData, size, &dw, NULL);
						CloseHandle(hFile);
					}
					delete [] pData;

					result += data.Left(head);
					if (vecToken[0] == L"[#IMAGE") {
						sFormat.Format(L"<img src=\"attach/%s/%s\" width=\"%s\" height=\"%s\" align=\"%s\" class=\"%s\" />",
							permal, vecToken[1], vecToken[4], vecToken[5], vecToken[3], vecToken[3] );
					}
					else {
						sFormat.Format(L"<a href=\"attach/%s/%s\">%s</a>",
							permal, vecToken[1], vecToken[4], vecToken[4]);
					}
					result += sFormat;
					data = data.Mid(bottom + 2);
					if (ltime + 1000 * 60 * 1 < timeGetTime()) {
						delete pExtraClient;
						pExtraClient = NULL;
					}
				}
			}
			if (!isFound) {
				result += data.Left(bottom + 2);
				data = data.Mid(bottom + 2);
			}
		}
		if (!isFound) break;
	}
	result += data;

	result.Replace(L"\r\n", L"<br /><!--rn-->");
	result.Replace(pDlg->m_sURL + L"/", L"./");

	contents = result;
}

CStringW GetTagWord(CStringW& source, const CStringW wTagOpen, const CStringW wTagClose)
{
	CStringW result = L"";
	int phaseA = source.Find(wTagOpen);
	if (phaseA >= 0) {
		result = source.Mid(phaseA + wTagOpen.GetLength());
		int phaseB = result.Find(wTagClose);
		if (phaseB >= 0) {
			result = result.Left(phaseB);
		}
		else {
			result = L"";
		}
	}

	return result;
}


CStringW A2U(char * pData) {
	int buffersize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1,NULL, 0);
	WCHAR* wBuffer = new WCHAR[buffersize];

	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1, wBuffer, buffersize);
	CStringW wData = wBuffer; delete [] wBuffer;
	return wData;
}

void AddLog(CStringW sMsg)
{
	pDlg->m_sStatus.SetWindowText(sMsg);
	pDlg->lstMessage.AddString(sMsg);
	pDlg->lstMessage.SendMessage(WM_VSCROLL, SB_BOTTOM);
}


void ConvertRes(CStringW& wComment)
{
	CStringW wKey[] = {
		L"<img src=\"http://md.egloos.com/img/eg/ico_comment.gif\" width=\"9\" height=\"9\" alt=\"\" border=\"0\" /></a> Commented  by ",
		L"<img src=\"http://md.egloos.com/img/eg/post_security3.gif\" width=\"9\" height=\"10\" border=\"0\" /></a> Commented  by ",
		L"<img src=\"http://md.egloos.com/img/eg/icon_depth.gif\" width=\"11\" height=\"8\" title=\"\" border=\"0\" /></a> Commented  by ",
		L"<img src=\"http://md.egloos.com/img/eg/ico_comment.gif\" width=\"9\" height=\"9\" alt=\"\" border=\"0\" /></a> Linked at  ",
		L"<img src=\"http://md.egloos.com/img/eg/ico_comment.gif\" width=\"9\" height=\"9\" alt=\"\" border=\"0\" /></a> Tracked from  ",
	};
	int iHead[5];
	CStringW wKeyB = L" <a href=\"#\" onclick=\"del";
	CStringW wKeyC = L" style=\"margin:0px;width:100%;overflow:hidden;word-break:break-all;\">";

	CStringW wParse;
	while (1) {

		int type, head, bottom;
		for (type = 0; type < 5; ++type) {
			iHead[type] = wComment.Find(wKey[type]);
		}
		head = wComment.GetLength();
		for (int i = 0; i < 5; ++i) {
			if (iHead[i] != -1 && head > iHead[i]) {
				head = iHead[i];
				type = i;
			}
		}
		if (head == wComment.GetLength()) break;

		if (head < 0) break;
		bottom = wComment.Find(wKeyB);
		if (bottom < 0) break;
		switch (type) {
				case 0: wParse += L"<dl class=\"res\">◇ "; break;
				case 1: wParse += L"<dl class=\"res-hidden\">♡ ";  break;
				case 2: wParse += L"<dl class=\"res-res\">└ ";  break;
				case 3: wParse += L"<dl class=\"ping\">§ Linked at  ";  break;
				case 4: wParse += L"<dl class=\"ping\">§ Tracked from  ";  break;
		}
		wParse += wComment.Mid(head + wKey[type].GetLength(), bottom - head - wKey[type].GetLength());
		wComment = wComment.Mid(bottom + wKeyB.GetLength());
		wParse += L"<dd>" + GetTagWord(wComment, wKeyC, L"</div>");
		wParse += L"</dd></dl>";
	}
	if (wParse != L"") wComment = wParse ;
}
CStringW GetResData(CStringA spID, CStringW permal)
{
	BYTE* pData = NULL;
	CStringW sURL;
	CStringW wData, wTrack, wPing, wComment;

	for (int i = 0; i < 3; ++i) {
		int page = 1;
		while (1) {
			bool isRepeat = false;
			switch (i) {
				case 0:
					sURL.Format(L"http://%s/egloo_trackback.php?eid=%S&srl=%s&xhtml=0", pDlg->m_sURL, spID, permal);
					break;
				case 1:
					sURL.Format(L"http://%s/egloo_pingback.php?eid=%S&srl=%s&xhtml=0", pDlg->m_sURL, spID, permal);
					break;
				case 2:
					sURL.Format(L"http://%s/egloo_comment.php?eid=%S&srl=%s&xhtml=0&page=%d", pDlg->m_sURL, spID, permal, page);
					break;

			}

			while (1) {
				DWORD size = HttpRead(false, sURL, &pDlg->client, &pData);
				int buffersize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1,NULL, 0);
				WCHAR* wBuffer = new WCHAR[buffersize];
				MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1, wBuffer, buffersize);
				wData = wBuffer; delete [] wBuffer;
				if (wData.Find(L"</html>") > 0) break;
			}
			wData.Replace(L"\t", L"");
			wData.Replace(L"\\r\\n", L"\r\n");
			wData.Replace(L"\\\"", L"\"");
			wData.Replace(L"\\'", L"'");
			switch (i) {
				case 0:
					wTrack = GetTagWord(wData, L"<div id=\"trackbacks\" name=\"trackbacks\" class=\"comment_line\"></div>\r\n", L"</div>\";");
					break;
				case 1:
					wPing = GetTagWord(wData, L"\" class=\"COMMENT_TAIL\">\r\n", L"</div>\"");
					wPing += GetTagWord(wData, L"\" class=\"comment_tail\">\r\n", L"</div>\"");
					break;
				case 2:
					wComment = GetTagWord(wData, L"\" class=\"COMMENT_TAIL\">\r\n", L"</div>\"") + wComment;
					wComment = GetTagWord(wData, L"\" class=\"comment_tail\">\r\n", L"</div>\"") + wComment;
					if (wData.Find(L"onclick=\"cmtview_more(") != -1 &&
						wData.Find(L", this); return false;\">이전 덧글 ") != -1) {
							isRepeat = true;
					}
			}
			if (i < 2 || !isRepeat) break;
			++page;
		}

	}
	delete [] pData;

	ConvertRes(wTrack);
	ConvertRes(wPing);
	ConvertRes(wComment);

	return wTrack + wPing + wComment;
}


static DWORD WINAPI HttpProc (LPVOID lpParameter)
{
	pDlg = static_cast<CDlg*> (lpParameter) ;

	mapTitle.clear();
	mapCate.clear();
	mapDate.clear();
	vecPermal.clear();

	CString sMessage;


	AddLog(L"로그인합니다.");

	pDlg->client.SetInternetUserAgent(L"Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.1; SV1)");
	pDlg->client.SetUseUtf8(TRUE);

	pDlg->client.AddParam(L"security", L"on");
	pDlg->client.AddParam(L"userid", pDlg->m_sID );
	pDlg->client.AddParam(L"userpwd", pDlg->m_sPWD);

	BYTE* pData = NULL;
	CString 	sURL ;
	{
		sURL = L"https://www.egloos.com/login/sauthid.php";

		HttpRead(true, sURL, &pDlg->client, &pData); // login
		pDlg->client.ClearParam();
		HttpRead(false, L"http://www.egloos.com/", &pDlg->client, &pData); // login

		CStringW buffer = A2U((CHAR*)pData);
		pDlg->m_sURL = GetTagWord(buffer, L"		<li class=\"start\"><a href=\"http://", L".egloos.com\">내이글루</a></li>");
		if (pDlg->m_sURL == L"") {
			delete [] pData;
			pDlg->MessageBox(_T("ID/패스워드를 확인해주세요."));
			return 0;

		}
		CreateDirectory(pDlg->m_sURL, NULL);
		SetCurrentDirectory(pDlg->m_sURL);
		CreateDirectory(L"attach", NULL);
		pDlg->m_sURL += L".egloos.com";
	}

	//// 이제 고유 ID를 얻어보자.

	pDlg->client.ClearParam();
	pDlg->client.AddHeader(L"Referer", L"http://" + pDlg->m_sURL + L"/");
	sURL = L"http://" + pDlg->m_sURL;
	HttpRead(false, sURL, &pDlg->client, &pData); // goto the homepage
	CStringA sData = (char*)pData;
	CStringA sTag = "<a href=\"http://www.egloos.com/egloo/insert.php?eid=";
	int phaseA = sData.Find(sTag);
	if (phaseA < 0) {
		delete [] pData;
		pDlg->MessageBox(_T("ID/패스워드를 확인해주세요."));
		return 0;
	}
	CStringA spID = sData.Mid(phaseA + sTag.GetLength());
	spID = spID.Left(spID.Find("\">"));



	FILE *fp = fopen("index.html", "rb");
	if (fp) {
		int phase = 0;
		AddLog(L"로컬 데이터를 불러옵니다.");
		char temp[8192];
		while (1) {
			if (fgets(temp, 8192, fp) == NULL) break;
			CStringW sData = A2U(temp);
			CStringW permal, date, cate;
			if (sData.Left(4) == "<li>") {
				permal = GetTagWord(sData, L" | <a href=\"", L".html\">");
				mapTitle[permal] = GetTagWord(sData, L".html\">", L"</a> | <small>");
				mapDate[permal] = GetTagWord(sData, L"<li>", L" |");
				mapCate[permal] = GetTagWord(sData, L"</a> | <small>", L"</small>");
				++phase;
			}
		}
		fclose(fp);
		sMessage.Format(L"%d개의 글을 확인했습니다.", phase);
		AddLog(sMessage);
	}


	// 글 목록을 읽어오자
	AddLog(L"글 목록을 읽어옵니다...");
	int page = 0;
	int count = 0;
	while (true) {
		bool isWord = false;
		++page;
		pDlg->progressBar.SetPos(page);
		sURL.Format(L"http://www.egloos.com/adm/post/chgpost_info.php?eid=%S&pagecount=50&pg=%d", spID, page);
		CStringW wData;
		while (1) {

			DWORD size = HttpRead(false, sURL, &pDlg->client, &pData);

			int buffersize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1,NULL, 0);
			WCHAR* wBuffer = new WCHAR[buffersize];

			MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1, wBuffer, buffersize);
			wData = wBuffer; delete [] wBuffer;
			if (wData.Find(L"</html>") > 0) break;
			Sleep(1500);
		}
		if (page == 1) {
			CStringW sCount = GetTagWord(wData, L" valign=\"middle\"> 전체 - <b>", L"</b>");
			count = _wtoi(sCount);
			count = count / 50 + count % 50 == 0 ? 0 : 1;
			pDlg->progressBar.SetRange(0, count);
		}
		bool isFound = false;
		while (true) {

			CStringW wTag(L"<a href=\"http://" + pDlg->m_sURL + L"/");
			CStringW wTagClose("\" title=\"");

			phaseA = wData.Find(wTag);
			if (phaseA < 0) break;
			CStringW wData2 = wData.Mid(phaseA + wTag.GetLength());
			CStringW wURL(wData2.Left(wData2.Find(wTagClose)));
			vecPermal.push_back(wURL);
			wData = wData.Mid(phaseA + wURL.GetLength());
			isFound = true;
		}
		if (!isFound) break;
	}
	// 목록 다 뽑아왔당.


	// 이제 update 모드로 들어가서 데이터를 퍼보자.

	sMessage.Format(L"총 %d 개의 게시물 확인.", vecPermal.size());
	AddLog(sMessage);

	pDlg->progressBar.SetRange(0, vecPermal.size() - 1);

	DWORD beginTime = timeGetTime();
	for (int i = 0; i < vecPermal.size(); ++i) {
		CStringW& permal = vecPermal[i];
		int length = WideCharToMultiByte(CP_ACP, 0, permal, permal.GetLength(), NULL, 0, "?", NULL);
		char *filename = new char[length + 10]; ZeroMemory(filename, length + 10);
		WideCharToMultiByte(CP_ACP, 0, vecPermal[i], vecPermal[i].GetLength(), filename, length + 1,  "?", NULL);
		strcat(filename, ".html");


		pDlg->progressBar.SetPos(i);
		FILE *fp = fopen(filename, "rb");
		if (fp && mapTitle.find(permal) != mapTitle.end()) {
			fclose(fp);
			sMessage = L"http://" + pDlg->m_sURL + L"/" + vecPermal[i] + L" 패스!";
			AddLog(sMessage);
		}
		else {
			if (fp) fclose(fp); fp = NULL;
			sMessage = L"http://" + pDlg->m_sURL + L"/" + vecPermal[i] + L" 처리중 ...";
			AddLog(sMessage);
			CStringW wData;
			vector<CStringW> vecToken;

			sURL.Format(L"http://www.egloos.com/egloo/update.php?eid=%S&srl=%s", spID, vecPermal[i]);
			while (1) {
				while (1) {
					DWORD size = HttpRead(false, sURL, &pDlg->client, &pData);
					int buffersize = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1,NULL, 0);
					WCHAR* wBuffer = new WCHAR[buffersize];
					MultiByteToWideChar(CP_UTF8, 0, (LPCSTR) pData, -1, wBuffer, buffersize);
					wData = wBuffer; delete [] wBuffer;
					if (wData.Find(L"</html>") > 0) break;
					Sleep(500);
				}

				int curpos = 0;
				CStringW sToken = wData.Tokenize(L"\n", curpos);
				int idx  = 0;
				vecToken.clear();
				while (sToken != L"") {
					vecToken.push_back(sToken);
					sToken.Format(L"%d %s", idx, sToken);
					/*
					OutputDebugString(sToken);
					OutputDebugString(L"\n");
					*/
					sToken = wData.Tokenize(L"\n", curpos);
					++idx;
				}
				if (vecToken.size() > 300)  break;
				Sleep(500);
			}
			if (pDlg->m_nOperationStat != 0) break;

			CStringW wTag;
			CStringW wTagClose;
			CStringW wData2, wTitle, wTagData, wCategory, wDate, wJSON ;
			bool isPub = true;

			// 타이틀파싱
			wTitle = GetTagWord(vecToken[35], L"'}], contents, \"", L"\", \"");
			if (wTitle == L"") {
				sMessage.Format(L"타이틀 분석에 실패했습니다. ");
				pDlg->lstMessage.AddString(sMessage);
				break;
			}
			mapTitle[permal] = wTitle;
			pDlg->m_sStatus.SetWindowText(wTitle);

			// isPub
			CStringW wPub = GetTagWord(wData, L"		<input type=\"hidden\" name=\"old_openflag\" value=\"",
				L"\" >");
			if (wPub == L"3") {
				mapTitle[permal] = L"<font color=red>[비공개] </font>" + wTitle;
				wTitle = mapTitle[permal];
			}


			// tag
			wTagData = GetTagWord(wData, L"		<input type=\"hidden\" name=\"old_posttag\" value=\"", L"\" >");


			// category
			wCategory = GetTagWord(wData, L"		<input type=\"hidden\" name=\"cgname\" value=\"", L"\">");
			mapCate[permal] = wCategory;

			// contents
			wJSON = L"{'" + GetTagWord(wData, L"		var contents ={'", L"'};\n") + L"'}";

			// date
			wDate = GetTagWord(wData, L"		<input type=\"hidden\" name=\"wrtdate\" value=\"", L"\" >");
			mapDate[permal] = wDate;

			json_spirit::wValue json_data;
			std::wstring wdata = wJSON;
			json_spirit::read(wdata, json_data);
			json_spirit::wObject baseobj = json_data.get_obj();
			wstring contents, moresubject, morecontents;

			for (int j = 0; j < baseobj.size(); ++j) {
				json_spirit::wPair wp = baseobj[j];
				if (wp.name_ == L"contents") contents = wp.value_.get_str();
				if (wp.name_ == L"moreSubject") moresubject = wp.value_.get_str();
				if (wp.name_ == L"moreContents") morecontents = wp.value_.get_str();
			}

			// 첨부파일 읽기.
			CStringW sContents(contents.c_str());
			CStringW sMoreSubject(moresubject.c_str());
			CStringW sMoreContents(morecontents.c_str());

			if (pDlg->m_nOperationStat != 0) break;
			ParseAttach(vecPermal[i], sContents);
			if (pDlg->m_nOperationStat != 0) break;

			ParseAttach(vecPermal[i], sMoreSubject);
			if (pDlg->m_nOperationStat != 0) break;

			ParseAttach(vecPermal[i], sMoreContents);
			if (pDlg->m_nOperationStat != 0) break;

			if (!sMoreContents.IsEmpty()) {
				CStringW sMore;
				sMore.Format(L"<p class=\"more\">%s</p><p>%s</p>", sMoreSubject, sMoreContents);
				sMoreContents = sMore;
			}

			CStringW sData;
			sData.Format(L"<html><head><title>%s</title><style>BODY{font-size:80%%;}</style><link rel=\"stylesheet\" type=\"text/css\" href=\"style.css\" /></head><body><h2>%s</h2><p>%s | %s</p><hr /><p class=\"contents\">%s</p>%s<hr /><p class=\"tag\"><small>%s</small></p><p class=\"feedback\">%s</p></body></html>",
				wTitle,
				wTitle,
				wCategory,
				wDate,
				sContents,
				sMoreContents,
				wTagData,
				GetResData(spID, vecPermal[i]));

			length = WideCharToMultiByte(CP_UTF8, 0, sData, sData.GetLength(), NULL, 0, NULL, NULL);
			char *p = new char[length + 2];p[length + 1] = 0;
			WideCharToMultiByte(CP_UTF8, 0, sData, sData.GetLength(), p, length , NULL, NULL);

			fp = fopen(filename, "wb");
			fwrite(p, length, 1, fp);
			fclose(fp);
			delete [] p;
		}

		delete [] filename;
//		break;

		DWORD elapsed = (timeGetTime() - beginTime) / 1000;
		DWORD delay = (elapsed ) * (vecPermal.size() ) / (i + 1) - (elapsed );
		sMessage.Format(L"%d초 | %d초쯤남았나?", elapsed, delay);
		pDlg->m_sLeftTime.SetWindowText(sMessage);
		if (pDlg->m_nOperationStat != 0) break;
	}
	pDlg->m_sLeftTime.SetWindowText(L"");
	{
		AddLog(L"인덱스 생성중 ...");

		FILE *fp = fopen("index.html", "wb");
		char p[] = "<html><body><ol>\r\n";
		char q[] = "</ol></body></html>\r\n";
		fwrite(&p, sizeof(p), 1, fp);

		for (int i = 0; i < vecPermal.size(); ++i) {
			CStringW& permal = vecPermal[i];
			CStringW sInfo;
			sInfo.Format(L"<li>%s | <a href=\"%s.html\">%s</a> | <small>%s</small>\r\n", mapDate[permal], permal, mapTitle[permal], mapCate[permal]);
			int length = WideCharToMultiByte(CP_UTF8, 0, sInfo, sInfo.GetLength(), NULL, 0, NULL, NULL);
			char *p = new char[length + 2];p[length + 1] = 0;
			WideCharToMultiByte(CP_UTF8, 0, sInfo, sInfo.GetLength(), p, length , NULL, NULL);
			fwrite(p, length, 1, fp);
			delete [] p;

		}
		fwrite(&q, sizeof(q), 1, fp);
		fclose(fp);
	}

	AddLog(L"작업을 종료합니다.");
	if (pDlg->m_nOperationStat == 0) pDlg->MessageBox(L"잘 끝난 것 같아요!");
	delete [] pData;
	delete pExtraClient;
	pDlg->SendMessage(WM_CLOSE);
	return 0;
}
void CDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDlg::OnClose()
{
	m_nOperationStat |= 1;
	// TODO: Add your message handler code here and/or call default
	if ( m_hThread ) {
		DWORD		dwRtn = ::WaitForSingleObject (m_hThread, 0) ;

		if ( dwRtn == WAIT_TIMEOUT ) {
			// Try again
			::Sleep (0) ;
			PostMessage (WM_CLOSE) ;
			return ;
		}

		if ( dwRtn == WAIT_OBJECT_0 ) ::CloseHandle (m_hThread) ;
		m_hThread = NULL ;
	}

	CDialog::OnClose();
}
