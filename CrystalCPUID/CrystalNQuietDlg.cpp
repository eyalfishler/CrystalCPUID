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
#include "CrystalNQuietDlg.h"
#include "../SysInfo/MultiplierTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CCrystalNQuiet Dialog

CCrystalNQuietDlg::CCrystalNQuietDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCrystalNQuietDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;

	//{{AFX_DATA_INIT(CCrystalNQuietDlg)
	m_EnableVoltage = FALSE;
	//}}AFX_DATA_INIT
}

void CCrystalNQuietDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCrystalNQuietDlg)
	DDX_Control(pDX, IDC_CQ_TRIGER_TYPE, m_TrigerType);
	DDX_Control(pDX, IDC_CQ_DUAL_WAIT_TIME, m_DualWaitTime);
	DDX_Control(pDX, IDC_CQ_WAIT_TIME, m_WaitTime);
	DDX_Control(pDX, IDC_CQ_EXIT_MODE, m_ExitMode);
	DDX_Control(pDX, IDC_CQ_MAX_THRESHOLD_DOWN, m_MaxThresholdDown);
	DDX_Control(pDX, IDC_CQ_MID_THRESHOLD_UP, m_MidThresholdUp);
	DDX_Control(pDX, IDC_CQ_MID_THRESHOLD_DOWN, m_MidThresholdDown);
	DDX_Control(pDX, IDC_CQ_MIN_THRESHOLD_UP, m_MinThresholdUp);
	DDX_Control(pDX, IDC_CQ_MAX_INTERVAL_TIME, m_MaxIntervalTime);
	DDX_Control(pDX, IDC_CQ_MID_INTERVAL_TIME, m_MidIntervalTime);
	DDX_Control(pDX, IDC_CQ_MIN_INTERVAL_TIME, m_MinIntervalTime);
	DDX_Control(pDX, IDC_CQ_ENABLE_VOLTAGE, m_EnableVoltageC);
	DDX_Control(pDX, IDC_CQ_MIN_VID, m_MinVID);
	DDX_Control(pDX, IDC_CQ_MIN_FID, m_MinFID);
	DDX_Control(pDX, IDC_CQ_MID_VID, m_MidVID);
	DDX_Control(pDX, IDC_CQ_MID_FID, m_MidFID);
	DDX_Control(pDX, IDC_CQ_MAX_VID, m_MaxVID);
	DDX_Control(pDX, IDC_CQ_MAX_FID, m_MaxFID);
	DDX_Check(pDX, IDC_CQ_ENABLE_VOLTAGE, m_EnableVoltage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCrystalNQuietDlg, CDialog)
	//{{AFX_MSG_MAP(CCrystalNQuietDlg)
	ON_BN_CLICKED(IDC_CQ_ENABLE_VOLTAGE, OnCqEnableVoltage)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CQ_APPLY, OnCqApply)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCrystalNQuiet Message Handler

void CCrystalNQuietDlg::OnCqEnableVoltage() 
{
	UpdateData( TRUE );
	m_MaxVID.EnableWindow( m_EnableVoltage );
	m_MinVID.EnableWindow( m_EnableVoltage );
	m_MidVID.EnableWindow( m_EnableVoltage );
	m_WaitTime.EnableWindow( m_EnableVoltage );
}

BOOL CCrystalNQuietDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CCrystalNQuietDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_CRYSTAL_N_QUIET,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_CrystalNQuietDlg = NULL;	
}

BOOL CCrystalNQuietDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	char str[256];
	double d;
	int i;
	DWORD id = 0;
	DWORD data;

	// Init ComboBox
	switch( P->m_LhType ){
	case LONG_HAUL_LEVEL_1:
	case LONG_HAUL_LEVEL_2:
		switch( P->m_LhTableType )
		{
		case SAMUEL:
			for(i = 0; i < 16 ; i++ ){
				cstr.Format("%.1fx", LongHaulSamuel[i] / 2.0);
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			break;
		case SAMUEL2:
			for(i = 0; i < 16 ; i++ ){
				cstr.Format("%.1fx", LongHaulSamuel2[i] / 2.0);
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			break;
		case EZRA:
			for(i = 0; i < 16 ; i++ ){
				cstr.Format("%.1fx", LongHaulEzra[i] / 2.0);
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			break;
		case EZRA_T:
			for(i = 0; i < 32 ; i++ ){
				cstr.Format("%.1fx", LongHaulEzraT[i] / 2.0);
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			break;
		case NEHEMIAH:
			for(i = 0; i < 32 ; i++ ){
				cstr.Format("%.1fx", LongHaulNehemiah[i] / 2.0);
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			break;
		default:
			break;
		}
		break;
	case LOVE_HAMMER_GEODE_LX:
		for( i = 0; i < 32; i++ ){
			cstr.Format("%d.0", i + 1);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		break;
	case LOVE_HAMMER_K6:
		for( i = 0; i < 8; i++ ){
			cstr.Format("%.1fx", MultiTableK6M13[MultiTableK6M13Map[i]] / 2.0);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		break;
	case LOVE_HAMMER_K7:
		int i;
		for( i = 0; i < 32; i++ ){
			cstr.Format("%.1fx", MultiTableMobileAthlon[MultiTableMobileAthlonMap[i]] / 2.0);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		if( P->m_LhK7Desktop ){
			for( i = 0; i < 31; i++ ){
				cstr.Format("%.3fV", VoltageTableDesktopAthlon[i] / 1000.0 );
				m_MaxVID.AddString(cstr);
				m_MinVID.AddString(cstr);
				m_MidVID.AddString(cstr);
			}
		}else{
			for( i = 0; i < 31; i++ ){
				cstr.Format("%.3fV", VoltageTableMobileAthlon[i] / 1000.0 );
				m_MaxVID.AddString(cstr);
				m_MinVID.AddString(cstr);
				m_MidVID.AddString(cstr);
			}
		}
		break;
	case LOVE_HAMMER_K8:
		for(d = 4.0; d <= 25.0; d += 0.5){
			cstr.Format("%.1fx",d);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		for(d = 1.550; d >= 0.800; d -= 0.025){
			cstr.Format("%.3fV",d);
			m_MaxVID.AddString(cstr);
			m_MinVID.AddString(cstr);
			m_MidVID.AddString(cstr);
		}
		break;
	case SPEED_STEP_PM:
		for(d = 6.0; d <= 32.0; d += 1.0){
			cstr.Format("%.1fx",d);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		for(d = 0.700; d <= 1.710; d += 0.016){
			cstr.Format("%.3fV",d);
			m_MaxVID.AddString(cstr);
			m_MinVID.AddString(cstr);
			m_MidVID.AddString(cstr);
		}
		break;
	case SPEED_STEP_P4:
		for(d = 6.0; d <= 32.0; d += 1.0){
			cstr.Format("%.1fx",d);
			m_MaxFID.AddString(cstr);
			m_MinFID.AddString(cstr);
			m_MidFID.AddString(cstr);
		}
		for(i = 0;i < 64;i++){
			cstr.Format("%.4fV", VoltageTablePentium4[i] / 10000.0);
			m_MaxVID.AddString(cstr);
			m_MinVID.AddString(cstr);
			m_MidVID.AddString(cstr);
		}
		break;
	case SPEED_STEP_CORE_MA:
	case SPEED_STEP_PENRYN:
		{
			P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
			P->SysInfo->GetInfo(LH_GET_MAX_MULTIPLIER, str);
			double MaxMultiplier = isdigit(*str) ? atof(str) : 10000.0;
			for(i = 0; i < 20; i++){
				d = 6.0 + i * (P->m_LhType >= SPEED_STEP_PENRYN ? 0.5 : 1.0);
				cstr.Format("%.1fx",d);
				if (d > MaxMultiplier) {
					cstr += " (X)";
				}
				m_MaxFID.AddString(cstr);
				m_MinFID.AddString(cstr);
				m_MidFID.AddString(cstr);
			}
			P->SysInfo->GetInfo(LH_GET_MIN_VOLTAGE,str);
			double MinVid = isdigit(*str) ? atof(str) : 0;
			P->SysInfo->GetInfo(LH_GET_MAX_VOLTAGE,str);
			double MaxVid = isdigit(*str) ? atof(str) : 10.0;
			for(i = 0;i < 64;i++){
				d = PentiumD_Voltage_Base + (double)i * PentiumD_Voltage_Step;
				if(d < (MinVid - PentiumD_Voltage_Step / 2) || d > (MaxVid + PentiumD_Voltage_Step / 2)){
					cstr.Format("%.3fV (X)",d);
				}else{
					cstr.Format("%.3fV",d);
				}
				m_MaxVID.AddString(cstr);
				m_MinVID.AddString(cstr);
				m_MidVID.AddString(cstr);
			}
		}
		break;
	default:

		break;
	}

	GetPrivateProfileString("Setting","CqWaitTime","0",str,256,P->m_ini);
	int temp = atoi(str);
	if( temp < 0 || temp > 100 ){	temp = 0; }
	cstr.Format("%dms",temp);
	m_WaitTime.AddString(cstr);
	m_WaitTime.SetCurSel(0);
	m_WaitTime.AddString("0ms");
	m_WaitTime.AddString("50ms");
	m_WaitTime.AddString("100ms");

	GetPrivateProfileString("Setting","CqDualWaitTime","1000",str,256,P->m_ini);
	temp = atoi(str);
	if( temp < 0 || temp > 10000 ){	temp = 1000; }
	cstr.Format("%dms",temp);
	m_DualWaitTime.AddString(cstr);
	m_DualWaitTime.SetCurSel(0);
	for(i = 0; i<=3000; i+= 100){
		cstr.Format("%dms",i);
		m_DualWaitTime.AddString(cstr);
	}

	SYSTEM_INFO si;
	GetSystemInfo(&si);
	if( si.dwNumberOfProcessors == 1 ){
		m_DualWaitTime.EnableWindow(FALSE);
		m_TrigerType.EnableWindow(FALSE);
	}

	m_TrigerType.AddString("Max");
	m_TrigerType.AddString("Average");
	for(i = 1; i <= (int)si.dwNumberOfProcessors; i++){
		cstr.Format("CPU%d/%d", i, si.dwNumberOfProcessors);
		m_TrigerType.AddString(cstr);
	}
	GetPrivateProfileString("Setting","CqTrigerType","-2",str,256,P->m_ini);
	temp = atoi(str);
	if( temp < CPU_USAGE_MAX || temp > (int)si.dwNumberOfProcessors){
		temp = CPU_USAGE_MAX;
	}
	m_TrigerType.SetCurSel(temp + 2);

	m_ExitMode.AddString("Maximum");
	m_ExitMode.AddString("Middle");
	m_ExitMode.AddString("Minimum");
	if(LOVE_HAMMER_K7 <= P->m_LhType && P->m_LhType <= LOVE_HAMMER_K8){
		m_ExitMode.AddString("Startup");
	}
	GetPrivateProfileString("Setting","CqExitMode","0",str,256,P->m_ini);
	temp = atoi(str);
	if( temp < 0 || temp > 3 ){	temp = 0; }
	m_ExitMode.SetCurSel(temp);

	UpdateData( FALSE );

	GetPrivateProfileString("Setting","CqMidThresholdUp","40%",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) >= 100 ){
		strcpy(str,"40%");
	}
	m_MidThresholdUp.AddString(str);
	m_MidThresholdUp.SetCurSel(0);
	GetPrivateProfileString("Setting","CqMinThresholdUp","40%",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 100 ){
		strcpy(str,"40%");
	}
	m_MinThresholdUp.AddString(str);
	m_MinThresholdUp.SetCurSel(0);
	GetPrivateProfileString("Setting","CqMaxThresholdDown","20%",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 100 ){
		strcpy(str,"20%");
	}

	m_MaxThresholdDown.AddString(str);
	m_MaxThresholdDown.SetCurSel(0);
	GetPrivateProfileString("Setting","CqMidThresholdDown","20%",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 100 ){
		strcpy(str,"20%");
	}
	m_MidThresholdDown.AddString(str);
	m_MidThresholdDown.SetCurSel(0);
	
	for( i = 5; i < 100; i += 5 ){
		cstr.Format("%d%%", i);
		m_MaxThresholdDown.AddString(cstr);
		m_MidThresholdUp.AddString(cstr);
		m_MidThresholdDown.AddString(cstr);
		m_MinThresholdUp.AddString(cstr);
	}

	GetPrivateProfileString("Setting","CqMaxIntervalTime","5000ms",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 10000 ){
		strcpy(str,"5000ms");
	}
	m_MaxIntervalTime.AddString(str);
	m_MaxIntervalTime.SetCurSel(0);
	GetPrivateProfileString("Setting","CqMidIntervalTime","500ms",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 10000 ){
		strcpy(str,"500ms");
	}
	m_MidIntervalTime.AddString(str);
	m_MidIntervalTime.SetCurSel(0);
	GetPrivateProfileString("Setting","CqMinIntervalTime","500ms",str,256,P->m_ini);
	if( atoi(str) <= 0 || atoi(str) > 10000 ){
		strcpy(str,"500ms");
	}
	m_MinIntervalTime.AddString(str);
	m_MinIntervalTime.SetCurSel(0);
	for(i = 100;i<= 1000;i+=100){
		cstr.Format("%dms",i);
		m_MaxIntervalTime.AddString(cstr);
		m_MidIntervalTime.AddString(cstr);
		m_MinIntervalTime.AddString(cstr);
	}
	for(i = 2000;i<= 10000;i+=1000){
		cstr.Format("%dms",i);
		m_MaxIntervalTime.AddString(cstr);
		m_MidIntervalTime.AddString(cstr);
		m_MinIntervalTime.AddString(cstr);
	}

	// Select Item
	if(P->m_LhType == LOVE_HAMMER_GEODE_LX || P->m_LhType == LOVE_HAMMER_K6 || P->m_LhType == LOVE_HAMMER_K7 || P->m_LhType == LOVE_HAMMER_K8 || P->m_LhType == SPEED_STEP_PM || P->m_LhType == SPEED_STEP_P4 || P->m_LhType == SPEED_STEP_CORE_MA || P->m_LhType == SPEED_STEP_PENRYN){
		P->SysInfo->GetData(LH_GET_CURRENT_STATUS, &data);
		GetPrivateProfileString("Setting","CqMaxFID","",str,256,P->m_ini);
		if (sscanf(str, "%d", &id) < 1) {
			P->SysInfo->GetData(LH_GET_CURRENT_FID, &data);
			id = data;
		}
		if(P->m_LhType == LOVE_HAMMER_K6){
			m_MaxFID.SetCurSel(MultiTableK6M13IdMap[id]);
		}else if(P->m_LhType == LOVE_HAMMER_K7){
			m_MaxFID.SetCurSel(MultiTableMobileAthlonIdMap[id]);
		}else{
			m_MaxFID.SetCurSel(id);
		}
		GetPrivateProfileString("Setting","CqMinFID","",str,256,P->m_ini);
		if (sscanf(str, "%d", &id) < 1) {
			P->SysInfo->GetData(LH_GET_CURRENT_FID, &data);
			id = data;
		}
		if(P->m_LhType == LOVE_HAMMER_K6){
			m_MinFID.SetCurSel(MultiTableK6M13IdMap[id]);
		}else if(P->m_LhType == LOVE_HAMMER_K7){
			m_MinFID.SetCurSel(MultiTableMobileAthlonIdMap[id]);
		}else{
			m_MinFID.SetCurSel(id);
		}
		GetPrivateProfileString("Setting","CqMidFID","",str,256,P->m_ini);
		if (sscanf(str, "%d", &id) < 1) {
			P->SysInfo->GetData(LH_GET_CURRENT_FID, &data);
			id = data;
		}
		if(P->m_LhType == LOVE_HAMMER_K6){
			m_MidFID.SetCurSel(MultiTableK6M13IdMap[id]);
		}else if(P->m_LhType == LOVE_HAMMER_K7){
			m_MidFID.SetCurSel(MultiTableMobileAthlonIdMap[id]);
		}else{
			m_MidFID.SetCurSel(id);
		}

		GetPrivateProfileString("Setting","CqMaxVID","-1",str,256,P->m_ini);
		if( atoi(str) < 0 ){
			P->SysInfo->GetData(LH_GET_CURRENT_VID, &data);
			m_MaxVID.SetCurSel( data );
		}else{
			m_MaxVID.SetCurSel( atoi(str) );
		}
		GetPrivateProfileString("Setting","CqMinVID","-1",str,256,P->m_ini);
		if( atoi(str) < 0){
			P->SysInfo->GetData(LH_GET_CURRENT_VID, &data);
			m_MinVID.SetCurSel( data );
		}else{
			m_MinVID.SetCurSel( atoi(str) );
		}
		GetPrivateProfileString("Setting","CqMidVID","-1",str,256,P->m_ini);
		if( atoi(str) < 0 ){
			P->SysInfo->GetData(LH_GET_CURRENT_VID, &data);
			m_MidVID.SetCurSel( data );
		}else{
			m_MidVID.SetCurSel( atoi(str) );
		}

		GetPrivateProfileString("Setting","CqEnableVoltage","0",str,256,P->m_ini);
		if( atoi(str) > 0 ){
			m_MaxVID.EnableWindow( TRUE );
			m_MinVID.EnableWindow( TRUE );
			m_MidVID.EnableWindow( TRUE );
			if(P->m_LhType >= LOVE_HAMMER_K7){
				m_EnableVoltage = TRUE;
			}
			UpdateData( FALSE );
		}

		if(P->m_LhType == LOVE_HAMMER_K6 || P->m_LhType == LOVE_HAMMER_GEODE_LX){
			m_EnableVoltageC.EnableWindow(FALSE);
		}
	}else{ // C3
		GetPrivateProfileString("Setting","CqMaxFID","-1",str,256,P->m_ini);
		if( atoi(str) < 0 ){
			cstr.Format("%.1f", atof( P->m_Multiplier ) );
			m_MaxFID.SetCurSel( m_MaxFID.FindString(0, cstr) );
		}else{
			m_MaxFID.SetCurSel( atoi(str) );
		}
		GetPrivateProfileString("Setting","CqMinFID","-1",str,256,P->m_ini);
		if( atoi(str) < 0 ){
			cstr.Format("%.1f", atof( P->m_Multiplier ) );
			m_MinFID.SetCurSel( m_MinFID.FindString(0, cstr) );
		}else{
			m_MinFID.SetCurSel( atoi(str) );
		}
		GetPrivateProfileString("Setting","CqMidFID","-1",str,256,P->m_ini);
		if( atoi(str) < 0 ){
			cstr.Format("%.1f", atof( P->m_Multiplier ) );
			m_MidFID.SetCurSel( m_MidFID.FindString(0, cstr) );
		}else{
			m_MidFID.SetCurSel( atoi(str) );
		}
		m_EnableVoltageC.EnableWindow(FALSE);
	}

	CButton* CqModeUpType0 = (CButton*)GetDlgItem(IDC_CQ_MODE_UP_TYPE_0);
	CButton* CqModeUpType1 = (CButton*)GetDlgItem(IDC_CQ_MODE_UP_TYPE_1);

	GetPrivateProfileString("Setting","CqModeUpType","1",str,256,P->m_ini);
	P->m_CqModeUpType = atoi(str);

	if( P->m_CqModeUpType == 0 ){
		CqModeUpType0->SetCheck(1);
	}else{
		CqModeUpType1->SetCheck(1);
	}
	strcpy(str,"");

	CButton* CqModeDownType0 = (CButton*)GetDlgItem(IDC_CQ_MODE_DOWN_TYPE_0);
	CButton* CqModeDownType1 = (CButton*)GetDlgItem(IDC_CQ_MODE_DOWN_TYPE_1);

	GetPrivateProfileString("Setting","CqModeDownType","0",str,256,P->m_ini);
	P->m_CqModeDownType = atoi(str);

	if( P->m_CqModeDownType == 0 ){
		CqModeDownType0->SetCheck(1);
	}else{
		CqModeDownType1->SetCheck(1);
	}

	CenterWindow();
	ShowWindow(SW_SHOW);
	
	return TRUE;
}

void CCrystalNQuietDlg::OnOK() 
{
	OnCqApply();
	OnClose();
}

void CCrystalNQuietDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}

void CCrystalNQuietDlg::OnCqApply() 
{
	CString cstr;

	cstr.Format("%d", m_TrigerType.GetCurSel() - 2);
	WritePrivateProfileString("Setting","CqTrigerType",cstr,P->m_ini);
	P->m_CqTrigerType = m_TrigerType.GetCurSel() - 2;

	cstr.Format("%d", m_ExitMode.GetCurSel() );
	WritePrivateProfileString("Setting","CqExitMode",cstr,P->m_ini);
	P->m_CqExitMode = m_ExitMode.GetCurSel();

	m_WaitTime.GetLBText(m_WaitTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqWaitTime",cstr,P->m_ini);
	P->m_CqWaitTime = atoi(cstr);

	m_DualWaitTime.GetLBText(m_DualWaitTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqDualWaitTime",cstr,P->m_ini);

	// Save Data & Update FID & VID
	if(P->m_LhType == LOVE_HAMMER_K6){
		cstr.Format("%d", MultiTableK6M13Map[m_MaxFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMaxFID",cstr,P->m_ini);
		cstr.Format("%d", MultiTableK6M13Map[m_MidFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMidFID",cstr,P->m_ini);
		cstr.Format("%d", MultiTableK6M13Map[m_MinFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMinFID",cstr,P->m_ini);
		P->m_CqMaxFID = MultiTableK6M13Map[m_MaxFID.GetCurSel()];
		P->m_CqMidFID = MultiTableK6M13Map[m_MidFID.GetCurSel()];
		P->m_CqMinFID = MultiTableK6M13Map[m_MinFID.GetCurSel()];
	}else if(P->m_LhType == LOVE_HAMMER_K7){
		cstr.Format("%d", MultiTableMobileAthlonMap[m_MaxFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMaxFID",cstr,P->m_ini);
		cstr.Format("%d", MultiTableMobileAthlonMap[m_MidFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMidFID",cstr,P->m_ini);
		cstr.Format("%d", MultiTableMobileAthlonMap[m_MinFID.GetCurSel()]);
		WritePrivateProfileString("Setting","CqMinFID",cstr,P->m_ini);
		P->m_CqMaxFID = MultiTableMobileAthlonMap[m_MaxFID.GetCurSel()];
		P->m_CqMidFID = MultiTableMobileAthlonMap[m_MidFID.GetCurSel()];
		P->m_CqMinFID = MultiTableMobileAthlonMap[m_MinFID.GetCurSel()];
	}else{
		cstr.Format("%d", m_MaxFID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMaxFID",cstr,P->m_ini);
		cstr.Format("%d", m_MidFID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMidFID",cstr,P->m_ini);
		cstr.Format("%d", m_MinFID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMinFID",cstr,P->m_ini);
		P->m_CqMaxFID = m_MaxFID.GetCurSel();
		P->m_CqMidFID = m_MidFID.GetCurSel();
		P->m_CqMinFID = m_MinFID.GetCurSel();
	}

	if( P->m_LhType == LOVE_HAMMER_K6 || P->m_LhType == LOVE_HAMMER_K7 || P->m_LhType == LOVE_HAMMER_K8 || P->m_LhType == SPEED_STEP_PM || P->m_LhType == SPEED_STEP_P4 || P->m_LhType == SPEED_STEP_CORE_MA || P->m_LhType == SPEED_STEP_PENRYN){
		cstr.Format("%d", m_MaxVID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMaxVID",cstr,P->m_ini);
		cstr.Format("%d", m_MidVID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMidVID",cstr,P->m_ini);
		cstr.Format("%d", m_MinVID.GetCurSel() );
		WritePrivateProfileString("Setting","CqMinVID",cstr,P->m_ini);

		P->m_CqMaxVID = m_MaxVID.GetCurSel();
		P->m_CqMidVID = m_MidVID.GetCurSel();
		P->m_CqMinVID = m_MinVID.GetCurSel();
	}

	m_MidThresholdUp.GetLBText(m_MidThresholdUp.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMidThresholdUp",cstr,P->m_ini);
	P->m_CqMidThresholdUp = atoi(cstr);
	m_MinThresholdUp.GetLBText(m_MinThresholdUp.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMinThresholdUp",cstr,P->m_ini);
	P->m_CqMinThresholdUp = atoi(cstr);

	m_MaxThresholdDown.GetLBText(m_MaxThresholdDown.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMaxThresholdDown",cstr,P->m_ini);
	P->m_CqMaxThresholdDown = atoi(cstr);
	m_MidThresholdDown.GetLBText(m_MidThresholdDown.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMidThresholdDown",cstr,P->m_ini);
	P->m_CqMidThresholdDown = atoi(cstr);

	m_MaxIntervalTime.GetLBText(m_MaxIntervalTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMaxIntervalTime",cstr,P->m_ini);
	P->m_CqMaxIntervalTime = atoi(cstr);
	m_MidIntervalTime.GetLBText(m_MidIntervalTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMidIntervalTime",cstr,P->m_ini);
	P->m_CqMidIntervalTime = atoi(cstr);
	m_MinIntervalTime.GetLBText(m_MinIntervalTime.GetCurSel(),cstr);
	WritePrivateProfileString("Setting","CqMinIntervalTime",cstr,P->m_ini);
	P->m_CqMinIntervalTime = atoi(cstr);

	CButton* CqModeUpType1 = (CButton*)GetDlgItem(IDC_CQ_MODE_UP_TYPE_1);
	cstr.Format("%d",CqModeUpType1->GetCheck());
	WritePrivateProfileString("Setting","CqModeUpType",cstr,P->m_ini);
	P->m_CqModeUpType = atoi(cstr);
	
	CButton* CqModeDownType1 = (CButton*)GetDlgItem(IDC_CQ_MODE_DOWN_TYPE_1);
	cstr.Format("%d",CqModeDownType1->GetCheck());
	WritePrivateProfileString("Setting","CqModeDownType",cstr,P->m_ini);
	P->m_CqModeDownType = atoi(cstr);

	UpdateData( TRUE );
	if( m_EnableVoltage ){
		WritePrivateProfileString("Setting","CqEnableVoltage","1",P->m_ini);
		P->m_CqEnableVoltage = TRUE;
	}else{
		WritePrivateProfileString("Setting","CqEnableVoltage","0",P->m_ini);		
		P->m_CqEnableVoltage = FALSE;
	}

	P->m_CqStatus = STATUS_OFF;
}
