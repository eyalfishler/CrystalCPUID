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
#include "CacheInfoDlg.h"
#include "CrystalCPUIDDlg.h"
#include "../common/GetFileVersion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CCacheInfoDlg 

CCacheInfoDlg::CCacheInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCacheInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CCacheInfoDlg)
	//}}AFX_DATA_INIT
}


void CCacheInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCacheInfoDlg)
	DDX_Control(pDX, IDC_CACHE_LIST, m_CacheList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCacheInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CCacheInfoDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCacheInfoDlg

BOOL CCacheInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LVCOLUMN myColumn;
	myColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER ;
	CRect cr;m_CacheList.GetClientRect(cr);
	DWORD width = cr.Width() - GetSystemMetrics(SM_CXVSCROLL);

	double comctl32 = GetFileVersion("comctl32.dll");
	if( comctl32 >= 470 ){
		long dwStyle = ListView_GetExtendedListViewStyle(m_CacheList.m_hWnd);
		dwStyle |=	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(m_CacheList.m_hWnd, dwStyle);
	}
	if( comctl32 >= 471 ){
		m_CacheList.GetHeaderCtrl()->EnableWindow(FALSE);
	}

	m_CacheList.DeleteAllItems();
	m_CacheList.InsertColumn(0,"Cache",LVCFMT_CENTER,width * 5 / 20,0);
	m_CacheList.InsertColumn(1,"Information",LVCFMT_LEFT,width * 15 / 20,0);

	LVITEM myItem;
	memset(&myItem,0,sizeof(LVITEM));
	myItem.mask = LVIF_TEXT;
	char str[256],str1[256],str2[256];
	int data;
	
	P->SysInfo->GetData(CPU_CACHE_L1ITU_WAYS,(DWORD*)&data);
	if(data > 0){
		myItem.iItem = 0;
		myItem.iSubItem = 0;
		myItem.pszText = (char*)(const char*)P->m_CacheL1ITU;
		m_CacheList.InsertItem(&myItem);
		myItem.iSubItem = 1;
		P->SysInfo->GetString(CPU_CACHE_L1ITU_WAYS,str1);
		P->SysInfo->GetString(CPU_CACHE_L1ITU_LINES,str2);
		if(atoi(str2) > 0){
			sprintf(str,"%s %s, %s way set associative, %s byte line size.",P->m_L1ITUCache,P->m_CacheL1ITUUnit,str1,str2);
		}else{
			sprintf(str,"%s %s, %s way set associative.",P->m_L1ITUCache,P->m_CacheL1ITUUnit,str1);
		}
		myItem.pszText = str;
		m_CacheList.SetItem(&myItem);
	}
	P->SysInfo->GetData(CPU_CACHE_L1D_WAYS,(DWORD*)&data);
	if(data > 0){
		myItem.iItem = 1;
		myItem.iSubItem = 0;
		myItem.pszText = "L1 D-Cache";
		m_CacheList.InsertItem(&myItem);
		myItem.iSubItem = 1;
		P->SysInfo->GetString(CPU_CACHE_L1D_WAYS,str1);
		P->SysInfo->GetString(CPU_CACHE_L1D_LINES,str2);
		sprintf(str,"%s KB, %s way set associative, %s byte line size.",P->m_L1DataCache,str1,str2);
		myItem.pszText = str;
		m_CacheList.SetItem(&myItem);
	}
	P->SysInfo->GetData(CPU_CACHE_L2_WAYS,(DWORD*)&data);
	if(data > 0){
		myItem.iItem = 2;
		myItem.iSubItem = 0;
		myItem.pszText = "L2 Cache";
		m_CacheList.InsertItem(&myItem);
		myItem.iSubItem = 1;
		P->SysInfo->GetString(CPU_CACHE_L2_WAYS,str1);
		P->SysInfo->GetString(CPU_CACHE_L2_LINES,str2);
		sprintf(str,"%s KB, %s way set associative, %s byte line size.",P->m_L2Cache,str1,str2);
		myItem.pszText = str;
		m_CacheList.SetItem(&myItem);
	}
	P->SysInfo->GetData(CPU_CACHE_L3_WAYS,(DWORD*)&data);
	if( data > 0){
		myItem.iItem = 3;
		myItem.iSubItem = 0;
		myItem.pszText = "L3 Cache";
		m_CacheList.InsertItem(&myItem);
		myItem.iSubItem = 1;
		P->SysInfo->GetString(CPU_CACHE_L3_WAYS,str1);
		P->SysInfo->GetString(CPU_CACHE_L3_LINES,str2);
		sprintf(str,"%s KB, %s way set associative, %s byte line size.",P->m_L3Cache,str1,str2);
		myItem.pszText = str;
		m_CacheList.SetItem(&myItem);
	}

	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CCacheInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CCacheInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CACHE_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_CacheInfoDlg = NULL;
}

void CCacheInfoDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CCacheInfoDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}
