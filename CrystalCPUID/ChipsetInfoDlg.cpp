/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CrystalCPUID.h"
#include "ChipsetInfoDlg.h"
#include "CrystalCPUIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CChipsetInfoDlg 


CChipsetInfoDlg::CChipsetInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CChipsetInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CChipsetInfoDlg)
	m_Chipset = _T("");
	m_NorthBridge = _T("");
	m_SouthBridge = _T("");
	m_VideoChip = _T("");
	m_VideoChipId = _T("");
	m_NorthBridgeId = _T("");
	m_SouthBridgeId = _T("");
	//}}AFX_DATA_INIT
}


void CChipsetInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChipsetInfoDlg)
	DDX_Text(pDX, IDC_CHIPSET, m_Chipset);
	DDX_Text(pDX, IDC_NORTH_BRIDGE, m_NorthBridge);
	DDX_Text(pDX, IDC_SOUTH_BRIDGE, m_SouthBridge);
	DDX_Text(pDX, IDC_VIDEO_CHIP, m_VideoChip);
	DDX_Text(pDX, IDC_VIDEO_CHIP_ID, m_VideoChipId);
	DDX_Text(pDX, IDC_NORTH_BRIDGE_ID, m_NorthBridgeId);
	DDX_Text(pDX, IDC_SOUTH_BRIDGE_ID, m_SouthBridgeId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChipsetInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CChipsetInfoDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChipsetInfoDlg Message Handler

void CChipsetInfoDlg::OnOK() 
{	
//	CDialog::OnOK();
}

void CChipsetInfoDlg::OnCancel() 
{	
//	CDialog::OnCancel();
	OnClose();
}

BOOL CChipsetInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CChipsetInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CHIPSET_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_ChipsetInfoDlg = NULL;
}

BOOL CChipsetInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Chipset = P->m_Chipset;
	m_NorthBridge = P->m_NorthBridge;
	m_SouthBridge = P->m_SouthBridge;
	m_VideoChip = P->m_VideoChip;
	m_NorthBridgeId = P->m_NorthBridgeId;
	m_SouthBridgeId = P->m_SouthBridgeId;
	m_VideoChipId = P->m_VideoChipId;

	UpdateData(FALSE);

	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}
