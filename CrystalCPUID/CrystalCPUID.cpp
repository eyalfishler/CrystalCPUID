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
#include "DummyDlg.h"

#include "../common/DialogTemplateEx.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <gdiplus.h>
using namespace Gdiplus;
GdiplusStartupInput gdiSI;
ULONG_PTR           gdiToken;
#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCrystalCPUIDApp

BEGIN_MESSAGE_MAP(CCrystalCPUIDApp, CWinApp)
	//{{AFX_MSG_MAP(CCrystalCPUIDApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrystalCPUIDApp 

CCrystalCPUIDApp::CCrystalCPUIDApp()
{
}

/////////////////////////////////////////////////////////////////////////////

CCrystalCPUIDApp theApp;

int gLanguage = ENGLISH;
CBitmap gBackBitmap;
CBrush gBackBrush;
COLORREF gTextColor;
BOOL gFlagBack = FALSE;

int gDialogID = IDD_CRYSTALCPUID_DIALOG; // Default Dialog Templete
char gDefaultFontFace[128];
int gDefaultFontSize = 9;

/////////////////////////////////////////////////////////////////////////////

struct IsFont
{
	BOOL flag;
	char face[128];
};

static int CALLBACK EnumFontsProc(
                LOGFONT *lplf, 
                TEXTMETRIC *lptm, DWORD dwType, 
                LPARAM lpData) 
{
	IsFont *fontCheck = (IsFont *)lpData;
	if(strcmp(lplf->lfFaceName, fontCheck->face) == 0){
		fontCheck->flag = TRUE;
		return 0;
	}else{
		return 1;
	}
}

BOOL CCrystalCPUIDApp::InitInstance()
{
//#define D_NoMutex

#ifndef D_NoMutex
	HANDLE hMutex = ::CreateMutex(NULL,FALSE,"CrystalCPUID");
	if (GetLastError()==ERROR_ALREADY_EXISTS){return FALSE;}
#endif

#ifdef _X86_64
	GdiplusStartup(&gdiToken, &gdiSI, NULL);
#endif // _X86_64

	// Init m_path & m_ini 
	char* ptrEnd;
	::GetModuleFileName(NULL, m_path, MAX_PATH);
	::GetModuleFileName(NULL, m_ini, MAX_PATH);
	if ( (ptrEnd = strrchr(m_path, '\\')) != NULL ){
		*ptrEnd = '\0';
	}
	if ( (ptrEnd = strrchr(m_ini, '.')) != NULL ){
		*ptrEnd = '\0';
		strcat(m_ini, ".ini");
	}

	// Setup Default Font
	IsFont MsUiGothic;
	strcpy(MsUiGothic.face, "MS UI Gothic");
	MsUiGothic.flag = FALSE;
	
	IsFont Tahoma;
	strcpy(Tahoma.face, "Tahoma");
	Tahoma.flag = FALSE;
	
	::EnumFonts(::GetDC(::GetDesktopWindow()),
				NULL, (FONTENUMPROC) EnumFontsProc, 
				(LPARAM)&MsUiGothic);
	::EnumFonts(::GetDC(::GetDesktopWindow()),
				NULL, (FONTENUMPROC) EnumFontsProc, 
				(LPARAM)&Tahoma);

	if(MsUiGothic.flag){
		strcpy(gDefaultFontFace, "MS UI Gothic");
		gDefaultFontSize = 9;
	}else if(Tahoma.flag){
		strcpy(gDefaultFontFace, "Tahoma");
		gDefaultFontSize = 9;
	}else{
		strcpy(gDefaultFontFace, "MS Sans Serif");
		gDefaultFontSize = 8;
	}

	// Select Dialog Templete & ini File
	for(int i=0; i<__argc; i++)
    {
		/*
		if( ! _stricmp(__argv[i],"-D24") || ! _stricmp(__argv[i],"/D24") ){
			gDialogID = IDD_CRYSTALCPUID_DIALOG_24;
		}
		if( ! _stricmp(__argv[i],"-D26") || ! _stricmp(__argv[i],"/D26") ){
			gDialogID = IDD_CRYSTALCPUID_DIALOG_26;
		}
		*/
 		if( ! _strnicmp(__argv[i],"-INI=",5) || ! _strnicmp(__argv[i],"/INI=",5) ){
			if(strstr(&__argv[i][5], "\\")){
				sprintf(m_ini,"%s",&__argv[i][5]);
			}else{
				sprintf(m_ini,"%s\\%s",m_path,&__argv[i][5]);
			}
		}
	}
	
	free((void*)m_pszProfileName);
	m_pszProfileName = strdup(m_ini);

	// Set Language
	char str[256];
	GetPrivateProfileString("Setting","Language","en",str,256,m_pszProfileName);
	if( ! strcmp(str,"en") ){
		gLanguage = ENGLISH;
	}else{
		gLanguage = JAPANESE;
	}

/*
	GetPrivateProfileString("Setting","DialogStyle","",str,256,m_pszProfileName);
	if( ! strcmp(str,"24") ){
		gDialogID = IDD_CRYSTALCPUID_DIALOG_24;
	}
	if( ! strcmp(str,"26") ){
		gDialogID = IDD_CRYSTALCPUID_DIALOG_26;
	}
*/
	// Multimedia Timer Setting
	TIMECAPS tc;
	timeGetDevCaps(&tc,sizeof(TIMECAPS));
	timeBeginPeriod(tc.wPeriodMin);

	AfxEnableControlContainer();

	DebugInfo("ConnectSysInfo( MODE_PCI | MODE_DMI )");
	ConnectSysInfo( MODE_PCI | MODE_DMI );

	CDummyDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	DebugInfo("DisconnectSysInfo()");
	DisconnectSysInfo();

	// Multimedia Timer Setting
	timeEndPeriod(tc.wPeriodMin);

#ifndef D_NoMutex
	::ReleaseMutex(hMutex);
	::CloseHandle(hMutex);
#endif

#ifdef _X86_64
	GdiplusShutdown(gdiToken);
#endif // _X86_64

	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////
// SysInfo.dll Load & Unload
/////////////////////////////////////////////////////////////////////////////

_ReadMSR pReadMSR = NULL;
_WriteMSR pWriteMSR = NULL;

BOOL CCrystalCPUIDApp::ConnectSysInfo(DWORD Mode)
{
#ifdef _X86_64
	hSysInfoLib = LoadLibrary("SysInfoX64.dll");
#else if
	hSysInfoLib = LoadLibrary("SysInfo.dll");
#endif
	if(hSysInfoLib != NULL){
		_CreateSysInfo pCreateSysInfo = (_CreateSysInfo) GetProcAddress (hSysInfoLib,"CreateSysInfo");
		SysInfo = pCreateSysInfo(Mode);
		pReadMSR = (_ReadMSR) GetProcAddress (hSysInfoLib,"ReadMSR");
		if( pReadMSR == NULL ){
			MessageBox(NULL,"Please install SysInfo 220 or later","CrystalCPUID",MB_OK);
			DisconnectSysInfo();
			exit(0);
			return FALSE;	
		}
		pWriteMSR = (_WriteMSR) GetProcAddress (hSysInfoLib,"WriteMSR");
		return TRUE;
	}else{
		MessageBoxEJ(gLanguage,
			IDS_NOT_FOUND_SYS_INFO_DLL_EN,
			IDS_NOT_FOUND_SYS_INFO_DLL_JP);
		exit(0);
		return FALSE;
	}
}

BOOL CCrystalCPUIDApp::DisconnectSysInfo()
{
	if(hSysInfoLib !=NULL){
		_DestroySysInfo pDestroySysInfo = (_DestroySysInfo) GetProcAddress (hSysInfoLib,"DestroySysInfo");
		pDestroySysInfo(SysInfo);
		FreeLibrary(hSysInfoLib);
		hSysInfoLib = NULL;
		SysInfo = NULL;
		return TRUE;
	}else{
		return FALSE;
	}
}