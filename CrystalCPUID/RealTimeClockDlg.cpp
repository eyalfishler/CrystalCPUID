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
#include "RealTimeClockDlg.h"
#include "LcdSetting.h"

#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern int gLanguage;

static void rdtsc(DWORD *EAX, DWORD *EDX)
{
#ifndef _X86_64
	DWORD A, D;
	_asm{
		rdtsc
		mov A, eax
		mov D, edx
	}
	*EAX = A;
	*EDX = D;
#else
	ULONG64 ul64;
	ul64 = __rdtsc();
	*EDX = (DWORD)(ul64 >> 32);
	*EAX = (DWORD)ul64;
#endif
}


CRealTimeClockDlg::CRealTimeClockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRealTimeClockDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CRealTimeClockDlg)
	//}}AFX_DATA_INIT
}


void CRealTimeClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRealTimeClockDlg)
	DDX_Control(pDX, IDC_RTC_CLOCK, m_Clock);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRealTimeClockDlg, CDialog)
	//{{AFX_MSG_MAP(CRealTimeClockDlg)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RTC_CLOCK, OnRtcClock)
	ON_WM_SHOWWINDOW()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRealTimeClockDlg 

void CRealTimeClockDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CRealTimeClockDlg::OnCancel() 
{
	OnClose();
//	CDialog::OnCancel();
}

BOOL CRealTimeClockDlg::OnInitDialog() 
{
	CString cstr;
	char str[256];

	CDialog::OnInitDialog();

	GetPrivateProfileString("Setting","IntervalTime","1000",str,256,P->m_ini);
	if( atoi(str) <= 0 ){
		m_IntervalTime = 1000;
	}else{
		m_IntervalTime = atoi(str);
	}

	GetPrivateProfileString("Setting","MethodType","0",str,256,P->m_ini);
	if( atoi(str) <= 0 ){
		m_MethodType = 0;
	}else{
		m_MethodType = atoi(str);
	}

	GetPrivateProfileString("Setting","TimerType","0",str,256,P->m_ini);
	if( atoi(str) <= 0 ){
		m_TimerType = 0;
	}else{
		m_TimerType = atoi(str);
	}
/*
	cstr.Format("%s [%s%s%s]",
		P->m_CPUName,
		P->m_Family,
		P->m_Model,
		P->m_Stepping);
*/

	if( P->m_FlagCrystalNQuiet ){
		cstr.Format("%03d %% - %s",
			P->m_CpuUsage,
			P->m_CPUName);
	}else{
		cstr.Format("%s",
			P->m_CPUName);
	}
	SetWindowText(cstr);

	double clock = atof(P->m_Clock);
	double multiplier = atof(P->m_Multiplier);
	double system = atof(P->m_SystemClock);

	m_multiplierori = atof(P->m_MultiplierOri);
	if(m_multiplierori > 0.0){
		P->SysInfo->GetData(CPU_FLAG_EIST_CORRECT, &m_FlagEistCorrect);
	}else{
		m_FlagEistCorrect = FALSE;
	}
/*
	P->SysInfo->GetString(CPU_MULTIPLIER_UPDATE,str);
	double multiplier = atof(str);
	double system = clock / multiplier;
*/

	if( multiplier > 0 ){
		cstr.Format("%9.2fMHz%6.2fx%5.2f",
			clock,
			system,
			multiplier);
	}else{
		cstr.Format("%9.2fMHz%9.2fGHz",
			clock,
			clock / 1000.0);
	}
	m_Clock.SetText(cstr);

	m_Clock.SetNumberOfLines(2);
	m_Clock.SetXCharsPerLine(12);
//	m_Clock.SetSize(CMatrixStatic::LARGE);

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
	m_Clock.SetDisplayColors( LcdGridRgb, LcdOnRgb, LcdOffRgb );
/*
	m_Clock.AdjustClientXToSize(12);
	m_Clock.AdjustClientYToSize(2);
	m_Clock.SetAutoPadding(true);
//	m_Clock.DoScroll(500, CMatrixStatic::RIGHT);

	// Adjust Window Size
	RECT rect,rect1;
	m_Clock.GetWindowRect(&rect);
	GetWindowRect(&rect1);

	MoveWindow(rect1.left,rect1.top,270,rect.bottom - rect1.top - 1);
*/

	GetPrivateProfileString("Setting","RtcFontSize","1",str,256,P->m_ini);
	if( atoi(str) < 0 || atoi(str) > 2 ){
		ChangeFontSize(1);
	}else{
		ChangeFontSize(atoi(str));	
	}

	int X, Y;
	RECT rect;
	GetPrivateProfileString("Setting","RtcXpoint","-9999",str,256,P->m_ini);
	X = atoi(str);
	GetPrivateProfileString("Setting","RtcYpoint","-9999",str,256,P->m_ini);
	Y = atoi(str);
	GetWindowRect(&rect);

	if(X == -9999 || Y == -9999){
		CenterWindow();
	}else{
		MoveWindow(X, Y, rect.right - rect.left, rect.bottom - rect.top);
	}
/*	
	if( rect.left - rect.right < X && X < GetDeviceCaps(::GetDC(NULL),HORZRES) &&
		-1 * GetSystemMetrics(SM_CYCAPTION) < Y && Y < GetDeviceCaps(::GetDC(NULL),VERTRES)){
		MoveWindow(X, Y, rect.right - rect.left, rect.bottom - rect.top);
	}else{
		CenterWindow();
	}
*/

	GetPrivateProfileString("Setting","TopMost","1",str,256,P->m_ini);
	if( atoi(str) >= 1 ){
		::SetWindowPos(m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); 
	}

	SetTimer(1, m_IntervalTime, NULL);
	ShowWindow(SW_SHOW);

	return TRUE;
}

void CRealTimeClockDlg::OnTimer(UINT nIDEvent) 
{
	static BOOL hide = TRUE;
	static BOOL	IsNT = P->m_IsNT;

	static double oldsystemclock = atof(P->m_SystemClock);

	if( P->m_FlagHideWindowOnStartReal && hide == TRUE){
		P->ShowWindow(SW_HIDE);
		hide = FALSE;
	}

	/*
	static DWORD count = GetTickCount();
	if( GetTickCount() - count < m_IntervalTime ){
		Sleep(0);
		return;
	}else{
		count = GetTickCount();
	}
	*/

	CString cstr;
	static DWORD StartWT, EndWT, TickCount;			// GetTickCount
	static DWORD StartMMT, EndMMT;					// timeGetTime
	static DWORD StartH, StartL, EndH, EndL;		// QPC
	static ULARGE_INTEGER lRdtscS, lRdtscE;
	static LARGE_INTEGER lStart, lEnd, lFrequency;
	static BOOL Flag = TRUE;
	static CString method;

	double clock;
	double multiplier;
	double systemclock;


	DWORD mask, m_mask = (1<<(P->m_CPUSelectC.GetCurSel()));
	if( IsNT ){
		mask = (DWORD)SetThreadAffinityMask(GetCurrentThread(), m_mask);
	}
/*
	int j;

	double qpc[10];
	double mmt[10];
	double wt[10];

	double ClockQPC;
	double ClockMMT;
	double ClockWT;

	if( m_MethodType == 7 ){ // New Type

		for(j = 0;j<10;j++){
			StartWT = GetTickCount();
			StartMMT = timeGetTime();
			rdtsc(&StartL,&StartH);
			QueryPerformanceCounter(&lStart);
			lRdtscS.HighPart = StartH;
			lRdtscS.LowPart  = StartL;

			volatile int i=0;
			TickCount = GetTickCount();
			while(GetTickCount() - TickCount < 100){
				i++;
				Sleep(50);
			}

			EndWT = GetTickCount();
			EndMMT = timeGetTime();
			rdtsc(&EndL,&EndH);
			QueryPerformanceCounter(&lEnd);
			QueryPerformanceFrequency(&lFrequency);

			lRdtscE.HighPart = EndH;
			lRdtscE.LowPart  = EndL;

			double totalTimeQPC = (double)(lEnd.QuadPart - lStart.QuadPart) / (double)(lFrequency.QuadPart);
			double totalTimeMMT = (double)(EndMMT - StartMMT) / 1000.0;
			double totalTimeWT  = (double)(EndWT - StartWT) / 1000.0;

			ClockQPC = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeQPC /1000000.0;
			ClockMMT = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeMMT /1000000.0;
			ClockWT  = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeWT /1000000.0;

			qpc[j] = ClockQPC;
			mmt[j] = ClockMMT;
			wt[j]  = ClockWT;
		}

	
		switch ( m_TimerType )
		{
		case 1: // Windows Timer
			clock = ClockWT;
			method = "W";
			break;
		case 2: // Multimedia Timer
			clock = ClockMMT;
			method = "M";
			break;
		case 3: // QOC
			clock = ClockQPC;
			method = "Q";
			break;
		default:

			clock = qpc[5];
			method = "Q";
			break;
		}

		char str[256];
		P->SysInfo->GetString(CPU_MULTIPLIER_UPDATE,str);
		multiplier = atof(str);
		systemclock = clock / multiplier;

		if( multiplier > 0.0 ){
			cstr.Format("%1s %7.2fMHz%6.2fx%5.2f",
				method,
				clock,
				systemclock,
				multiplier);
		}else{
			cstr.Format("%1s %7.2fMHz%9.2fGHz",
				method,
				clock,
				clock / 1000.0);
		}
		m_Clock.SetText(cstr);

		StartWT = EndWT;
		StartMMT = EndMMT;
		lStart = lEnd;
		lRdtscS.HighPart = lRdtscE.HighPart;
		lRdtscS.LowPart = lRdtscE.LowPart;

	}else */ if( m_MethodType == 0 ){ // No Load
		if( Flag ){
			QueryPerformanceCounter(&lStart);
			rdtsc(&StartL,&StartH);
			StartMMT = timeGetTime();
			StartWT  = GetTickCount();
			lRdtscS.HighPart = StartH;
			lRdtscS.LowPart  = StartL;
			Flag = FALSE;
		}else{
			EndWT = GetTickCount();
			EndMMT = timeGetTime();
			rdtsc(&EndL,&EndH);
			QueryPerformanceCounter(&lEnd);
			QueryPerformanceFrequency(&lFrequency);

			lRdtscE.HighPart = EndH;
			lRdtscE.LowPart  = EndL;

			double totalTimeQPC = (double)(lEnd.QuadPart - lStart.QuadPart) / (double)(lFrequency.QuadPart);
			double totalTimeMMT = (double)(EndMMT - StartMMT) / 1000.0;
			double totalTimeWT  = (double)(EndWT - StartWT) / 1000.0;

			double ClockQPC = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeQPC /1000000.0;
			double ClockMMT = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeMMT /1000000.0;
			double ClockWT  = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeWT /1000000.0;

			switch ( m_TimerType )
			{
			case 1: // Windows Timer
				clock = ClockWT;
				method = "W";
				break;
			case 2: // Multimedia Timer
				clock = ClockMMT;
				method = "M";
				break;
			case 3: // QOC
				clock = ClockQPC;
				method = "Q";
				break;
			default:
				if( ClockMMT * ERROR_RATE_MIN < ClockQPC && ClockQPC < ClockMMT * ERROR_RATE_MAX  ){
					clock = ClockQPC;
					method = "Q";
				}else{
					clock = ClockMMT;
					method = "M";
				}
				break;
			}

			char str[256];
			P->SysInfo->GetString(CPU_MULTIPLIER_UPDATE,str);
			multiplier = atof(str);
			if(m_FlagEistCorrect){clock = clock * multiplier / m_multiplierori;}
			systemclock = clock / multiplier;

			if( multiplier > 0 ){
				cstr.Format("%1s %7.2fMHz%6.2fx%5.2f",
					method,
					clock,
					systemclock,
					multiplier);

				if( systemclock < 10.0 ){
					KillTimer(1);
					MessageBoxEJ(gLanguage,
						IDS_REAL_TIME_CLOCK_NO_LOAD_EN,
						IDS_REAL_TIME_CLOCK_NO_LOAD_JP);
					PostMessage(WM_CLOSE);
					return ;
				}
			}else{
				cstr.Format("%1s %7.2fMHz%9.2fGHz",
					method,
					clock,
					clock / 1000.0);
			}

			StartWT = EndWT;
			StartMMT = EndMMT;
			lStart = lEnd;
			lRdtscS.HighPart = lRdtscE.HighPart;
			lRdtscS.LowPart = lRdtscE.LowPart;
			if( oldsystemclock - 1 < systemclock && systemclock <= oldsystemclock + 1)
			{
				m_Clock.SetText(cstr);
			}
			oldsystemclock = systemclock;
		}
	}else{ // With Load
		StartWT = GetTickCount();
		StartMMT = timeGetTime();
		rdtsc(&StartL,&StartH);
		QueryPerformanceCounter(&lStart);
		lRdtscS.HighPart = StartH;
		lRdtscS.LowPart  = StartL;

		volatile int i=0;
		TickCount = GetTickCount();
		while(GetTickCount() - TickCount < 100){
			i++;
			Sleep(0);
		}

		EndWT = GetTickCount();
		EndMMT = timeGetTime();
		rdtsc(&EndL,&EndH);
		QueryPerformanceCounter(&lEnd);
		QueryPerformanceFrequency(&lFrequency);

		lRdtscE.HighPart = EndH;
		lRdtscE.LowPart  = EndL;

		double totalTimeQPC = (double)(lEnd.QuadPart - lStart.QuadPart) / (double)(lFrequency.QuadPart);
		double totalTimeMMT = (double)(EndMMT - StartMMT) / 1000.0;
		double totalTimeWT  = (double)(EndWT - StartWT) / 1000.0;

		double ClockQPC = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeQPC /1000000.0;
		double ClockMMT = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeMMT /1000000.0;
		double ClockWT  = (__int64)(lRdtscE.QuadPart - lRdtscS.QuadPart) / totalTimeWT /1000000.0;
	
		switch ( m_TimerType )
		{
		case 1: // Windows Timer
			clock = ClockWT;
			method = "W";
			break;
		case 2: // Multimedia Timer
			clock = ClockMMT;
			method = "M";
			break;
		case 3: // QOC
			clock = ClockQPC;
			method = "Q";
			break;
		default:
			/*
			if( ClockMMT * ERROR_RATE_MIN < ClockQPC && ClockQPC < ClockMMT * ERROR_RATE_MAX  ){
				clock = ClockQPC;
			}else{
				clock = ClockMMT;
			}
			*/
			clock = ClockQPC;
			method = "Q";
			break;
		}

		char str[256];
		P->SysInfo->GetString(CPU_MULTIPLIER_UPDATE,str);
		multiplier = atof(str);
		if(m_FlagEistCorrect){clock = clock * multiplier / m_multiplierori;}
		systemclock = clock / multiplier;

		if( multiplier > 0.0 ){
			cstr.Format("%1s %7.2fMHz%6.2fx%5.2f",
				method,
				clock,
				systemclock,
				multiplier);
		}else{
			cstr.Format("%1s %7.2fMHz%9.2fGHz",
				method,
				clock,
				clock / 1000.0);
		}

		StartWT = EndWT;
		StartMMT = EndMMT;
		lStart = lEnd;
		lRdtscS.HighPart = lRdtscE.HighPart;
		lRdtscS.LowPart = lRdtscE.LowPart;

		if( oldsystemclock - 1 < systemclock && systemclock <= oldsystemclock + 1)
		{
			m_Clock.SetText(cstr);
		}
		oldsystemclock = systemclock;
	}

//FeedBack Main Dialog 5.0 or later ???
/*
	cstr.Format("%7.2f", clock);
	P->m_ClockC.SetWindowText(cstr);
	cstr.Format("%7.2f", systemclock);
	P->m_SystemClockC.SetWindowText(cstr);
	cstr.Format("%7.2f", multiplier);
	P->m_MultiplierC.SetWindowText(cstr);

	if(atoi(P->m_ClockOri) > 0){
		cstr.Format("%.2f", ((clock / atoi(P->m_ClockOri)) - 1) * 100.0);
		P->m_OverClockC.SetWindowText(cstr);
	}
*/

	if( P->m_FlagCrystalNQuiet ){
		cstr.Format("%03d %% - %s",
			P->m_CpuUsage,
			P->m_CPUName);
	}else{
		cstr.Format("%s",
			P->m_CPUName);
	}
	SetWindowText(cstr);

	if( IsNT ){
		SetThreadAffinityMask(GetCurrentThread(), mask);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CRealTimeClockDlg::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
}

BOOL CRealTimeClockDlg::Create()
{
	return CDialog::Create( IDD, P );
}

void CRealTimeClockDlg::OnClose() 
{
	CString cstr;
	RECT rect;
	GetWindowRect(&rect);
	cstr.Format("%d",rect.left);
	WritePrivateProfileString("Setting","RtcXpoint",cstr,P->m_ini);
	cstr.Format("%d",rect.top);
	WritePrivateProfileString("Setting","RtcYpoint",cstr,P->m_ini);

	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_REAL_TIME_CLOCK,MF_ENABLED);
	P->SetMenu(menu);
	P->ShowWindow(SW_SHOW);

	CDialog::OnClose();
	delete this;
	P->m_RealTimeClockDlg = NULL;
}

void CRealTimeClockDlg::OnRtcClock() 
{
	P->ShowWindow( ! P->IsWindowVisible() );
}

void CRealTimeClockDlg::ChangeFontSize(int type)
{
	int size;
	int width;
	switch(type)
	{
	case 0:
		size = CMatrixStatic::SMALL;
		width = 200; 
		break;
	case 1:
		size = CMatrixStatic::LARGE;
		width = 270;
		break;
	case 2:
		size = CMatrixStatic::TINY;
		width = 140; 
		break;
	default:
		return;
	}
	m_Clock.SetSize(size);
	m_Clock.AdjustClientXToSize(12);
	m_Clock.AdjustClientYToSize(2);
	m_Clock.SetAutoPadding(true);

	ShowWindow(SW_HIDE);
	RECT rect,rect1;
	m_Clock.GetWindowRect(&rect);
	GetWindowRect(&rect1);
	MoveWindow(rect1.left, rect1.top, width, rect.bottom - rect1.top - 1);
	ShowWindow(SW_SHOW);
}

void CRealTimeClockDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	P->ShowWindow(SW_SHOW);
}

void CRealTimeClockDlg::ChangeIntervalTime(int time)
{
	m_IntervalTime = time;
	KillTimer(1);
	SetTimer(1, time, NULL);
}

void CRealTimeClockDlg::ChangeMethodType(int method)
{
	m_MethodType = method;
}

void CRealTimeClockDlg::ChangeTimerType(int timer)
{
	m_TimerType = timer;
}

void CRealTimeClockDlg::StartTimer()
{
	SetTimer(1, m_IntervalTime, NULL);
}

void CRealTimeClockDlg::EndTimer()
{
	KillTimer(1);
}



void CRealTimeClockDlg::OnDestroy() 
{
	CString cstr;
	RECT rect;
	GetWindowRect(&rect);
	cstr.Format("%d",rect.left);
	WritePrivateProfileString("Setting","RtcXpoint",cstr,P->m_ini);
	cstr.Format("%d",rect.top);
	WritePrivateProfileString("Setting","RtcYpoint",cstr,P->m_ini);
	
	CDialog::OnDestroy();

}
