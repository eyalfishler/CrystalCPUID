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
#include "CustomizeDlg.h"
#include "LcdSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gLanguage;
extern CBitmap gBackBitmap;
extern CBrush gBackBrush;
extern COLORREF gTextColor;
extern BOOL gFlagBack;
extern char gDefaultFontFace[128];
extern int gDefaultFontSize;

static CCrystalCPUIDDlg* P;

#include "../common/GetOSInfo.h"

/////////////////////////////////////////////////////////////////////////////
// CCustomizeDlg Dialog

CCustomizeDlg::CCustomizeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomizeDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	P->GetClientRect(&m_rect);
	//{{AFX_DATA_INIT(CCustomizeDlg)
	m_FileName = _T("");
	m_WindowSize = _T("");
	m_ImageSize = _T("");
	m_TextColor = _T("");
	m_Gray = FALSE;
	m_LcdOnRgb = RGB( 0, 0, 0 );
	m_LcdOffRgb = RGB( 0, 0, 0 );
	m_LcdGridRgb = RGB( 0, 0, 0 );
	m_LcdGrid = _T("");
	m_LcdOff = _T("");
	m_LcdOn = _T("");
	m_FontInfoMain = _T("");
	m_FontInfoOther = _T("");
	m_TtiDisable = FALSE;
	//}}AFX_DATA_INIT
}

void CCustomizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomizeDlg)
	DDX_Control(pDX, IDC_TTI_NORMAL_ICON, m_TtiNormalIcon);
	DDX_Control(pDX, IDC_TTI_MIN_ICON, m_TtiMinIcon);
	DDX_Control(pDX, IDC_TTI_MID_ICON, m_TtiMidIcon);
	DDX_Control(pDX, IDC_TTI_MAX_ICON, m_TtiMaxIcon);
	DDX_Control(pDX, IDC_RTC_DIALOG_FONT, m_RtcFontSize);
	DDX_Control(pDX, IDC_LCD_SAMPLE_11, m_LcdSample11);
	DDX_Control(pDX, IDC_RTC_TOPMOST, m_RtcTopMost);
	DDX_Control(pDX, IDC_LCD_SAMPLE_9, m_LcdSample9);
	DDX_Control(pDX, IDC_LCD_SAMPLE_8, m_LcdSample8);
	DDX_Control(pDX, IDC_LCD_SAMPLE_7, m_LcdSample7);
	DDX_Control(pDX, IDC_LCD_SAMPLE_6, m_LcdSample6);
	DDX_Control(pDX, IDC_LCD_SAMPLE_5, m_LcdSample5);
	DDX_Control(pDX, IDC_LCD_SAMPLE_4, m_LcdSample4);
	DDX_Control(pDX, IDC_LCD_SAMPLE_3, m_LcdSample3);
	DDX_Control(pDX, IDC_LCD_SAMPLE_2, m_LcdSample2);
	DDX_Control(pDX, IDC_LCD_SAMPLE_10, m_LcdSample10);
	DDX_Control(pDX, IDC_LCD_SAMPLE_1, m_LcdSample1);
	DDX_Control(pDX, IDC_INTERVAL_TIME, m_IntervalTime);
	DDX_Control(pDX, IDC_LCD_SAMPLE, m_LcdSample);
	DDX_Control(pDX, IDC_GRAY, m_GrayC);
	DDX_Text(pDX, IDC_FILE_NAME, m_FileName);
	DDX_Text(pDX, IDC_WINDOW_SIZE, m_WindowSize);
	DDX_Text(pDX, IDC_IMAGE_SIZE, m_ImageSize);
	DDX_Text(pDX, IDC_TEXT_COLOR, m_TextColor);
	DDX_Check(pDX, IDC_GRAY, m_Gray);
	DDX_Text(pDX, IDC_LCD_GRID, m_LcdGrid);
	DDX_Text(pDX, IDC_LCD_OFF, m_LcdOff);
	DDX_Text(pDX, IDC_LCD_ON, m_LcdOn);
	DDX_Text(pDX, IDC_FONT_INFO_MAIN, m_FontInfoMain);
	DDX_Text(pDX, IDC_FONT_INFO_OTHER, m_FontInfoOther);
	DDX_Check(pDX, IDC_TTI_DISABLE, m_TtiDisable);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCustomizeDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomizeDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_SELECT_FILE, OnSelectFile)
	ON_BN_CLICKED(IDC_SELECT_COLOR, OnSelectColor)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_GRAY, OnGray)
	ON_BN_CLICKED(IDC_SELECT_LCD_ON, OnSelectLcdOn)
	ON_BN_CLICKED(IDC_SELECT_LCD_OFF, OnSelectLcdOff)
	ON_BN_CLICKED(IDC_SELECT_LCD_GRID, OnSelectLcdGrid)
	ON_BN_CLICKED(IDC_RTC_HELP, OnRtcHelp)
	ON_CBN_SELCHANGE(IDC_INTERVAL_TIME, OnSelchangeIntervalTime)
	ON_BN_CLICKED(IDC_RTC_NO_LOAD, OnRtcNoLoad)
	ON_BN_CLICKED(IDC_RTC_WITH_LOAD, OnRtcWithLoad)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_1, OnLcdSample1)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_2, OnLcdSample2)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_3, OnLcdSample3)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_4, OnLcdSample4)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_5, OnLcdSample5)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_6, OnLcdSample6)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_7, OnLcdSample7)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_8, OnLcdSample8)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_9, OnLcdSample9)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_10, OnLcdSample10)
	ON_BN_CLICKED(IDC_RTC_TOPMOST, OnRtcTopmost)
	ON_BN_CLICKED(IDC_RTC_NEW_TYPE, OnRtcNewType)
	ON_BN_CLICKED(IDC_RTC_NO_LOAD_MMT, OnRtcNoLoadMmt)
	ON_BN_CLICKED(IDC_RTC_NO_LOAD_QPC, OnRtcNoLoadQpc)
	ON_BN_CLICKED(IDC_LCD_SAMPLE_11, OnLcdSample11)
	ON_BN_CLICKED(IDC_SELECT_FONT_MAIN, OnSelectFontMain)
	ON_BN_CLICKED(IDC_SELECT_FONT_OTHER, OnSelectFontOther)
	ON_CBN_SELCHANGE(IDC_RTC_DIALOG_FONT, OnSelchangeRtcDialogFont)
	ON_BN_CLICKED(IDC_TTI_MAX, OnTtiMax)
	ON_BN_CLICKED(IDC_TTI_NORMAL, OnTtiNormal)
	ON_BN_CLICKED(IDC_TTI_MID, OnTtiMid)
	ON_BN_CLICKED(IDC_TTI_MIN, OnTtiMin)
	ON_BN_CLICKED(IDC_TTI_DEFAULT, OnTtiDefault)
	ON_BN_CLICKED(IDC_RTC_NEW_TYPE, OnRtcNewType)
	ON_BN_CLICKED(IDC_TTI_DISABLE, OnTtiDisable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CCustomizeDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CCustomizeDlg::OnOK() 
{
//	CDialog::OnOK();
}


static void InitLcdSample(CMatrixStatic& ctrl,COLORREF grid, COLORREF on, COLORREF off);

void InitLcdSample(CMatrixStatic& ctrl, COLORREF grid, COLORREF on, COLORREF off)
{
	ctrl.SetText("*");
	ctrl.SetNumberOfLines(1);
	ctrl.SetXCharsPerLine(1);
	ctrl.SetSize(CMatrixStatic::LARGE);
	ctrl.SetDisplayColors( grid, on, off );
	ctrl.AdjustClientYToSize(1);
	ctrl.AdjustClientXToSize(1);
	ctrl.SetAutoPadding(true);
}

BOOL CCustomizeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//////////////////////////////////////////////
// Background
//////////////////////////////////////////////
	char str[MAX_PATH];
	GetPrivateProfileString("Setting","BackgroundImage","",str,MAX_PATH,P->m_ini);
	m_FileName = str;

	GetPrivateProfileString("Setting","Gray","0",str,MAX_PATH,P->m_ini);
	if( atoi(str) == 0 ){
		m_Gray = FALSE;
	}else{
		m_Gray = TRUE;
	}

	GetPrivateProfileString("Setting","TextColor","",str,MAX_PATH,P->m_ini);
	if( strcmp(str,"") == 0){
		gTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	}else{
		gTextColor = atoi(str);
	}
	m_TextColor.Format("RGB( %d, %d, %d )",
		gTextColor & 0x000000FF,
		(gTextColor & 0x0000FF00) >> 8,
		(gTextColor & 0x00FF0000) >> 16
		);
	if( ! m_FileName.IsEmpty() && ( Dib.LoadBmp(m_FileName)
#ifdef USEJPEG
		|| Dib.LoadJpeg(m_FileName)
#endif
#ifdef USEPNG
		|| Dib.LoadPng(m_FileName)
#endif
		)
	)
	{
		m_ImageSize.Format("%d x %d x %d", Dib.Width(), Dib.Height(), Dib.Depth() ) ;
		m_GrayC.EnableWindow( Dib.Depth() == 24 );
		Dib.Destroy();
	}
	m_WindowSize.Format("%d x %d", m_rect.right, m_rect.bottom);

//////////////////////////////////////////////
// LCD ( Real Time Clock / About )
//////////////////////////////////////////////
	m_LcdSample.SetText("\
CPU\
MMX\
SSE\
HTT\
SDR\
DDR\
QDR\
***\
abc\
def\
ghi\
jkl\
mno\
pqr\
stu\
vwx\
yz.\
***\
ABC\
DEF\
GHI\
JKL\
MNO\
PQR\
STU\
VWX\
YZ_\
***\
012\
345\
678\
9$#\
+-%\
!@|\
***\
***\
777\
***\
***\
");
//////////////////////////////
// Cuurent LCD Color		
	GetPrivateProfileString("Setting","LcdGrid",DEFAULT_LCD_GRID,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdGridRgb = atoi(str);
	}else{
		m_LcdGridRgb = atoi(DEFAULT_LCD_GRID);
	}
	m_LcdGrid.Format("RGB( %d, %d, %d )",
		m_LcdGridRgb & 0x000000FF,
		(m_LcdGridRgb & 0x0000FF00) >> 8,
		(m_LcdGridRgb & 0x00FF0000) >> 16
		);	

	GetPrivateProfileString("Setting","LcdOn",DEFAULT_LCD_ON,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdOnRgb = atoi(str);
	}else{
		m_LcdOnRgb = atoi(DEFAULT_LCD_ON);
	}
	m_LcdOn.Format("RGB( %d, %d, %d )",
		m_LcdOnRgb & 0x000000FF,
		(m_LcdOnRgb & 0x0000FF00) >> 8,
		(m_LcdOnRgb & 0x00FF0000) >> 16
		);

	GetPrivateProfileString("Setting","LcdOff",DEFAULT_LCD_OFF,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdOffRgb = atoi(str);
	}else{
		m_LcdOffRgb = atoi(DEFAULT_LCD_OFF);
	}
	m_LcdOff.Format("RGB( %d, %d, %d )",
		m_LcdOffRgb & 0x000000FF,
		(m_LcdOffRgb & 0x0000FF00) >> 8,
		(m_LcdOffRgb & 0x00FF0000) >> 16
		);

//////////////////////////////
// User LCD Color		
	GetPrivateProfileString("Setting","LcdGridUser",DEFAULT_LCD_GRID,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdGridUserRgb = atoi(str);
	}else{
		m_LcdGridUserRgb = m_LcdGridRgb;
	}

	GetPrivateProfileString("Setting","LcdOnUser",DEFAULT_LCD_ON,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdOnUserRgb = atoi(str);
	}else{
		m_LcdOnUserRgb = m_LcdOnRgb;
	}

	GetPrivateProfileString("Setting","LcdOffUser",DEFAULT_LCD_OFF,str,256,P->m_ini);
	if( strcmp(str,"") != 0 ){
		m_LcdOffUserRgb = atoi(str);
	}else{
		m_LcdOffUserRgb = m_LcdOffRgb;
	}

	m_LcdSample.SetNumberOfLines(2);
	m_LcdSample.SetXCharsPerLine(3);
	m_LcdSample.SetSize(CMatrixStatic::LARGE);
	m_LcdSample.SetDisplayColors( m_LcdGridRgb, m_LcdOnRgb, m_LcdOffRgb );
	m_LcdSample.AdjustClientYToSize(2);
	m_LcdSample.AdjustClientXToSize(3);
	m_LcdSample.DoScroll(500, CMatrixStatic::UP);
	m_LcdSample.SetAutoPadding(true);

	InitLcdSample(m_LcdSample1,LCD_GRID_1,LCD_ON_1,LCD_OFF_1);
	InitLcdSample(m_LcdSample2,LCD_GRID_2,LCD_ON_2,LCD_OFF_2);
	InitLcdSample(m_LcdSample3,LCD_GRID_3,LCD_ON_3,LCD_OFF_3);
	InitLcdSample(m_LcdSample4,LCD_GRID_4,LCD_ON_4,LCD_OFF_4);
	InitLcdSample(m_LcdSample5,LCD_GRID_5,LCD_ON_5,LCD_OFF_5);
	InitLcdSample(m_LcdSample6,LCD_GRID_6,LCD_ON_6,LCD_OFF_6);
	InitLcdSample(m_LcdSample7,LCD_GRID_7,LCD_ON_7,LCD_OFF_7);
	InitLcdSample(m_LcdSample8,LCD_GRID_8,LCD_ON_8,LCD_OFF_8);
	InitLcdSample(m_LcdSample9,LCD_GRID_9,LCD_ON_9,LCD_OFF_9);
	InitLcdSample(m_LcdSample10,LCD_GRID_10,LCD_ON_10,LCD_OFF_10);
	InitLcdSample(m_LcdSample11,m_LcdGridUserRgb, m_LcdOnUserRgb, m_LcdOffUserRgb);

//////////////////////////////////////////////
// Real Time Clock Setting
//////////////////////////////////////////////

	int i;
	CString cstr;
	GetPrivateProfileString("Setting","IntervalTime","1000",str,256,P->m_ini);
	if( atoi(str) <= 0 ){
		strcpy(str,"1000");
	}
	m_IntervalTime.AddString(str);
	m_IntervalTime.SetCurSel(0);
	for(i = 100;i< 1000;i+=100){
		cstr.Format("%d",i);
		m_IntervalTime.AddString(cstr);
	}
	for(i = 1000;i<= 10000;i+=1000){
		cstr.Format("%d",i);
		m_IntervalTime.AddString(cstr);
	}
	GetPrivateProfileString("Setting","RtcFontSize","1",str,256,P->m_ini);
	m_RtcFontSize.AddString("SMALL");
	m_RtcFontSize.AddString("LARGE");
	m_RtcFontSize.AddString("TINY");
	if( atoi(str) < 0 || atoi(str) > 2 ){
		m_RtcFontSize.SetCurSel( 1 );	
	}else{
		m_RtcFontSize.SetCurSel( atoi(str) );	
	}

	CButton* NewType = (CButton*)GetDlgItem(IDC_RTC_NEW_TYPE);
	CButton* NoLoad = (CButton*)GetDlgItem(IDC_RTC_NO_LOAD);
	CButton* WithLoad = (CButton*)GetDlgItem(IDC_RTC_WITH_LOAD);
	CButton* NoLoadMMT = (CButton*)GetDlgItem(IDC_RTC_NO_LOAD_MMT);
	CButton* NoLoadQPC = (CButton*)GetDlgItem(IDC_RTC_NO_LOAD_QPC);

	GetPrivateProfileString("Setting","MethodType","0",str,256,P->m_ini);
	DWORD MethodType = atoi(str);
	GetPrivateProfileString("Setting","TimerType","0",str,256,P->m_ini);
	DWORD TimerType = atoi(str);

	if( MethodType == 1 ){
		WithLoad->SetCheck(1);
	}else if( MethodType == 7 ){
		NewType->SetCheck(1);
	}else{
		if( TimerType == 2 ){
			NoLoadMMT->SetCheck(1);
		}else if( TimerType == 3 ){
			NoLoadQPC->SetCheck(1);
		}else{
			NoLoad->SetCheck(1);
		}
	}

	GetPrivateProfileString("Setting","TopMost","1",str,256,P->m_ini);
	if( atoi(str) >= 1 ){
		m_RtcTopMost.SetCheck(1);
	}
// Font Setting
	HDC hDC = ::GetDC(NULL);
	int logPixel = GetDeviceCaps(hDC, LOGPIXELSY);
	DeleteDC(hDC);

	GetPrivateProfileString("Setting", "MainFontFace", gDefaultFontFace, str,256,AfxGetApp()->m_pszProfileName);
	m_FontInfoMain = str;
	cstr.Format(" ( %d pt )", GetPrivateProfileInt("Setting", "MainFontSize", gDefaultFontSize, AfxGetApp()->m_pszProfileName));
	GetPrivateProfileString("Setting","MainFontBold","0",str,256,P->m_ini);
	m_FontInfoMain += cstr;
	if( atoi(str) > 0 ){
		m_FontInfoMain += " [B]";
	}
	GetPrivateProfileString("Setting","MainFontItalic","0",str,256,P->m_ini);
	if( atoi(str) > 0 ){
		m_FontInfoMain += " [I]";
	}

	GetPrivateProfileString("Setting","OtherFontFace",gDefaultFontFace,str,256,P->m_ini);
	m_FontInfoOther = str;
	UINT size = GetPrivateProfileInt("Setting","OtherFontSize",gDefaultFontSize,P->m_ini);
	cstr.Format(" ( %d pt )", size);
	GetPrivateProfileString("Setting","OtherFontBold","0",str,256,P->m_ini);
	m_FontInfoOther += cstr;
	if( atoi(str) > 0 ){
		m_FontInfoOther += " [B]";
	}
	GetPrivateProfileString("Setting","OtherFontItalic","0",str,256,P->m_ini);
	if( atoi(str) > 0 ){
		m_FontInfoOther += " [I]";
	}

//////////////////////////////////////////////
// Task Tray Icon Setting
//////////////////////////////////////////////

	m_TtiNormalIcon.SetIcon(P->m_hTaskIcon);
	m_TtiNormalIcon.Invalidate(TRUE);
	m_TtiMaxIcon.SetIcon(P->m_hTaskIconMax);
	m_TtiMaxIcon.Invalidate(TRUE);
	m_TtiMidIcon.SetIcon(P->m_hTaskIconMid);
	m_TtiMidIcon.Invalidate(TRUE);
	m_TtiMinIcon.SetIcon(P->m_hTaskIconMin);
	m_TtiMinIcon.Invalidate(TRUE);

	GetPrivateProfileString("Setting","TaskTrayIconDisable","0",str,MAX_PATH,P->m_ini);
	if(atoi(str) == 0){
		m_TtiDisable = FALSE;
	}else{
		m_TtiDisable = TRUE;
	}
	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

///////////////////////////////////////////////////////////////
// Background Image Setting
///////////////////////////////////////////////////////////////

void CCustomizeDlg::OnSelectFile() 
{
	CFileDialog dlg(TRUE,
		"","",
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		"Supported Image Files|*.bmp;*.png;*.jpg;*.jpeg;*.gif|BMP (*.bmp)|*.bmp|PNG (*.png)|*.png|JPEG (*.jpg *.jpeg)|*.jpg;*.jpeg|GIF (*.gif)|*.gif;|All Files (*.*)|*.*||");
#ifndef _X86_64
	FakeCFileDialog(dlg);
#endif
	if( dlg.DoModal() == IDOK )
	{
		m_FileName = dlg.GetPathName();
		if( Dib.LoadBmp(m_FileName) 
#ifdef USEJPEG
		|| Dib.LoadJpeg(m_FileName)
#endif
#ifdef USEPNG
		|| Dib.LoadPng(m_FileName) 
#endif
		)
		{
			m_ImageSize.Format("%d x %d x %d", Dib.Width(), Dib.Height(), Dib.Depth() ) ;
			m_GrayC.EnableWindow( Dib.Depth() == 24 );
			Dib.Destroy();
		}
		UpdateData(FALSE);
		WritePrivateProfileString("Setting","BackgroundImage",m_FileName,P->m_ini);
		P->CreateWallPaper(1,NULL,(char*)(const char*)m_FileName,0,m_Gray);
		P->Invalidate();
	}
}

void CCustomizeDlg::OnSelectColor() 
{
	CColorDialog dlg;
	if( dlg.DoModal() == IDOK )
	{
		gTextColor = dlg.GetColor();
		m_TextColor.Format("RGB( %d, %d, %d )",
			gTextColor & 0x000000FF,
			(gTextColor & 0x0000FF00) >> 8,
			(gTextColor & 0x00FF0000) >> 16
			);
		UpdateData(FALSE);

		CString cstr;
		cstr.Format("%d",gTextColor);
		WritePrivateProfileString("Setting","TextColor",cstr,P->m_ini);
		P->Invalidate();
	}
}

void CCustomizeDlg::OnClear() 
{
	WritePrivateProfileString("Setting","BackgroundImage","",P->m_ini);
	WritePrivateProfileString("Setting","TextColor","",P->m_ini);
	WritePrivateProfileString("Setting","Gray","",P->m_ini);
	
	gTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	m_TextColor.Format("RGB( %d, %d, %d )",
		gTextColor & 0x000000FF,
		(gTextColor & 0x0000FF00) >> 8,
		(gTextColor & 0x00FF0000) >> 16
		);
	m_FileName = "";
	m_ImageSize = "";
	m_Gray = FALSE;

	UpdateData(FALSE);

	P->CreateWallPaper(1,NULL,(char*)(const char*)m_FileName,0,FALSE);
	P->Invalidate();
}

void CCustomizeDlg::OnGray() 
{
	UpdateData(TRUE);
	CString cstr;
	cstr.Format("%d",m_Gray);
	WritePrivateProfileString("Setting","Gray",cstr,P->m_ini);
	P->CreateWallPaper(1,NULL,(char*)(const char*)m_FileName,0,m_Gray);
	P->Invalidate();
}

///////////////////////////////////////////////////////////////
// Select LCD Color
///////////////////////////////////////////////////////////////

void CCustomizeDlg::OnSelectLcdOn() 
{
	SelectLcdColor(m_LcdOnRgb, m_LcdOn, "LcdOn");
}

void CCustomizeDlg::OnSelectLcdOff() 
{
	SelectLcdColor(m_LcdOffRgb, m_LcdOff, "LcdOff");	
}

void CCustomizeDlg::OnSelectLcdGrid() 
{
	SelectLcdColor(m_LcdGridRgb, m_LcdGrid, "LcdGrid");
}

void CCustomizeDlg::SelectLcdColor(COLORREF &color, CString &ctrl, CString section)
{
	CColorDialog dlg;
	if( dlg.DoModal() == IDOK )
	{
		color = dlg.GetColor();
		SetLcdColor(color, ctrl, section, 1);/* Update LcdUser*/
	}
}

void CCustomizeDlg::SetLcdColor(COLORREF &color, CString &ctrl, CString section, DWORD type /* 1 = Update LcdUser */)
{
	CString cstr;
	ctrl.Format("RGB( %d, %d, %d )",
		color & 0x000000FF,
		(color & 0x0000FF00) >> 8,
		(color & 0x00FF0000) >> 16
		);
	UpdateData(FALSE);
	m_LcdSample.SetDisplayColors( m_LcdGridRgb, m_LcdOnRgb, m_LcdOffRgb );
	if( type == 1 ){
		m_LcdGridUserRgb = m_LcdGridRgb;
		m_LcdOnUserRgb = m_LcdOnRgb;
		m_LcdOffUserRgb = m_LcdOffRgb;
		m_LcdSample11.SetDisplayColors( m_LcdGridUserRgb, m_LcdOnUserRgb, m_LcdOffUserRgb );
		cstr.Format("%d",m_LcdGridUserRgb);
		WritePrivateProfileString("Setting","LcdGridUser",cstr,P->m_ini);
		cstr.Format("%d",m_LcdOnUserRgb);
		WritePrivateProfileString("Setting","LcdOnUser",cstr,P->m_ini);
		cstr.Format("%d",m_LcdOffUserRgb);
		WritePrivateProfileString("Setting","LcdOffUser",cstr,P->m_ini);
	}
	cstr.Format("%d",color);
	WritePrivateProfileString("Setting",section,cstr,P->m_ini);

	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->m_Clock.SetDisplayColors( m_LcdGridRgb, m_LcdOnRgb, m_LcdOffRgb );
	}
	if( P->m_AboutDlg != NULL ){
		P->m_AboutDlg->m_Main.SetDisplayColors( m_LcdGridRgb, m_LcdOnRgb, m_LcdOffRgb );
		P->m_AboutDlg->m_Sub.SetDisplayColors( m_LcdGridRgb, m_LcdOnRgb, m_LcdOffRgb );
	}
}

///////////////////////////////////////////////////////////////
// Interval Time
///////////////////////////////////////////////////////////////

void CCustomizeDlg::OnSelchangeIntervalTime() 
{
	CString cstr;
	m_IntervalTime.GetLBText(m_IntervalTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","IntervalTime",cstr,P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeIntervalTime(atoi(cstr));
	}
}

///////////////////////////////////////////////////////////////
// Method Type
///////////////////////////////////////////////////////////////

void CCustomizeDlg::OnRtcHelp() 
{
	MessageBoxEJ(gLanguage,
		IDS_REAL_TIME_CLOCK_HELP_EN,
		IDS_REAL_TIME_CLOCK_HELP_JP
		);
}

void CCustomizeDlg::OnRtcNewType() 
{
	WritePrivateProfileString("Setting","MethodType","7",P->m_ini);
	WritePrivateProfileString("Setting","TimerType","0",P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeMethodType(7);
		P->m_RealTimeClockDlg->ChangeTimerType(0);
	}
}

void CCustomizeDlg::OnRtcNoLoad() 
{
	WritePrivateProfileString("Setting","MethodType","0",P->m_ini);
	WritePrivateProfileString("Setting","TimerType","0",P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeMethodType(0);
		P->m_RealTimeClockDlg->ChangeTimerType(0);
	}
}

void CCustomizeDlg::OnRtcWithLoad() 
{
	WritePrivateProfileString("Setting","MethodType","1",P->m_ini);
	WritePrivateProfileString("Setting","TimerType","3",P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeMethodType(1);
		P->m_RealTimeClockDlg->ChangeTimerType(3);
	}
}

void CCustomizeDlg::OnRtcNoLoadMmt() 
{
	WritePrivateProfileString("Setting","MethodType","0",P->m_ini);
	WritePrivateProfileString("Setting","TimerType","2",P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeMethodType(0);
		P->m_RealTimeClockDlg->ChangeTimerType(2);
	}
}

void CCustomizeDlg::OnRtcNoLoadQpc() 
{
	WritePrivateProfileString("Setting","MethodType","0",P->m_ini);
	WritePrivateProfileString("Setting","TimerType","3",P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeMethodType(0);
		P->m_RealTimeClockDlg->ChangeTimerType(3);
	}	
}


///////////////////////////////////////////////////////////////
// Setup Lcd Sample Color
///////////////////////////////////////////////////////////////

void CCustomizeDlg::SetupLcdSampleColor( COLORREF grid, COLORREF on, COLORREF off )
{
	m_LcdGridRgb = grid;
	m_LcdOnRgb   = on;
	m_LcdOffRgb  = off;
	SetLcdColor(m_LcdGridRgb, m_LcdGrid, "LcdGrid");
	SetLcdColor(m_LcdOnRgb, m_LcdOn, "LcdOn");
	SetLcdColor(m_LcdOffRgb, m_LcdOff, "LcdOff");
}

void CCustomizeDlg::OnLcdSample1() 
{
	SetupLcdSampleColor(LCD_GRID_1,LCD_ON_1,LCD_OFF_1);
}

void CCustomizeDlg::OnLcdSample2() 
{
	SetupLcdSampleColor(LCD_GRID_2,LCD_ON_2,LCD_OFF_2);	
}

void CCustomizeDlg::OnLcdSample3() 
{
	SetupLcdSampleColor(LCD_GRID_3,LCD_ON_3,LCD_OFF_3);
}

void CCustomizeDlg::OnLcdSample4() 
{
	SetupLcdSampleColor(LCD_GRID_4,LCD_ON_4,LCD_OFF_4);	
}

void CCustomizeDlg::OnLcdSample5() 
{
	SetupLcdSampleColor(LCD_GRID_5,LCD_ON_5,LCD_OFF_5);
}

void CCustomizeDlg::OnLcdSample6() 
{
	SetupLcdSampleColor(LCD_GRID_6,LCD_ON_6,LCD_OFF_6);	
}

void CCustomizeDlg::OnLcdSample7() 
{
	SetupLcdSampleColor(LCD_GRID_7,LCD_ON_7,LCD_OFF_7);
}

void CCustomizeDlg::OnLcdSample8() 
{
	SetupLcdSampleColor(LCD_GRID_8,LCD_ON_8,LCD_OFF_8);
}

void CCustomizeDlg::OnLcdSample9() 
{
	SetupLcdSampleColor(LCD_GRID_9,LCD_ON_9,LCD_OFF_9);
}

void CCustomizeDlg::OnLcdSample10() 
{
	SetupLcdSampleColor(LCD_GRID_10,LCD_ON_10,LCD_OFF_10);
}

void CCustomizeDlg::OnLcdSample11() 
{
	SetupLcdSampleColor(m_LcdGridUserRgb, m_LcdOnUserRgb, m_LcdOffUserRgb);	
}


void CCustomizeDlg::OnRtcTopmost() 
{
	CString cstr;
	cstr.Format("%d", m_RtcTopMost.GetCheck() );
	WritePrivateProfileString("Setting","TopMost",cstr,P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		if( m_RtcTopMost.GetCheck() ){
			::SetWindowPos(P->m_RealTimeClockDlg->m_hWnd,HWND_TOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); 
		}else{
			::SetWindowPos(P->m_RealTimeClockDlg->m_hWnd,HWND_NOTOPMOST,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE); 
		}
	}		
}

void CCustomizeDlg::OnSelectFontMain() 
{
	CString cstr;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	HDC hDC = ::GetDC(NULL);
	int logPixel = GetDeviceCaps(hDC, LOGPIXELSY);
	DeleteDC(hDC);

	char str[256];

	GetPrivateProfileString("Setting","MainFontFace", gDefaultFontFace,str,256,P->m_ini);
	strcpy(lf.lfFaceName,str);
	UINT size = GetPrivateProfileInt("Setting","MainFontSize",gDefaultFontSize,P->m_ini);
	lf.lfHeight = size * logPixel / 72;

	CFontDialog FontDlg(&lf,CF_SCREENFONTS,NULL,NULL);
	if( FontDlg.DoModal() == IDOK ){
		WritePrivateProfileString("Setting","MainFontFace",FontDlg.GetFaceName(),P->m_ini);
		cstr.Format("%d", FontDlg.GetSize() / 10 );
		WritePrivateProfileString("Setting","MainFontSize",cstr,P->m_ini);
		cstr.Format("%d", FontDlg.IsBold() );
		WritePrivateProfileString("Setting","MainFontBold",cstr,P->m_ini);
		cstr.Format("%d", FontDlg.IsItalic());
		WritePrivateProfileString("Setting","MainFontItalic",cstr,P->m_ini);

		// flash
		WritePrivateProfileString(NULL,NULL,NULL,P->m_ini);

		m_FontInfoMain.Format("%s ( %s pt )", lf.lfFaceName, cstr);
		UpdateData(FALSE);

		P->Exit();
	}
}


void CCustomizeDlg::OnSelectFontOther() 
{
	CString cstr;
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	HDC hDC = ::GetDC(NULL);
	int logPixel = GetDeviceCaps(hDC, LOGPIXELSY);
	DeleteDC(hDC);

	char str[256];

	GetPrivateProfileString("Setting","OtherFontFace",gDefaultFontFace,str,256,P->m_ini);
	strcpy(lf.lfFaceName,str);
	UINT size = GetPrivateProfileInt("Setting","OtherFontSize",gDefaultFontSize,P->m_ini);
	lf.lfHeight = size * logPixel / 72;

	CFontDialog FontDlg(&lf,CF_SCREENFONTS,NULL,NULL);
	if( FontDlg.DoModal() == IDOK ){
		WritePrivateProfileString("Setting","OtherFontFace",FontDlg.GetFaceName(),P->m_ini);
		cstr.Format("%d", FontDlg.GetSize() / 10 );
		WritePrivateProfileString("Setting","OtherFontSize",cstr,P->m_ini);
		cstr.Format("%d", FontDlg.IsBold() );
		WritePrivateProfileString("Setting","OtherFontBold",cstr,P->m_ini);
		cstr.Format("%d", FontDlg.IsItalic());
		WritePrivateProfileString("Setting","OtherFontItalic",cstr,P->m_ini);

		m_FontInfoOther.Format("%s ( %s pt )", lf.lfFaceName, cstr);
		UpdateData(FALSE);

		P->Exit();
	}	
}


BOOL CCustomizeDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CCustomizeDlg::OnClose() 
{
	if(P->m_FlagDisableTaskTrayIcon){
		P->ShowWindow(SW_SHOW);
	}

	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CUSTOMIZE,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_CustomizeDlg = NULL;
}

void CCustomizeDlg::OnSelchangeRtcDialogFont() 
{
	CString cstr;
	cstr.Format("%d",m_RtcFontSize.GetCurSel());
	WritePrivateProfileString("Setting","RtcFontSize",cstr,P->m_ini);
	if( P->m_RealTimeClockDlg != NULL ){
		P->m_RealTimeClockDlg->ChangeFontSize(atoi(cstr));
	}	
}


void CCustomizeDlg::OnTtiNormal() 
{
	CFileDialog dlg(TRUE,
		"", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		"Supported Image Files|*.ico;|ICON (*.ico)|*.ico|All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK && LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE))
	{
		P->m_hTaskIcon = (HICON)LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		WritePrivateProfileString("Setting", "TaskTrayIcon", dlg.GetPathName(), P->m_ini);
		m_TtiNormalIcon.SetIcon(P->m_hTaskIcon);
		m_TtiNormalIcon.Invalidate(TRUE);
		P->ModifyTaskTrayIcon(STATUS_UPDATE);
	}
}

void CCustomizeDlg::OnTtiMax() 
{
	CFileDialog dlg(TRUE,
		"", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		"Supported Image Files|*.ico;|ICON (*.ico)|*.ico|All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK && LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE))
	{
		P->m_hTaskIconMax = (HICON)LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		WritePrivateProfileString("Setting", "TaskTrayIconMax", dlg.GetPathName(), P->m_ini);
		m_TtiMaxIcon.SetIcon(P->m_hTaskIconMax);
		m_TtiMaxIcon.Invalidate(TRUE);
		P->ModifyTaskTrayIcon(STATUS_UPDATE);
	}
}

void CCustomizeDlg::OnTtiMid() 
{
	CFileDialog dlg(TRUE,
		"", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		"Supported Image Files|*.ico;|ICON (*.ico)|*.ico|All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK && LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE))
	{
		P->m_hTaskIconMid = (HICON)LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		WritePrivateProfileString("Setting", "TaskTrayIconMid", dlg.GetPathName(), P->m_ini);
		m_TtiMidIcon.SetIcon(P->m_hTaskIconMid);
		m_TtiMidIcon.Invalidate(TRUE);
		P->ModifyTaskTrayIcon(STATUS_UPDATE);
	}
}

void CCustomizeDlg::OnTtiMin() 
{
	CFileDialog dlg(TRUE,
		"", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
		"Supported Image Files|*.ico;|ICON (*.ico)|*.ico|All Files (*.*)|*.*||");
	if(dlg.DoModal() == IDOK && LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE))
	{
		P->m_hTaskIconMin = (HICON)LoadImage(0, dlg.GetPathName(), IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
		WritePrivateProfileString("Setting", "TaskTrayIconMin", dlg.GetPathName(), P->m_ini);
		m_TtiMinIcon.SetIcon(P->m_hTaskIconMin);
		m_TtiMinIcon.Invalidate(TRUE);
		P->ModifyTaskTrayIcon(STATUS_UPDATE);
	}
}

void CCustomizeDlg::OnTtiDefault() 
{
	P->m_hTaskIcon = AfxGetApp()->LoadIcon(IDR_TASKTRAY);
	WritePrivateProfileString("Setting", "TaskTrayIcon", "", P->m_ini);
	m_TtiNormalIcon.SetIcon(P->m_hTaskIcon);
	m_TtiNormalIcon.Invalidate(TRUE);
	P->m_hTaskIconMax = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MAX);
	WritePrivateProfileString("Setting", "TaskTrayIconMax", "", P->m_ini);
	m_TtiMaxIcon.SetIcon(P->m_hTaskIconMax);
	m_TtiMaxIcon.Invalidate(TRUE);
	P->m_hTaskIconMid = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MID);
	WritePrivateProfileString("Setting", "TaskTrayIconMid", "", P->m_ini);
	m_TtiMidIcon.SetIcon(P->m_hTaskIconMid);
	m_TtiMidIcon.Invalidate(TRUE);
	P->m_hTaskIconMin = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MIN);
	WritePrivateProfileString("Setting", "TaskTrayIconMin", "", P->m_ini);
	m_TtiMinIcon.SetIcon(P->m_hTaskIconMin);
	m_TtiMinIcon.Invalidate(TRUE);

	P->ModifyTaskTrayIcon(STATUS_UPDATE);
}

void CCustomizeDlg::OnTtiDisable() 
{
	UpdateData(TRUE);
	CString cstr;
	cstr.Format("%d",m_TtiDisable);
	WritePrivateProfileString("Setting","TaskTrayIconDisable",cstr,P->m_ini);
	if(atoi(cstr)==0){
		P->m_FlagDisableTaskTrayIcon = FALSE;
		P->AddTaskTray();
	}else{
		P->DelTaskTray();
		P->m_FlagDisableTaskTrayIcon = TRUE;
	}	
}
