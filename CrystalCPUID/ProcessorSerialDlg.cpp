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
#include "CrystalCPUIDDlg.h"
#include "ProcessorSerialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CProcessorSerialDlg Dialog

CProcessorSerialDlg::CProcessorSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessorSerialDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CProcessorSerialDlg)
	m_ProcessorSerial = _T("");
	//}}AFX_DATA_INIT
}

void CProcessorSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcessorSerialDlg)
	DDX_Text(pDX, IDC_PROCESSOR_SERIAL, m_ProcessorSerial);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcessorSerialDlg, CDialog)
	//{{AFX_MSG_MAP(CProcessorSerialDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcessorSerialDlg Message Handler

void CProcessorSerialDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CProcessorSerialDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

BOOL CProcessorSerialDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	char str[256];
	m_ProcessorSerial = P->SysInfo->GetInfo(CPU_PROCESSOR_SERIAL,str);
	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CProcessorSerialDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CProcessorSerialDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_PROCESSOR_SERIAL,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_ProcessorSerialDlg = NULL;
}
