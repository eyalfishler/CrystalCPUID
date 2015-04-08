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
#include "SpeedStepDlg.h"
#include "CrystalCPUIDDlg.h"
#include "../SysInfo/MultiplierTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern int gLanguage;

/////////////////////////////////////////////////////////////////////////////
// CSpeedStepDlg Dialog


CSpeedStepDlg::CSpeedStepDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpeedStepDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CSpeedStepDlg)
	m_CurrentFID = _T("");
	m_CurrentVID = _T("");
	m_MaxFID = _T("");
	m_MaxVID = _T("");
//	m_StartupFID = _T("");
//	m_StartupVID = _T("");
	m_EnableVoltage = FALSE;
	//}}AFX_DATA_INIT
}


void CSpeedStepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpeedStepDlg)
	DDX_Control(pDX, IDC_SS_ENABLE_VOLTAGE, m_EnableVoltageC);
	DDX_Control(pDX, IDC_SS_CONFIRMED_DIALOG, m_SsConfirmedDialog);
	DDX_Control(pDX, IDC_SS_HELP, m_Help);
	DDX_Control(pDX, IDC_SS_SET_VID, m_SetVID);
	DDX_Control(pDX, IDC_SS_SET_FID, m_SetFID);
	DDX_Text(pDX, IDC_SS_CURRENT_FID, m_CurrentFID);
	DDX_Text(pDX, IDC_SS_CURRENT_VID, m_CurrentVID);
	DDX_Text(pDX, IDC_SS_MAX_FID, m_MaxFID);
	DDX_Text(pDX, IDC_SS_MIN_VID, m_MinVID);
	DDX_Text(pDX, IDC_SS_MAX_VID, m_MaxVID);
//	DDX_Text(pDX, IDC_SS_STARTUP_FID, m_StartupFID);
//	DDX_Text(pDX, IDC_SS_STARTUP_VID, m_StartupVID);
	DDX_Check(pDX, IDC_SS_ENABLE_VOLTAGE, m_EnableVoltage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSpeedStepDlg, CDialog)
	//{{AFX_MSG_MAP(CSpeedStepDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_SS_SET_FID, OnSelchangeSsSetFid)
	ON_BN_CLICKED(IDC_SS_CREATE_LINK, OnSsCreateLink)
	ON_CBN_SELCHANGE(IDC_SS_SET_VID, OnSelchangeSsSetVid)
	ON_BN_CLICKED(IDC_SS_ENABLE_VOLTAGE, OnSsEnableVoltage)
	ON_BN_CLICKED(IDC_SS_HELP, OnSsHelp)
	ON_BN_CLICKED(IDC_SS_CONFIRMED_DIALOG, OnSsConfirmedDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpeedStepDlg 
void CSpeedStepDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CSpeedStepDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CSpeedStepDlg::Refresh()
{
	DWORD data;
	char str[256];

	// Init Current Status
	P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
	m_CurrentFID = P->SysInfo->GetInfo(LH_GET_CURRENT_MULTIPLIER,str);	m_CurrentFID += "x";
	m_MaxFID     = P->SysInfo->GetInfo(LH_GET_MAX_MULTIPLIER,str);		m_MaxFID += "x";
//	m_StartupFID = P->SysInfo->GetInfo(LH_GET_STARTUP_MULTIPLIER,str);	m_StartupFID += "x";
	m_CurrentVID = P->SysInfo->GetInfo(LH_GET_CURRENT_VOLTAGE,str);		m_CurrentVID += "V";
//	m_StartupVID = P->SysInfo->GetInfo(LH_GET_STARTUP_VOLTAGE,str);		m_StartupVID += "V";
	m_MinVID     = P->SysInfo->GetInfo(LH_GET_MIN_VOLTAGE,str);
	if (!m_MinVID.IsEmpty()) {
		m_MinVID += "V";
	}
	m_MaxVID     = P->SysInfo->GetInfo(LH_GET_MAX_VOLTAGE,str);			m_MaxVID += "V";

	UpdateData(FALSE);
	UpdateWindow();
}

BOOL CSpeedStepDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	char str[256];
	double d;
	int i;
	DWORD id = 0;
//	DWORD data;

	Refresh();
	// Init ComboBox
	if(P->m_LhType == SPEED_STEP_PM){
		for(d = 6.0; d <= 32.0; d += 1.0){
			if(d == 7.0 || d > atof(m_MaxFID)){
				cstr.Format("%.1fx (X)",d);
			}else{
				cstr.Format("%.1fx",d);
			}
			m_SetFID.AddString(cstr);
		}
		for(d = 0.700; d <= 1.710; d += 0.016){
			if(d > atof(m_MaxVID) + 0.008){
				cstr.Format("%.3fV (X)",d);
			}else{
				cstr.Format("%.3fV",d);
			}
			m_SetVID.AddString(cstr);
		}
	}else if(P->m_LhType == SPEED_STEP_P4){
		for(d = 6.0; d <= 32.0; d += 1.0){
			if(d < 14.0 || d > atof(m_MaxFID)){
				cstr.Format("%.1fx (X)",d);
			}else{
				cstr.Format("%.1fx",d);
			}
			m_SetFID.AddString(cstr);
		}
		for(i = 0;i < 64;i++){
			cstr.Format("%.4fV",VoltageTablePentium4[i] / 10000.0);
			m_SetVID.AddString(cstr);
		}
	}else if(P->m_LhType == SPEED_STEP_CORE_MA || P->m_LhType == SPEED_STEP_PENRYN){
		for(i = 0; i < 20; i++){
			d = 6.0 + i * (P->m_LhType >= SPEED_STEP_PENRYN ? 0.5 : 1.0);
			cstr.Format("%.1fx",d);
			if (d > atof(m_MaxFID)) {
				cstr += " (X)";
			}
			m_SetFID.AddString(cstr);
		}
		for(i = 0;i < 64;i++){
			d = PentiumD_Voltage_Base + (double)i * PentiumD_Voltage_Step;
			if(d < (atof(m_MinVID) - PentiumD_Voltage_Step / 2) || d > (atof(m_MaxVID) + PentiumD_Voltage_Step / 2)){
				cstr.Format("%.3fV (X)",d);
			}else{
				cstr.Format("%.3fV",d);
			}
			m_SetVID.AddString(cstr);
		}
	}

	GetPrivateProfileString("Setting","ShowConfirmedDialog","0",str,256,P->m_ini);
	if( atoi(str) >= 1 ){
		m_SsConfirmedDialog.SetCheck(1);
	}

	// Init Current Status
	// P->SysInfo->GetData(SS_GET_CURRENT_STATUS, &data);
	P->SysInfo->GetData(LH_GET_CURRENT_FID, &id);
	m_SetFID.SetCurSel(id);
	P->SysInfo->GetData(LH_GET_CURRENT_VID, &id);
	m_SetVID.SetCurSel(id);

	m_SetVID.EnableWindow(FALSE);

	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE;
}

void CSpeedStepDlg::OnSelchangeSsSetFid() 
{
	if( P->m_FlagCrystalNQuiet ){
		MessageBoxEJ(gLanguage,
			IDS_STOP_CRYSTAL_N_QUIET_EN,
			IDS_STOP_CRYSTAL_N_QUIET_JP);
		return ;
	}

	CString cstr;
	m_SetFID.GetLBText(m_SetFID.GetCurSel() , cstr);
	if( (! m_SsConfirmedDialog.GetCheck() && cstr.Find("(", 0) == -1)
	||    MessageBoxEJ(gLanguage,
			IDS_SET_NEW_FID_EN,
			IDS_SET_NEW_FID_JP,
			MB_YESNO) == IDYES 
	){
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->EndTimer();
		}
		P->SysInfo->SetData(LH_SET_FID, m_SetFID.GetCurSel());
		Refresh();
		P->Refresh(0);
		if(P->m_RealTimeClockDlg != NULL){
			P->m_RealTimeClockDlg->StartTimer();
		}
	}
}

void CSpeedStepDlg::OnSelchangeSsSetVid() 
{
	if( P->m_FlagCrystalNQuiet ){
		MessageBoxEJ(gLanguage,
			IDS_STOP_CRYSTAL_N_QUIET_EN,
			IDS_STOP_CRYSTAL_N_QUIET_JP);
		return ;
	}

	CString cstr;
	m_SetVID.GetLBText(m_SetVID.GetCurSel() , cstr);
	if( (! m_SsConfirmedDialog.GetCheck() && cstr.Find("(", 0) == -1)
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

void CSpeedStepDlg::OnSsCreateLink() 
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
	fid = m_SetFID.GetCurSel();

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

void CSpeedStepDlg::OnSsEnableVoltage() 
{
	UpdateData( TRUE );
	m_SetVID.EnableWindow( m_EnableVoltage );
}

void CSpeedStepDlg::OnSsHelp() 
{
	CString cstr, message;

	if( gLanguage == JAPANESE ){
		cstr.LoadString(IDS_LH_COMMAND_LINE_JP);
	}else{
		cstr.LoadString(IDS_LH_COMMAND_LINE_EN);
	}
	message += cstr;

	MessageBox(message);
}

void CSpeedStepDlg::OnSsConfirmedDialog() 
{
	CString cstr;
	cstr.Format("%d", m_SsConfirmedDialog.GetCheck() );
	WritePrivateProfileString("Setting","ShowConfirmedDialog",cstr,P->m_ini);	
}

BOOL CSpeedStepDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CSpeedStepDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_SPEED_STEP,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_SpeedStepDlg = NULL;
}
