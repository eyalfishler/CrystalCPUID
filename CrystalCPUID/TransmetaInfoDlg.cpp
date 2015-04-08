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
#include "TransmetaInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

CTransmetaInfoDlg::CTransmetaInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTransmetaInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;

	//{{AFX_DATA_INIT(CTransmetaInfoDlg)
	m_TmClock = _T("");
	m_TmNominalClock = _T("");
	m_TmNameString = _T("");
	m_TmCurrentGateDelay = _T("");
	m_TmCurrentPerformance = _T("");
	m_TmCurrentVoltage = _T("");
	m_TmHardwareVersion = _T("");
	m_TmSoftwareVersion = _T("");
	//}}AFX_DATA_INIT
}

void CTransmetaInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransmetaInfoDlg)
	DDX_Text(pDX, IDC_TM_CLOCK, m_TmClock);
	DDX_Text(pDX, IDC_TM_NOMINAL_CLOCK, m_TmNominalClock);
	DDX_Text(pDX, IDC_TM_NAME_STRING, m_TmNameString);
	DDX_Text(pDX, IDC_TM_CURRENT_GATE_DELAY, m_TmCurrentGateDelay);
	DDX_Text(pDX, IDC_TM_CURRENT_PERFORMANCE, m_TmCurrentPerformance);
	DDX_Text(pDX, IDC_TM_CURRENT_VOLTAGE, m_TmCurrentVoltage);
	DDX_Text(pDX, IDC_TM_HARDWARE_VERSION, m_TmHardwareVersion);
	DDX_Text(pDX, IDC_TM_SOFTWARE_VERSION, m_TmSoftwareVersion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTransmetaInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CTransmetaInfoDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_TM_REFRESH, OnTmRefresh)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CTransmetaInfoDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CTransmetaInfoDlg::OnOK() 
{
//	CDialog::OnOK();
}

BOOL CTransmetaInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnTmRefresh();
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

void CTransmetaInfoDlg::OnTmRefresh() 
{
// for Transmeta
	char str[256];
	DWORD data;
//	P->SysInfo->GetData(CPU_UPDATE_BASE + P->m_Mask,(DWORD*)&data);

	P->SysInfo->GetData(CPU_TM_UPDATE,&data);
	P->SysInfo->GetString(CPU_TM_CLOCK,str);					m_TmClock = str;
	P->SysInfo->GetString(CPU_TM_NOMINAL_CLOCK,str);			m_TmNominalClock = str;
	P->SysInfo->GetString(CPU_TM_CURRENT_VOLTAGE,str);			m_TmCurrentVoltage = str;
	P->SysInfo->GetString(CPU_TM_CURRENT_PERFORMANCE,str);		m_TmCurrentPerformance = str;
	P->SysInfo->GetString(CPU_TM_CURRENT_GATE_DELAY,str);		m_TmCurrentGateDelay = str;
	P->SysInfo->GetString(CPU_TM_HARDWARE_VERSION,str);			m_TmHardwareVersion = str;
	P->SysInfo->GetString(CPU_TM_SOFTWARE_VERSION,str);			m_TmSoftwareVersion = str;
	P->SysInfo->GetString(CPU_TM_NAME_STRING,str);				m_TmNameString = str;

	UpdateData(FALSE);
}

BOOL CTransmetaInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CTransmetaInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_TRANSMETA_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_TransmetaInfoDlg = NULL;
}

