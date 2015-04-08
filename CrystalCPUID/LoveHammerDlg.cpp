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
#include "LoveHammerDlg.h"
#include "../SysInfo/MultiplierTable.h"
#include <float.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern int gLanguage;

/////////////////////////////////////////////////////////////////////////////
// CLoveHammerDlg Dialog

CLoveHammerDlg::CLoveHammerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLoveHammerDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CLoveHammerDlg)
	m_CurrentFID = _T("");
	m_CurrentVID = _T("");
	m_MaxFID = _T("");
	m_MaxVID = _T("");
	m_StartupFID = _T("");
	m_StartupVID = _T("");
	m_EnableVoltage = FALSE;
	//}}AFX_DATA_INIT
}

void CLoveHammerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoveHammerDlg)
	DDX_Control(pDX, IDC_LH_ENABLE_VOLTAGE, m_EnableVoltageC);
	DDX_Control(pDX, IDC_LH_CONFIRMED_DIALOG, m_LhConfirmedDialog);
	DDX_Control(pDX, IDC_LH_HELP, m_Help);
	DDX_Control(pDX, IDC_LH_SET_VID, m_SetVID);
	DDX_Control(pDX, IDC_LH_SET_FID, m_SetFID);
	DDX_Text(pDX, IDC_LH_CURRENT_FID, m_CurrentFID);
	DDX_Text(pDX, IDC_LH_CURRENT_VID, m_CurrentVID);
	DDX_Text(pDX, IDC_LH_MAX_FID, m_MaxFID);
	DDX_Text(pDX, IDC_LH_MAX_VID, m_MaxVID);
	DDX_Text(pDX, IDC_LH_STARTUP_FID, m_StartupFID);
	DDX_Text(pDX, IDC_LH_STARTUP_VID, m_StartupVID);
	DDX_Check(pDX, IDC_LH_ENABLE_VOLTAGE, m_EnableVoltage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLoveHammerDlg, CDialog)
	//{{AFX_MSG_MAP(CLoveHammerDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_LH_SET_FID, OnSelchangeLhSetFid)
	ON_BN_CLICKED(IDC_LH_CREATE_LINK, OnLhCreateLink)
	ON_CBN_SELCHANGE(IDC_LH_SET_VID, OnSelchangeLhSetVid)
	ON_BN_CLICKED(IDC_LH_ENABLE_VOLTAGE, OnLhEnableVoltage)
	ON_BN_CLICKED(IDC_LH_HELP, OnLhHelp)
	ON_BN_CLICKED(IDC_LH_CONFIRMED_DIALOG, OnLhConfirmedDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CLoveHammerDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CLoveHammerDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CLoveHammerDlg::Refresh()
{
	DWORD data;
	char str[256];

	// Init Current Status
	P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
	m_CurrentFID = P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str);		m_CurrentFID += "x";
	if(P->m_LhType >= 6){
		m_MaxFID     = P->SysInfo->GetInfo(LH_GET_MAX_MULTIPLIER,str);		m_MaxFID += "x";
	}
	if(P->m_LhType >= 7){
		m_StartupFID = P->SysInfo->GetInfo(LH_GET_STARTUP_MULTIPLIER,str);	m_StartupFID += "x";
		m_CurrentVID = P->SysInfo->GetInfo(LH_GET_CURRENT_VOLTAGE,str);		m_CurrentVID += "V";
		m_StartupVID = P->SysInfo->GetInfo(LH_GET_STARTUP_VOLTAGE,str);		m_StartupVID += "V";
		m_MaxVID     = P->SysInfo->GetInfo(LH_GET_MAX_VOLTAGE,str);			m_MaxVID += "V";
	}
	if(P->m_LhType == 8){
		P->SysInfo->GetData(CPU_FLAG_K8_100MHZ_STEPS, &data);
		m_FlagK8100MHzSteps = data;
		P->SysInfo->GetData(CPU_FLAG_K8_UNDER_1100V, &data);
		m_FlagK8Under1100V = data;
		P->SysInfo->GetInfo(CPU_K8_HYPER_TRANSPORT,str);
		m_K8HyperTransport = atof(str);
	}

	UpdateData(FALSE);
	UpdateWindow();
}

BOOL CLoveHammerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	char str[256];
	double d;
	int i;
	DWORD id = 0;

	Refresh();
	// Init ComboBox
	if( P->m_LhType == 8 ){
		for(d = 4.0, i = 0; d <= 25.0; d += 0.5, i++){
			if(i % 2 && ! m_FlagK8100MHzSteps){
				cstr.Format("%.1fx (X)",d);
			}else if(d < 5.0){// m_K8HyperTransport >= 5.0){/* Hyper Transport < 5.0x */
				cstr.Format("%.1fx (?)",d);
			}else if(d > atof(m_MaxFID)){
				cstr.Format("%.1fx (X)",d);
			}else{
				cstr.Format("%.1fx",d);
			}
			m_SetFID.AddString(cstr);
		}
		if( P->m_LhK8LowVoltage ){
			for(d = 1.450; d >= 0.700; d -= 0.025){
				if(d > atof(m_MaxVID) + 0.01){
					cstr.Format("%.3fV (X)",d);
				}else{
					cstr.Format("%.3fV",d);
				}
				m_SetVID.AddString(cstr);
			}
		}else{
			for(d = 1.550; d >= 0.800; d -= 0.025){
				if(d > atof(m_MaxVID) + 0.01){
					cstr.Format("%.3fV (X)",d);
				}else if(d < 1.10 - 0.01 && ! m_FlagK8Under1100V){
					cstr.Format("%.3fV (?)",d);
				}else{
					cstr.Format("%.3fV",d);
				}
				m_SetVID.AddString(cstr);
			}
		}
	}else if( P->m_LhType == 7 ){
		for( i = 0; i < 32; i++ ){
			cstr.Format("%.1fx", MultiTableMobileAthlon[MultiTableMobileAthlonMap[i]] / 2.0);
			m_SetFID.AddString(cstr);
		}
		if( P->m_LhK7Desktop ){
			for( i = 0; i < 31; i++ ){
				cstr.Format("%.3fV", VoltageTableDesktopAthlon[i] / 1000.0 );
				m_SetVID.AddString(cstr);
			}
		}else{
			for( i = 0; i < 31; i++ ){
				cstr.Format("%.3fV", VoltageTableMobileAthlon[i] / 1000.0 );
				m_SetVID.AddString(cstr);
			}
		}
	}else if( P->m_LhType == 6 ){
		for( i = 0; i < 8; i++ ){
			cstr.Format("%.1fx", MultiTableK6M13[MultiTableK6M13Map[i]] / 2.0);
			m_SetFID.AddString(cstr);
		}
	}else if( P->m_LhType == 5 ){
		for( i = 0; i < 32; i++ ){
			cstr.Format("%d.0", i + 1);
			m_SetFID.AddString(cstr);
		}
	}

	GetPrivateProfileString("Setting","ShowConfirmedDialog","0",str,256,P->m_ini);
	if( atoi(str) >= 1 ){
		m_LhConfirmedDialog.SetCheck(1);
	}
	
	// Init Current Status
	// P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
	P->SysInfo->GetData(LH_GET_CURRENT_FID, &id);
	if(P->m_LhType == 6){
		m_SetFID.SetCurSel(MultiTableK6M13IdMap[id]);
	}else if(P->m_LhType == 7){
		m_SetFID.SetCurSel(MultiTableMobileAthlonIdMap[id]);
	}else{
		m_SetFID.SetCurSel(id);
	}
	P->SysInfo->GetData(LH_GET_CURRENT_VID, &id);
	m_SetVID.SetCurSel(id);

	m_SetVID.EnableWindow(FALSE);
	
	if(P->m_LhType == 6 || P->m_LhType == 5){
		m_EnableVoltageC.EnableWindow(FALSE);
	}

	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

void CLoveHammerDlg::OnSelchangeLhSetFid() 
{
	if( P->m_FlagCrystalNQuiet ){
		MessageBoxEJ(gLanguage,
			IDS_STOP_CRYSTAL_N_QUIET_EN,
			IDS_STOP_CRYSTAL_N_QUIET_JP);
		return ;
	}

	CString cstr;
	m_SetFID.GetLBText(m_SetFID.GetCurSel() , cstr);
	if( (! m_LhConfirmedDialog.GetCheck() && cstr.Find("(", 0) == -1)
	||    MessageBoxEJ(gLanguage,
			IDS_SET_NEW_FID_EN,
			IDS_SET_NEW_FID_JP,
			MB_YESNO) == IDYES 
	){
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->EndTimer();
		}
		if(P->m_LhType == 6){
			P->SysInfo->SetData(LH_SET_FID, MultiTableK6M13Map[m_SetFID.GetCurSel()]);
		}else if(P->m_LhType == 7){
			P->SysInfo->SetData(LH_SET_FID, MultiTableMobileAthlonMap[m_SetFID.GetCurSel()]);
		}else{
			P->SysInfo->SetData(LH_SET_FID, m_SetFID.GetCurSel());
		}
		Refresh();
		P->Refresh(0);
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->StartTimer();
		}
	}
}

void CLoveHammerDlg::OnSelchangeLhSetVid() 
{
	if( P->m_FlagCrystalNQuiet ){
		MessageBoxEJ(gLanguage,
			IDS_STOP_CRYSTAL_N_QUIET_EN,
			IDS_STOP_CRYSTAL_N_QUIET_JP);
		return ;
	}

	CString cstr;
	m_SetVID.GetLBText(m_SetVID.GetCurSel() , cstr);
	if( (! m_LhConfirmedDialog.GetCheck() && cstr.Find("(", 0) == -1)
	||  MessageBoxEJ(gLanguage,
			IDS_SET_NEW_VID_EN,
			IDS_SET_NEW_VID_JP,
			MB_YESNO) == IDYES 
	){
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->EndTimer();
		}
		P->SysInfo->SetData(LH_SET_VID,m_SetVID.GetCurSel());
		Refresh();
		P->Refresh(0);
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->StartTimer();
		}
	}
}

void CLoveHammerDlg::OnLhCreateLink() 
{
	UpdateData( TRUE );
	CString cstr,cstr1,cstr2;
	CString option;
	CString strobj;
	CString message;
	TCHAR buf[MAX_PATH];

	m_SetFID.GetLBText(m_SetFID.GetCurSel(),cstr1);
	m_SetVID.GetLBText(m_SetVID.GetCurSel(),cstr2);

	// Get Desktop Path
    LPMALLOC pMalloc;
    if(SUCCEEDED(SHGetMalloc(&pMalloc)))
    {
		LPITEMIDLIST pItemIDList;
		SHGetSpecialFolderLocation(
            m_hWnd,
            CSIDL_DESKTOP,
            &pItemIDList);

        SHGetPathFromIDList(pItemIDList,buf);

        pMalloc -> Free(pItemIDList);
        pMalloc -> Release();
    }

	int fid;
	if( P->m_LhType == 6 ){
		fid = MultiTableK6M13Map[m_SetFID.GetCurSel()];
	}else if( P->m_LhType == 7 ){
		fid = MultiTableMobileAthlonMap[m_SetFID.GetCurSel()];
	}else{
		fid = m_SetFID.GetCurSel();
	}

	if( m_EnableVoltage ){
		option.Format("/F%d /V%d /P%d /E", fid, m_SetVID.GetCurSel(), P->m_Mask + 1);
		strobj.Format("%s\\%s %s.lnk", buf, cstr1, cstr2);
	}else{
		option.Format("/F%d /P%d /E", fid, P->m_Mask + 1);
		strobj.Format("%s\\%s.lnk", buf, cstr1);
	}

	if( gLanguage == JAPANESE ){
		message.LoadString(IDS_CREATE_SHORTCUT_JP);
	}else{
		message.LoadString(IDS_CREATE_SHORTCUT_EN);
	}
	message += "\r\n" + option;

	IShellLink *pisl;  
	WORD str[MAX_PATH];
	HRESULT hr = ::CoInitialize(NULL);
	if (SUCCEEDED(hr))
	{
		hr = ::CoCreateInstance(CLSID_ShellLink,
			NULL, CLSCTX_INPROC_SERVER,
			IID_IShellLink, (void **)&pisl); 
		if (SUCCEEDED(hr)) 
		{
			char s[MAX_PATH];
			::GetModuleFileName(NULL, s, MAX_PATH);
			CString strObjectPath = s;

			hr = pisl->SetPath((const char *)strObjectPath);
			if (SUCCEEDED(hr)) 
			{
				IPersistFile *pipf; 
				hr = pisl->QueryInterface(IID_IPersistFile, 
					(void **)&pipf);
				if (SUCCEEDED(hr)) 
				{
					pisl->SetShowCmd(SW_SHOWMINIMIZED);
					pisl->SetArguments(option);

					MultiByteToWideChar(CP_ACP, 0,
						(const char *)strobj,
						-1, (LPWSTR)str, MAX_PATH);  
					hr = pipf->Save((LPCOLESTR)str, TRUE);
					if (FAILED(hr)){
						MessageBox("Create Shortcut Failure!!");
					}else{
						MessageBox(message);
					}
					pipf->Release();
				}
			}
			pisl->Release();
		}
		::CoUninitialize();
	}
}

void CLoveHammerDlg::OnLhEnableVoltage() 
{
	UpdateData( TRUE );
	m_SetVID.EnableWindow( m_EnableVoltage );
}

void CLoveHammerDlg::OnLhHelp() 
{
	CString cstr, message;
	if( m_FlagLH == 8 ){
		cstr.LoadString(IDS_LH_K8_VOLTAGE);
		message += cstr;
	}
	if( gLanguage == JAPANESE ){
		cstr.LoadString(IDS_LH_COMMAND_LINE_JP);
	}else{
		cstr.LoadString(IDS_LH_COMMAND_LINE_EN);
	}
	message += cstr;

	MessageBox(message);
}

void CLoveHammerDlg::OnLhConfirmedDialog() 
{
	CString cstr;
	cstr.Format("%d", m_LhConfirmedDialog.GetCheck() );
	WritePrivateProfileString("Setting","ShowConfirmedDialog",cstr,P->m_ini);	
}

BOOL CLoveHammerDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CLoveHammerDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_LOVE_HAMMER,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_LoveHammerDlg = NULL;
}
