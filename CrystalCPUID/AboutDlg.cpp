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
#include "AboutDlg.h"
#include "../SysInfo/CpuInfoID.h"
#include "LcdSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg 

CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_MAIN, m_Main);
	DDX_Control(pDX, IDC_SUB, m_Sub);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

void CAboutDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CAboutDlg::OnCancel() 
{	
//	CDialog::OnCancel();
	OnClose();
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	char SysInfoDate[16];
	P->SysInfo->GetString(SI_DATE,SysInfoDate);

#ifdef _X86_64
	cstr.Format("\
Author      %24s\
Modified    %24s\
Version     %24s\
CrystalCPUIDx64.exe       %10s\
SysInfoX64.dll            %10s\
    Copyright (C) 2002-2008 hiyohiyo\
",
CRYSTAL_CPUID_AUTHOR,
CRYSTAL_CPUID_MODIFIED,
P->CrystalCPUIDVersion,
CRYSTAL_CPUID_DATE,
SysInfoDate
);
#else
	cstr.Format("\
Author      %24s\
Modified    %24s\
Version     %24s\
CrystalCPUID.exe          %10s\
SysInfo.dll               %10s\
    Copyright (C) 2002-2008 hiyohiyo\
",
CRYSTAL_CPUID_AUTHOR,
CRYSTAL_CPUID_MODIFIED,
P->CrystalCPUIDVersion,
CRYSTAL_CPUID_DATE,
SysInfoDate
);
#endif
//            The modified BSD license

	m_Main.SetText(cstr);
	m_Main.SetNumberOfLines(6);
	m_Main.SetXCharsPerLine(36);
	m_Main.SetSize(CMatrixStatic::TINY);

	char str[256];
	DWORD LcdGridRgb,  LcdOnRgb, LcdOffRgb;
	GetPrivateProfileString("Setting","LcdGrid",DEFAULT_LCD_GRID,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		LcdGridRgb = atoi(str);
	}else{
		LcdGridRgb = atoi(DEFAULT_LCD_GRID);
	}

	GetPrivateProfileString("Setting","LcdOn",DEFAULT_LCD_ON,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		LcdOnRgb = atoi(str);
	}else{
		LcdOnRgb = atoi(DEFAULT_LCD_ON);
	}

	GetPrivateProfileString("Setting","LcdOff",DEFAULT_LCD_OFF,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		LcdOffRgb = atoi(str);
	}else{
		LcdOffRgb = atoi(DEFAULT_LCD_OFF);
	}
	m_Main.SetDisplayColors( LcdGridRgb, LcdOnRgb, LcdOffRgb );


	m_Main.AdjustClientYToSize(6);
	m_Main.AdjustClientXToSize(36);
	m_Main.SetAutoPadding(true);
//	m_Main.DoScroll(500, CMatrixStatic::RIGHT);

	m_Sub.SetText("\
[ Special Thanks ]                  \
 Alex Simonov (patch)               \
 KACH (idea)                        \
 NujiNuji (advice)                  \
 Magnum (test)                      \
 ita (info)                         \
 To (advice/patch)                  \
 bMAO (icons)                       \
 DULL (test/patch)                  \
 nejimaki (patch)                   \
                                    \
 Morf (SysID)                       \
 Cappuccino (CCPUID)                \
 H.Oda! (WCPUID)                    \
 Franck Delattre (CPU-Z)            \
 Petr Koc (CPUMSR)                  \
 Hideki EIRAKU (cpu%.exe)           \
                                    \
[ Reference ]                       \
 sandpile.org                       \
 codeproject.com                    \
 Intel                              \
 AMD                                \
 VIA                                \
 Wikipedia                          \
                                    \
");
	m_Sub.SetNumberOfLines(12);
	m_Sub.SetXCharsPerLine(36);
	m_Sub.SetSize(CMatrixStatic::TINY);
	m_Sub.SetDisplayColors( LcdGridRgb, LcdOnRgb, LcdOffRgb );
	m_Sub.AdjustClientXToSize(36);
	m_Sub.AdjustClientYToSize(12);
	m_Sub.SetAutoPadding(true);			//demonstrates auto padding with defalt character
	m_Sub.DoScroll(1000, CMatrixStatic::UP);

	// Adjust Window Size
	RECT rect,rect1,rect2;
	m_Main.GetClientRect(&rect);
	m_Sub.MoveWindow(rect.left,rect.bottom + 2, 400, 185);
	GetWindowRect(&rect1);
	m_Sub.GetWindowRect(&rect2);

	MoveWindow(rect1.left,rect1.top,405,rect2.bottom - rect1.top + 2);
	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

BOOL CAboutDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CAboutDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_ABOUT,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_AboutDlg = NULL;
}
