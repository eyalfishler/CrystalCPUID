/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2008 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CrystalCPUID.h"
#include "CrystalCPUIDDlg.h"
#include <process.h>

#include "../SysInfo/MultiplierTable.h"
#include "../SysInfo/ItemID.h"
#include "../common/CpuUsage.h"
#include "../common/DialogTemplateEx.h"

#include "wbemcli.h"     // WMI interface declarations

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

extern int gDialogID;
extern char gDefaultFontFace[128];
extern int gDefaultFontSize;

static void ExecuteFile(char* FileName);
static int IsCPUID();
void GetCPUID(DWORD id,DWORD* A,DWORD* B,DWORD* C,DWORD* D);

//static void CrystalNQuiet( LPVOID pParam );
unsigned int __stdcall CrystalNQuiet(void*);

void CreateIndirectDialog(CDialog *dlg, int id);
void LoveHammer(CCrystalCPUIDDlg* P, int FID, int VID);
void SetupFVID(CCrystalCPUIDDlg* P, int mode, int* FID, int* VID);

// TaskTray
enum {
	MY_EXIT = (WM_USER + 1),
	MY_SETTING,
	MY_MENUCLOSE,
	MY_MAINDIALOG,
	MY_CRYSTAL_N_QUIET,
	MY_ENABLE_CRYSTAL_N_QUIET,
	MY_CRYSTAL_N_QUIET_MAX,
	MY_CRYSTAL_N_QUIET_MID,
	MY_CRYSTAL_N_QUIET_MIN,
	MY_CRYSTAL_N_QUIET_STARTUP
};

UINT RegMessageID = ::RegisterWindowMessage("CrystalCPUID");
UINT RegIconID = ::RegisterWindowMessage("CrystalCPUIDIcon");
UINT CCrystalCPUIDDlg::wmTaskbarCreated = RegisterWindowMessage("TaskbarCreated");

/////////////////////////////////////////////////////////////////////////////

CCrystalCPUIDDlg::CCrystalCPUIDDlg(CWnd* pParent /*=NULL*/)
//	: CDialog( gDialogID, pParent)
{
	//{{AFX_DATA_INIT(CCrystalCPUIDDlg)
	m_BrandID = _T("");
	m_SystemBusHT = _T("");
	m_CacheL1ITU = _T("");
	m_CacheL1ITUUnit = _T("");
	m_Clock = _T("");
	m_ClockOri = _T("");
	m_CPUName = _T("");
	m_CPUNumber = _T("");
	m_DataRate = _T("");
	m_ExFamily = _T("");
	m_ExFamilyEx = _T("");
	m_ExModel = _T("");
	m_ExStepping = _T("");
	m_Family = _T("");
	m_FamilyEx = _T("");
	m_L1DataCache = _T("");
	m_L1ITUCache = _T("");
	m_L2Cache = _T("");
	m_L2CacheSpeed = _T("");
	m_L2CacheSpeedMHz = _T("");
	m_L3Cache = _T("");
	m_Model = _T("");
	m_Multiplier = _T("");
	m_MultiplierOri = _T("");
	m_NameString = _T("");
	m_OverClock = _T("");
	m_Stepping = _T("");
	m_SystemBus = _T("");
	m_SystemBusOri = _T("");
	m_SystemClock = _T("");
	m_SystemClockOri = _T("");
	m_VendorString = _T("");
	m_HyperThreading = _T("");
	m_Platform = _T("");
	m_CPUType = _T("");
	m_Apic = _T("");
	m_CodeName = _T("");
	m_OS = _T("");
	m_ProcessRule = _T("");
	m_Memory = _T("");
	m_CurrentDate = _T("");
	m_Signature = _T("");
	m_PlatformID = _T("");
	m_MicrocodeID = _T("");
	m_CpuTypeId = _T("");
	m_Architecture = _T("");
	m_MeasureMode = _T("");
	m_CoreNumber = _T("");
	m_ProcessUnit = _T("");
	//}}AFX_DATA_INIT

//	hSysInfoLib = NULL;
	SysInfo = ( (CCrystalCPUIDApp*)AfxGetApp() )->SysInfo;
	m_IsWMI = FALSE;
	m_IsSysInfoInit = FALSE;
	m_SysInfoStatus = 0;

	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);

	if( OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT ){
		m_IsNT = TRUE;
	}else{
		m_IsNT = FALSE;
	}

	m_RealTimeClockDlg = NULL;
	m_DmiInfoDlg = NULL;
	m_BiosInfoDlg = NULL;
	m_FeatureDlg = NULL;
	m_CpuidDlg = NULL;
	m_CustomizeDlg = NULL;
	m_CacheInfoDlg = NULL;
	m_AboutDlg = NULL;
	m_LoveHammerDlg = NULL;
	m_ChipsetInfoDlg = NULL;
	m_PciInfoDlg = NULL;
	m_CrystalNQuietDlg = NULL;	
	m_ProcessorSerialDlg = NULL;
	m_MsrEditDlg = NULL;
	m_MsrWalkerDlg = NULL;
	m_SpeedStepDlg = NULL;


#ifndef _X86_64
	m_LongHaulDlg = NULL;
	m_ProcessorSerialDlg = NULL;
	m_TransmetaInfoDlg = NULL;
#endif

	m_pFont = NULL;
	m_FlagHideDate = FALSE;
	m_FlagHideOriginalClock = FALSE;
	m_FlagShowCustomize = FALSE;
	m_FlagCrystalNQuiet = FALSE;
	m_FlagHaltCnQ = FALSE;
	m_FlagHideWindowOnStart = FALSE;
	m_FlagHideWindowOnStartReal = FALSE;
	m_FlagResident = FALSE;
	m_TrayString = CRYSTAL_CPUID_PRODUCT;
	m_FlagExitNow = FALSE;
	m_LhK7Desktop = FALSE;
	m_LhK8LowVoltage = FALSE;
	m_FlagStopCrystalNQuiet = FALSE;
	m_FlagDisableTaskTrayIcon = FALSE;
	m_CqWaitTime = 0;
}

CCrystalCPUIDDlg::~CCrystalCPUIDDlg()
{
	if( m_pFont != NULL ){delete m_pFont;}
//	DisconnectSysInfo();
}

void CCrystalCPUIDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrystalCPUIDDlg)
	DDX_Control(pDX, IDC_CPU_DUAL_CORE, m_DualCore);
	DDX_Control(pDX, IDC_CPU_OVER_CLOCK, m_OverClockC);
	DDX_Control(pDX, IDC_CPU_CLOCK_ORI, m_ClockOriC);
	DDX_Control(pDX, IDC_CPU_SYSTEM_CLOCK, m_SystemClockC);
	DDX_Control(pDX, IDC_CPU_MULTIPLIER, m_MultiplierC);
	DDX_Control(pDX, IDC_CPU_NX, m_NX);
	DDX_Control(pDX, IDC_CPU_CURRENT_DATE, m_CurrentDateC);
	DDX_Control(pDX, IDC_CPU_IA32E, m_IA32e);
	DDX_Control(pDX, IDC_CPU_LONG_HAUL, m_LongHaul);
	DDX_Control(pDX, IDC_CPU_LONG_RUN, m_LongRun);
	DDX_Control(pDX, IDC_CPU_POWER_NOW, m_PowerNow);
	DDX_Control(pDX, IDC_CPU_SPEED_STEP, m_SpeedStep);
	DDX_Control(pDX, IDC_CPU_CPU_NAME, m_CpuNameC);
	DDX_Control(pDX, IDC_CPU_CLOCK, m_ClockC);
	DDX_Control(pDX, IDC_CPU_CPU_SELECT, m_CPUSelectC);
	DDX_Control(pDX, IDC_CPU_SSE3, m_SSE3);
	DDX_Control(pDX, IDC_CPU_SSSE3, m_SSSE3);
	DDX_Control(pDX, IDC_CPU_SSE41, m_SSE41);
	DDX_Control(pDX, IDC_CPU_SSE42, m_SSE42);
	DDX_Control(pDX, IDC_CPU_SSE4A, m_SSE4A);
	DDX_Control(pDX, IDC_CPU_SSE5, m_SSE5);
	DDX_Control(pDX, IDC_CPU_AVX, m_AVX);
	DDX_Control(pDX, IDC_CPU_INTEL_64, m_IA64);
	DDX_Control(pDX, IDC_CPU_AMD_64, m_AA64);
	DDX_Control(pDX, IDC_CPU_SSE, m_SSE);
	DDX_Control(pDX, IDC_CPU_SSE2, m_SSE2);
	DDX_Control(pDX, IDC_CPU_MMX_EX, m_MMXEx);
	DDX_Control(pDX, IDC_CPU_MMX, m_MMX);
	DDX_Control(pDX, IDC_CPU_3DNOW, m_3DNow);
	DDX_Control(pDX, IDC_CPU_3DNOW_EX, m_3DNowEx);
	DDX_Control(pDX, IDC_CPU_HT, m_HT);
	DDX_Control(pDX, IDC_CPU_VT, m_VT);
	DDX_Control(pDX, IDC_CPU_AMD_V, m_AmdV);
	DDX_Text(pDX, IDC_CPU_BRAND_ID, m_BrandID);
	DDX_Text(pDX, IDC_CPU_SYSTEM_BUS_HT, m_SystemBusHT);
	DDX_Text(pDX, IDC_CPU_CACHE_L1_ITU, m_CacheL1ITU);
	DDX_Text(pDX, IDC_CPU_CACHE_L1_ITU_UNIT, m_CacheL1ITUUnit);
	DDX_Text(pDX, IDC_CPU_CLOCK, m_Clock);
	DDX_Text(pDX, IDC_CPU_CLOCK_ORI, m_ClockOri);
	DDX_Text(pDX, IDC_CPU_CPU_NAME, m_CPUName);
	DDX_Text(pDX, IDC_CPU_DATA_RATE, m_DataRate);
	DDX_Text(pDX, IDC_CPU_EX_FAMILY, m_ExFamily);
	DDX_Text(pDX, IDC_CPU_EX_MODEL, m_ExModel);
	DDX_Text(pDX, IDC_CPU_EX_STEPPING, m_ExStepping);
	DDX_Text(pDX, IDC_CPU_FAMILY, m_Family);
	DDX_Text(pDX, IDC_CPU_FAMILY_EX, m_FamilyEx);
	DDX_Text(pDX, IDC_CPU_L1_D_CACHE, m_L1DataCache);
	DDX_Text(pDX, IDC_CPU_L1_ITU_CACHE, m_L1ITUCache);
	DDX_Text(pDX, IDC_CPU_L2_CACHE, m_L2Cache);
	DDX_Text(pDX, IDC_CPU_L2_CACHE_SPEED, m_L2CacheSpeed);
	DDX_Text(pDX, IDC_CPU_L2_CACHE_SPEED_MHZ, m_L2CacheSpeedMHz);
	DDX_Text(pDX, IDC_CPU_L3_CACHE, m_L3Cache);
	DDX_Text(pDX, IDC_CPU_MODEL, m_Model);
	DDX_Text(pDX, IDC_CPU_MODEL_EX, m_ModelEx);
	DDX_Text(pDX, IDC_CPU_MULTIPLIER, m_Multiplier);
	DDX_Text(pDX, IDC_CPU_MULTIPLIER_ORI, m_MultiplierOri);
	DDX_Text(pDX, IDC_CPU_NAME_STRING, m_NameString);
	DDX_Text(pDX, IDC_CPU_OVER_CLOCK, m_OverClock);
	DDX_Text(pDX, IDC_CPU_STEPPING, m_Stepping);
	DDX_Text(pDX, IDC_CPU_SYSTEM_BUS, m_SystemBus);
	DDX_Text(pDX, IDC_CPU_SYSTEM_BUS_ORI, m_SystemBusOri);
	DDX_Text(pDX, IDC_CPU_SYSTEM_CLOCK, m_SystemClock);
	DDX_Text(pDX, IDC_CPU_SYSTEM_CLOCK_ORI, m_SystemClockOri);
	DDX_Text(pDX, IDC_CPU_VENDOR_STRING, m_VendorString);
	DDX_Text(pDX, IDC_CPU_HYPER_THREADING, m_HyperThreading);
	DDX_Text(pDX, IDC_CPU_PLATFORM, m_Platform);
	DDX_Text(pDX, IDC_CPU_TYPE, m_CPUType);
	DDX_Text(pDX, IDC_CPU_APIC, m_Apic);
	DDX_Text(pDX, IDC_CPU_CODE_NAME, m_CodeName);
	DDX_Text(pDX, IDC_CPU_OS, m_OS);
	DDX_Text(pDX, IDC_CPU_PROCESS_RULE, m_ProcessRule);
	DDX_Text(pDX, IDC_CPU_MEMORY, m_Memory);
	DDX_Text(pDX, IDC_CPU_CURRENT_DATE, m_CurrentDate);
	DDX_Text(pDX, IDC_CPU_SIGNATURE, m_Signature);
	DDX_Text(pDX, IDC_CPU_PLATFORM_ID, m_PlatformID);
	DDX_Text(pDX, IDC_CPU_MICROCODE_ID, m_MicrocodeID);
	DDX_Text(pDX, IDC_CPU_TYPE_ID, m_CpuTypeId);
	DDX_Text(pDX, IDC_CPU_ARCHITECTURE, m_Architecture);
	DDX_Text(pDX, IDC_CPU_MEASURE_MODE, m_MeasureMode);
	DDX_Text(pDX, IDC_CPU_CORE_NUMBER, m_CoreNumber);
	DDX_Text(pDX, IDC_CPU_PROCESS_UNIT, m_ProcessUnit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCrystalCPUIDDlg, CDialog)
	//{{AFX_MSG_MAP(CCrystalCPUIDDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_CPU_CPU_SELECT, OnSelchangeCpuSelect)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(IDM_REFRESH, OnRefresh)
	ON_COMMAND(IDM_CRYSTAL_DEW_WORLD, OnCrystalDewWorld)
	ON_COMMAND(IDM_CUSTOMIZE, OnCustomize)
	ON_COMMAND(IDM_CPU_INFO_TEXT, OnCpuInfoText)
	ON_COMMAND(IDM_CRYSTAL_CPUID_TEXT, OnCrystalCpuidText)
	ON_COMMAND(IDM_ENGLISH, OnEnglish)
	ON_COMMAND(IDM_JAPANESE, OnJapanese)
	ON_COMMAND(IDM_FEATURE_FLAGS, OnFeatureFlags)
	ON_COMMAND(IDM_CPUID_DETAIL, OnCpuidDetail)
	ON_COMMAND(IDM_LICENSE, OnLicense)
	ON_COMMAND(IDM_READ_ME, OnReadMe)
	ON_WM_CTLCOLOR()
	ON_COMMAND(IDM_LOVE_HAMMER, OnLoveHammer)
	ON_COMMAND(IDM_SPEED_STEP, OnSpeedStep)
	ON_COMMAND(IDM_HIDE_ORIGINAL_CLOCK, OnHideOriginalClock)
	ON_COMMAND(IDM_HIDE_DATE, OnHideDate)
	ON_COMMAND(IDM_SAVE_IMAGE, OnSaveImage)
	ON_COMMAND(IDM_PREVIEW_TEXT, OnPreviewText)
	ON_COMMAND(IDM_COPY_TEXT, OnCopyText)
	ON_COMMAND(IDM_SAVE_TEXT, OnSaveText)
	ON_COMMAND(IDM_QUICK_SAVE_IMAGE, OnQuickSaveImage)
	ON_COMMAND(IDM_BIOS_INFO, OnBiosInfo)
	ON_COMMAND(IDM_MSR_EDIT, OnMsrEdit)
	ON_COMMAND(IDM_REAL_TIME_CLOCK, OnRealTimeClock)
	ON_COMMAND(IDM_ABOUT, OnAbout)
#ifndef _X86_64
	ON_COMMAND(IDM_TRANSMETA_INFO, OnTransmetaInfo)
	ON_COMMAND(IDM_PROCESSOR_SERIAL, OnProcessorSerial)
	ON_COMMAND(IDM_LONG_HAUL, OnLongHaul)
#endif
	ON_COMMAND(IDM_CACHE_INFO, OnCacheInfo)
	ON_COMMAND(IDM_REFRESH_MMT, OnRefreshMmt)
	ON_COMMAND(IDM_REFRESH_QPC, OnRefreshQpc)
	ON_COMMAND(IDM_REFRESH_WT, OnRefreshWt)
	ON_COMMAND(IDM_REFRESH_NOLOAD, OnRefreshNoload)
	ON_COMMAND(IDM_DMI_INFO, OnDmiInfo)
	ON_COMMAND(IDM_PCI_INFO, OnPciInfo)
	ON_COMMAND(IDM_CHIPSET_INFO, OnChipsetInfo)
	ON_COMMAND(IDM_ENABLE_CRYSTAL_N_QUIET, OnEnableCrystalNQuiet)
	ON_COMMAND(IDM_CRYSTAL_N_QUIET, OnCrystalNQuiet)
	ON_WM_WINDOWPOSCHANGING()
	ON_COMMAND(IDM_CRYSTAL_CPUID_INI, OnCrystalCpuidIni)
	ON_COMMAND(IDM_MSR_WALKER, OnMsrWalker)
//	ON_COMMAND(IDM_CPU_MARK_2005, OnCpuMark2005)
	ON_WM_TIMER()
	ON_MESSAGE(WM_POWERBROADCAST,OnPowerBroadcast)
	ON_COMMAND(IDM_CPU_INFO_HISTORY_TEXT, OnCpuInfoHistoryText)
	ON_COMMAND(IDM_CRYSTAL_CPUID_HISTORY_TEXT, OnCrystalCpuidHistoryText)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(RegMessageID,OnRegMessage)
	ON_REGISTERED_MESSAGE(wmTaskbarCreated, OnTaskbarCreated)

END_MESSAGE_MAP()

/*
/////////////////////////////////////////////////////////////////////////////
// SysInfo.dll Load & Unload
/////////////////////////////////////////////////////////////////////////////

typedef ISysInfo* (*_CreateSysInfo) (DWORD);
typedef void (*_DestroySysInfo) (ISysInfo*);

BOOL CCrystalCPUIDDlg::ConnectSysInfo(DWORD Mode)
{
	hSysInfoLib = LoadLibrary("SysInfo.dll");
	if(hSysInfoLib != NULL){
		_CreateSysInfo pCreateSysInfo = (_CreateSysInfo) GetProcAddress (hSysInfoLib,"CreateSysInfo");
		SysInfo = pCreateSysInfo(Mode);
		return TRUE;
	}else{
		MessageBox("Not found \"SysInfo.dll\".");
		exit(0);
		return FALSE;
	}
}

BOOL CCrystalCPUIDDlg::DisconnectSysInfo()
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
*/
/////////////////////////////////////////////////////////////////////////////
// OnInitDialog
/////////////////////////////////////////////////////////////////////////////
BOOL CCrystalCPUIDDlg::OnInitDialog()
{
	DebugInfo("CDialog::OnInitDialog()");
	CDialog::OnInitDialog();
	char str[256];
	int FID = -1;
	int VID = -1;
	int i;
	DWORD data;
	m_Mask = 0;

	m_LhType = -1;
	m_LhTableType = -1;
	m_CqEnableVoltage = FALSE;
	m_hThread = NULL;

	m_path = ((CCrystalCPUIDApp*)AfxGetApp())->m_path;
	m_ini = ((CCrystalCPUIDApp*)AfxGetApp())->m_ini;

	SYSTEM_INFO si;
	GetSystemInfo(&si);

	// Select Processor
	for(i=0; i<__argc; i++)
    {
		if( ! _strnicmp(__argv[i],"-P",2) || ! _strnicmp(__argv[i],"/P",2) ){
			m_Mask = atoi(&__argv[i][2]) - 1;
			if( m_Mask >= si.dwNumberOfProcessors ){
				m_Mask = 0;
			}
			break;
		}
    }
	if( m_IsNT ){SetThreadAffinityMask(GetCurrentThread(), 1 << m_Mask );}

//	ConnectSysInfo( MODE_PCI | MODE_DMI );

	m_hAccelerator = ::LoadAccelerators(AfxGetInstanceHandle(),
		                                MAKEINTRESOURCE(IDR_ACCELERATOR));
	m_hIcon = AfxGetApp()->LoadIcon(IDR_TASKTRAY);//AfxGetApp()->LoadIcon(IDR_MAINFRAME);

///////////
	GetPrivateProfileString("Setting", "TaskTrayIcon", "0", str, 256, m_ini);
	m_hTaskIcon = (HICON)LoadImage(0, str, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	if(m_hTaskIcon == NULL){m_hTaskIcon = AfxGetApp()->LoadIcon(IDR_TASKTRAY);}
	GetPrivateProfileString("Setting", "TaskTrayIconMax", "0", str, 256, m_ini);
	m_hTaskIconMax = (HICON)LoadImage(0, str, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	if(m_hTaskIconMax == NULL){m_hTaskIconMax = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MAX);}
	GetPrivateProfileString("Setting", "TaskTrayIconMid", "0", str, 256, m_ini);
	m_hTaskIconMid = (HICON)LoadImage(0, str, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	if(m_hTaskIconMid == NULL){m_hTaskIconMid = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MID);}
	GetPrivateProfileString("Setting", "TaskTrayIconMin", "0", str, 256, m_ini);
	m_hTaskIconMin = (HICON)LoadImage(0, str, IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
	if(m_hTaskIconMin == NULL){m_hTaskIconMin = AfxGetApp()->LoadIcon(IDR_TASKTRAY_MIN);}

	SetIcon(m_hIcon, TRUE);
//	SetIcon(m_hIcon, FALSE);
	

// Background Image Setting
	GetPrivateProfileString("Setting", "Gray", "0", str, 256, m_ini);
	BOOL flagGray = atoi(str);
	GetPrivateProfileString("Setting", "BackgroundImage", "", str, 256, m_ini);
	CreateWallPaper(1, NULL, str, 0, flagGray);

	GetPrivateProfileString("Setting", "TextColor", "", str, 256, m_ini);
	if( strcmp(str,"") == 0 ){
		gTextColor = ::GetSysColor(COLOR_WINDOWTEXT);
	}else{
		gTextColor = atoi(str);
	}

	DebugInfo("Refresh( 0 )");

	try{
		Refresh( 0 );
	}catch(...){
		DebugInfo("Exception:Refresh( 0 )");	
	}

// Init Menu 
	DebugInfo("CMenu *menu = GetMenu()");
	CMenu *menu = GetMenu();
	DebugInfo("menu->RemoveMenu(SC_MAXIMIZE, FALSE)");
	menu->RemoveMenu(SC_MAXIMIZE, FALSE);
	menu->RemoveMenu(SC_SIZE, FALSE);

	DebugInfo("IsCPUID()");
	if( ! IsCPUID() ){
		menu->EnableMenuItem(IDM_FEATURE_FLAGS,MF_GRAYED);
		menu->EnableMenuItem(IDM_CPUID_DETAIL,MF_GRAYED);
	}

	
	DebugInfo("SysInfo->GetData(SI_STATUS,&m_SysInfoStatus)");
	SysInfo->GetData(SI_STATUS,&m_SysInfoStatus);
	if( m_SysInfoStatus == 0 /* DLLSTATUS_NOERROR */ 
	|| m_SysInfoStatus == 4 /* DLLSTATUS_MODE_X86_64 */){
		m_SysInfoStatus = 0;
		SysInfo->GetData(CPU_FLAG_MSR,&data);
		if( data != 0 ){
			menu->EnableMenuItem(IDM_MSR_EDIT,MF_ENABLED);
			if( m_IsNT ){
				menu->EnableMenuItem(IDM_MSR_WALKER,MF_ENABLED);
			}
		}
		menu->EnableMenuItem(IDM_PCI_INFO,MF_ENABLED);
		menu->EnableMenuItem(IDM_CHIPSET_INFO,MF_ENABLED);
		m_IsSysInfoInit = TRUE;
	}else{
		MessageBox(ERROR_MESSAGE,ERROR_CAPTION,MB_OK);
	}

	DebugInfo("SysInfo->GetData(DMI_STATUS,&data)");
	SysInfo->GetData(DMI_STATUS,&data);
	if( data == TRUE ){
		menu->EnableMenuItem(IDM_DMI_INFO,MF_ENABLED);
	}

	DebugInfo("SysInfo->GetString(CPU_CLOCK, str)");
	SysInfo->GetString(CPU_CLOCK, str);
	if( atof(str) > 0 ){
		menu->EnableMenuItem(IDM_REAL_TIME_CLOCK,MF_ENABLED);
	}

	DebugInfo("SysInfo->GetData(CPU_FLAG_PROCESSOR_SERIAL,&data)");
	SysInfo->GetData(CPU_FLAG_PROCESSOR_SERIAL,&data);
	if( data == TRUE ){
		menu->EnableMenuItem(IDM_PROCESSOR_SERIAL,MF_ENABLED);
	}

	DebugInfo("SysInfo->GetString(CPU_VENDOR_STRING, str)");
	SysInfo->GetString(CPU_VENDOR_STRING, str);
	if( strcmp(str,"GenuineTMx86") == 0 ){
		menu->EnableMenuItem(IDM_TRANSMETA_INFO,MF_ENABLED);
		// Transmeta Information
		//CString cstr;
		m_TransmetaInfo = "\r\n";
		SysInfo->GetString(CPU_TM_HARDWARE_VERSION,str);
		cstr.Format("TM Hardware Ver.: %s\r\n",str);
		m_TransmetaInfo += cstr;
		SysInfo->GetString(CPU_TM_SOFTWARE_VERSION,str);
		cstr.Format("TM Software Ver.: %s\r\n",str);
		m_TransmetaInfo += cstr;
		SysInfo->GetString(CPU_TM_NAME_STRING,str);
		cstr.Format(" TM Name String : %s\r\n",str);
		m_TransmetaInfo += cstr;
	}

	DebugInfo("BOOL FlagExit = FALSE");
	BOOL FlagExit = FALSE;
	BOOL FlagRealTimeClock = FALSE;
	BOOL FlagCrystalNQuiet = FALSE;

	for(i=0; i<__argc; i++)
    {
		if( ! _stricmp(__argv[i],"-E") || ! _stricmp(__argv[i],"/E") ){
			FlagExit = TRUE;
		}
        
		if( ! _stricmp(__argv[i],"-Q") || ! _stricmp(__argv[i],"/Q") ){
			CenterWindow();
			ShowWindow(SW_SHOW);
			ImageCopy( 3 /* Quick Save */ );
		}

		if( ! _stricmp(__argv[i],"-R") || ! _stricmp(__argv[i],"/R") ){
			FlagRealTimeClock = TRUE;
		}

		if( ! _stricmp(__argv[i],"-S") || ! _stricmp(__argv[i],"/S") ){
			TextCopy( 4 /* Quick Save */ );
		}
		
		if( ! _strnicmp(__argv[i],"-F",2) || ! _strnicmp(__argv[i],"/F",2) ){
			FID = atoi(&__argv[i][2]);
		}
		
		if( ! _strnicmp(__argv[i],"-V",2) || ! _strnicmp(__argv[i],"/V",2) ){
			VID = atoi(&__argv[i][2]);
		}

		if( ! _strnicmp(__argv[i],"-CQ",3) || ! _strnicmp(__argv[i],"/CQ",3) ){
			FlagCrystalNQuiet = TRUE;
		}
		if( ! _strnicmp(__argv[i],"-HIDE",5) || ! _strnicmp(__argv[i],"/HIDE",5) ){
			m_FlagHideWindowOnStart = TRUE;
			m_FlagHideWindowOnStartReal = TRUE;
		}
		if( ! _strnicmp(__argv[i],"-RESI",5) || ! _strnicmp(__argv[i],"/RESI",5) ){
			m_FlagResident = TRUE;
		}

		// for VIA VT-310DP
		if( ! _strnicmp(__argv[i],"-VT310DP",8) || ! _strnicmp(__argv[i],"/VT310DP",8) ){
			SysInfo->SetData(SET_VIA_VT_310DP_PIPELINE, 0);
		}
    }

	////////////////////////////////////////////////
	// Love Hammer & LongHaul Change Multiplier
	////////////////////////////////////////////////
	DebugInfo("Love Hammer & LongHaul Change Multiplier");
	int fid, vid;
	SysInfo->GetData(LH_GET_TYPE, (DWORD*)&m_LhType);
	SysInfo->GetData(LH_GET_TABLE_TYPE, (DWORD*)&m_LhTableType);

	GetPrivateProfileString("Setting", "CqWaitTime", "0", str, 256, m_ini);
	if( abs(atoi(str)) > 100 ){
		m_CqWaitTime = 100;
	}else{
		m_CqWaitTime = abs(atoi(str));
	}

	if( m_LhType == 7 ){
		// Set K7 Desktop Voltage
		for(i=0; i<__argc; i++)
		{
			if( ! _strnicmp(__argv[i],"-K7D",4) || ! _strnicmp(__argv[i],"/K7D",4) ){
				SysInfo->SetData(LH_SET_K7_DESKTOP, TRUE);
				m_LhK7Desktop = TRUE;
				break;
			}
		}
		GetPrivateProfileString("Setting", "K7DesktopVoltage", "0", str, 256, m_ini);
		if(atoi(str) > 0){
			SysInfo->SetData(LH_SET_K7_DESKTOP, TRUE);
			m_LhK7Desktop = TRUE;
		}
	}

	if( m_LhType == 8 ){
		// Set K8 Low Voltage
		for(i=0; i<__argc; i++)
		{
			if( ! _strnicmp(__argv[i],"-K8LV",4) || ! _strnicmp(__argv[i],"/K8LV",4) ){
				m_LhK8LowVoltage = TRUE;
				SysInfo->SetData(LH_SET_K8_LOW_VOLTAGE, TRUE);
				break;
			}
		}
		GetPrivateProfileString("Setting", "K8LowVoltage", "0", str, 256, m_ini);
		if(atoi(str) > 0){
			m_LhK8LowVoltage = TRUE;
			SysInfo->SetData(LH_SET_K8_LOW_VOLTAGE, TRUE);
		}
	}

	if( m_SysInfoStatus == 0 /* DLLSTATUS_NOERROR */ ){
		switch ( m_LhType )
		{
		case LONG_HAUL_LEVEL_1:
			if( 0 <= FID && FID <= 15 ){
				SysInfo->SetData(LH_SET_FID, FID);
			}
			menu->EnableMenuItem(IDM_LONG_HAUL,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			break;
		case LONG_HAUL_LEVEL_2:
			if( 0 <= FID && FID <= 31 ){
				SysInfo->SetData(LH_SET_FID, FID);
			}
			menu->EnableMenuItem(IDM_LONG_HAUL,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			break;

		case LOVE_HAMMER_GEODE_LX:
			if( 0 <= FID && FID <= 31 ){
				SysInfo->SetData(LH_SET_FID, FID);
			}
			menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_ENABLED);
		//	menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
		//	menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			break;
		case LOVE_HAMMER_K6:
			if( 0 <= FID && FID <= 7 ){
				SysInfo->SetData(LH_SET_FID, FID);
			}
			menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			break;
		case LOVE_HAMMER_K7:
			{
			BOOL flag = FALSE;
			double multi;
			char str[256];
			SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
			SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
			SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
			multi = atof( SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str) );
			if( (MultiTableMobileAthlon[FID] / 2.0) >= multi ){
				if( 0 <= VID && VID <= 30 && VID != vid ){
					SysInfo->SetData(LH_SET_VID, VID);
					flag = TRUE;
				}
				if( 0 <= FID && FID <= 31 && FID != fid ){
					if(flag){Sleep(m_CqWaitTime);}
					SysInfo->SetData(LH_SET_FID, FID);
				}
			}else{
				if( 0 <= FID && FID <= 31 && FID != fid ){
					SysInfo->SetData(LH_SET_FID, FID);
					flag = TRUE;
				}
				if( 0 <= VID && VID <= 30 && VID != vid ){
					if(flag){Sleep(m_CqWaitTime);}
					SysInfo->SetData(LH_SET_VID, VID);
				}
			}
			menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			}
			break;
		case LOVE_HAMMER_K8:
			{
			BOOL flag = FALSE;
			SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
			SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
			SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
			if( FID >= fid ){
				if( 0 <= VID && VID <= 31 && VID != vid ){
					SysInfo->SetData(LH_SET_VID, VID);
					flag = TRUE;
					SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
					SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
				}
				if( 0 <= FID && FID <= 31 && FID != fid ){
					if(flag){Sleep(m_CqWaitTime);}
					SysInfo->SetData(LH_SET_FID, FID);
				}
			}else{
				if( 0 <= FID && FID <= 31 && FID != fid ){
					SysInfo->SetData(LH_SET_FID, FID);
					flag = TRUE;
					SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
					SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
				}
				if( 0 <= VID && VID <= 31 && VID != vid ){
					if(flag){Sleep(m_CqWaitTime);}
					SysInfo->SetData(LH_SET_VID, VID);
				}
			}
			menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
			}
			break;
		case SPEED_STEP_PM:
		case SPEED_STEP_P4:
		case SPEED_STEP_CORE_MA:
		case SPEED_STEP_PENRYN:
			{
			BOOL flag = FALSE;
			SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
			SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
			SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
			menu->EnableMenuItem(IDM_SPEED_STEP,MF_ENABLED);
			menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
			menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);

			int f, v;
			if( FID == fid && VID == vid ){
				break;
			}
			if( FID == -1 && VID == -1 ){
				break;
			}
			if( FID != -1 ){
				f = FID;
			}else{
				f = fid;
			}
			if( VID != -1 ){
				v = VID;
			}else{
				v = vid;
			}
			DWORD fvid = m_LhType >= SPEED_STEP_PENRYN
					? (((f / 2 + 6) << 8) | ((f % 2) << 14))
					: (((f + 6) << 8) | v);
			SysInfo->SetData(LH_SET_FVID, fvid);

			}
			break;
		default:
			break;
		}
	}

  DebugInfo("SetMenu(menu)");
// Edit Menu for x64 Edition
#ifdef _X86_64
	menu->DeleteMenu(IDM_LONG_HAUL, MF_BYCOMMAND);
	menu->DeleteMenu(IDM_TRANSMETA_INFO, MF_BYCOMMAND);
	menu->DeleteMenu(IDM_PROCESSOR_SERIAL, MF_BYCOMMAND);
#endif
	
	SetMenu(menu);

	if( FlagExit ){
		OnCancel();
		return FALSE;
	}

	DebugInfo("Crystal'n'Quiet");
	// Crystal'n'Quiet
	GetPrivateProfileString("Setting","CqMidThresholdUp","60",str,256,m_ini);		m_CqMidThresholdUp = atoi(str);
	if( m_CqMidThresholdUp <= 0 || m_CqMidThresholdUp > 100){m_CqMidThresholdUp = 60;}
	GetPrivateProfileString("Setting","CqMinThresholdUp","60",str,256,m_ini);		m_CqMinThresholdUp = atoi(str);
	if( m_CqMinThresholdUp <= 0 || m_CqMinThresholdUp > 100){m_CqMinThresholdUp = 60;}
	GetPrivateProfileString("Setting","CqMaxThresholdDown","30",str,256,m_ini);	m_CqMaxThresholdDown = atoi(str);
	if( m_CqMaxThresholdDown <= 0 || m_CqMaxThresholdDown > 100){m_CqMaxThresholdDown = 30;}
	GetPrivateProfileString("Setting","CqMidThresholdDown","30",str,256,m_ini);	m_CqMidThresholdDown = atoi(str);
	if( m_CqMidThresholdDown <= 0 || m_CqMidThresholdDown > 100){m_CqMidThresholdDown = 30;}
	GetPrivateProfileString("Setting","CqMaxIntervalTime","1000",str,256,m_ini);m_CqMaxIntervalTime = atoi(str);
	if( m_CqMaxIntervalTime <= 0 || m_CqMaxIntervalTime > 10000){m_CqMaxIntervalTime = 1000;}
	GetPrivateProfileString("Setting","CqMidIntervalTime","100",str,256,m_ini);	m_CqMidIntervalTime = atoi(str);
	if( m_CqMidIntervalTime <= 0 || m_CqMidIntervalTime > 10000){m_CqMidIntervalTime = 100;}
	GetPrivateProfileString("Setting","CqMinIntervalTime","100",str,256,m_ini);	m_CqMinIntervalTime = atoi(str);
	if( m_CqMinIntervalTime <= 0 || m_CqMinIntervalTime > 10000){m_CqMinIntervalTime = 100;}

	GetPrivateProfileString("Setting","CqModeUpType","1",str,256,m_ini);	m_CqModeUpType = atoi(str);
	if( m_CqModeUpType != 0 ){m_CqModeUpType = 1;}
	GetPrivateProfileString("Setting","CqModeDownType","0",str,256,m_ini);	m_CqModeDownType = atoi(str);
	if( m_CqModeDownType != 0 ){m_CqModeDownType = 1;}

	GetPrivateProfileString("Setting","CqMaxFID","-1",str,256,m_ini);m_CqMaxFID = atoi(str);
	GetPrivateProfileString("Setting","CqMinFID","-1",str,256,m_ini);m_CqMinFID = atoi(str);
	GetPrivateProfileString("Setting","CqMidFID","-1",str,256,m_ini);m_CqMidFID = atoi(str);
	GetPrivateProfileString("Setting","CqMaxVID","-1",str,256,m_ini);m_CqMaxVID = atoi(str);
	GetPrivateProfileString("Setting","CqMinVID","-1",str,256,m_ini);m_CqMinVID = atoi(str);
	GetPrivateProfileString("Setting","CqMidVID","-1",str,256,m_ini);m_CqMidVID = atoi(str);
	GetPrivateProfileString("Setting","CqEnableVoltage","0",str,256,m_ini);
	if( atoi(str) > 0 ){
		m_CqEnableVoltage = TRUE;
	}

	GetPrivateProfileString("Setting","CqTrigerType","-2",str,256,m_ini);
	int temp = atoi(str);
	if( temp < CPU_USAGE_MAX || temp > (int)si.dwNumberOfProcessors){
		temp = CPU_USAGE_MAX;
	}
	m_CqTrigerType = temp;

	GetPrivateProfileString("Setting","CqExitMode","0",str,256,m_ini);
	temp = atoi(str);
	if( temp < 0 || temp > 3 ){
		temp = 0;
	}
	m_CqExitMode = temp;

	if( FlagCrystalNQuiet && m_LhType > 0 ){
		OnEnableCrystalNQuiet();
	}

	// Font Setting
	DebugInfo("Font Setting");
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	GetFont()->GetLogFont(&lf);
	GetPrivateProfileString("Setting","MainFontBold","0",str,256,m_ini);
	if( atoi(str) > 0 ){lf.lfWeight = FW_BOLD;}
	GetPrivateProfileString("Setting","MainFontItalic","0",str,256,m_ini);
	if( atoi(str) > 0 ){lf.lfItalic = TRUE;}

	if( lf.lfWeight || lf.lfItalic ){
		m_pFont = new CFont;
		/*
		GetPrivateProfileString("Setting","FontUnderline","0",str,256,m_ini);
		if( atoi(str) > 0 ){lf.lfUnderline = TRUE;}
		GetPrivateProfileString("Setting","FontStrikeOut","0",str,256,m_ini);
		if( atoi(str) > 0 ){lf.lfStrikeOut = TRUE;}
		*/
		m_pFont->CreateFontIndirect(&lf);
	}

	DebugInfo("Refresh( -1 )");
	try{
		Refresh( -1 );
	}catch(...){
		DebugInfo("Exception:Refresh( -1 )");	
	}

	GetPrivateProfileString("Setting","TaskTrayIconDisable","0",str,MAX_PATH,m_ini);
	if(atoi(str) == 0){
		m_FlagDisableTaskTrayIcon = FALSE;
	}else{
		m_FlagDisableTaskTrayIcon = TRUE;
		m_FlagHideWindowOnStart = FALSE;
		m_FlagHideWindowOnStartReal = FALSE;
		m_FlagResident = FALSE;
	}
	
	DebugInfo("AddTaskTray()");
	AddTaskTray();

	SysInfo->GetString(SI_VERSION,str);
	wsprintf(CrystalCPUIDVersion,"%s.%s%s", CRYSTAL_CPUID_VERSION, str, CRYSTAL_CPUID_STATUS);
	cstr.Format("%s %s", CRYSTAL_CPUID_PRODUCT, CrystalCPUIDVersion);
	SetWindowText(cstr);

//	ShowWindow(SW_HIDE);
	CenterWindow();
/*	
	if( m_FlagHideWindowOnStart ){
		ShowWindow(SW_HIDE);
	}else{
		ShowWindow(SW_SHOW);
	}
*/	
//	BringWindowToTop();
	if( FlagRealTimeClock ){
		OnRealTimeClock();
	}

	if( m_FlagShowCustomize ){
		OnCustomize();
	}

// Set Language
	if( gLanguage == ENGLISH ){
		OnEnglish();
	}else{
		OnJapanese();
	}

	// CStaticTT
	// m_CurrentDateC.EnableBalloonTooltip();
	// m_CurrentDateC.SetTooltipText("YYYY/MM/DD hh:mm:ss",TRUE);

	return TRUE;
}

void CCrystalCPUIDDlg::OnPaint() 
{
	CDialog::OnPaint();
}

HCURSOR CCrystalCPUIDDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CCrystalCPUIDDlg::OnDestroy() 
{
	DelTaskTray();

	m_FlagExitNow = TRUE;
	m_FlagCrystalNQuiet = FALSE;

/*
	while( m_hThread ){
		Sleep(100);
	}	
	if( m_FlagCrystalNQuiet ){
		int FID = -1, VID = -1;
		SetupFVID(this, m_CqExitMode, &FID, &VID);
		LoveHammer(this, FID, VID);
	}
	*/
	WaitForSingleObject(m_hThread, 10000);
	CloseHandle( m_hThread );

	CDialog::OnDestroy();
}

void CCrystalCPUIDDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CCrystalCPUIDDlg::OnCancel() 
{
	if( m_FlagResident == TRUE ){
		ShowWindow( SW_HIDE );
	}else{
		CDialog::OnCancel();
	}
}

/////////////////////////////////////////////////////////////////////////////
// Refresh All Info
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::Refresh(DWORD TimerType)
{
	m_CPUName = "";  
	m_VendorString = "";  
	m_NameString = "";  
	m_DataRate = "";  
	m_Clock = "";
	m_SystemBus = "";    
	m_SystemClock = "";    
	m_Multiplier = "";    
	m_ClockOri = "";    
	m_SystemClockOri = "";    
	m_SystemBusOri = "";  
	m_MultiplierOri = "";  
	m_CPUNumber = "";  
	m_Family = "";  
	m_Model = "";  
	m_Stepping = "";  
	m_ExFamily = "";  
	m_FamilyEx = "";  
	m_ExModel = "";  
	m_ModelEx = "";  
	m_ExStepping = "";  
	m_OverClock = "";  
	m_L1ITUCache = "";  
	m_L1DataCache = "";  
	m_L2Cache = "";  
	m_L3Cache = "";
	m_L2CacheSpeedMHz = "";
	m_L2CacheSpeed = "";
	m_Memory = "";
	m_Apic = "";
	m_BrandID = "";
	m_Platform = "";
	m_CPUType = "";
	m_CodeName = "";
	m_ProcessRule = "";
	m_OS = "";
	m_HyperThreading = "";
	m_CurrentDate = "";
	m_Signature = "";
	m_PlatformID = "";
	m_MicrocodeID = "";
	m_CpuTypeId = "";
	m_Architecture = "";
	m_MeasureMode = "";
	m_ProcessUnit = "";
	m_CoreNumber = "";

	char str[256];

	m_MMX.EnableWindow(FALSE);
	m_MMXEx.EnableWindow(FALSE);
	m_SSE.EnableWindow(FALSE);
	m_SSE2.EnableWindow(FALSE);
	m_SSE3.EnableWindow(FALSE);
	m_SSSE3.EnableWindow(FALSE);
	m_SSE41.EnableWindow(FALSE);
	m_SSE42.EnableWindow(FALSE);
	m_SSE4A.EnableWindow(FALSE);
	m_SSE5.EnableWindow(FALSE);
	m_AVX.EnableWindow(FALSE);
	m_3DNow.EnableWindow(FALSE);
	m_3DNowEx.EnableWindow(FALSE);
	m_HT.EnableWindow(FALSE);
	m_VT.EnableWindow(FALSE);
	m_AmdV.EnableWindow(FALSE);
	m_IA64.EnableWindow(FALSE);
	m_AA64.EnableWindow(FALSE);

	m_SpeedStep.EnableWindow(FALSE);
	m_PowerNow.EnableWindow(FALSE);
	m_LongHaul.EnableWindow(FALSE);
	m_LongRun.EnableWindow(FALSE);
	m_IA32e.EnableWindow(FALSE);
	m_NX.EnableWindow(FALSE);
	m_DualCore.EnableWindow(FALSE);

	UpdateData(FALSE);
	InvalidateRect(NULL,TRUE);
	UpdateWindow();
	DebugInfo("BeginWaitCursor()");
	BeginWaitCursor();

// Current Date
	if( ! m_FlagHideDate ){
		SYSTEMTIME st;
		GetLocalTime(&st);
		m_CurrentDate.Format("%04d/%02d/%02d %d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	}
// Signature
	m_Signature = "Copyright (C) 2002-2008 hiyohiyo";

// CPU Info
	int data;
	DWORD Version,VersionEx;

	if( TimerType != -1 ){
		DebugInfo("SysInfo->GetData(CPU_UPDATE_BASE + m_Mask, &TimerType)");
		SysInfo->GetData(CPU_UPDATE_BASE + m_Mask, &TimerType);
	}

	CString OSName,OSType,OSCSD,OSVersion,OSBuild,OSInfo;
	DebugInfo("GetOSInfo(OSName,OSType,OSCSD,OSVersion,OSBuild,OSInfo)");
	GetOSInfo(OSName,OSType,OSCSD,OSVersion,OSBuild,OSInfo);
	
	m_OS = OSName;
	if( ! OSType.IsEmpty() ){
		m_OS += " " + OSType;
	}
	if( ! OSCSD.IsEmpty() ){
		m_OS += " " + OSCSD;
	}
	m_OS += " [ " + OSVersion + " Build " + OSBuild + " ]";
	m_OS.Replace("Service Pack ","SP");

	DebugInfo("SysInfo->GetString(CPU_VENDOR_STRING,str)");
	SysInfo->GetString(CPU_VENDOR_STRING,str);
	m_VendorString = str;
	DebugInfo("m_VendorString.GetLength()");
	if(m_VendorString.GetLength() > 0){
		DebugInfo("SysInfo->GetData(CPU_FLAG_BRAND,(DWORD*)&data)");
		SysInfo->GetData(CPU_FLAG_BRAND,(DWORD*)&data);
	}

	DebugInfo("SysInfo->GetString(CPU_FULL_NAME,str)");
	SysInfo->GetString(CPU_FULL_NAME,str);			m_CPUName = str;
	DebugInfo("SysInfo->GetString(CPU_NAME_STRING,str)");
	SysInfo->GetString(CPU_NAME_STRING,str);		m_NameString = str;

	DebugInfo("SysInfo->GetString(CPU_CODE_NAME,str)");
	SysInfo->GetString(CPU_CODE_NAME,str);			m_CodeName = str;
	DebugInfo("SysInfo->GetString(CPU_PROCESS_RULE,str)");
	SysInfo->GetString(CPU_PROCESS_RULE,str);		m_ProcessRule = str;
	if(atof(m_ProcessRule) < 0.1){
		m_ProcessRule.Format("%d", int(atof(m_ProcessRule) * 1000));
		m_ProcessUnit = "nm";
	}else{
		m_ProcessUnit = "um";
	}

	DebugInfo("SysInfo->GetString(CPU_PLATFORM_NAME,str)");
	SysInfo->GetString(CPU_PLATFORM_NAME,str);		m_Platform = str;

	DebugInfo("SysInfo->GetString(CPU_TYPE_NAME,str)");
	SysInfo->GetString(CPU_TYPE_NAME,str);

	DebugInfo("cstr = str");
	cstr = str;
	DebugInfo("cstr.Replace");
	cstr.Replace(" processor","");
	DebugInfo("m_CPUType  = cstr");
	m_CPUType  = cstr;

	DebugInfo("SysInfo->GetString(CPU_FSB_MODE,str)");
	SysInfo->GetString(CPU_FSB_MODE,str);			m_DataRate = str;

	SysInfo->GetString(CPU_K8_HYPER_TRANSPORT, str);
	if(atof(str) > 0.0){
		m_SystemBusHT = "   HT Link";
	}else{
		m_SystemBusHT = "System Bus";
	}

	DebugInfo("SysInfo->GetData(CPU_NUMBER,(DWORD*)&data)");
	SysInfo->GetData(CPU_NUMBER,(DWORD*)&data);			if(data>0){m_CPUNumber.Format("%d",data);};
	DWORD Number = data;
	DebugInfo("SysInfo->GetData(CPU_CORE_NUMBER,(DWORD*)&data)");
	SysInfo->GetData(CPU_PHYSICAL_CORE_NUM,(DWORD*)&data);if(data>0){m_CoreNumber.Format("%d",data);};

	DebugInfo("SysInfo->GetData(CPU_PLATFORM_ID,(DWORD*)&data)");
	SysInfo->GetData(CPU_PLATFORM_ID,(DWORD*)&data);	if(data>0){m_PlatformID.Format("%d",data);};
	DebugInfo("SysInfo->GetData(CPU_MICROCODE_ID,(DWORD*)&data)");
	SysInfo->GetData(CPU_MICROCODE_ID,(DWORD*)&data);	if(data>0){m_MicrocodeID.Format("%02X",data);};
	DebugInfo("SysInfo->GetData(CPU_TYPE_ID,(DWORD*)&data)");
	SysInfo->GetData(CPU_TYPE_ID,(DWORD*)&data);		if(data>=0){m_CpuTypeId.Format("%X",data);};

// Clock Information 
	DebugInfo("SysInfo->GetString(CPU_CLOCK,str)");
	SysInfo->GetString(CPU_CLOCK,str);				if(atof(str)>0.0){m_Clock = str;}
	DebugInfo("SysInfo->GetString(CPU_SYSTEM_BUS,str)");
	SysInfo->GetString(CPU_SYSTEM_BUS,str);			if(atof(str)>0.0){m_SystemBus = str;}
	DebugInfo("SysInfo->GetString(CPU_SYSTEM_CLOCK,str)");
	SysInfo->GetString(CPU_SYSTEM_CLOCK,str);		if(atof(str)>0.0){m_SystemClock = str;}
	DebugInfo("SysInfo->GetString(CPU_MULTIPLIER,str)");
	SysInfo->GetString(CPU_MULTIPLIER,str);			if(atof(str)>0.0){m_Multiplier = str;}

	DebugInfo("SysInfo->GetString(CPU_CLOCK_ORI,str)");
	SysInfo->GetString(CPU_CLOCK_ORI,str);
	if( ! m_FlagHideOriginalClock && atof(str) > 0.0 ){
		SysInfo->GetString(CPU_CLOCK_ORI,str);			if(atof(str)>0.0){m_ClockOri = str;}
		SysInfo->GetString(CPU_SYSTEM_CLOCK_ORI,str);	if(atof(str)>0.0){m_SystemClockOri = str;}
		SysInfo->GetString(CPU_SYSTEM_BUS_ORI,str);		if(atof(str)>0.0){m_SystemBusOri = str;}
		SysInfo->GetString(CPU_MULTIPLIER_ORI,str);		if(atof(str)>0.0){m_MultiplierOri = str;}
	}
// OverClock
	DebugInfo("atof(m_Clock) > 0.0 && atof(m_ClockOri) > 0.0");
	if(atof(m_Clock) > 0.0 && atof(m_ClockOri) > 0.0){
		sprintf(str,"%.2f",( atof(m_Clock) / atof(m_ClockOri) ) * 100.0 - 100.0);
		m_OverClock = str;
	}
// L2 Cache Spped
	DebugInfo("SysInfo->GetString(CPU_CACHE_SPEED,str)");
	SysInfo->GetString(CPU_CACHE_SPEED,str);		if(atof(str) > 0.0){cstr.Format("%4.2f",atof(str) * atof(m_Clock));	m_L2CacheSpeedMHz  = cstr;}
	SysInfo->GetString(CPU_CACHE_SPEED_STR,str);	if(strcmp(str,"") != 0){m_L2CacheSpeed = str;}

// Measure Mode
	SysInfo->GetString(CPU_MEASURE_MODE,str);		if(strcmp(str,"") != 0){m_MeasureMode = str;}

// Setup ComboBox
	DebugInfo("m_CPUSelectC.ResetContent()");
	m_CPUSelectC.ResetContent();
	for(DWORD i=1;i <= Number;i++){
		cstr.Format("CPU%d/%d",i,Number);
		m_CPUSelectC.AddString(cstr);
	}
	m_CPUSelectC.SetCurSel(m_Mask);


	SysInfo->GetData(CPU_VERSION,&Version);
	if(Version != 0){
		m_Family = SysInfo->GetInfo(CPU_FAMILY,str);
		m_Model = SysInfo->GetInfo(CPU_MODEL,str);
		m_Stepping = SysInfo->GetInfo(CPU_STEPPING,str);
	}
	SysInfo->GetData(CPU_VERSION_EX,&VersionEx);

	if((Version & 0xFFFF0000) || VersionEx != 0){
		m_ExFamily = SysInfo->GetInfo(CPU_EX_FAMILY_X,str);
		m_ExModel = SysInfo->GetInfo(CPU_EX_MODEL_X,str);
		m_ExStepping = SysInfo->GetInfo(CPU_EX_STEPPING,str);
	}
	/*
	SysInfo->GetData(CPU_VERSION,&Version);
	if(Version != 0){
		m_Family = SysInfo->GetInfo(CPU_FAMILY,str);
		m_Model = SysInfo->GetInfo(CPU_MODEL,str);
		m_Stepping = SysInfo->GetInfo(CPU_STEPPING,str);
		if(! m_Family.Compare("F")){
			m_FamilyEx = SysInfo->GetInfo(CPU_FAMILY_EX,str);
		}
		SysInfo->GetData(CPU_MODEL_EX,(DWORD*)&data);
		if(data > 0 || ! m_Model.Compare("F")){
			m_ModelEx = SysInfo->GetInfo(CPU_MODEL_EX,str);
		}
	}
	SysInfo->GetData(CPU_VERSION_EX,&VersionEx);
	if(VersionEx != 0){
		m_ExFamily = SysInfo->GetInfo(CPU_EX_FAMILY,str);
		m_ExModel = SysInfo->GetInfo(CPU_EX_MODEL,str);
		m_ExStepping = SysInfo->GetInfo(CPU_EX_STEPPING,str);
		if(! m_ExFamily.Compare("F")){
			m_FamilyEx = SysInfo->GetInfo(CPU_FAMILY_EX,str);
		}
		SysInfo->GetData(CPU_MODEL_EX,(DWORD*)&data);
		if(data > 0 || ! m_ExModel.Compare("F")){
			m_ModelEx = SysInfo->GetInfo(CPU_MODEL_EX,str);
		}
	}
	*/

	SysInfo->GetData(CPU_BRAND_ID,(DWORD*)&data);			if(data > 0){cstr.Format("%02X",data);m_BrandID = cstr;}
	SysInfo->GetData(CPU_APIC,(DWORD*)&data);				if(data >= 0){cstr.Format("%02X",data);m_Apic = cstr;}
	SysInfo->GetData(CPU_HYPER_THREAD_NUM,(DWORD*)&data);	if(data > 1){cstr.Format("%d",data);m_HyperThreading = cstr;}

	SysInfo->GetData(CPU_FLAG_MMX,(DWORD*)&data);			if(data != -1){m_MMX.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_MMX_EX,(DWORD*)&data);		if(data != -1){m_MMXEx.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE,(DWORD*)&data);			if(data != -1){m_SSE.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE2,(DWORD*)&data);			if(data != -1){m_SSE2.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE3,(DWORD*)&data);			if(data != -1){m_SSE3.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSSE3,(DWORD*)&data);			if(data != -1){m_SSSE3.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE41,(DWORD*)&data);			if(data != -1){m_SSE41.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE42,(DWORD*)&data);			if(data != -1){m_SSE42.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_SSE4A,(DWORD*)&data);			if(data != -1){m_SSE4A.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_AVX,(DWORD*)&data);			if(data != -1){m_AVX.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_3DNOW,(DWORD*)&data);			if(data != -1){m_3DNow.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_3DNOW_EX,(DWORD*)&data);		if(data != -1){m_3DNowEx.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_HYPER_THREAD,(DWORD*)&data);	if(data != -1){m_HT.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_VT,(DWORD*)&data);			if(data != -1){m_VT.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_AMD_V,(DWORD*)&data);			if(data != -1){m_AmdV.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_IA64,(DWORD*)&data);			if(data != -1){m_IA64.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_AA64,(DWORD*)&data);			if(data != -1){m_AA64.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_IA32E,(DWORD*)&data);			if(data != -1){m_IA32e.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_NX,(DWORD*)&data);			if(data != -1){m_NX.EnableWindow(data);}
	SysInfo->GetData(CPU_FLAG_DUAL_CORE,(DWORD*)&data);		if(data != -1){m_DualCore.EnableWindow(data);}

// Cache Info //
	cstr.Empty();m_CacheL1ITU = "L1 I-Cache";m_CacheL1ITUUnit = "KB";
	SysInfo->GetData(CPU_CACHE_L1U,(DWORD*)&data);	if(data > 0){cstr.Format("%d",data);m_CacheL1ITU = "L1 Unified";m_CacheL1ITUUnit = "KB";}
	SysInfo->GetData(CPU_CACHE_L1I,(DWORD*)&data);	if(data > 0){cstr.Format("%d",data);m_CacheL1ITU = "L1 I-Cache";m_CacheL1ITUUnit = "KB";}
	SysInfo->GetData(CPU_CACHE_L1T,(DWORD*)&data);	if(data > 0){cstr.Format("%d",data);m_CacheL1ITU = "L1 T-Cache";m_CacheL1ITUUnit = "KuOps";}
	m_L1ITUCache = cstr;
	SysInfo->GetData(CPU_CACHE_L1D,(DWORD*)&data);
	cstr.Format("%d",data);if(data > 0){m_L1DataCache = cstr;}
	SysInfo->GetData(CPU_CACHE_L2,(DWORD*)&data);
	cstr.Format("%d",data);if(data > 0){m_L2Cache  = cstr;}
	SysInfo->GetData(CPU_CACHE_L3,(DWORD*)&data);
	cstr.Format("%d",data);if(data > 0){m_L3Cache  = cstr;}

	DWORD data1,data2;
	SysInfo->GetData(CPU_RDMSR_EAX_1,(DWORD*)&data1);
	SysInfo->GetData(CPU_RDMSR_EDX_1,(DWORD*)&data2);
	cstr.Format("                : %08X %08X",data1,data2);
	m_MSRInfo = cstr;
	SysInfo->GetData(CPU_RDMSR_EAX_2,(DWORD*)&data1);
	SysInfo->GetData(CPU_RDMSR_EDX_2,(DWORD*)&data2);
	cstr.Format(" %08X %08X\r\n",data1,data2);
	m_MSRInfo += cstr;

// Memory
	DebugInfo("GlobalMemoryStatus(&MemStatus)");

	BOOL (WINAPI *GlobalMemoryStatusEX)(LPMEMORYSTATUSEX);
	GlobalMemoryStatusEX = (BOOL (WINAPI *)(LPMEMORYSTATUSEX) )GetProcAddress(GetModuleHandle("kernel32.dll"), "GlobalMemoryStatusEx");
	if(GlobalMemoryStatusEX != NULL){
		MEMORYSTATUSEX MemStatusEx;
		MemStatusEx.dwLength = sizeof(MemStatusEx);
		GlobalMemoryStatusEX(&MemStatusEx);
		m_Memory.Format("%d", MemStatusEx.ullTotalPhys / 1024 / 1024 + 1);
	}else{
		MEMORYSTATUS MemStatus;
		GlobalMemoryStatus(&MemStatus);
		m_Memory.Format("%d", MemStatus.dwTotalPhys / 1024 / 1024 + 1);
	}
	DebugInfo("m_Memory.Format");

// Power Mangement Flag
	DebugInfo("SysInfo->GetData(CPU_FLAG_SPEED_STEP,(DWORD*)&data)");
	SysInfo->GetData(CPU_FLAG_SPEED_STEP,(DWORD*)&data);
	if( data > 0 ){
		m_SpeedStep.EnableWindow(TRUE);
	}
	DebugInfo("SysInfo->GetData(CPU_FLAG_POWER_NOW,(DWORD*)&data)");
	SysInfo->GetData(CPU_FLAG_POWER_NOW,(DWORD*)&data);
	if( data > 0 ){
		m_PowerNow.EnableWindow(TRUE);
	}
	DebugInfo("SysInfo->GetData(CPU_FLAG_LONG_HAUL,(DWORD*)&data)");
	SysInfo->GetData(CPU_FLAG_LONG_HAUL,(DWORD*)&data);
	if( data > 0 ){
		m_LongHaul.EnableWindow(TRUE);
	}
	DebugInfo("SysInfo->GetData(CPU_FLAG_LONG_RUN,(DWORD*)&data)");
	SysInfo->GetData(CPU_FLAG_LONG_RUN,(DWORD*)&data);
	if( data > 0 ){
		m_LongRun.EnableWindow(TRUE);
	}

// Architecture
	DebugInfo("m_Architecture = SysInfo->GetInfo(CPU_ARCHITECTURE,str)");
	m_Architecture = SysInfo->GetInfo(CPU_ARCHITECTURE,str);

	DebugInfo("UpdateData(FALSE)");
	UpdateData(FALSE);
	// DisconnectSysInfo();
	DebugInfo("EndWaitCursor()");
	EndWaitCursor();
	DebugInfo("ExitRefresh");
}

void CCrystalCPUIDDlg::OnSelchangeCpuSelect() 
{
//	CString cstr;
	m_CPUSelectC.GetLBText(m_CPUSelectC.GetCurSel(),cstr);
	cstr.Replace("CPU","");
	m_Mask = atoi(cstr) - 1;
	try{
		Refresh( 0 );
	}catch(...){
		DebugInfo("OnSelchangeCpuSelectException:Refresh( 0 )");	
	}
	if( m_IsNT ){SetThreadAffinityMask(GetCurrentThread(), 1 << m_Mask );}
}

void CCrystalCPUIDDlg::OnExit() 
{
	CDialog::OnCancel();
}

void CCrystalCPUIDDlg::Exit() 
{
	EndDialog(999);
}

/////////////////////////////////////////////////////////////////////////////
// Refresh
// 0 = QPC + MMT (Standard)
// 1 = WT (Windows Timer)
// 2 = MMT (Multimedia Timer)
// 3 = QPC
/////////////////////////////////////////////////////////////////////////////

void CCrystalCPUIDDlg::OnRefresh() 
{
	char str[256];
	GetPrivateProfileString("Setting","MethodType","0",str,256,m_ini);
	if( atoi(str) == 1 ){ // With Load
		try{
			Refresh(3 /* QPC */ );
		}catch(...){
			DebugInfo("OnRefresh:Refresh( 3 )");	
		}
	}else{
		try{
			Refresh( 0 );
		}catch(...){
			DebugInfo("OnRefresh:Refresh( 0 )");	
		}
	}
}

void CCrystalCPUIDDlg::OnRefreshWt() 
{
	try{
		Refresh( 1 /* WT */ );
	}catch(...){
		DebugInfo("Refresh( 1 /* WT */ )");	
	}
}

void CCrystalCPUIDDlg::OnRefreshMmt() 
{
	try{
		Refresh( 2 /* MMT */ );
	}catch(...){
		DebugInfo("Refresh( 2 /* MMT */ )");	
	}
}

void CCrystalCPUIDDlg::OnRefreshQpc() 
{
	try{
		Refresh( 3 /* QPC */ );
	}catch(...){
		DebugInfo("Refresh( 3 /* QPC */ )");	
	}	
}

void CCrystalCPUIDDlg::OnRefreshNoload() 
{
	try{
		Refresh( 4 /* QPC + No Load */ );
	}catch(...){
		DebugInfo("Refresh( 4 /* QPC + No Load */ )");	
	}			
}

/////////////////////////////////////////////////////////////////////////////
// TextCopy
// type = 1 : Clipboard
// type = 2 : File
// type = 3 : Preview
// type = 4 : Quick Save (temp.txt)
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::TextCopy(DWORD type)
{
	UpdateData(TRUE);
	CString clip;
//	CString cstr;

clip = "\
----------------------------------------------------------------\r\n\
%PRODUCT% %VERSION% (C) 2002-2008 hiyohiyo\r\n\
                    Crystal Dew World : http://crystalmark.info/\r\n\
----------------------------------------------------------------\r\n\
\r\n\
             OS : %OS%\r\n\
           Date : %Date%\r\n\
\r\n\
       CPU Name : %CPUName%\r\n\
  Vendor String : %VendorString%\r\n\
    Name String : %NameString%\r\n\
   Architecture : %ARCHITECTURE%\r\n\
   Process Rule : %ProcessRule%\r\n\
       Platform : %Platform%\r\n\
       CPU Type : %CPUType%\r\n\
 Number (Total) : %CPUNumber%\r\n\
  Physical Core : %PhysicalCore%\r\n\
";
	clip.Replace("%PRODUCT%", CRYSTAL_CPUID_PRODUCT);
	clip.Replace("%VERSION%",CrystalCPUIDVersion);
	clip.Replace("%ARCHITECTURE%",m_Architecture);
/*
	CString OSName,OSType,OSCSD,OSVersion,OSBuild,OSInfo;
	GetOSInfo(OSName,OSType,OSCSD,OSVersion,OSBuild,OSInfo);
	cstr = OSName + " " + OSType + " " + OSCSD;
*/
	clip.Replace("%OS%",m_OS);

	SYSTEMTIME st;
	GetLocalTime(&st);
	cstr.Format("%04d/%02d/%02d %d:%02d:%02d",st.wYear,st.wMonth,st.wDay,st.wHour,st.wMinute,st.wSecond);
	clip.Replace("%Date%",cstr);

	if( ! m_CodeName.IsEmpty() ){
		cstr.Format("%s (%s)",m_CPUName,m_CodeName);
	}else{
		cstr = m_CPUName;
	}
	clip.Replace("%CPUName%",cstr);

	clip.Replace("%VendorString%",m_VendorString);
	clip.Replace("%NameString%",m_NameString);
	clip.Replace("%CPUNumber%",m_CPUNumber);
	clip.Replace("%PhysicalCore%",m_CoreNumber);

	if( m_CPUType.IsEmpty() ){
		clip.Replace("%CPUType%",m_CPUType);
	}else{
		clip.Replace("%CPUType%",m_CPUType + " processor [" + m_CpuTypeId + "]");
	}
	if( m_PlatformID.IsEmpty() ){
		clip.Replace("%Platform%",m_Platform);
	}else{
		clip.Replace("%Platform%",m_Platform + " [" + m_PlatformID + "]");
	}

	if( ! m_ProcessRule.IsEmpty() ){
		clip.Replace("%ProcessRule%",m_ProcessRule + " " + m_ProcessUnit);
	}else{
		clip.Replace("%ProcessRule%",m_ProcessRule);
	}

	if(! m_HyperThreading.IsEmpty()){	clip += "Hyper-Threading : " + m_HyperThreading + "\r\n";}
	if(! m_Family.IsEmpty()){			clip += "         Family : " + m_Family + "h\r\n";}
	if(! m_ExFamily.IsEmpty()){			clip += "    Ext. Family : " + m_ExFamily + "h\r\n";}
	if(! m_FamilyEx.IsEmpty()){			clip += "       FamilyEx : " + m_FamilyEx + "h\r\n";}
	if(! m_Model.IsEmpty()){			clip += "          Model : " + m_Model + "h\r\n";}
	if(! m_ExModel.IsEmpty()){			clip += "     Ext. Model : " + m_ExModel + "h\r\n";}
	if(! m_ModelEx.IsEmpty()){			clip += "        ModelEx : " + m_ModelEx + "h\r\n";}
	if(! m_Stepping.IsEmpty()){			clip += "       Stepping : " + m_Stepping + "h\r\n";}
	if(! m_ExStepping.IsEmpty()){		clip += "  Ext. Stepping : " + m_ExStepping + "h\r\n";}
	if(! m_Apic.IsEmpty()){				clip += "           APIC : " + m_Apic + "h\r\n";}
	if(! m_BrandID.IsEmpty()){			clip += "       Brand ID : " + m_BrandID + "h\r\n";}
	if(! m_MicrocodeID.IsEmpty()){		clip += "   Microcode ID : " + m_MicrocodeID + "h\r\n";}

	clip += "        Feature :";
	if( m_MMX.IsWindowEnabled() ){clip += " MMX";}
	if( m_SSE.IsWindowEnabled() ){clip += " SSE";}
	if( m_SSE2.IsWindowEnabled() ){clip += " SSE2";}
	if( m_SSE3.IsWindowEnabled() ){clip += " SSE3";}
	if( m_SSSE3.IsWindowEnabled() ){clip += " SSSE3";}
	if( m_SSE41.IsWindowEnabled() ){clip += " SSE4.1";}
	if( m_SSE42.IsWindowEnabled() ){clip += " SSE4.2";}
	if( m_SSE4A.IsWindowEnabled() ){clip += " SSE4A";}
	if( m_SSE5.IsWindowEnabled() ){clip += " SSE5";}
	if( m_AVX.IsWindowEnabled() ){clip += " AVX";}
	if( m_MMXEx.IsWindowEnabled() ){clip += " MMX+";}
	if( m_3DNow.IsWindowEnabled() ){clip += " 3DNow!";}
	if( m_3DNowEx.IsWindowEnabled() ){clip += " 3DNow!+";}
	if( m_HT.IsWindowEnabled() ){clip += " HTT";}
	if( m_NX.IsWindowEnabled() ){
		if(m_VendorString == "GenuineIntel"){
			clip += " XD";
		}else{
			clip += " NX";
		}
	}
	if( m_VT.IsWindowEnabled() ){clip += " VT";}
	if( m_AmdV.IsWindowEnabled() ){clip += " AMD-V";}

	if( m_IA64.IsWindowEnabled() ){clip += " IA-64";}
	if( m_AA64.IsWindowEnabled() ){clip += " AMD64";}
	if( m_IA32e.IsWindowEnabled() ){clip += " Intel 64";}

//	if( m_DualCore.IsWindowEnabled() ){clip += " DualCore";}
	clip += "\r\n";

	clip += "PowerManagement :";
	if( m_SpeedStep.IsWindowEnabled() ){clip += " SpeedStep";}
	if( m_PowerNow.IsWindowEnabled() ){clip += " PowerNow!/Cool'n'Quiet";}
	if( m_LongHaul.IsWindowEnabled() ){clip += " LongHaul";}
	if( m_LongRun.IsWindowEnabled() ){clip += " LongRun";}
	clip += "\r\n";

	clip += m_TransmetaInfo;

	if(atof(m_ClockOri) > 0.0){
		clip += "\r\n                    Current        Original\r\n";
	}else{
		clip +="\r\n";
	}
    
	if(atof(m_Clock) > 0.0){
		cstr.Format("%7.2f",atof(m_Clock));
		clip += "          Clock : " + cstr + " MHz";
	}
	if(atof(m_ClockOri) > 0.0){
		cstr.Format("%7.2f",atof(m_ClockOri));
		clip += "     " + cstr + " MHz";
	}
	if(atof(m_Clock) > 0.0){
		clip +="\r\n";
	}

	if(atof(m_SystemClock) > 0.0){
		cstr.Format("%7.2f",atof(m_SystemClock));
		clip += "   System Clock : " + cstr + " MHz";
	}
	if(atof(m_SystemClockOri) > 0.0){
		cstr.Format("%7.2f",atof(m_SystemClockOri));
		clip += "     " + cstr + " MHz";
	}
	if(atof(m_SystemClock) > 0.0){
		clip +="\r\n";
	}

	if(atof(m_SystemBus) > 0.0){
		cstr.Format("%7.2f",atof(m_SystemBus));
		clip += "     " + m_SystemBusHT + " : " + cstr + " MHz";
	}
	if(atof(m_SystemBusOri) > 0.0){
		cstr.Format("%7.2f",atof(m_SystemBusOri));
		clip += "     " + cstr + " MHz";
	}
	if(atof(m_SystemBus) > 0.0){
		clip +="\r\n";
	}

	if(atof(m_Multiplier) > 0.0){
		cstr.Format("%7.2f",atof(m_Multiplier));
		clip += "     Multiplier : " + cstr;
	}
	if(atof(m_MultiplierOri) > 0.0){
		cstr.Format("%7.2f",atof(m_MultiplierOri));
		clip += "         " + cstr;
	}
	if(atof(m_Multiplier) > 0.0){
		clip +="\r\n";
	}

	if(! m_DataRate.IsEmpty()){
		clip += "      Data Rate :    " + m_DataRate + "\r\n";
	}
	
	if(atof(m_ClockOri) > 0.0){
		cstr.Format("%7.2f %%",atof(m_OverClock));
		clip += "     Over Clock : " + cstr + "\r\n";
	}

	if(atoi(m_L1ITUCache) > 0){
		cstr.Format("%4d",atoi(m_L1ITUCache));
		clip += "\r\n     " + m_CacheL1ITU + " : " + cstr + " " + m_CacheL1ITUUnit + "\r\n"; 
	}
	if(atoi(m_L1DataCache) > 0){
		cstr.Format("%4d",atoi(m_L1DataCache));
		clip += "     L1 D-Cache : " + cstr + " KB\r\n";
	}
	if(atoi(m_L2Cache) > 0){
		cstr.Format("%4d",atoi(m_L2Cache));
		clip += "       L2 Cache : " + cstr + " KB [" + m_L2CacheSpeed + ":" + m_L2CacheSpeedMHz + " MHz]\r\n";
	}
	if(atoi(m_L3Cache) > 0){
		cstr.Format("%4d",atoi(m_L3Cache));
		clip += "       L3 Cache : " + cstr + " KB\r\n";
	}
	if(atoi(m_Memory) > 0){
		cstr.Format("%4d",atoi(m_Memory));
		clip += "         Memory : " + cstr + " MB\r\n";
	}

	clip += "\r\n";
	// CPUID 
	DWORD i=0x00;
	DWORD dwEAX,dwEBX,dwECX,dwEDX;
	cstr.Format("       CPUID    : EAX      EBX      ECX      EDX\r\n");
	clip += cstr;
	cstr.Format("       ----------------------------------------------\r\n");
	clip += cstr;

	GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	DWORD MaxCPUID = dwEAX;
	for(i=0;i<=MaxCPUID;i++){
		GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
		cstr.Format("       %08X : %08X %08X %08X %08X\r\n",i,dwEAX,dwEBX,dwECX,dwEDX);
		clip += cstr;
	}

	GetCPUID(0x80000000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0x80000000){
		DWORD MaxCPUIDEx = dwEAX;
		for(i=0x80000000;i<=MaxCPUIDEx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			cstr.Format("       %08X : %08X %08X %08X %08X\r\n",i,dwEAX,dwEBX,dwECX,dwEDX);
			clip += cstr;
		}
	}

	GetCPUID(0x80860000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0x80000000){
		DWORD MaxCPUIDTx = dwEAX;
		for(i=0x80860000;i<=MaxCPUIDTx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			cstr.Format("       %08X : %08X %08X %08X %08X\r\n",i,dwEAX,dwEBX,dwECX,dwEDX);
			clip += cstr;
		}
	}

	GetCPUID(0xC0000000,&dwEAX,&dwEBX,&dwECX,&dwEDX);
	if(dwEAX > 0xC0000000){
		DWORD MaxCPUIDEx = dwEAX;
		for(i=0xC0000000;i<=MaxCPUIDEx;i++){
			GetCPUID(i,&dwEAX,&dwEBX,&dwECX,&dwEDX);
			cstr.Format("       %08X : %08X %08X %08X %08X\r\n",i,dwEAX,dwEBX,dwECX,dwEDX);
			clip += cstr;
		}
	}

	clip += "\r\n";
	cstr.Format("       MSR      : EAX1     EDX1     EAX2     EDX2\r\n");
	clip += cstr;
	cstr.Format("       ----------------------------------------------\r\n");
	clip += m_MSRInfo;

	LARGE_INTEGER QPF;
	QueryPerformanceFrequency(&QPF);
	cstr.Format("       QPF      : %f MHz\r\n",QPF.QuadPart / 1000.0 / 1000.0);
	clip += cstr;

	ChipsetInfo();
	clip += m_ChipsetInfo;
	BiosInfo();
	clip += m_BiosInfo;
	DmiInfo();
	clip += m_DmiInfo;
	PciDeviceInfo();
	clip += m_PciDeviceInfo;

	switch( type ){
	case 1: // Clipboard
		if(OpenClipboard())
		{
			HGLOBAL clipbuffer;
			char * buffer;
			EmptyClipboard();
			clipbuffer = GlobalAlloc(GMEM_DDESHARE, clip.GetLength()+1);
			buffer = (char*)GlobalLock(clipbuffer);
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
	case 3: // Preview
		{
			CString fullpath = m_path;
			fullpath += "\\temp.txt";
			CStdioFile write(fullpath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
			write.WriteString(clip);
			ShellExecute(NULL,NULL,fullpath,NULL,NULL,SW_SHOW);
		}
		break;
	case 4: // Quick Save
		{
			CString fullpath = m_path;
			fullpath += "\\temp.txt";
			CStdioFile write(fullpath,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
			write.WriteString(clip);
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Image Copy
// type = 1 : // Clipboard
// type = 2 : // File
// type = 3 : // Quick Save
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::ImageCopy(DWORD type)
{
	switch( type )
	{
	case 1: // Clipboard
		break;
	case 2: // File
		{
		CString cstr1,cstr2,cstr3;
		#ifdef USEPNG
		cstr1 = "png";
		cstr2 = "";
		cstr3 = "PNG (*.png)|*.png|BMP (*.bmp)|*.bmp";
		#else
		cstr1 = "bmp";
		cstr2 = "";
		cstr3 = "BMP (*.bmp)|*.bmp";
		#endif
		CFileDialog save(FALSE,cstr1,cstr2,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,cstr3);
		#ifndef _X86_64
		FakeCFileDialog(save);
		#endif
		if( save.DoModal() == IDOK )
		{
			Invalidate(TRUE);
			UpdateWindow();
			RECT rect;
			CDC *pDC = GetWindowDC();
			GetWindowRect(&rect);
			CDibSection dib;
			dib.Create(rect.right - rect.left, rect.bottom - rect.top, 24);
			dib.Copy(pDC->m_hDC);
#ifdef USEPNG
			if( save.GetFileExt() == "png"){
				dib.SavePng(save.GetFileName());
			}else 
#endif				
			if( save.GetFileExt() == "bmp"){
				dib.SaveBmp(save.GetFileName());
			}
			pDC->DeleteDC();	
		}
		}
		break;
	case 3: // Quick Save
		{
		Invalidate(TRUE);
		UpdateWindow();
		RECT rect;
		CDC *pDC = GetWindowDC();
		GetWindowRect(&rect);
		CDibSection dib;
		dib.Create(rect.right - rect.left, rect.bottom - rect.top, 24);
		dib.Copy(pDC->m_hDC);
		CString fullpath = m_path;
#ifdef USEPNG
		fullpath += "\\temp.png";
		dib.SavePng(fullpath);
#else
		fullpath += "\\temp.bmp";
		dib.SaveBmp(fullpath);
#endif
		pDC->DeleteDC();
		}
		break;
	}
}

/////////////////////////////////////////////////////////////////////////////
// Change Language Jpanese -> English 
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnJapanese() 
{
	gLanguage = JAPANESE;
	CMenu *menu = GetMenu();

	menu->ModifyMenu( 0, MF_BYPOSITION | MF_STRING, 0, "ファイル (&F)" );
	menu->ModifyMenu( 1, MF_BYPOSITION | MF_STRING, 1, "編集 (&E)" );
	menu->ModifyMenu( 2, MF_BYPOSITION | MF_STRING, 2, "拡張機能 (&U)" );
	menu->ModifyMenu( 3, MF_BYPOSITION | MF_STRING, 3, "ヘルプ (&H)" );

	menu->ModifyMenu( IDM_HIDE_ORIGINAL_CLOCK, MF_STRING, IDM_HIDE_ORIGINAL_CLOCK, "オリジナルクロックと倍率を表示しない (&H)\tCtrl + H" );
	menu->ModifyMenu( IDM_HIDE_DATE, MF_STRING, IDM_HIDE_DATE, "日時を表示しない (&D)\tCtrl + D" );
	menu->ModifyMenu( IDM_CUSTOMIZE, MF_STRING, IDM_CUSTOMIZE, "カスタマイズ (&C)" );
	menu->ModifyMenu( IDM_CRYSTAL_N_QUIET, MF_STRING, IDM_CRYSTAL_N_QUIET, "Multiplier Management の設定 (&M)" );
	menu->ModifyMenu( IDM_EXIT, MF_STRING, IDM_EXIT, "終了 (&E)\tAlt + F4" );
	menu->ModifyMenu( IDM_COPY_TEXT, MF_STRING, IDM_COPY_TEXT, "コピー (&C)\tCtrl + C" );
	menu->ModifyMenu( IDM_PREVIEW_TEXT, MF_STRING, IDM_PREVIEW_TEXT, "プレビュー (&P)\tCtrl + P" );
	menu->ModifyMenu( IDM_SAVE_TEXT, MF_STRING, IDM_SAVE_TEXT, "テキスト保存 (&T)\tCtrl + S" );
	menu->ModifyMenu( IDM_SAVE_IMAGE, MF_STRING, IDM_SAVE_IMAGE, "画像保存 (&I)" );
	menu->ModifyMenu( IDM_REFRESH, MF_STRING, IDM_REFRESH, "リフレッシュ [QPC+MMT] (&R)\tF5" );
	menu->ModifyMenu( IDM_REFRESH_WT , MF_STRING, IDM_REFRESH_WT,  "リフレッシュ [WT] (&W)\tF6" );
	menu->ModifyMenu( IDM_REFRESH_MMT, MF_STRING, IDM_REFRESH_MMT, "リフレッシュ [MMT] (&M)\tF7" );
	menu->ModifyMenu( IDM_REFRESH_QPC, MF_STRING, IDM_REFRESH_QPC, "リフレッシュ [QPC] (&Q)\tF8" );
	menu->ModifyMenu( IDM_REFRESH_NOLOAD, MF_STRING, IDM_REFRESH_NOLOAD, "リフレッシュ [QPC (負荷なし)] (&N)\tF9" );
	menu->ModifyMenu( IDM_QUICK_SAVE_IMAGE, MF_STRING, IDM_QUICK_SAVE_IMAGE, "クイック画像保存 (&Q)\tCtrl + Q" );
//	menu->ModifyMenu( IDM_LICENSE, MF_STRING, IDM_LICENSE, "ライセンス" );
	menu->ModifyMenu( IDM_ABOUT, MF_STRING, IDM_ABOUT, "CrystalCPUID について... (&A)" );

	menu->CheckMenuItem(IDM_ENGLISH,MF_UNCHECKED);
	menu->CheckMenuItem(IDM_JAPANESE,MF_CHECKED);
	menu->EnableMenuItem(IDM_ENGLISH,MF_ENABLED);
	menu->EnableMenuItem(IDM_JAPANESE,MF_GRAYED);

	if( m_CustomizeDlg != NULL ){
		menu->EnableMenuItem(IDM_CUSTOMIZE,MF_GRAYED);
	}
	
	if( m_AboutDlg != NULL ){
		menu->EnableMenuItem(IDM_ABOUT,MF_GRAYED);
	}

	if( m_LhType < 1 || m_CrystalNQuietDlg != NULL || m_SysInfoStatus != 0 ){
		menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_GRAYED);
	}

	if( m_LhType < 1 || m_SysInfoStatus != 0 ){
		menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_GRAYED);
	}

	WritePrivateProfileString("Setting","Language","jp",m_ini);

	if( m_FlagHideOriginalClock ){
		menu->CheckMenuItem(IDM_HIDE_ORIGINAL_CLOCK,MF_CHECKED);
	}
	if( m_FlagHideDate ){
		menu->CheckMenuItem(IDM_HIDE_DATE,MF_CHECKED);
	}
	if( m_FlagCrystalNQuiet ){
		menu->CheckMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_CHECKED);
	}

	SetMenu(menu);
}

/////////////////////////////////////////////////////////////////////////////
// Change Language English -> Jpanese
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnEnglish() 
{
	gLanguage = ENGLISH;
	CMenu *menu = GetMenu();

	menu->ModifyMenu( 0, MF_BYPOSITION | MF_STRING, 0, "&File" );
	menu->ModifyMenu( 1, MF_BYPOSITION | MF_STRING, 1, "&Edit" );
	menu->ModifyMenu( 2, MF_BYPOSITION | MF_STRING, 2, "F&unction" );
	menu->ModifyMenu( 3, MF_BYPOSITION | MF_STRING, 3, "&Help" );

	menu->ModifyMenu( IDM_HIDE_ORIGINAL_CLOCK, MF_STRING, IDM_HIDE_ORIGINAL_CLOCK, "&Hide Original Clock && Multiplier\tCtrl + H" );
	menu->ModifyMenu( IDM_HIDE_DATE, MF_STRING, IDM_HIDE_DATE, "Hide &Date && Time\tCtrl + D" );
	menu->ModifyMenu( IDM_CUSTOMIZE, MF_STRING, IDM_CUSTOMIZE, "&Customize" );
	menu->ModifyMenu( IDM_CRYSTAL_N_QUIET, MF_STRING, IDM_CRYSTAL_N_QUIET, "&Multiplier Management Setting" );
	menu->ModifyMenu( IDM_EXIT, MF_STRING, IDM_EXIT, "&Exit\tAlt + F4" );
	menu->ModifyMenu( IDM_COPY_TEXT, MF_STRING, IDM_COPY_TEXT, "&Copy\tCtrl + C" );
	menu->ModifyMenu( IDM_PREVIEW_TEXT, MF_STRING, IDM_PREVIEW_TEXT, "&Preview\tCtrl + P" );
	menu->ModifyMenu( IDM_SAVE_TEXT, MF_STRING, IDM_SAVE_TEXT, "Save &Text\tCtrl + S" );
	menu->ModifyMenu( IDM_SAVE_IMAGE, MF_STRING, IDM_SAVE_IMAGE, "Save &Image" );
	menu->ModifyMenu( IDM_REFRESH, MF_STRING, IDM_REFRESH, "&Refresh [QPC+MMT]\tF5" );
	menu->ModifyMenu( IDM_REFRESH_WT , MF_STRING, IDM_REFRESH_WT,  "Refresh [&WT]\tF6" );
	menu->ModifyMenu( IDM_REFRESH_MMT, MF_STRING, IDM_REFRESH_MMT, "Refresh [&MMT]\tF7" );
	menu->ModifyMenu( IDM_REFRESH_QPC, MF_STRING, IDM_REFRESH_QPC, "Refresh [&QPC]\tF8" );
	menu->ModifyMenu( IDM_REFRESH_NOLOAD, MF_STRING, IDM_REFRESH_NOLOAD, "Refresh [QPC (&No Load)]\tF9" );
	menu->ModifyMenu( IDM_QUICK_SAVE_IMAGE, MF_STRING, IDM_QUICK_SAVE_IMAGE, "&Quick Save Image\tCtrl + Q" );
//	menu->ModifyMenu( IDM_LICENSE, MF_STRING, IDM_LICENSE, "License" );
	menu->ModifyMenu( IDM_ABOUT, MF_STRING, IDM_ABOUT, "&About CrystalCPUID..." );

	menu->CheckMenuItem(IDM_ENGLISH,MF_CHECKED);
	menu->CheckMenuItem(IDM_JAPANESE,MF_UNCHECKED);
	menu->EnableMenuItem(IDM_ENGLISH,MF_GRAYED);
	menu->EnableMenuItem(IDM_JAPANESE,MF_ENABLED);

	if( m_CustomizeDlg != NULL ){
		menu->EnableMenuItem(IDM_CUSTOMIZE,MF_GRAYED);
	}

	if( m_AboutDlg != NULL ){
		menu->EnableMenuItem(IDM_ABOUT,MF_GRAYED);
	}

	if( m_LhType < 1 || m_CrystalNQuietDlg != NULL || m_SysInfoStatus != 0 ){
		menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_GRAYED);
	}

	if( m_LhType < 1 || m_SysInfoStatus != 0 ){
		menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_GRAYED);
	}
	
	WritePrivateProfileString("Setting","Language","en",m_ini);

	if( m_FlagHideOriginalClock ){
		menu->CheckMenuItem(IDM_HIDE_ORIGINAL_CLOCK,MF_CHECKED);
	}
	if( m_FlagHideDate ){
		menu->CheckMenuItem(IDM_HIDE_DATE,MF_CHECKED);
	}
	if( m_FlagCrystalNQuiet ){
		menu->CheckMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_CHECKED);
	}

	SetMenu(menu);
}

/////////////////////////////////////////////////////////////////////////////
// Open/Execute File
/////////////////////////////////////////////////////////////////////////////
static void ExecuteFile(char* FileName)
{
	char path[MAX_PATH],file[MAX_PATH];
	char* ptrEnd;
	::GetModuleFileName(NULL,path, MAX_PATH);
	if ( (ptrEnd = strrchr(path, '\\')) != NULL ){
		*ptrEnd = '\0';
	}
	if(strstr(((CCrystalCPUIDApp*)AfxGetApp())->m_ini, "\\")){
		wsprintf(file,"%s",FileName);
	}else{
		wsprintf(file,"%s\\%s",path,FileName);
	}
	ShellExecute(NULL,NULL,file,NULL,NULL,SW_SHOWNORMAL);	
}

/////////////////////////////////////////////////////////////////////////////
// Setup WallPaper
/////////////////////////////////////////////////////////////////////////////
BOOL CCrystalCPUIDDlg::CreateWallPaper(DWORD Mode,char* FileName,char* ResourceName,COLORREF Color,BOOL flagGray)
{
	gFlagBack = FALSE;
	gBackBitmap.DeleteObject();
	gBackBrush.DeleteObject();

	switch(Mode)
	{
	case 1:
		if( strcmp(ResourceName,"") == 0 ){
			return FALSE;
		}
		if(	gBackBitmap.LoadBitmap(ResourceName) ){
			gBackBrush.CreatePatternBrush(&gBackBitmap);
		}else if( Dib.LoadBmp(ResourceName) ){
			if(flagGray){Dib.Gray();}
			gBackBitmap.Attach((HGDIOBJ)Dib.Handle());
			gBackBrush.CreatePatternBrush(&gBackBitmap);
#ifdef USEJPEG
		}else if( Dib.LoadJpeg(ResourceName) ){
			if(flagGray){Dib.Gray();}
			gBackBitmap.Attach((HGDIOBJ)Dib.Handle());
			gBackBrush.CreatePatternBrush(&gBackBitmap);
#endif	
#ifdef USEPNG
		}else if( Dib.LoadPng(ResourceName) ){
			if(flagGray){Dib.Gray();}
			gBackBitmap.Attach((HGDIOBJ)Dib.Handle());
			gBackBrush.CreatePatternBrush(&gBackBitmap);
#endif
		}else{
			return FALSE;
		}
		gFlagBack = TRUE;
		return TRUE;
		break;
	case 2:
		gBackBrush.CreateSolidBrush(Color);
		return TRUE;
		break;
	case 0:
	default:
		gFlagBack = FALSE;
		return TRUE;
		break;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////
// OnCtlColor (Render Background Image)
/////////////////////////////////////////////////////////////////////////////
HBRUSH CCrystalCPUIDDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if( m_pFont != NULL ){
		pDC->SelectObject(m_pFont);
	}

	if( gFlagBack ){
		switch (nCtlColor)
		{
			case CTLCOLOR_STATIC:
			{
				pDC->SetTextColor(gTextColor);
				pDC->SetBkMode(TRANSPARENT);
				return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
			}
			case CTLCOLOR_DLG:
			{
				return (HBRUSH)gBackBrush;
			}
		}
	}else{
		switch (nCtlColor)
		{
			case CTLCOLOR_STATIC:
			{
				pDC->SetTextColor(gTextColor);
				pDC->SetBkMode(TRANSPARENT);
			}
		}
	}

	return hbr;
}

/////////////////////////////////////////////////////////////////////////////
// Get CPUID
/////////////////////////////////////////////////////////////////////////////
#ifdef _X86_64
extern "C" {
void __fastcall _cpuid(DWORD dwOP, DWORD *lpAX, DWORD *lpBX, DWORD *lpCX, DWORD *lpDX);
}
#endif
void GetCPUID(DWORD id,DWORD* A,DWORD* B,DWORD* C,DWORD* D)
{
#ifdef _X86_64
	_cpuid(id,A,B,C,D);
#else 
	DWORD dwEAX,dwEBX,dwECX,dwEDX;
	_asm{
		mov eax,id
		mov ecx, 0
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
#endif
}


//////////////////////////////////////////////////////////////////////
// CPUID Check
//////////////////////////////////////////////////////////////////////
static int IsCPUID()
{
#ifdef _X86_64
	return TRUE;
#else
	BOOL FlagCPUID;
	DWORD flag_1, flag_2;
	_asm{
			pushfd
			pop		eax
			mov		flag_1, eax
			xor		eax, 00200000h
			push	eax
			popfd
			pushfd
			pop		eax
			mov		flag_2, eax
		}

    if( flag_1 == flag_2){	// NOT Available CPUID
		FlagCPUID = FALSE;
    }else{					// Available CPUID
		FlagCPUID = TRUE;
	}
	return FlagCPUID;
#endif
}

/////////////////////////////////////////////////////////////////////////////
// CrystalCPUMark 2005
/////////////////////////////////////////////////////////////////////////////
/*
void CCrystalCPUIDDlg::OnCpuMark2005() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CPU_MARK_2005,MF_GRAYED);
	SetMenu(menu);

	m_CpuMark2005Dlg = new CCpuMark2005Dlg(this);
	CreateIndirectDialog(m_CpuMark2005Dlg, IDD_CPU_MARK_2005);

//	CCpuMark2005Dlg dlg(this);
//	dlg.DoModal();
}
*/

/////////////////////////////////////////////////////////////////////////////
// Background Image Customize
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnCustomize() 
{
//	CCustomizeDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CUSTOMIZE,MF_GRAYED);
	SetMenu(menu);
	m_CustomizeDlg = new CCustomizeDlg(this);
	CreateIndirectDialog(m_CustomizeDlg, IDD_CUSTOMIZE);
}


/////////////////////////////////////////////////////////////////////////////
// Setting Crystal'n'Quiet
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnCrystalNQuiet() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_GRAYED);
	SetMenu(menu);

	m_CrystalNQuietDlg = new CCrystalNQuietDlg(this);
	CreateIndirectDialog(m_CrystalNQuietDlg, IDD_CRYSTAL_N_QUIET);
}

//////////////////////////////////////////////////////////////////////
// Feature Flags Dialog
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnFeatureFlags() 
{
//	CFeatureDlg dlg;
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_FEATURE_FLAGS,MF_GRAYED);
	SetMenu(menu);
	m_FeatureDlg = new CFeatureDlg(this);
	CreateIndirectDialog(m_FeatureDlg, IDD_FEATURE_DIALOG);
}

//////////////////////////////////////////////////////////////////////
// CPUID Detail Info Dialog
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnCpuidDetail() 
{
//	CCpuidDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CPUID_DETAIL,MF_GRAYED);
	SetMenu(menu);
	m_CpuidDlg = new CCpuidDlg(this);
	CreateIndirectDialog(m_CpuidDlg, IDD_CPUID_DIALOG);
}

//////////////////////////////////////////////////////////////////////
// Love Hammer Dialog
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnLoveHammer() 
{
//	CLoveHammerDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_GRAYED);
	SetMenu(menu);
	m_LoveHammerDlg = new CLoveHammerDlg(this);
	CreateIndirectDialog(m_LoveHammerDlg, IDD_LOVE_HAMMER);
}

//////////////////////////////////////////////////////////////////////
// LongHaul Dialog
//////////////////////////////////////////////////////////////////////
#ifndef _X86_64
void CCrystalCPUIDDlg::OnLongHaul() 
{
//	CLongHaulDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_LONG_HAUL,MF_GRAYED);
	SetMenu(menu);
	m_LongHaulDlg = new CLongHaulDlg(this);
	CreateIndirectDialog(m_LongHaulDlg, IDD_LONG_HAUL);
}
#endif
//////////////////////////////////////////////////////////////////////
// Speed Step Dialog
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnSpeedStep() 
{
//	CSpeedStepDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_SPEED_STEP,MF_GRAYED);
	SetMenu(menu);
	m_SpeedStepDlg = new CSpeedStepDlg(this);
	CreateIndirectDialog(m_SpeedStepDlg, IDD_SPEED_STEP);
}

//////////////////////////////////////////////////////////////////////
// Bios Info Dialog by WMI
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnBiosInfo() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_BIOS_INFO,MF_GRAYED);
	SetMenu(menu);
	BiosInfo();
	m_BiosInfoDlg = new CBiosInfoDlg(this);
	CreateIndirectDialog(m_BiosInfoDlg, IDD_BIOS_INFO);
}

//////////////////////////////////////////////////////////////////////
// Cache Info Dialog
//////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnCacheInfo() 
{
//	CCacheInfoDlg dlg(this);;
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CACHE_INFO,MF_GRAYED);
	SetMenu(menu);
	m_CacheInfoDlg = new CCacheInfoDlg(this);
	CreateIndirectDialog(m_CacheInfoDlg, IDD_CACHE_INFO);
}

//////////////////////////////////////////////////////////////////////
// Transmeta Info Dialog
//////////////////////////////////////////////////////////////////////
#ifndef _X86_64
void CCrystalCPUIDDlg::OnTransmetaInfo() 
{
//	CTransmetaInfoDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_TRANSMETA_INFO,MF_GRAYED);
	SetMenu(menu);
	m_TransmetaInfoDlg = new CTransmetaInfoDlg(this);
	CreateIndirectDialog(m_TransmetaInfoDlg, IDD_TRANSMETA_INFO);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// MSR Editor
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnMsrEdit() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_MSR_EDIT,MF_GRAYED);
	SetMenu(menu);
	m_MsrEditDlg = new CMsrEditDlg(this);
	CreateIndirectDialog(m_MsrEditDlg, IDD_MSR_EDIT);
}

/////////////////////////////////////////////////////////////////////////////
// MSR Walker
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnMsrWalker() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_MSR_WALKER,MF_GRAYED);
	SetMenu(menu);
	m_MsrWalkerDlg = new CMsrWalkerDlg(this);
	CreateIndirectDialog(m_MsrWalkerDlg, IDD_MSR_WALKER);
}

/////////////////////////////////////////////////////////////////////////////
// Processor Serial Number Dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef _X86_64
void CCrystalCPUIDDlg::OnProcessorSerial() 
{
//	CProcessorSerialDlg dlg(this);
//	dlg.DoModal();	
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_PROCESSOR_SERIAL,MF_GRAYED);
	SetMenu(menu);
	m_ProcessorSerialDlg = new CProcessorSerialDlg(this);
	CreateIndirectDialog(m_ProcessorSerialDlg, IDD_PROCESSOR_SERIAL);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// System Information by DMI Dialog
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnDmiInfo() 
{
//	CDmiInfoDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_DMI_INFO,MF_GRAYED);
	SetMenu(menu);
	DmiInfo();
	m_DmiInfoDlg = new CDmiInfoDlg(this);
	CreateIndirectDialog(m_DmiInfoDlg, IDD_DMI_INFO);
}

/////////////////////////////////////////////////////////////////////////////
// Chipset Information
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnChipsetInfo() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_CHIPSET_INFO,MF_GRAYED);
	SetMenu(menu);
	ChipsetInfo();
	m_ChipsetInfoDlg = new CChipsetInfoDlg(this);
	CreateIndirectDialog(m_ChipsetInfoDlg, IDD_CHIPSET_INFO);
}

/////////////////////////////////////////////////////////////////////////////
// PCI Device Information
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnPciInfo() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_PCI_INFO,MF_GRAYED);
	SetMenu(menu);
	m_PciInfoDlg = new CPciInfoDlg(this);
	CreateIndirectDialog(m_PciInfoDlg, IDD_PCI_INFO);		
}

/////////////////////////////////////////////////////////////////////////////
// Real Time Clock
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnRealTimeClock() 
{
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_REAL_TIME_CLOCK,MF_GRAYED);
	SetMenu(menu);
	m_RealTimeClockDlg = new CRealTimeClockDlg(this);
	m_RealTimeClockDlg->Create();

/*
	if( m_RealTimeClockDlg == NULL){
	}else{
		m_RealTimeClockDlg->BringWindowToTop();
	}
	CRealTimeClockDlg dlg(this);
	dlg.DoModal();		
*/
}

/////////////////////////////////////////////////////////////////////////////
// About
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnAbout() 
{
//	CAboutDlg dlg(this);
//	dlg.DoModal();
	CMenu *menu = GetMenu();
	menu->EnableMenuItem(IDM_ABOUT,MF_GRAYED);
	SetMenu(menu);
	m_AboutDlg = new CAboutDlg(this);
	m_AboutDlg->Create();
}

/////////////////////////////////////////////////////////////////////////////
// Hide Original Clock & Multiplier Info
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnHideOriginalClock() 
{
	CMenu *menu = GetMenu();

	if( m_FlagHideOriginalClock ){
		m_FlagHideOriginalClock = FALSE;
		menu->CheckMenuItem(IDM_HIDE_ORIGINAL_CLOCK,MF_UNCHECKED);
	}else{
		m_FlagHideOriginalClock = TRUE;
		menu->CheckMenuItem(IDM_HIDE_ORIGINAL_CLOCK,MF_CHECKED);
	}

	try{
		Refresh( 0 );
	}catch(...){
		DebugInfo("OnHideOriginalClock:Refresh( 0 )");	
	}
//	WritePrivateProfileString("Setting","HideOriginalClock","1",m_ini);

	SetMenu(menu);
}

/////////////////////////////////////////////////////////////////////////////
// Hide Date && Time
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnHideDate() 
{
	CMenu *menu = GetMenu();

	if( m_FlagHideDate ){
		m_FlagHideDate = FALSE;
		menu->CheckMenuItem(IDM_HIDE_DATE,MF_UNCHECKED);
	}else{
		m_FlagHideDate = TRUE;
		menu->CheckMenuItem(IDM_HIDE_DATE,MF_CHECKED);
	}
	try{
		Refresh( 0 );
	}catch(...){
		DebugInfo("OnHideDate:Refresh( 0 )");	
	}
	SetMenu(menu);
}

/////////////////////////////////////////////////////////////////////////////
// Force Hide Original Clock & Multiplier Info
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::ForceHideOriginalClock() 
{
	m_FlagHideOriginalClock = FALSE;
	OnHideOriginalClock();
}

/////////////////////////////////////////////////////////////////////////////
// MENU MENU MENU
/////////////////////////////////////////////////////////////////////////////
//void CCrystalCPUIDDlg::OnCopyImage()		{ImageCopy( 1 /* Clipboard */ );}
void CCrystalCPUIDDlg::OnSaveImage()		{ImageCopy( 2 /* File */ );}
void CCrystalCPUIDDlg::OnQuickSaveImage()	{ImageCopy( 3 /* Quick Save */ );}
void CCrystalCPUIDDlg::OnCopyText()			{TextCopy( 1 /* Clipboard */ );}
void CCrystalCPUIDDlg::OnSaveText()			{TextCopy( 2 /* File */ );}
void CCrystalCPUIDDlg::OnPreviewText()		{TextCopy( 3 /* Preview */ );}

void CCrystalCPUIDDlg::OnCpuInfoText(){
	ExecuteFile("SysInfo.txt");
}

void CCrystalCPUIDDlg::OnCrystalCpuidText(){
	ExecuteFile("CrystalCPUID.txt");
}

void CCrystalCPUIDDlg::OnCrystalCpuidIni() 
{
	ExecuteFile(m_ini);
}

void CCrystalCPUIDDlg::OnLicense()
{
	if(	gLanguage == JAPANESE ){
		ExecuteFile("COPYRIGHT-ja.txt");
	}else{
		ExecuteFile("COPYRIGHT.txt");
	}
}

void CCrystalCPUIDDlg::OnReadMe() 
{
	if(	gLanguage == JAPANESE ){
		ExecuteFile("ReadMeCpuidJ.txt");
	}else{
		ExecuteFile("ReadMeCpuid.txt");
	}
}


void CCrystalCPUIDDlg::OnCpuInfoHistoryText() 
{
	if(	gLanguage == JAPANESE ){
		ExecuteFile("HistorySysInfoJ.txt");
	}else{
		ExecuteFile("HistorySysInfo.txt");
	}
}

void CCrystalCPUIDDlg::OnCrystalCpuidHistoryText() 
{
	if(	gLanguage == JAPANESE ){
		ExecuteFile("HistoryCpuidJ.txt");
	}else{
		ExecuteFile("HistoryCpuid.txt");
	}
}


// jump to Crystal Dew World
void CCrystalCPUIDDlg::OnCrystalDewWorld() 
{
	if(	gLanguage == JAPANESE ){
		ShellExecute(NULL,NULL,URL_JAPANESE,NULL,NULL,SW_SHOWNORMAL);
	}else{
		ShellExecute(NULL,NULL,URL_ENGLISH,NULL,NULL,SW_SHOWNORMAL);
	}	
}

BOOL CCrystalCPUIDDlg::PreTranslateMessage(MSG* pMsg) 
{
	if( 0 != ::TranslateAccelerator(m_hWnd, m_hAccelerator, pMsg) )
	{
		return TRUE;
	}

	/*
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F1  ){
		return FALSE;
	}
	*/
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CCrystalCPUIDDlg::ChipsetInfo()
{
/*
	static BOOL flagInit = FALSE;
	
	if( flagInit == TRUE ){
		return FALSE;
	}else{
		flagInit = TRUE;
	}
*/	
	DWORD data;
	SysInfo->GetData(SI_STATUS,&data);
	if( data != 0 /* DLLSTATUS_NOERROR */ ){
		return FALSE;
	}

	char str[256];
//	CString cstr;
	DWORD data1,data2,data3;
	
	m_Chipset.Format(" %s",SysInfo->GetInfo(PCI_CHIP_SET_NAME_SI,str));
	m_NorthBridge.Format(" %s",SysInfo->GetInfo(PCI_NORTH_DEVICE_NAME,str));
	m_SouthBridge.Format(" %s",SysInfo->GetInfo(PCI_SOUTH_DEVICE_NAME,str));
	m_VideoChip.Format(" %s",SysInfo->GetInfo(PCI_VIDEO_DEVICE_NAME,str));

	SysInfo->GetData(PCI_NORTH_VENDOR_ID,&data1);
	SysInfo->GetData(PCI_NORTH_DEVICE_ID,&data2);
	SysInfo->GetData(PCI_NORTH_REVISION_ID,&data3);
	m_NorthBridgeId.Format("%04X:%04X.%02X",data1,data2,data3);
	SysInfo->GetData(PCI_SOUTH_VENDOR_ID,&data1);
	SysInfo->GetData(PCI_SOUTH_DEVICE_ID,&data2);
	SysInfo->GetData(PCI_SOUTH_REVISION_ID,&data3);
	m_SouthBridgeId.Format("%04X:%04X.%02X",data1,data2,data3);
	SysInfo->GetData(PCI_VIDEO_VENDOR_ID,&data1);
	SysInfo->GetData(PCI_VIDEO_DEVICE_ID,&data2);
	SysInfo->GetData(PCI_VIDEO_REVISION_ID,&data3);
	m_VideoChipId.Format("%04X:%04X.%02X",data1,data2,data3);

	m_ChipsetInfo = "\r\n";
	m_ChipsetInfo += "----------------------------------------------------------------\r\n";
	m_ChipsetInfo += " Chipset Information\r\n";
	m_ChipsetInfo += "----------------------------------------------------------------\r\n";
	cstr.Format("        Chipset :%s\r\n",m_Chipset);m_ChipsetInfo += cstr;
	cstr.Format("   North Bridge : [%s]%s\r\n",m_NorthBridgeId,m_NorthBridge);m_ChipsetInfo += cstr;
	cstr.Format("   South Bridge : [%s]%s\r\n",m_SouthBridgeId,m_SouthBridge);m_ChipsetInfo += cstr;
	cstr.Format("     Video Chip : [%s]%s\r\n",m_VideoChipId,m_VideoChip);m_ChipsetInfo += cstr;

	return TRUE;
}

BOOL CCrystalCPUIDDlg::PciDeviceInfo()
{
	DWORD data;
	DWORD i;
	char str[256];
	DWORD NumberOfDevice;

	SysInfo->GetData(SI_STATUS,&data);
	if( data != 0 /* DLLSTATUS_NOERROR */ ){
		return FALSE;
	}


	m_PciDeviceInfo = "\r\n";
	m_PciDeviceInfo += "----------------------------------------------------------------\r\n";
	m_PciDeviceInfo += " PCI Device Information\r\n";
	m_PciDeviceInfo += "----------------------------------------------------------------\r\n";
//	m_PciDeviceInfo += "No Bus Dev Fnc VendorDeviceRev SubSystem  Class  DeviceType\r\n";
	m_PciDeviceInfo += "No Bus Dev Fnc VendorDeviceRev Class   DeviceType\r\n";

	SysInfo->GetData(PCI_NUMBER_OF_DEVICE, &NumberOfDevice);
	for(i = 0;i < NumberOfDevice;i++){
		cstr.Format("%2d ",i);m_PciDeviceInfo += cstr;
		SysInfo->GetData(PCI_PCI_BUS_BASE + i,&data);
		cstr.Format("%3d", data);m_PciDeviceInfo += cstr + " ";
		SysInfo->GetData(PCI_PCI_DEVICE_BASE + i,&data);
		cstr.Format("%3d", data);m_PciDeviceInfo += cstr + " ";
		SysInfo->GetData(PCI_PCI_FUNCTION_BASE + i,&data);
		cstr.Format("%3d", data);m_PciDeviceInfo += cstr + " ";
		SysInfo->GetData(PCI_VENDOR_ID_BASE + i,&data);
		cstr.Format("%04Xh:", data);m_PciDeviceInfo += cstr;
		SysInfo->GetData(PCI_DEVICE_ID_BASE + i,&data);
		cstr.Format("%04Xh.", data);m_PciDeviceInfo += cstr;
		SysInfo->GetData(PCI_REVISION_ID_BASE + i,&data);
		cstr.Format("%02Xh", data);m_PciDeviceInfo += cstr + " ";
//		SysInfo->GetData(PCI_SUBSYSTEM_ID_BASE + i,&data);
//		cstr.Format("%08Xh", data);m_PciDeviceInfo += cstr + " ";
		SysInfo->GetData(PCI_CLASS_ID_BASE + i,&data);
		cstr.Format("%06Xh", data);m_PciDeviceInfo += cstr + " ";
		SysInfo->GetString(PCI_CLASS_NAME_BASE + i,str);
		m_PciDeviceInfo += str;
		m_PciDeviceInfo += "\r\n";
	}
	m_PciDeviceInfo += "\r\n";
	m_PciDeviceInfo += "No DeviceName\r\n";
	for(i = 0;i < NumberOfDevice;i++){
		cstr.Format("%2d ",i);m_PciDeviceInfo += cstr;
		SysInfo->GetString(PCI_VENDOR_NAME_BASE + i,str);
		m_PciDeviceInfo += str; m_PciDeviceInfo += " ";
		SysInfo->GetString(PCI_DEVICE_NAME_BASE + i,str);
		m_PciDeviceInfo += str; m_PciDeviceInfo += " ";
		m_PciDeviceInfo += "\r\n";
	}
	m_PciDeviceInfo += "\r\n";

	return TRUE;
}

BOOL CCrystalCPUIDDlg::DmiInfo()
{
/*
	static BOOL flagInit = FALSE;
	
	if( flagInit == TRUE ){
		return FALSE;
	}else{
		flagInit = TRUE;
	}
*/	
	DWORD data;
	SysInfo->GetData(DMI_STATUS,&data);
	if( data == FALSE ){
		return FALSE;
	}	
	
	char str[256];
//	CString cstr;

	m_DmiVersion.Format(" %s",SysInfo->GetInfo(DMI_VERSION,str));
	m_DmiBiosVendor.Format(" %s",SysInfo->GetInfo(DMI_BIOS_VENDOR,str));
	m_DmiBiosVersion.Format(" %s",SysInfo->GetInfo(DMI_BIOS_VERSION,str));
	m_DmiBiosReleaseDate.Format(" %s",SysInfo->GetInfo(DMI_BIOS_RELEASE_DATE,str));
	m_DmiBiosRomSize.Format(" %s",SysInfo->GetInfo(DMI_BIOS_ROM_SIZE,str));
	m_DmiMotherManufacturer.Format(" %s",SysInfo->GetInfo(DMI_MOTHER_MANUFACTURER,str));
	m_DmiMotherProduct.Format(" %s",SysInfo->GetInfo(DMI_MOTHER_PRODUCT,str));
	m_DmiMotherVersion.Format(" %s",SysInfo->GetInfo(DMI_MOTHER_VERSION,str));
	m_DmiCpuSocket.Format(" %s",SysInfo->GetInfo(DMI_CPU_SOCKET,str));
	m_DmiCpuManufacturer.Format(" %s",SysInfo->GetInfo(DMI_CPU_MANUFACTURER,str));
	m_DmiCpuVersion.Format(" %s",SysInfo->GetInfo(DMI_CPU_VERSION,str));
	m_DmiCpuCurrentClock.Format(" %s",SysInfo->GetInfo(DMI_CPU_CURRENT_CLOCK,str));
	m_DmiCpuExternalClock.Format(" %s",SysInfo->GetInfo(DMI_CPU_EXTERNAL_CLOCK,str));
	m_DmiCpuMaxClock.Format(" %s",SysInfo->GetInfo(DMI_CPU_MAX_CLOCK,str));

	m_DmiInfo = "\r\n";
	m_DmiInfo += "----------------------------------------------------------------\r\n";
	m_DmiInfo += " System Information by DMI (Desktop Management Interface)\r\n";
	m_DmiInfo += "----------------------------------------------------------------\r\n";
	cstr.Format("SM BIOS Version :%s\r\n",m_DmiVersion);m_DmiInfo += cstr;
	cstr.Format("    BIOS Vendor :%s\r\n",m_DmiBiosVendor);m_DmiInfo += cstr;
	cstr.Format("   BIOS Version :%s\r\n",m_DmiBiosVersion);m_DmiInfo += cstr;
	cstr.Format("      BIOS Date :%s\r\n",m_DmiBiosReleaseDate);m_DmiInfo += cstr;
	cstr.Format("  BIOS ROM Size :%s KB\r\n",m_DmiBiosRomSize);m_DmiInfo += cstr;
	cstr.Format("  Mother Vendor :%s\r\n",m_DmiMotherManufacturer);m_DmiInfo += cstr;
	cstr.Format(" Mother Product :%s\r\n",m_DmiMotherProduct);m_DmiInfo += cstr;
	cstr.Format(" Mother Version :%s\r\n",m_DmiMotherVersion);m_DmiInfo += cstr;
	cstr.Format("     CPU Socket :%s\r\n",m_DmiCpuSocket);m_DmiInfo += cstr;
	cstr.Format("     CPU Vendor :%s\r\n",m_DmiCpuManufacturer);m_DmiInfo += cstr;
	cstr.Format("    CPU Version :%s\r\n",m_DmiCpuVersion);m_DmiInfo += cstr;
	cstr.Format("  Current Clock :%s MHz\r\n",m_DmiCpuCurrentClock);m_DmiInfo += cstr;
	cstr.Format(" External Clock :%s MHz\r\n",m_DmiCpuExternalClock);m_DmiInfo += cstr;
	cstr.Format("      Max Clock :%s MHz\r\n",m_DmiCpuMaxClock);m_DmiInfo += cstr;

	return TRUE;
}

typedef HRESULT(WINAPI * _CoInitializeSecurity_)(PSECURITY_DESCRIPTOR pVoid,LONG cAuthSvc,SOLE_AUTHENTICATION_SERVICE * asAuthSvc,void * pReserved1,
DWORD dwAuthnLevel,DWORD dwImpLevel,SOLE_AUTHENTICATION_LIST * pAuthList,DWORD dwCapabilities,void * pReserved3);

BOOL CCrystalCPUIDDlg::BiosInfo()
{

	static BOOL flagInit = FALSE;
	
	if( flagInit == TRUE ){
		return FALSE;
	}else{
		flagInit = TRUE;
	}

	try{
		HRESULT hRes = S_OK;
//		CString cstr;
		DWORD uReturned = 1;
		char text[256];

		IWbemLocator *pIWbemLocator = NULL;
		IWbemServices* pIWbemServices = NULL;
		IEnumWbemClassObject *pEnumCOMDevs = NULL;
		IWbemClassObject  *pCOMDev = NULL;

		CoInitialize( NULL );

		_CoInitializeSecurity_ CoInitializeSecurity = NULL;
		HINSTANCE hDLL = LoadLibrary( "ole32.dll" );
		if(  hDLL != NULL ){
			CoInitializeSecurity = (_CoInitializeSecurity_)GetProcAddress( hDLL, "CoInitializeSecurity" );
		}
		if(CoInitializeSecurity == NULL){
			FreeLibrary(hDLL);
			return FALSE;
		}
		CoInitializeSecurity(NULL, -1, NULL, NULL,
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,EOAC_SECURE_REFS,NULL );
		FreeLibrary(hDLL);

		if( CoCreateInstance(CLSID_WbemLocator,NULL,
			CLSCTX_INPROC_SERVER,IID_IWbemLocator,
			(LPVOID *) &pIWbemLocator) != S_OK ){return FALSE;}
		if( pIWbemLocator->ConnectServer(SysAllocString(L"\\\\.\\root\\cimv2"),
			NULL,NULL,0L,0L,NULL,NULL,&pIWbemServices) != S_OK ){return FALSE;}

		CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
                          NULL, RPC_C_AUTHN_LEVEL_CALL, 
                          RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

		pIWbemLocator->Release();

	/////////////////
	//BIOS
		pIWbemServices->ExecQuery(SysAllocString(L"WQL"),
			SysAllocString(L"select ReleaseDate,Version,Manufacturer,Caption,SMBIOSBIOSVersion,SMBIOSMajorVersion,SMBIOSMinorVersion from Win32_BIOS"),0,NULL,&pEnumCOMDevs);
		if( SUCCEEDED( pEnumCOMDevs->Next(2000,1,&pCOMDev,&uReturned )) && uReturned == 1){
			VARIANT  pVal;VariantClear(&pVal);
			if( pCOMDev->Get(L"ReleaseDate",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				cstr = pVal.bstrVal;
				if(cstr.GetLength() >= 8 && atoi(cstr) > 0){
					strncpy(text,(LPCSTR)cstr+4,2);strcpy(text+2,"/");
					strncpy(text+3,(LPCSTR)cstr+6,2);strcpy(text+5,"/");
					strncpy(text+6,(LPCSTR)cstr,4);strcpy(text+10,"\0");
					m_BiosDate = " ";
					m_BiosDate += text;
					m_BiosDate.Replace("190","200");
				}
				VariantClear(&pVal);
			}

			if( pCOMDev->Get(L"Version",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_BiosVersion = " ";
				m_BiosVersion += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"Manufacturer",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_BiosManufacturer = " ";
				m_BiosManufacturer += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"Caption",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_BiosCaption = " ";
				m_BiosCaption += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"SMBIOSBIOSVersion",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_SmBiosCaption = " ";
				m_SmBiosCaption += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"SMBIOSMajorVersion",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_SmBiosVersion.Format(" %d",pVal.ulVal);
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"SMBIOSMinorVersion",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				cstr.Format(".%d",pVal.ulVal);
				m_SmBiosVersion += cstr;
			}
			pCOMDev->Release();
		}

	/////////////////
	//Computer System
		pIWbemServices->ExecQuery(SysAllocString(L"WQL"),
			SysAllocString(L"select Manufacturer,Model from Win32_ComputerSystem"),0,NULL,&pEnumCOMDevs);

		if( SUCCEEDED( pEnumCOMDevs->Next(2000,1,&pCOMDev,&uReturned )) && uReturned == 1){
			VARIANT  pVal;VariantClear(&pVal);
			if( pCOMDev->Get(L"Manufacturer",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_SystemManufacturer = " ";
				m_SystemManufacturer += pVal.bstrVal;
				VariantClear(&pVal);}
			if( pCOMDev->Get(L"Model",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_SystemModel = " ";
				m_SystemModel += pVal.bstrVal;
			}
			pCOMDev->Release();
		}
	/////////////////
	//Mother
		pIWbemServices->ExecQuery(SysAllocString(L"WQL"),
			SysAllocString(L"select Manufacturer,Product,Version from Win32_BaseBoard"),0,NULL,&pEnumCOMDevs);
		if( SUCCEEDED( pEnumCOMDevs->Next(2000,1,&pCOMDev,&uReturned )) && uReturned == 1){
			VARIANT  pVal;VariantClear(&pVal);
			if( pCOMDev->Get(L"Version",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_MotherVersion = " ";
				m_MotherVersion += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"Product",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_MotherProduct = " ";
				m_MotherProduct += pVal.bstrVal;
				VariantClear(&pVal);
			}
			if( pCOMDev->Get(L"Manufacturer",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL ){
				m_MotherManufacturer = " ";
				m_MotherManufacturer += pVal.bstrVal;
			}
			pCOMDev->Release();
		}
	}catch(...){
		return FALSE;
	}

//	CString cstr;
	m_BiosInfo = "\r\n";
	m_BiosInfo += "----------------------------------------------------------------\r\n";
	m_BiosInfo += " BIOS Information by WMI (Windows Management Interface)\r\n";
	m_BiosInfo += "----------------------------------------------------------------\r\n";
	cstr.Format("    BIOS Vendor :%s\r\n",m_BiosManufacturer);m_BiosInfo += cstr;
	cstr.Format("   BIOS Caption :%s\r\n",m_BiosCaption);m_BiosInfo += cstr;
	cstr.Format("   BIOS Version :%s\r\n",m_BiosVersion);m_BiosInfo += cstr;
	cstr.Format("      BIOS Date :%s\r\n",m_BiosDate);m_BiosInfo += cstr;
	cstr.Format("SM BIOS Caption :%s\r\n",m_SmBiosCaption);m_BiosInfo += cstr;
	cstr.Format("SM BIOS Version :%s\r\n",m_SmBiosVersion);m_BiosInfo += cstr;
	cstr.Format("  Mother Vendor :%s\r\n",m_MotherManufacturer);m_BiosInfo += cstr;
	cstr.Format(" Mother Product :%s\r\n",m_MotherProduct);m_BiosInfo += cstr;
	cstr.Format(" Mother Version :%s\r\n",m_MotherVersion);m_BiosInfo += cstr;
	cstr.Format("  System Vendor :%s\r\n",m_SystemManufacturer);m_BiosInfo += cstr;
	cstr.Format("   System Model :%s\r\n",m_SystemModel);m_BiosInfo += cstr;

	m_IsWMI = TRUE;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Enable Crystal'n'Quiet
/////////////////////////////////////////////////////////////////////////////
void CCrystalCPUIDDlg::OnEnableCrystalNQuiet() 
{
	CMenu *menu = GetMenu();
	unsigned threadID;

	if( m_FlagCrystalNQuiet ){
		m_FlagCrystalNQuiet = FALSE;
		menu->CheckMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_UNCHECKED);

//		WaitForSingleObject(m_hThread, 10000);
//		menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_GRAYED);
	}else{
		m_FlagCrystalNQuiet = TRUE;
		menu->CheckMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_CHECKED);
	}
	if( m_FlagCrystalNQuiet && m_hThread == NULL ){
	//	SetTimer(1,1000,NULL);
		InitCpuUsage();
		m_hThread = (HANDLE)_beginthreadex(NULL, 0, CrystalNQuiet, this, 0, &threadID);
		// AfxBeginThread(CrystalNQuiet, (LPVOID) this, THREAD_PRIORITY_NORMAL, 0, 0,NULL);
	}else{
	//	KillTimer(1);
	}

	SetMenu(menu);
}

//void CCrystalCPUIDDlg::OnTimer(UINT nIDEvent) 
unsigned int __stdcall CrystalNQuiet(void* pParam)
//void CrystalNQuiet( LPVOID pParam )
{
	CCrystalCPUIDDlg * P = (CCrystalCPUIDDlg*) pParam;

	static int FID = -1, VID = -1;
	static int count = 0;
	static int SleepTime = 500;
	static int usage = -1;
	static int usageOld = -1;
	P->m_CqStatus = STATUS_OFF;
	CString cstr;
	char str[256];

	cstr.Format("Multiplier=%s", P->m_Multiplier);
	P->ModifyTaskTrayTips(cstr);

	while( P->m_FlagCrystalNQuiet )
	{
		usage = GetCpuUsage(P->m_CqTrigerType);
		if( usage != usageOld ){
			cstr.Format("%03d%% %s %s", usage, CRYSTAL_CPUID_PRODUCT, P->CrystalCPUIDVersion);
			P->SetWindowText(cstr);
			usageOld = usage;
			P->m_CpuUsage = usage;
		}

		VID = -1;
		switch( P->m_CqStatus )
		{
		case STATUS_MAX:
			if( usage <= P->m_CqMaxThresholdDown ){
				if( P->m_CqModeDownType == 0 ){
					FID = P->m_CqMidFID;
					P->m_CqStatus = STATUS_MID;
					SleepTime = P->m_CqMidIntervalTime;
					if( P->m_CqEnableVoltage ){
						VID = P->m_CqMidVID;
					}
				}else{
					FID = P->m_CqMinFID;
					P->m_CqStatus = STATUS_MIN;
					SleepTime = P->m_CqMinIntervalTime;
					if( P->m_CqEnableVoltage ){
						VID = P->m_CqMinVID;
					}
				}
			}else{
				Sleep( SleepTime );
				continue;
			}
			break;
		case STATUS_MID:
			if( usage >= P->m_CqMidThresholdUp ){
				FID = P->m_CqMaxFID;
				P->m_CqStatus = STATUS_MAX;
				SleepTime = P->m_CqMaxIntervalTime;
				if( P->m_CqEnableVoltage ){
					VID = P->m_CqMaxVID;
				}
			}else if( usage <= P->m_CqMidThresholdDown ){
				FID = P->m_CqMinFID;
				P->m_CqStatus = STATUS_MIN;
				SleepTime = P->m_CqMinIntervalTime;
				if( P->m_CqEnableVoltage ){
					VID = P->m_CqMinVID;
				}
			}else{
				Sleep( SleepTime );
				continue;
			}
			break;
		case STATUS_MIN:
			if( usage >= P->m_CqMinThresholdUp ){
				if( P->m_CqModeUpType == 0 ){
					FID = P->m_CqMidFID;
					P->m_CqStatus = STATUS_MID;
					SleepTime = P->m_CqMidIntervalTime;
					if( P->m_CqEnableVoltage ){
						VID = P->m_CqMidVID;
					}
				}else{
					FID = P->m_CqMaxFID;
					P->m_CqStatus = STATUS_MAX;
					SleepTime = P->m_CqMaxIntervalTime;
					if( P->m_CqEnableVoltage ){
						VID = P->m_CqMaxVID;
					}
				}
			}else{
				Sleep( SleepTime );
				continue;
			}
			break;
		case STATUS_STARTUP:
			SetupFVID(P,STATUS_STARTUP,&FID,&VID);
			break;
		case STATUS_OFF:
		default:
			if( usage >= P->m_CqMidThresholdUp ){
				FID = P->m_CqMaxFID;
				P->m_CqStatus = STATUS_MAX;
				SleepTime = P->m_CqMaxIntervalTime;
				if( P->m_CqEnableVoltage ){
					VID = P->m_CqMaxVID;
				}
			}else if( usage <= P->m_CqMidThresholdDown ){
				FID = P->m_CqMinFID;
				P->m_CqStatus = STATUS_MIN;
				SleepTime = P->m_CqMinIntervalTime;
				if( P->m_CqEnableVoltage ){
					VID = P->m_CqMinVID;
				}
			}else{
				FID = P->m_CqMidFID;
				P->m_CqStatus = STATUS_MID;
				SleepTime = P->m_CqMidIntervalTime;
				if( P->m_CqEnableVoltage ){
					VID = P->m_CqMidVID;
				}
			}
			break;
		}

		if(! P->m_FlagStopCrystalNQuiet ){
			LoveHammer(P, FID, VID);
			DWORD data;
			P->SysInfo->GetString(CPU_MULTIPLIER_UPDATE,str);
			P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
			P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str);
			double multiplier = atof(str);
			P->SysInfo->GetInfo(LH_GET_CURRENT_VOLTAGE,str);
			double voltage = atof(str);

			if( VID >= 0 ){
				cstr.Format("Multiplier=%.1fx, Voltage=%.3fV", multiplier, voltage);
			}else{
				cstr.Format("Multiplier=%.1fx", multiplier);
			}
			P->ModifyTaskTrayTips(cstr);
			P->ModifyTaskTrayIcon(P->m_CqStatus);
			Sleep( SleepTime );
		}
	}

	if(! P->m_FlagStopCrystalNQuiet ){
		SetupFVID(P, P->m_CqExitMode, &FID, &VID);
		LoveHammer(P, FID, VID);
	}

	CleanupCpuUsage();

	if( ! P->m_FlagExitNow ){
		P->ModifyTaskTrayIcon(STATUS_OFF);
		P->ModifyTaskTrayTips("CrystalCPUID");
/*
		CMenu *menu = P->GetMenu();
		menu->EnableMenuItem(IDM_ENABLE_CRYSTAL_N_QUIET,MF_ENABLED);
		P->SetMenu(menu);
*/
		cstr.Format("%s %s", CRYSTAL_CPUID_PRODUCT, P->CrystalCPUIDVersion);
		P->SetWindowText(cstr);
	}

	P->m_hThread = NULL;
	P->m_FlagStopCrystalNQuiet = FALSE;

    _endthreadex( 0 );
    return 0;
}

void SetupFVID(CCrystalCPUIDDlg* P, int mode, int* FID, int* VID)
{
	switch( mode ){
	case STATUS_MID:
		P->m_CqStatus = STATUS_MID;
		*FID = P->m_CqMidFID;
		if( P->m_CqEnableVoltage ){
			*VID = P->m_CqMidVID;
		}
		break;
	case STATUS_MIN:
		P->m_CqStatus = STATUS_MIN;
		*FID = P->m_CqMinFID;
		if( P->m_CqEnableVoltage ){
			*VID = P->m_CqMinVID;
		}
		break;
	case STATUS_MAX:
		P->m_CqStatus = STATUS_MAX;
		*FID = P->m_CqMaxFID;
		if( P->m_CqEnableVoltage ){
			*VID = P->m_CqMaxVID;
		}
		break;
	case STATUS_STARTUP:
	default:// STATUS_STARTUP
		P->SysInfo->GetData(LH_GET_STARTUP_FID, (unsigned long *)FID);
		P->SysInfo->GetData(LH_GET_STARTUP_VID, (unsigned long *)VID);
		break;
	}
}

void LoveHammer(CCrystalCPUIDDlg* P, int FID, int VID)
{
	DWORD data;
	int fid, vid;

	switch ( P->m_LhType )
	{
	case LONG_HAUL_LEVEL_1:
		if( 0 <= FID && FID <= 15 ){
			P->SysInfo->SetData(LH_SET_FID, FID);
		}
		break;
	case LONG_HAUL_LEVEL_2:
		if( 0 <= FID && FID <= 31 ){
			P->SysInfo->SetData(LH_SET_FID, FID);
		}
		break;
	case LONG_HAUL_LEVEL_3:
		{
		double multi;
		char str[256];
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		P->SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
		multi = atof( P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str) );
		if( 0 <= FID && FID <= 31 && FID != fid ){
			P->SysInfo->SetData(LH_SET_FID, FID);
		}
		}
		break;	
	case LOVE_HAMMER_K6:
		{
		double multi;
		char str[256];
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		P->SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
		multi = atof( P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str) );
		if( 0 <= FID && FID <= 7 && FID != fid ){
			P->SysInfo->SetData(LH_SET_FID, FID);
		}
		}
		break;	
	case LOVE_HAMMER_K7:
		{
		BOOL flag = FALSE;
		double multi;
		char str[256];
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		P->SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
		P->SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
		multi = atof( P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str) );
		if( (MultiTableMobileAthlon[FID] / 2.0) >= multi ){
			if( 0 <= VID && VID <= 30 && VID != vid ){
				P->SysInfo->SetData(LH_SET_VID, VID);
				flag = TRUE;
			}
			if( 0 <= FID && FID <= 31 && FID != fid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_FID, FID);
			}
		}else{
			if( 0 <= FID && FID <= 31 && FID != fid ){
				P->SysInfo->SetData(LH_SET_FID, FID);
				flag = TRUE;
			}
			if( 0 <= VID && VID <= 30 && VID != vid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_VID, VID);
			}
		}
		}
		break;
	case LOVE_HAMMER_K8:
		{
		BOOL flag = FALSE;
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		P->SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
		P->SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
		if( FID >= fid ){
			if( 0 <= VID && VID <= 31 && VID != vid ){
				P->SysInfo->SetData(LH_SET_VID, VID);
				flag = TRUE;
			}
			if( 0 <= FID && FID <= 31 && FID != fid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_FID, FID);
			}
		}else{
			if( 0 <= FID && FID <= 31 && FID != fid ){
				P->SysInfo->SetData(LH_SET_FID, FID);
				flag = TRUE;
			}
			if( 0 <= VID && VID <= 31 && VID != vid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_VID, VID);
			}
		}
		}
		break;
	case SPEED_STEP_PM:
	case SPEED_STEP_P4:
	case SPEED_STEP_CORE_MA:
	case SPEED_STEP_PENRYN:
		{
		BOOL flag = FALSE;
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		P->SysInfo->GetData(LH_GET_CURRENT_FID, (DWORD*)&fid);
		P->SysInfo->GetData(LH_GET_CURRENT_VID, (DWORD*)&vid);
/*
		CString cstr1,cstr2;
		cstr1.Format("FID=%d, VID=%d", FID, VID);
		cstr2.Format("fid=%d, vid=%d", fid, vid);
		MessageBox(NULL,cstr1,cstr2,MB_OK);
*/
		int f, v;
		if( FID == fid && VID == vid ){
			break;
		}
		if( FID == -1 && VID == -1 ){
			break;
		}
		if( FID != -1 ){
			f = FID;
		}else{
			f = fid;
		}
		if( VID != -1 ){
			v = VID;
		}else{
			v = vid;
		}
		DWORD fvid = P->m_LhType >= SPEED_STEP_PENRYN
				? (((f / 2 + 6) << 8) | ((f % 2) << 14))
				: (((f + 6) << 8) | v);
		P->SysInfo->SetData(LH_SET_FVID, fvid);

/*

		if( FID + 6 >= fid ){
			if( 0 <= VID && VID <= 64 && VID != vid ){
				P->SysInfo->SetData(LH_SET_VID, VID);
				flag = TRUE;
//		MessageBox(NULL,"VID","OK",MB_OK);
			}
			if( 0 <= FID && FID <= 256 && FID != fid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_FID, FID);
//		MessageBox(NULL,"FID","OK",MB_OK);
			}
		}else{
			if( 0 <= FID && FID <= 64 && FID != fid ){
				P->SysInfo->SetData(LH_SET_FID, FID);
				flag = TRUE;
//		MessageBox(NULL,"FID","OK",MB_OK);
			}
			if( 0 <= VID && VID <= 256 && VID != vid ){
				if(flag){Sleep(P->m_CqWaitTime);}
				P->SysInfo->SetData(LH_SET_VID, VID);
//		MessageBox(NULL,"VID","OK",MB_OK);
			}
		}
//		MessageBox(NULL,"OK","OK",MB_OK);
*/
		}

		break;
	default:
		break;
	}
}

LONG CCrystalCPUIDDlg::OnRegMessage(WPARAM wParam,LPARAM lParam)
{
	POINT pt;
	if(lParam == WM_LBUTTONDOWN){
		ShowWindow( ! IsWindowVisible() );
	}else if(lParam == WM_RBUTTONDOWN){
		CreateMainMenu();
		SetForegroundWindow();
		GetCursorPos(&pt);
		TrackPopupMenu(m_hMenu,TPM_LEFTALIGN|TPM_RIGHTBUTTON, pt.x,pt.y, 0, m_hWnd, NULL);
		PostMessage(WM_NULL);
	}
	return 0;
}

LRESULT CCrystalCPUIDDlg::OnTaskbarCreated(WPARAM, LPARAM) 
{ 
	AddTaskTray();
	return 0;
}

// Add TaskTray
void CCrystalCPUIDDlg::AddTaskTray()
{
	NOTIFYICONDATA nidata;
	nidata.cbSize = sizeof(NOTIFYICONDATA);
	nidata.hWnd = m_hWnd;
	nidata.uID = RegIconID;
	nidata.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	nidata.uCallbackMessage = RegMessageID;
	nidata.hIcon = m_hTaskIcon;
	wsprintf(nidata.szTip,m_TrayString);
	if(! m_FlagDisableTaskTrayIcon){
		if(::Shell_NotifyIcon(NIM_ADD,&nidata)){
		//	MessageBox("AddTaskTray TRUE");
		}else{
		//	MessageBox("AddTaskTray FALSE");
			for(int i=0;i<60;i++){
				if(::Shell_NotifyIcon(NIM_ADD, &nidata)){
					return;
				}
				Sleep(1000);
			}
			MessageBox("Falied Add Task Tray Icon!! Please reExecute.");
			OnCancel(); // Exit CrystalCPUID...
		}
	}
}

// Update TaskTray Icon
void CCrystalCPUIDDlg::ModifyTaskTrayIcon(int data)
{
	HICON hIcon;
	static int status = STATUS_OFF;

	switch(data)
	{
	case STATUS_UPDATE:
		switch (status)
		{
			case STATUS_MAX:hIcon = m_hTaskIconMax;break;
			case STATUS_MID:hIcon = m_hTaskIconMid;break;
			case STATUS_MIN:hIcon = m_hTaskIconMin;break;
			case STATUS_OFF:hIcon = m_hTaskIcon;break;
			default:hIcon = m_hTaskIcon;break;
		}
		break;
	case STATUS_MAX:hIcon = m_hTaskIconMax;status = STATUS_MAX;break;
	case STATUS_MID:hIcon = m_hTaskIconMid;status = STATUS_MID;break;
	case STATUS_MIN:hIcon = m_hTaskIconMin;status = STATUS_MIN;break;
	case STATUS_OFF:hIcon = m_hTaskIcon;status = STATUS_OFF;break;
	default:hIcon = m_hTaskIcon;status = STATUS_OFF;break;
	}

	NOTIFYICONDATA nidata;
	nidata.cbSize = sizeof(NOTIFYICONDATA);
	nidata.hWnd = m_hWnd;
	nidata.uID = RegIconID;
	nidata.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	nidata.uCallbackMessage = RegMessageID;
	nidata.hIcon = hIcon;
	wsprintf(nidata.szTip,m_TrayString);
	if(! m_FlagDisableTaskTrayIcon){
		::Shell_NotifyIcon(NIM_MODIFY,&nidata);
	}
}

// Update TaskTray Tips
void CCrystalCPUIDDlg::ModifyTaskTrayTips(CString cstr)
{
	m_TrayString = cstr;

	NOTIFYICONDATA nidata;
	nidata.cbSize = sizeof(NOTIFYICONDATA);
	nidata.hWnd = m_hWnd;
	nidata.uID = RegIconID;
	nidata.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	nidata.uCallbackMessage = RegMessageID;
	nidata.hIcon = m_hTaskIcon;
	wsprintf(nidata.szTip,m_TrayString);
	if(! m_FlagDisableTaskTrayIcon){
		::Shell_NotifyIcon(NIM_MODIFY,&nidata);
	}
}

// Remove TaskTray

void CCrystalCPUIDDlg::DelTaskTray()
{
	NOTIFYICONDATA nidata;
	nidata.cbSize = sizeof(NOTIFYICONDATA);
	nidata.hWnd = m_hWnd;
	nidata.uID = RegIconID;
	nidata.uFlags = NIF_TIP | NIF_ICON | NIF_MESSAGE;
	nidata.uCallbackMessage = RegMessageID;
	nidata.hIcon = m_hTaskIcon;
	strcpy(nidata.szTip,m_TrayString);
	if(! m_FlagDisableTaskTrayIcon){
		::Shell_NotifyIcon(NIM_DELETE,&nidata);
	}
}

static void LoadStringEx(char* str, int EnCode, int JaCode);

static void LoadStringEx(char* str, int EnCode, int JaCode)
{
	CString cstr;
	if( gLanguage == JAPANESE ){
		cstr.LoadString(JaCode);
	}else{
		cstr.LoadString(EnCode);
	}
	strcpy(str,cstr);
}

void CCrystalCPUIDDlg::CreateMainMenu()
{
	char str[256];

	m_hMenu = CreatePopupMenu();
	HMENU hSubMenu = CreateMenu();

	MENUITEMINFO menuinfo;
	ZeroMemory(&menuinfo,sizeof(MENUITEMINFO));
	menuinfo.cbSize=sizeof(MENUITEMINFO);
	menuinfo.fMask=MIIM_CHECKMARKS|MIIM_TYPE|MIIM_STATE|MIIM_ID|MIIM_SUBMENU | MIIM_ID;
	menuinfo.fType=MFT_RADIOCHECK;
	menuinfo.hbmpChecked=NULL;
	menuinfo.fState=MFS_UNCHECKED;

	MENUITEMINFO submenuinfo;
	ZeroMemory(&submenuinfo,sizeof(MENUITEMINFO));
	submenuinfo.cbSize=sizeof(MENUITEMINFO);
	submenuinfo.fMask=MIIM_CHECKMARKS|MIIM_TYPE|MIIM_STATE|MIIM_ID;
	submenuinfo.fType=MFT_RADIOCHECK;
	submenuinfo.hbmpChecked=NULL;
	submenuinfo.fState=MFS_UNCHECKED;

	submenuinfo.hSubMenu = NULL;
	submenuinfo.dwTypeData = "Maximum";
	submenuinfo.wID= MY_CRYSTAL_N_QUIET_MAX;
	InsertMenuItem(hSubMenu,-1,TRUE,&submenuinfo);
	submenuinfo.dwTypeData = "Middle";
	submenuinfo.wID= MY_CRYSTAL_N_QUIET_MID;
	InsertMenuItem(hSubMenu,-1,TRUE,&submenuinfo);
	submenuinfo.dwTypeData = "Minimum";
	submenuinfo.wID= MY_CRYSTAL_N_QUIET_MIN;
	InsertMenuItem(hSubMenu,-1,TRUE,&submenuinfo);

	if(m_LhType == 7 || m_LhType == 8){
		submenuinfo.dwTypeData = "Startup";
		submenuinfo.wID= MY_CRYSTAL_N_QUIET_STARTUP;
		InsertMenuItem(hSubMenu,-1,TRUE,&submenuinfo);
	}

	menuinfo.hSubMenu = hSubMenu;
	menuinfo.dwTypeData = "Quick Multiplier";
	menuinfo.wID= NULL;
	if( m_LhType > 0 && m_IsSysInfoInit ){
		InsertMenuItem(m_hMenu,-1,TRUE,&menuinfo);
	}

	menuinfo.hSubMenu = NULL;
	if( m_FlagCrystalNQuiet ){
		menuinfo.fState = MFS_CHECKED;
		LoadStringEx(str,
			IDS_POP_DISABLE_CRYSTAL_N_QUIET_EN,
			IDS_POP_DISABLE_CRYSTAL_N_QUIET_JP);
	}else{
		menuinfo.fState = MFS_UNCHECKED;
		LoadStringEx(str,
			IDS_POP_ENABLE_CRYSTAL_N_QUIET_EN,
			IDS_POP_ENABLE_CRYSTAL_N_QUIET_JP);
	}
	menuinfo.dwTypeData = str;
	menuinfo.wID= MY_ENABLE_CRYSTAL_N_QUIET;
	if( m_LhType > 0 && m_SysInfoStatus == 0 ){
		InsertMenuItem(m_hMenu,-1,TRUE,&menuinfo);
	}

	if( IsWindowVisible() ){
		LoadStringEx(str,
			IDS_POP_HIDE_MAIN_DIALOG_EN,
			IDS_POP_HIDE_MAIN_DIALOG_JP);
	}else{
		LoadStringEx(str,
			IDS_POP_SHOW_MAIN_DIALOG_EN,
			IDS_POP_SHOW_MAIN_DIALOG_JP);
	}
	menuinfo.hSubMenu = NULL;
	menuinfo.fState=MFS_UNCHECKED;
	menuinfo.dwTypeData = str;
	menuinfo.wID= MY_MAINDIALOG;
	InsertMenuItem(m_hMenu,-1,TRUE,&menuinfo);

	LoadStringEx(str,
		IDS_POP_EXIT_EN,
		IDS_POP_EXIT_JP);
	menuinfo.hSubMenu = NULL;
	menuinfo.fState=MFS_UNCHECKED;
	menuinfo.dwTypeData = str;
	menuinfo.wID= MY_EXIT;
	InsertMenuItem(m_hMenu,-1,TRUE,&menuinfo);
/*
	wsprintf(str,"Close Menu");
	menuinfo.hSubMenu = NULL;
	menuinfo.fState=MFS_UNCHECKED;
	menuinfo.dwTypeData = str;
	menuinfo.wID= MY_MENUCLOSE;
	InsertMenuItem(m_hMenu,-1,TRUE,&menuinfo);
*/
}

BOOL CCrystalCPUIDDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int FID, VID;

	switch (wParam){
	case MY_CRYSTAL_N_QUIET:
		OnCrystalNQuiet();
		break;
	case MY_ENABLE_CRYSTAL_N_QUIET:
		OnEnableCrystalNQuiet();
		break;
	case MY_MAINDIALOG:
		ShowWindow( ! IsWindowVisible() );
		break;
	case MY_EXIT:
		OnExit();
		break;
	case MY_CRYSTAL_N_QUIET_MAX:
		if( m_FlagCrystalNQuiet ){
			m_FlagStopCrystalNQuiet = TRUE;
			OnEnableCrystalNQuiet();
			Sleep(100);
		}
		SetupFVID(this,STATUS_MAX,&FID,&VID);
		LoveHammer(this,FID,VID);
		break;
	case MY_CRYSTAL_N_QUIET_MID:
		if( m_FlagCrystalNQuiet ){
			m_FlagStopCrystalNQuiet = TRUE;
			OnEnableCrystalNQuiet();
			Sleep(100);
		}
		SetupFVID(this,STATUS_MID,&FID,&VID);
		LoveHammer(this,FID,VID);
		break;
	case MY_CRYSTAL_N_QUIET_MIN:
		if( m_FlagCrystalNQuiet ){
			m_FlagStopCrystalNQuiet = TRUE;
			OnEnableCrystalNQuiet();
			Sleep(100);
		}
		SetupFVID(this,STATUS_MIN,&FID,&VID);
		LoveHammer(this,FID,VID);
		break;
	case MY_CRYSTAL_N_QUIET_STARTUP:
		if( m_FlagCrystalNQuiet ){
			m_FlagStopCrystalNQuiet = TRUE;
			OnEnableCrystalNQuiet();
			Sleep(100);
		}
		SetupFVID(this,STATUS_STARTUP,&FID,&VID);
		LoveHammer(this,FID,VID);
		break;
	default:
		break;
	}
	
	return CDialog::OnCommand(wParam, lParam);
}

void CCrystalCPUIDDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
  if((lpwndpos->flags & SWP_SHOWWINDOW) && m_FlagHideWindowOnStart )
  {
		lpwndpos->flags &= ~SWP_SHOWWINDOW;
		m_FlagHideWindowOnStart = FALSE;
  }
  CDialog::OnWindowPosChanging(lpwndpos);
}


void CreateIndirectDialog(CDialog *dlg, int id)
{
	char str[256];
	CDialogTemplateEx tml;
	tml.Load(MAKEINTRESOURCE(id));
	GetPrivateProfileString("Setting", "OtherFontFace", gDefaultFontFace, str,256,AfxGetApp()->m_pszProfileName);
	CString face = str;
	USHORT size = GetPrivateProfileInt("Setting", "OtherFontSize", gDefaultFontSize, AfxGetApp()->m_pszProfileName);
	if( size <= 0 ){size = 9;}
	tml.SetFont(face,size);
	dlg->CreateIndirect((HGLOBAL)tml.Detach());
}

LONG CCrystalCPUIDDlg::OnPowerBroadcast(UINT wParam, LONG lParam)
{
	switch( wParam )
	{
	case PBT_APMRESUMESUSPEND:
		if( m_FlagHaltCnQ ){
			SysInfo->SetData(LH_RESET_FVID_FLAG, 0);
			OnEnableCrystalNQuiet(); // Enable Crystal'n'Quiet
			m_FlagHaltCnQ = FALSE;
		}
//		MessageBox("PBT_APMRESUMESUSPEND");
		break;
	case PBT_APMSUSPEND:
		if( m_FlagCrystalNQuiet ){
			OnEnableCrystalNQuiet(); // Disable Crystal'n'Quiet
			m_FlagHaltCnQ = TRUE;
		}
//		MessageBox("PBT_APMSUSPEND");
		break;
	default:
		break;
	}
	return TRUE;
}

