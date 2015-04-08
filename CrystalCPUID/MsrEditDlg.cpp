/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/
#include "stdafx.h"
#include "crystalcpuid.h"
#include "MsrEditDlg.h"
#include "CrystalCPUIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern _ReadMSR pReadMSR;
extern _WriteMSR pWriteMSR;

CMsrEditDlg::CMsrEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMsrEditDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CMsrEditDlg)
	m_AccessMsr = _T("");
	m_RdmsrEax = _T("");
	m_RdmsrEdx = _T("");
	m_WrmsrEax = _T("");
	m_WrmsrEdx = _T("");
	//}}AFX_DATA_INIT
}

void CMsrEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMsrEditDlg)
	DDX_Text(pDX, IDC_ACCESS_MSR, m_AccessMsr);
	DDV_MaxChars(pDX, m_AccessMsr, 10);
	DDX_Text(pDX, IDC_RDMSR_EAX, m_RdmsrEax);
	DDV_MaxChars(pDX, m_RdmsrEax, 10);
	DDX_Text(pDX, IDC_RDMSR_EDX, m_RdmsrEdx);
	DDV_MaxChars(pDX, m_RdmsrEdx, 10);
	DDX_Text(pDX, IDC_WRMSR_EAX, m_WrmsrEax);
	DDV_MaxChars(pDX, m_WrmsrEax, 10);
	DDX_Text(pDX, IDC_WRMSR_EDX, m_WrmsrEdx);
	DDV_MaxChars(pDX, m_WrmsrEdx, 10);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMsrEditDlg, CDialog)
	//{{AFX_MSG_MAP(CMsrEditDlg)
	ON_BN_CLICKED(IDC_RDMSR, OnRdmsr)
	ON_BN_CLICKED(IDC_WRMSR, OnWrmsr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CMsrEditDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CMsrEditDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

BOOL CMsrEditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_AccessMsr = "0x00000000";

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CMsrEditDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CMsrEditDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_MSR_EDIT,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_MsrEditDlg = NULL;
}



void CMsrEditDlg::OnRdmsr() 
{
	DWORD AccessMsr;
	DWORD EAX, EDX;

	UpdateData(TRUE);
	if( m_AccessMsr.IsEmpty() || strstr(m_AccessMsr,"0x") == NULL ){
		MessageBox("Please input hexadecimal(HEX) style.");
		return ;
	}
	AccessMsr = strtoul(m_AccessMsr, NULL, 0);
	pReadMSR(AccessMsr, &EAX, &EDX);

	m_RdmsrEax.Format("0x%08X",EAX);
	m_RdmsrEdx.Format("0x%08X",EDX);
	UpdateData(FALSE);
}

void CMsrEditDlg::OnWrmsr() 
{
	DWORD AccessMsr;
	DWORD EAX, EDX;

	UpdateData(TRUE);
	if( m_AccessMsr.IsEmpty() || strstr(m_AccessMsr,"0x") == NULL
	||  m_WrmsrEax.IsEmpty()  || strstr(m_WrmsrEax,"0x") == NULL
	||  m_WrmsrEdx.IsEmpty()  || strstr(m_WrmsrEdx,"0x") == NULL
	){
		MessageBox("Please input hexadecimal(HEX) style.");
		return ;
	}
	AccessMsr = strtoul(m_AccessMsr, NULL, 0);
	EAX = strtoul(m_WrmsrEax, NULL, 0);
	EDX = strtoul(m_WrmsrEdx, NULL, 0);
	pWriteMSR(AccessMsr, &EAX, &EDX);
/*
	CString cstr;
	cstr.Format("%08X",EAX);
	MessageBox(cstr);
*/
	UpdateData(FALSE);	
}
