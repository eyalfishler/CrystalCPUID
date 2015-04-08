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
#include "PciInfoDlg.h"
#include "CrystalCPUIDDlg.h"
#include "../common/GetFileVersion.h"
#include "../SysInfo/ItemID.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static _TCHAR THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CPciInfoDlg Dialog


CPciInfoDlg::CPciInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPciInfoDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CPciInfoDlg)
	m_PciDeviceId = _T("");
	m_PciClassId = _T("");
	m_PciDeviceName = _T("");
	m_PciRevision = _T("");
	m_PciBus = _T("");
	m_PciClassName = _T("");
	m_PciDevice = _T("");
	m_PciFunction = _T("");
	m_PciSubSystemId = _T("");
	m_PciSubSystemName = _T("");
	m_PciVendorId = _T("");
	m_PciVendorName = _T("");
	//}}AFX_DATA_INIT
}


void CPciInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPciInfoDlg)
	DDX_Control(pDX, IDC_PCI_LIST, m_PciList);
	DDX_Text(pDX, IDC_PCI__DEVICE_ID, m_PciDeviceId);
	DDX_Text(pDX, IDC_PCI__CLASS_ID, m_PciClassId);
	DDX_Text(pDX, IDC_PCI__DEVICE_NAME, m_PciDeviceName);
	DDX_Text(pDX, IDC_PCI__REVISION, m_PciRevision);
	DDX_Text(pDX, IDC_PCI_BUS, m_PciBus);
	DDX_Text(pDX, IDC_PCI_CLASS_NAME, m_PciClassName);
	DDX_Text(pDX, IDC_PCI_DEVICE, m_PciDevice);
	DDX_Text(pDX, IDC_PCI_FUNCTION, m_PciFunction);
	DDX_Text(pDX, IDC_PCI_SUB_SYSTEM_ID, m_PciSubSystemId);
	DDX_Text(pDX, IDC_PCI_SUB_SYSTEM_NAME, m_PciSubSystemName);
	DDX_Text(pDX, IDC_PCI_VENDOR_ID, m_PciVendorId);
	DDX_Text(pDX, IDC_PCI_VENDOR_NAME, m_PciVendorName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPciInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CPciInfoDlg)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_PCI_LIST, OnItemchangedPciList)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_COPY, OnCopy)
	ON_COMMAND(IDM_SAVE_TEXT, OnSaveText)
	ON_COMMAND(IDM_EXIT, OnExit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPciInfoDlg Message Handler

void CPciInfoDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CPciInfoDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

BOOL CPciInfoDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CPciInfoDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_PCI_INFO,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_PciInfoDlg = NULL;
}

BOOL CPciInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));

// リストビューの構築
	LVCOLUMN myColumn;
	myColumn.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_ORDER ;
	_TCHAR str[256];

	// コントロールの幅を取得
	CRect cr;m_PciList.GetClientRect(cr);
	DWORD width = cr.Width() - GetSystemMetrics(SM_CXVSCROLL);

	double comctl32 = GetFileVersion("comctl32.dll");
	if( comctl32 >= 470 ){
		long dwStyle = ListView_GetExtendedListViewStyle(m_PciList.m_hWnd);
		dwStyle |=	LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
		ListView_SetExtendedListViewStyle(m_PciList.m_hWnd, dwStyle);
	}
	if( comctl32 >= 471 ){
		m_PciList.GetHeaderCtrl()->EnableWindow(FALSE);
	}
	if( comctl32 >= 470 ){
		m_PciList.InsertColumn(0,"",LVCFMT_LEFT,0);
		m_PciList.InsertColumn(1,"Bus",LVCFMT_CENTER,width * 35 / 555);
	}else{
		m_PciList.InsertColumn(0,"Bus",LVCFMT_LEFT,width * 35 / 555);
		m_PciList.InsertColumn(1,"",LVCFMT_LEFT,0);
	}
	m_PciList.InsertColumn(2,"Dev",LVCFMT_CENTER,width * 35 / 555);
	m_PciList.InsertColumn(3,"Fnc",LVCFMT_CENTER,width * 35 / 555);
	m_PciList.InsertColumn(4,"Vendor",LVCFMT_CENTER,width * 55 / 555);
	m_PciList.InsertColumn(5,"Device",LVCFMT_CENTER,width * 55 / 555);
	m_PciList.InsertColumn(6,"Rev",LVCFMT_CENTER,width * 35 / 555);
	m_PciList.InsertColumn(7,"SubSystem",LVCFMT_CENTER,width * 80 / 555);
	m_PciList.InsertColumn(8,"Class",LVCFMT_CENTER,width * 65 / 555);
	m_PciList.InsertColumn(9,"Device Type",LVCFMT_LEFT, width * 160 / 555);
	m_PciList.InsertColumn(10,"Vendor Name",LVCFMT_LEFT,0);
	m_PciList.InsertColumn(11,"Device Name",LVCFMT_LEFT,0);
	m_PciList.InsertColumn(12,"Sub Vendor Name",LVCFMT_LEFT,0);

	DWORD data1,data2,data3,data4;
	DWORD NumberOfDevice;
	CString cstr;
	LVITEM myItem;
	memset(&myItem,0,sizeof(LVITEM));
	myItem.mask = LVIF_TEXT;

	P->SysInfo->GetData(PCI_NUMBER_OF_DEVICE, &NumberOfDevice);
	for(DWORD i=0;i < NumberOfDevice;i++){
		myItem.iItem = i;
		P->SysInfo->GetData(PCI_PCI_BUS_BASE + i,&data1);
		myItem.iSubItem = 0;wsprintf(str,"%d",data1);myItem.pszText = str;m_PciList.InsertItem(&myItem);
		myItem.iSubItem = 1;wsprintf(str,"%d",data1);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetData(PCI_PCI_DEVICE_BASE + i,&data2);
		myItem.iSubItem = 2;wsprintf(str,"%d",data2);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetData(PCI_PCI_FUNCTION_BASE + i,&data3);
		myItem.iSubItem = 3;wsprintf(str,"%d",data3);myItem.pszText = str;m_PciList.SetItem(&myItem);
		//wsprintf(str,"%1d:%2d:%1d ",data1,data2,data3);cstr += str;
		
		P->SysInfo->GetData(PCI_VENDOR_ID_BASE + i,&data1);
		myItem.iSubItem = 4;wsprintf(str,"%04Xh",data1);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetData(PCI_DEVICE_ID_BASE + i,&data2);
		myItem.iSubItem = 5;wsprintf(str,"%04Xh",data2);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetData(PCI_REVISION_ID_BASE + i,&data3);
		myItem.iSubItem = 6;wsprintf(str,"%02Xh",data3);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetData(PCI_SUBSYSTEM_ID_BASE + i,&data4);
		myItem.iSubItem = 7;wsprintf(str,"%08Xh",data4);myItem.pszText = str;m_PciList.SetItem(&myItem);
		//wsprintf(str,"%04X%04X:%02X <%08X> ",data1,data2,data3,data4);cstr += str;

		P->SysInfo->GetData(PCI_CLASS_ID_BASE + i,&data1);
		myItem.iSubItem = 8;wsprintf(str,"%06Xh",data1);myItem.pszText = str;m_PciList.SetItem(&myItem);
		//wsprintf(str,"| %06X ",data1);cstr += str;
		P->SysInfo->GetString(PCI_CLASS_NAME_BASE + i,str);
		myItem.iSubItem = 9;wsprintf(str,"%s",str);myItem.pszText = str;m_PciList.SetItem(&myItem);

		P->SysInfo->GetString(PCI_VENDOR_NAME_BASE + i,str);
		myItem.iSubItem = 10;wsprintf(str,"%s",str);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetString(PCI_DEVICE_NAME_BASE + i,str);
		myItem.iSubItem = 11;wsprintf(str,"%s",str);myItem.pszText = str;m_PciList.SetItem(&myItem);
		P->SysInfo->GetString(PCI_SUB_VENDOR_NAME_BASE + i,str);
		myItem.iSubItem = 12;wsprintf(str,"%s",str);myItem.pszText = str;m_PciList.SetItem(&myItem);
		
		//cstr += str;cstr += "\r\n";
	}
	m_hotItem = 0;
	m_NumberOfItem = NumberOfDevice;
	SetPciDetailInfo(0);
	m_PciList.SetItemState(0, LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED );

	
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

void CPciInfoDlg::SetPciDetailInfo(DWORD hotItem) 
{
	if(0 <= hotItem && hotItem < m_NumberOfItem){ 
		CString cstr;
		m_PciBus = m_PciList.GetItemText(hotItem,1);
		m_PciDevice = m_PciList.GetItemText(hotItem,2);
		m_PciFunction = m_PciList.GetItemText(hotItem,3);
		m_PciVendorId = m_PciList.GetItemText(hotItem,4);
		m_PciDeviceId = m_PciList.GetItemText(hotItem,5);
		m_PciRevision = m_PciList.GetItemText(hotItem,6);
		m_PciSubSystemId = m_PciList.GetItemText(hotItem,7);
		m_PciClassId = m_PciList.GetItemText(hotItem,8);
		m_PciClassName = " " + m_PciList.GetItemText(hotItem,9);
		m_PciVendorName = " " + m_PciList.GetItemText(hotItem,10);
		m_PciDeviceName = " " + m_PciList.GetItemText(hotItem,11);
		m_PciSubSystemName = " " + m_PciList.GetItemText(hotItem,12);
		m_PciList.SetSelectionMark(hotItem);

		UpdateData(FALSE);
	}
}

void CPciInfoDlg::OnCopy() 
{
	TextCopy( 1 /* Clipboard */ );
}

void CPciInfoDlg::OnSaveText() 
{
	TextCopy( 2 /* File */ );	
}

/////////////////////////////////////////////////////////////////////////////
// TextCopy
// type = 1 : Clipboard
// type = 2 : File
/////////////////////////////////////////////////////////////////////////////
void CPciInfoDlg::TextCopy(DWORD type)
{
	DWORD i;
	CString cstr, text, clip;
// PCI Device List
	text = "No Bus Dev Fnc VendorDeviceRev SubSystem  Class  DeviceType\r\n";
	for(i=0;i<m_NumberOfItem;i++){
		cstr.Format("%2d ",i);text += cstr;	
		cstr.Format("%3s",m_PciList.GetItemText(i,1));text += cstr + " ";
		cstr.Format("%3s",m_PciList.GetItemText(i,2));text += cstr + " ";
		cstr.Format("%3s ",m_PciList.GetItemText(i,3));text += cstr;
		cstr = m_PciList.GetItemText(i,4);text += cstr + ":";
		cstr = m_PciList.GetItemText(i,5);text += cstr + ".";
		cstr = m_PciList.GetItemText(i,6);text += cstr + " ";

		cstr = m_PciList.GetItemText(i,7);text += cstr + " ";
		cstr = m_PciList.GetItemText(i,8);text += cstr + " ";
		cstr = m_PciList.GetItemText(i,9);text += cstr + "";
		text += "\r\n";
	}
	text += "\r\n";

	text += "No DeviceName | SubSystem Vendor\r\n";
	for(i=0;i<m_NumberOfItem;i++){
		cstr.Format("%2d ",i);text += cstr;
		cstr = m_PciList.GetItemText(i,10);text += cstr + " ";
		cstr = m_PciList.GetItemText(i,11);text += cstr + "";	
		cstr = m_PciList.GetItemText(i,12);		
		if(! cstr.IsEmpty()){
			text += " | ";
			text += cstr;
			//text += "]";
		}
		text += "\r\n";
	}

clip = "\
----------------------------------------------------------------\r\n\
%PRODUCT% %VERSION% (C) 2002-2008 hiyohiyo\r\n\
                    Crystal Dew World [http://crystalmark.info/]\r\n\
----------------------------------------------------------------\r\n\
";
	clip.Replace("%PRODUCT%", CRYSTAL_CPUID_PRODUCT);
	clip.Replace("%VERSION%",P->CrystalCPUIDVersion);
	
	clip += text;
	
	switch( type ){
	case 1: // Clipboard
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			_TCHAR * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, clip.GetLength()+1);
			buffer = (_TCHAR*)GlobalLock(clipbuffer);
			strcpy(buffer, LPCSTR(clip));
			GlobalUnlock(clipbuffer);
			SetClipboardData(CF_TEXT,clipbuffer);
			CloseClipboard();
		}
		break;
	case 2: // File
		{
			CString cstr1,cstr2,cstr3;
			cstr1 = "txt";
			cstr2 = "";
			cstr3 = "text (*.txt)|*.txt|All Files (*.*)|*.*";
			CFileDialog save(FALSE,cstr1,cstr2,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,cstr3);
			#ifndef _X86_64
			FakeCFileDialog(save);
			#endif
			if( save.DoModal() == IDOK )
			{
				CStdioFile write(save.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				write.WriteString(clip);
			}
		}
		break;
	}
}

void CPciInfoDlg::OnExit() 
{
	OnClose();
}

BOOL CPciInfoDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( 0 != ::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg) )
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CPciInfoDlg::OnItemchangedPciList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->iItem >= 0)
	{
		m_hotItem = pNMListView->iItem;
		SetPciDetailInfo(m_hotItem);
	}	

	*pResult = 0;
}
