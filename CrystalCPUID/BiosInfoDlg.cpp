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
#include "BiosInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gLanguage;
static CCrystalCPUIDDlg* P;

#define URL_WMI_JAPANESE "http://www.microsoft.com/downloads/details.aspx?displaylang=ja&FamilyID=AFE41F46-E213-4CBF-9C5B-FBF236E0E875"
#define URL_WMI_ENGLISH  "http://www.microsoft.com/downloads/details.aspx?displaylang=en&FamilyID=AFE41F46-E213-4CBF-9C5B-FBF236E0E875"


CBiosInfoDlg::CBiosInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBiosInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CBiosInfoDlg)
	m_BiosCaption = _T("");
	m_BiosDate = _T("");
	m_BiosManufacturer = _T("");
	m_BiosVersion = _T("");
	m_MotherManufacturer = _T("");
	m_MotherProduct = _T("");
	m_MotherVersion = _T("");
	m_SystemManufacturer = _T("");
	m_SystemModel = _T("");
	m_SmBiosCaption = _T("");
	m_SmBiosVersion = _T("");
	//}}AFX_DATA_INIT
}

void CBiosInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBiosInfoDlg)
	DDX_Control(pDX, IDC_GET_WMI_MESSAGE, m_GetWmiMessage);
	DDX_Control(pDX, IDC_GET_WMI, m_GetWmi);
	DDX_Text(pDX, IDC_BIOS_CAPTION, m_BiosCaption);
	DDX_Text(pDX, IDC_BIOS_DATE, m_BiosDate);
	DDX_Text(pDX, IDC_BIOS_MANUFACTURER, m_BiosManufacturer);
	DDX_Text(pDX, IDC_BIOS_VERSION, m_BiosVersion);
	DDX_Text(pDX, IDC_MOTHER_MANUFACTURER, m_MotherManufacturer);
	DDX_Text(pDX, IDC_MOTHER_PRODUCT, m_MotherProduct);
	DDX_Text(pDX, IDC_MOTHER_VERSION, m_MotherVersion);
	DDX_Text(pDX, IDC_SYSTEM_MANUFACTURER, m_SystemManufacturer);
	DDX_Text(pDX, IDC_SYSTEM_MODEL, m_SystemModel);
	DDX_Text(pDX, IDC_SM_BIOS_CAPTION, m_SmBiosCaption);
	DDX_Text(pDX, IDC_SM_BIOS_VERSION, m_SmBiosVersion);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBiosInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CBiosInfoDlg)
	ON_BN_CLICKED(IDC_GET_WMI, OnGetWmi)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CBiosInfoDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CBiosInfoDlg::OnOK() 
{
//	CDialog::OnOK();
}

BOOL CBiosInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_BiosDate = P->m_BiosDate;
	m_BiosVersion = P->m_BiosVersion;
	m_BiosManufacturer = P->m_BiosManufacturer;
	m_BiosCaption = P->m_BiosCaption;
	m_SmBiosVersion = P->m_SmBiosVersion;
	m_SmBiosCaption = P->m_SmBiosCaption;
	m_SystemManufacturer = P->m_SystemManufacturer;
	m_SystemModel = P->m_SystemModel;
	m_MotherVersion = P->m_MotherVersion;
	m_MotherProduct = P->m_MotherProduct;
	m_MotherManufacturer = P->m_MotherManufacturer;

	m_GetWmi.EnableWindow( ! P->m_IsWMI );
	m_GetWmiMessage.EnableWindow( ! P->m_IsWMI );

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

void CBiosInfoDlg::OnGetWmi() 
{
	if(	gLanguage == JAPANESE ){
		ShellExecute(NULL,NULL,URL_WMI_JAPANESE,NULL,NULL,SW_SHOWNORMAL);
	}else{
		ShellExecute(NULL,NULL,URL_WMI_ENGLISH,NULL,NULL,SW_SHOWNORMAL);
	}
}


BOOL CBiosInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CBiosInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_BIOS_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_DmiInfoDlg = NULL;
}
