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
#include "DmiInfoDlg.h"
#include "CrystalCPUIDDlg.h"
#include "../SysInfo/ItemID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CDmiInfoDlg Dialog

CDmiInfoDlg::CDmiInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmiInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CDmiInfoDlg)
	m_DmiBiosReleaseDate = _T("");
	m_DmiBiosRomSize = _T("");
	m_DmiBiosVendor = _T("");
	m_DmiBiosVersion = _T("");
	m_DmiCpuCurrentClock = _T("");
	m_DmiCpuExternalClock = _T("");
	m_DmiCpuManufacturer = _T("");
	m_DmiCpuMaxClock = _T("");
	m_DmiCpuSocket = _T("");
	m_DmiCpuVersion = _T("");
	m_DmiVersion = _T("");
	m_DmiMotherManufacturer = _T("");
	m_DmiMotherProduct = _T("");
	m_DmiMotherVersion = _T("");
	m_QueryPerformance = _T("");
	//}}AFX_DATA_INIT
}


void CDmiInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDmiInfoDlg)
	DDX_Text(pDX, IDC_DMI_BIOS_RELEASE_DATE, m_DmiBiosReleaseDate);
	DDX_Text(pDX, IDC_DMI_BIOS_ROM_SIZE, m_DmiBiosRomSize);
	DDX_Text(pDX, IDC_DMI_BIOS_VENDOR, m_DmiBiosVendor);
	DDX_Text(pDX, IDC_DMI_BIOS_VERSION, m_DmiBiosVersion);
	DDX_Text(pDX, IDC_DMI_CPU_CURRENT_CLOCK, m_DmiCpuCurrentClock);
	DDX_Text(pDX, IDC_DMI_CPU_EXTERNAL_CLOCK, m_DmiCpuExternalClock);
	DDX_Text(pDX, IDC_DMI_CPU_MANUFACTURER, m_DmiCpuManufacturer);
	DDX_Text(pDX, IDC_DMI_CPU_MAX_CLOCK, m_DmiCpuMaxClock);
	DDX_Text(pDX, IDC_DMI_CPU_SOCKET, m_DmiCpuSocket);
	DDX_Text(pDX, IDC_DMI_CPU_VERSION, m_DmiCpuVersion);
	DDX_Text(pDX, IDC_DMI_VERSION, m_DmiVersion);
	DDX_Text(pDX, IDC_DMI_MOTHER_MANUFACTURER, m_DmiMotherManufacturer);
	DDX_Text(pDX, IDC_DMI_MOTHER_PRODUCT, m_DmiMotherProduct);
	DDX_Text(pDX, IDC_DMI_MOTHER_VERSION, m_DmiMotherVersion);
	DDX_Text(pDX, IDC_QUERY_PERFORMANCE, m_QueryPerformance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDmiInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CDmiInfoDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDmiInfoDlg Message Handler

void CDmiInfoDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CDmiInfoDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

BOOL CDmiInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_DmiVersion = P->m_DmiVersion;
	m_DmiBiosVendor = P->m_DmiBiosVendor;
	m_DmiBiosVersion = P->m_DmiBiosVersion;
	m_DmiBiosReleaseDate = P->m_DmiBiosReleaseDate;
	if( ! P->m_DmiBiosRomSize.IsEmpty() ){
		m_DmiBiosRomSize = P->m_DmiBiosRomSize + "KB";
	}
	m_DmiMotherManufacturer = P->m_DmiMotherManufacturer;
	m_DmiMotherProduct = P->m_DmiMotherProduct;
	m_DmiMotherVersion = P->m_DmiMotherVersion;
	m_DmiCpuSocket = P->m_DmiCpuSocket;
	m_DmiCpuManufacturer = P->m_DmiCpuManufacturer;
	m_DmiCpuVersion = P->m_DmiCpuVersion;
	m_DmiCpuCurrentClock = P->m_DmiCpuCurrentClock;
	m_DmiCpuExternalClock = P->m_DmiCpuExternalClock;
	m_DmiCpuMaxClock = P->m_DmiCpuMaxClock;

	LARGE_INTEGER QPF;
	QueryPerformanceFrequency(&QPF);
	m_QueryPerformance.Format("%f",QPF.QuadPart / 1000.0 / 1000.0);

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CDmiInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CDmiInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_DMI_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_DmiInfoDlg = NULL;
}
