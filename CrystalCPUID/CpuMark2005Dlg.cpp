// CpuMark2005Dlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "crystalcpuid.h"
#include "CrystalCPUIDDlg.h"
#include "CpuMark2005Dlg.h"
#include <process.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern int gLanguage;

int ExecAndWait(char *pszCmd, BOOL bNoWindow);

/////////////////////////////////////////////////////////////////////////////
// CCpuMark2005Dlg ダイアログ

CCpuMark2005Dlg::CCpuMark2005Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCpuMark2005Dlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CCpuMark2005Dlg)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_DATA_INIT
}


void CCpuMark2005Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCpuMark2005Dlg)
	DDX_Control(pDX, IDC_MARK_CPU_CLOCK, m_MarkCpuClock);
	DDX_Control(pDX, IDC_MARK_64, m_Mark64);
	DDX_Control(pDX, IDC_MARK_32, m_Mark32);
	DDX_Control(pDX, IDC_MARK_CPU_INFO, m_MarkCpuInfo);
	DDX_Control(pDX, IDC_MARK_SCORE_64, m_MarkScore64);
	DDX_Control(pDX, IDC_MARK_SCORE_32, m_MarkScore32);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCpuMark2005Dlg, CDialog)
	//{{AFX_MSG_MAP(CCpuMark2005Dlg)
	ON_BN_CLICKED(IDC_MARK_32, OnMark32)
	ON_BN_CLICKED(IDC_MARK_64, OnMark64)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCpuMark2005Dlg メッセージ ハンドラ

void RayTrace32(void *thread)
{
	CCpuMark2005Dlg *p = (CCpuMark2005Dlg*)thread;
	int result;
	if(result = ExecAndWait("RayTrace32.exe", TRUE) >= 0){
		p->m_MarkScore32.SetWidth(result);
	}else{
		p->m_MarkScore32.SetWidth(0);
	}

	p->m_Mark32.EnableWindow(TRUE);
	p->m_Mark64.EnableWindow(TRUE);
}

void CCpuMark2005Dlg::OnMark32() 
{

	m_Mark32.EnableWindow(FALSE);
	m_Mark64.EnableWindow(FALSE);

	_beginthread(RayTrace32,0,this);
	
}

void CCpuMark2005Dlg::OnMark64() 
{
	int result;

	m_Mark32.EnableWindow(FALSE);
	m_Mark64.EnableWindow(FALSE);
	if(result = ExecAndWait("RayTrace64.exe", TRUE) >= 0){
		m_MarkScore64.SetWidth(result);
	}else{
		m_MarkScore64.SetWidth(0);
	}
	m_Mark32.EnableWindow(TRUE);
	m_Mark64.EnableWindow(TRUE);	
}

void CCpuMark2005Dlg::OnOK() 
{
//	CDialog::OnOK();
}

BOOL CCpuMark2005Dlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CCpuMark2005Dlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CCpuMark2005Dlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CPU_MARK_2005,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_CpuMark2005Dlg = NULL;
}

BOOL CCpuMark2005Dlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	RECT rc;

/*
	char str[256];
	GetPrivateProfileString("Setting","OtherFontFace","MS UI Gothic",str,256,P->m_ini);
	m_MarkScore64.SetFontFace(str);
*/
	m_MarkScore32.GetWindowRect(&rc);
	m_MarkScore32.SetFontSize( (rc.bottom - rc.top) * 7 );
	m_MarkScore64.GetWindowRect(&rc);
	m_MarkScore64.SetFontSize( (rc.bottom - rc.top) * 7 );

	m_MarkScore32.SetMaxWidth(10000);
	m_MarkScore64.SetMaxWidth(10000);
	m_MarkScore32.SetWidth(7777);
	m_MarkScore64.SetWidth(9999);

	P->Refresh();
	if( P->m_CodeName.IsEmpty() ){
		cstr.Format("%s", P->m_CPUName);
	}else{
		cstr.Format("%s (%s)", 	P->m_CPUName, P->m_CodeName);
	}
	m_MarkCpuInfo.SetWindowText(cstr);

	if( P->m_SystemClock.IsEmpty() ){
		cstr.Format("%sMHz x %s", P->m_Clock, P->m_CPUNumber); 
	}else{
		cstr.Format("%s(%sx%s)MHz x %s", P->m_Clock, P->m_SystemClock, P->m_Multiplier,  P->m_CPUNumber); 
	}
	m_MarkCpuClock.SetWindowText(cstr);

	CenterWindow();
	ShowWindow(SW_SHOW);


	return TRUE;
}

int ExecAndWait(char *pszCmd, BOOL bNoWindow)
{
	DWORD Code;
	BOOL bSuccess;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	if (bNoWindow) {
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	char* ptrEnd;
	char path[MAX_PATH];
	::GetModuleFileName(NULL, path, MAX_PATH);
	if ( (ptrEnd = strrchr(path, '\\')) != NULL ){
		*ptrEnd = '\0';
	}


	bSuccess = CreateProcess(
		NULL,	// lpApplicationName
		pszCmd,	// lpCommandLine
		NULL,	// lpProcessAttributes
		NULL,	// lpThreadAttributes
		FALSE,	// bInheritHandle
		0,		// dwCreationFlag
		NULL,	// lpEnvironment
		path,	// lpCurrentDirectory
		&si,	// lpStartupInfo
		&pi		// lpProcessInformation
	);
	if (bSuccess != TRUE) return -1;

	WaitForInputIdle(pi.hProcess, INFINITE);
	WaitForSingleObject(pi.hProcess, INFINITE);

	GetExitCodeProcess(pi.hProcess, &Code);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

	return Code;
}
