#include "stdafx.h"
#include "crystalcpuid.h"
#include "MsrWalkerDlg.h"
#include "CrystalCPUIDDlg.h"
#include <process.h>

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern _ReadMSR pReadMSR;
extern _WriteMSR pWriteMSR;

static void MsrSearch( LPVOID pParam );

/////////////////////////////////////////////////////////////////////////////
// CMsrWalkerDlg Dialog


CMsrWalkerDlg::CMsrWalkerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsrWalkerDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CMsrWalkerDlg)
	m_MsrStart = _T("");
	m_MsrEnd = _T("");
	m_MsrTime = _T("");
	m_MsrResult = _T("");
	//}}AFX_DATA_INIT
}


void CMsrWalkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsrWalkerDlg)
	DDX_Control(pDX, IDC_MSR_UPDATE_RESULT, m_MsrUpdateResult);
	DDX_Control(pDX, IDC_MSR_RESULT, m_MsrResultC);
	DDX_Control(pDX, IDC_MSR_STOP, m_MsrStop);
	DDX_Control(pDX, IDC_MSR_TIME, m_MsrTimeC);
	DDX_Control(pDX, IDC_MSR_END, m_MsrEndC);
	DDX_Control(pDX, IDC_MSR_START, m_MsrStartC);
	DDX_Control(pDX, IDC_MSR_SEARCH, m_MsrSearch);
	DDX_Text(pDX, IDC_MSR_START, m_MsrStart);
	DDV_MaxChars(pDX, m_MsrStart, 10);
	DDX_Text(pDX, IDC_MSR_END, m_MsrEnd);
	DDV_MaxChars(pDX, m_MsrEnd, 10);
	DDX_Text(pDX, IDC_MSR_TIME, m_MsrTime);
	DDX_Text(pDX, IDC_MSR_RESULT, m_MsrResult);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMsrWalkerDlg, CDialog)
	//{{AFX_MSG_MAP(CMsrWalkerDlg)
	ON_BN_CLICKED(IDC_MSR_SEARCH, OnMsrSearch)
	ON_BN_CLICKED(IDC_MSR_STOP, OnMsrStop)
	ON_BN_CLICKED(IDC_MSR_UPDATE_RESULT, OnMsrUpdateResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsrWalkerDlg Message handler

BOOL CMsrWalkerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MsrStart.Format("0x00000000");
	m_MsrEnd.Format("0x0000FFFF");

	m_MsrUpdateFlag = TRUE;
	m_MsrUpdateResult.SetCheck(TRUE);

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CMsrWalkerDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CMsrWalkerDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_MSR_WALKER,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_MsrWalkerDlg = NULL;
}

void CMsrWalkerDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CMsrWalkerDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CMsrWalkerDlg::OnMsrSearch() 
{
	m_MsrSearch.EnableWindow(FALSE);
	UpdateData(TRUE);
	DWORD temp;
	m_hThread = NULL;

	if( m_MsrStart.IsEmpty() || strstr(m_MsrStart,"0x") == NULL 
	||	m_MsrEnd.IsEmpty() || strstr(m_MsrEnd,"0x") == NULL 
	){
		MessageBox("Please input hexadecimal(HEX) style.");
		m_MsrSearch.EnableWindow(TRUE);
		return ;
	}
	m_Start = strtoul(m_MsrStart, NULL, 0);
	m_End = strtoul(m_MsrEnd, NULL, 0);

	if( m_Start > m_End ){ // swap
		temp = m_Start;
		m_Start = m_End;
		m_End = temp;
	}
	m_MsrStartC.EnableWindow(FALSE);
	m_MsrEndC.EnableWindow(FALSE);
	m_MsrResult = "";
	UpdateData(FALSE);

	m_TimeStart = timeGetTime();
	m_hThread = (unsigned long *)_beginthread(MsrSearch, 0, (LPVOID)this);
}

void CMsrWalkerDlg::OnMsrStop() 
{
	m_hThread = NULL;
}

void MsrSearch( LPVOID pParam )
{
	CMsrWalkerDlg *P = (CMsrWalkerDlg*) pParam;
	DWORD EAX, EDX;
	DWORD i=0, j=0;
	DWORD progress = 0;
	DWORD threshold = (P->m_End - P->m_Start) / 100;
	CString cstr;

	P->m_MsrStop.EnableWindow(TRUE);
	P->SetWindowText("MSR Walker - Searching Now... 0 %");
	cstr = "MSR      : 63-32    31-0\r\n";
	cstr+= "----------------------------";
	P->m_MsrResult = cstr;

	for(i=P->m_Start;i<=P->m_End;i++){

		if( pReadMSR(i, &EAX, &EDX) == 0 ){
			continue;
		}

		cstr.Format("\r\n%08X : %08X %08X", i, EDX, EAX);
		P->m_MsrResult += cstr;
		if( P->m_MsrUpdateFlag ){
			P->UpdateData(FALSE);
		}
		cstr.Format("%.3f", (timeGetTime() - P->m_TimeStart) / 1000.0);
		P->m_MsrTimeC.SetWindowText(cstr);

		j++;
		if(threshold == j){
			progress++;
			cstr.Format("MSR Walker - Searching Now... %d %%", progress);
			P->SetWindowText(cstr);
			cstr.Format("%.3f", (timeGetTime() - P->m_TimeStart) / 1000.0);
			P->m_MsrTimeC.SetWindowText(cstr);
			j=0;
		}
		if(P->m_hThread == NULL || i == 0xFFFFFFFF){
			break;
		}
	}

	P->m_MsrStartC.EnableWindow(TRUE);
	P->m_MsrEndC.EnableWindow(TRUE);
	P->m_MsrSearch.EnableWindow(TRUE);
	P->m_MsrStop.EnableWindow(FALSE);

	cstr.Format("%.3f", (timeGetTime() - P->m_TimeStart) / 1000.0);
	P->m_MsrTimeC.SetWindowText(cstr);
	P->SetWindowText("MSR Walker");
	P->UpdateData(FALSE);
}

void CMsrWalkerDlg::OnMsrUpdateResult() 
{
	m_MsrUpdateFlag = !m_MsrUpdateFlag;
	UpdateData(FALSE);
}
