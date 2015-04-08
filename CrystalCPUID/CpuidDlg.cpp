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
#include "CrystalCPUIDDlg.h"
#include "CpuidDlg.h"
#include "../common/GetFileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

extern void GetCPUID(DWORD id,DWORD* A,DWORD* B,DWORD* C,DWORD* D);

/////////////////////////////////////////////////////////////////////////////
// CCpuidDlg Dialog


CCpuidDlg::CCpuidDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCpuidDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CCpuidDlg)
	//}}AFX_DATA_INIT
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_count = 0;
}


void CCpuidDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCpuidDlg)
	DDX_Control(pDX, IDC_CPUID_LIST, m_CpuidList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCpuidDlg, CDialog)
	//{{AFX_MSG_MAP(CCpuidDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCpuidDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CCpuidDlg::OnOK() 
{
//	CDialog::OnOK();
}

BOOL CCpuidDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);
	
// Init List View
	LVCOLUMN myColumn;
	myColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER ;
	// Get Cntrol Width
	CRect cr;
	m_CpuidList.GetClientRect(cr);
	DWORD width = cr.Width() - GetSystemMetrics(SM_CXVSCROLL);

	// Version Check "comctl32.dll"
	double comctl32 = GetFileVersion("comctl32.dll");
	if( comctl32 >= 470 ){
		long dwStyle = ListView_GetExtendedListViewStyle(m_CpuidList.m_hWnd);
		dwStyle |=	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(m_CpuidList.m_hWnd, dwStyle);
	}
	if( comctl32 >= 471 ){
		m_CpuidList.GetHeaderCtrl()->EnableWindow(FALSE);
	}

	while(m_CpuidList.DeleteColumn(0)){}
	m_CpuidList.DeleteAllItems();
	m_CpuidList.InsertColumn(0,"CPUID",LVCFMT_CENTER,width / 5,0);
	m_CpuidList.InsertColumn(1,"EAX",LVCFMT_LEFT,width / 5,0);
	m_CpuidList.InsertColumn(2,"EBX",LVCFMT_LEFT,width / 5,0);
	m_CpuidList.InsertColumn(3,"ECX",LVCFMT_LEFT,width / 5,0);
	m_CpuidList.InsertColumn(4,"EDX",LVCFMT_LEFT,width / 5,0);

	LVITEM myItem;
	memset(&myItem,0,sizeof(LVITEM));
	myItem.mask = LVIF_TEXT;

	CString cstr;
	DWORD i=0x00;
	DWORD dwEAX,dwEBX,dwECX,dwEDX;

	GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	DWORD MaxCPUID = dwEAX;
	InsertCpuidInfo(i,dwEAX,dwEBX,dwECX,dwEDX);
	for(i=1;i<=MaxCPUID;i++){
		GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
		InsertCpuidInfo(i,dwEAX,dwEBX,dwECX,dwEDX);
	}

	GetCPUID(0x80000000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0x80000000){
		DWORD MaxCPUIDEx = dwEAX;
		for(i=0x80000000;i<=MaxCPUIDEx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			InsertCpuidInfo(i,dwEAX,dwEBX,dwECX,dwEDX);
		}
	}

	GetCPUID(0x80860000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0x80860000){
		DWORD MaxCPUIDTx = dwEAX;
		for(i=0x80860000;i<=MaxCPUIDTx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			InsertCpuidInfo(i,dwEAX,dwEBX,dwECX,dwEDX);
		}
	}

	GetCPUID(0xC0000000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0xC0000000){
		DWORD MaxCPUIDCx = dwEAX;
		for(i=0xC0000000;i<=MaxCPUIDCx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			InsertCpuidInfo(i,dwEAX,dwEBX,dwECX,dwEDX);
		}
	}

	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Insert CPUID Info
/////////////////////////////////////////////////////////////////////////////
void CCpuidDlg::InsertCpuidInfo(DWORD id,DWORD A,DWORD B,DWORD C,DWORD D)
{
	CString str;
	str.Format("%08Xh",id);	m_CpuidList.InsertItem(m_count,str);
	str.Format("%08Xh",A);		m_CpuidList.SetItemText(m_count,1,str);
	str.Format("%08Xh",B);		m_CpuidList.SetItemText(m_count,2,str);
	str.Format("%08Xh",C);		m_CpuidList.SetItemText(m_count,3,str);
	str.Format("%08Xh",D);		m_CpuidList.SetItemText(m_count,4,str);
	m_count++;
}

/////////////////////////////////////////////////////////////////////////////
// Get CPUID Info
/////////////////////////////////////////////////////////////////////////////
/*
static void GetCPUID(DWORD id,DWORD* A,DWORD* B,DWORD* C,DWORD* D)
{
	DWORD dwEAX,dwEBX,dwECX,dwEDX;
	_asm{
		mov eax,id
		cpuid
		mov dwEAX , eax
		mov dwEBX , ebx	
		mov dwECX , ecx
		mov dwEDX , edx
	}
	*A = dwEAX;
	*B = dwEBX;
	*C = dwECX;
	*D = dwEDX;
}
*/


BOOL CCpuidDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CCpuidDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CPUID_DETAIL,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_CpuidDlg = NULL;
}
