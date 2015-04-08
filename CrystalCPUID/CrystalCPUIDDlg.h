/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CRYSTALCPUIDDLG_H__0EA235B8_265E_4CB3_8765_D4DC5A075721__INCLUDED_)
#define AFX_CRYSTALCPUIDDLG_H__0EA235B8_265E_4CB3_8765_D4DC5A075721__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SysInfo/CpuInfoID.h"
#include "../SysInfo/ISysInfo.h"

#include "../common/DibSection.h"

#include "CustomizeDlg.h"
#include "LoveHammerDlg.h"
#include "LongHaulDlg.h"
#include "BiosInfoDlg.h"
#include "TransmetaInfoDlg.h"
#include "ProcessorSerialDlg.h"
#include "RealTimeClockDlg.h"
#include "CacheInfoDlg.h"
#include "AboutDlg.h"
#include "CpuidDlg.h"
#include "FeatureDlg.h"
#include "DmiInfoDlg.h"
#include "PciInfoDlg.h"
#include "ChipsetInfoDlg.h"
#include "CrystalNQuietDlg.h"
#include "MsrEditDlg.h"
#include "MsrWalkerDlg.h"
#include "SpeedStepDlg.h"

#include "../common/GetOSInfo.h"

static enum Status {
	STATUS_MAX,
	STATUS_MID,
	STATUS_MIN,
	STATUS_OFF,
	STATUS_STARTUP,
	STATUS_UPDATE
};

/////////////////////////////////////////////////////////////////////////////
// CCrystalCPUIDDlg Dialog

class CCrystalCPUIDDlg : public CDialog
{
public:
	CCrystalCPUIDDlg(CWnd* pParent = NULL);
	~CCrystalCPUIDDlg();
	char CrystalCPUIDVersion[32];
	char* m_ini;
	char* m_path;
	int  CreateWallPaper(DWORD Mode,char* FileName,char* ResourceName,COLORREF Color,BOOL flagGray=FALSE);
	void Refresh(DWORD TimerType = 0);
	void TextCopy(DWORD type);
	void ImageCopy(DWORD type);
	void EndRealTimeClock();
	void ForceHideOriginalClock();
	void Exit();

	CFont* m_pFont;
	DWORD m_Mask;

	// LoveHamer
	HANDLE m_hThread;
	int m_LhType;
	int m_LhTableType;
	int m_LhK7Desktop;
	int m_LhK8LowVoltage;

	int m_CpuUsage;

	// TaskTray
	HMENU m_hMenu;
	CString m_TrayString;
	void AddTaskTray();
	void DelTaskTray();
	void ModifyTaskTrayIcon(int status);
	void ModifyTaskTrayTips(CString cstr);
	HICON m_hTaskIcon;
	HICON m_hTaskIconMax;
	HICON m_hTaskIconMid;
	HICON m_hTaskIconMin;

	static UINT wmTaskbarCreated;
	void CreateMainMenu();

	// Crystal'n'Quiet
	int m_CqMidThresholdUp;
	int m_CqMinThresholdUp;
	int m_CqMaxThresholdDown;
	int m_CqMidThresholdDown;
	int	m_CqMaxIntervalTime;
	int	m_CqMidIntervalTime;
	int	m_CqMinIntervalTime;
	int m_CqMaxFID;
	int m_CqMinFID;
	int m_CqMidFID;
	int m_CqMaxVID;
	int m_CqMinVID;
	int m_CqMidVID;
	int m_CqModeUpType;
	int m_CqModeDownType;
	int m_CqExitMode;
	int m_CqTrigerType;

	Status m_CqStatus;
	int m_CqWaitTime;
	BOOL m_CqEnableVoltage;
	BOOL m_FlagExitNow;
	BOOL m_FlagHideWindowOnStart;
	BOOL m_FlagHideWindowOnStartReal;
	BOOL m_FlagResident;
	BOOL m_FlagStopCrystalNQuiet;
	BOOL m_FlagDisableTaskTrayIcon;
	
	CRealTimeClockDlg* m_RealTimeClockDlg;
	CDmiInfoDlg* m_DmiInfoDlg;
	CBiosInfoDlg* m_BiosInfoDlg;
	CFeatureDlg* m_FeatureDlg;
	CCpuidDlg*	m_CpuidDlg;
	CCustomizeDlg*	m_CustomizeDlg;
	CTransmetaInfoDlg*	m_TransmetaInfoDlg;
	CCacheInfoDlg*	m_CacheInfoDlg;
	CAboutDlg*	m_AboutDlg;
	CProcessorSerialDlg*	m_ProcessorSerialDlg;
	CLoveHammerDlg*	m_LoveHammerDlg;
	CLongHaulDlg*	m_LongHaulDlg;
	CChipsetInfoDlg*	m_ChipsetInfoDlg;
	CPciInfoDlg*	m_PciInfoDlg;
	CCrystalNQuietDlg*	m_CrystalNQuietDlg;
	CMsrEditDlg* m_MsrEditDlg;
	CMsrWalkerDlg* m_MsrWalkerDlg;
	CSpeedStepDlg* m_SpeedStepDlg;

	// SysInfo
	ISysInfo* SysInfo;
	BOOL m_IsSysInfoInit;
	DWORD m_SysInfoStatus;
/*
	HINSTANCE hSysInfoLib;
	BOOL ConnectSysInfo(DWORD Mode);
	BOOL DisconnectSysInfo();
*/	
	// DibSection
	CDibSection	Dib;

	// Flag
	BOOL m_FlagShowCustomize;
	BOOL m_FlagCrystalNQuiet;
	BOOL m_FlagHaltCnQ;
	BOOL m_IsNT;

	// BIOS Information by WMI
	BOOL m_IsWMI;
	CString m_BiosDate;
	CString m_BiosVersion;
	CString m_BiosManufacturer;
	CString m_BiosCaption;
	CString m_SmBiosVersion;
	CString m_SmBiosCaption;
	CString m_SystemManufacturer;
	CString m_SystemModel;
	CString m_MotherVersion;
	CString m_MotherProduct;
	CString m_MotherManufacturer;

	// DMI Information
	CString m_DmiVersion;
	CString m_DmiBiosVendor;
	CString m_DmiBiosVersion;
	CString m_DmiBiosReleaseDate;
	CString m_DmiBiosRomSize;
	CString m_DmiMotherManufacturer;
	CString m_DmiMotherProduct;
	CString m_DmiMotherVersion;
	CString m_DmiCpuSocket;
	CString m_DmiCpuManufacturer;
	CString m_DmiCpuVersion;
	CString m_DmiCpuCurrentClock;
	CString m_DmiCpuExternalClock;
	CString m_DmiCpuMaxClock;

	// Chipset Information
	CString	m_Chipset;
	CString	m_NorthBridge;
	CString	m_SouthBridge;
	CString	m_VideoChip;
	CString	m_NorthBridgeId;
	CString	m_SouthBridgeId;
	CString	m_VideoChipId;

// Dialog Data
	//{{AFX_DATA(CCrystalCPUIDDlg)
	enum { IDD = IDD_CRYSTALCPUID_DIALOG };
	CStatic	m_DualCore;
	CStatic	m_OverClockC;
	CStatic	m_ClockOriC;
	CStatic	m_SystemClockC;
	CStatic	m_MultiplierC;
	CStatic	m_NX;
	CStatic	m_CurrentDateC;
	CStatic	m_IA32e;
	CStatic	m_LongHaul;
	CStatic	m_LongRun;
	CStatic	m_PowerNow;
	CStatic	m_SpeedStep;
	CStatic	m_CpuNameC;
	CStatic	m_ClockC;
	CComboBox	m_CPUSelectC;
	CStatic	m_SSE3;
	CStatic	m_SSSE3;
	CStatic	m_SSE41;
	CStatic	m_SSE42;
	CStatic	m_SSE4A;
	CStatic	m_SSE5;
	CStatic	m_AVX;
	CStatic	m_IA64;
	CStatic	m_AA64;
	CStatic	m_SSE;
	CStatic	m_SSE2;
	CStatic	m_MMXEx;
	CStatic	m_MMX;
	CStatic	m_3DNow;
	CStatic	m_3DNowEx;
	CStatic	m_HT;
	CStatic	m_VT;
	CStatic	m_AmdV;
	CString	m_BrandID;
	CString m_SystemBusHT;
	CString	m_CacheL1ITU;
	CString	m_CacheL1ITUUnit;
	CString	m_Clock;
	CString	m_ClockOri;
	CString	m_CPUName;
	CString	m_CPUNumber;
	CString	m_DataRate;
	CString	m_ExFamily;
	CString	m_ExFamilyEx;
	CString	m_ExModel;
	CString	m_ExModelEx;
	CString	m_ExStepping;
	CString	m_Family;
	CString	m_FamilyEx;
	CString	m_L1DataCache;
	CString	m_L1ITUCache;
	CString	m_L2Cache;
	CString	m_L2CacheSpeed;
	CString	m_L2CacheSpeedMHz;
	CString	m_L3Cache;
	CString	m_Model;
	CString	m_ModelEx;
	CString	m_Multiplier;
	CString	m_MultiplierOri;
	CString	m_NameString;
	CString	m_OverClock;
	CString	m_Stepping;
	CString	m_SystemBus;
	CString	m_SystemBusOri;
	CString	m_SystemClock;
	CString	m_SystemClockOri;
	CString	m_VendorString;
	CString	m_HyperThreading;
	CString	m_Platform;
	CString	m_CPUType;
	CString	m_Apic;
	CString	m_CodeName;
	CString	m_OS;
	CString	m_ProcessRule;
	CString	m_Memory;
	CString	m_CurrentDate;
	CString	m_Signature;
	CString	m_PlatformID;
	CString	m_MicrocodeID;
	CString m_CpuTypeId;
	CString m_Architecture;
	CString	m_MeasureMode;
	CString	m_CoreNumber;
	CString	m_ProcessUnit;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCrystalCPUIDDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV Support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

protected:
	HICON m_hIcon;
	CString m_MSRInfo;
	BOOL m_FlagHideOriginalClock;
	BOOL m_FlagHideDate;
	HACCEL m_hAccelerator;

	CString m_TransmetaInfo;
	CString m_BiosInfo;
	CString m_DmiInfo;
	CString m_ChipsetInfo;
	CString m_PciDeviceInfo;
	CString cstr;

	BOOL BiosInfo();
	BOOL DmiInfo();
	BOOL ChipsetInfo();
	BOOL PciDeviceInfo();
	LONG OnPowerBroadcast(UINT wParam, LONG lParam);

	//{{AFX_MSG(CCrystalCPUIDDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeCpuSelect();
	afx_msg void OnExit();
	afx_msg void OnRefresh();
	afx_msg void OnCrystalDewWorld();
	afx_msg void OnCustomize();
	afx_msg void OnCpuInfoText();
	afx_msg void OnCrystalCpuidText();
	afx_msg void OnEnglish();
	afx_msg void OnJapanese();
	afx_msg void OnFeatureFlags();
	afx_msg void OnCpuidDetail();
	afx_msg void OnLicense();
	afx_msg void OnReadMe();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLoveHammer();
	afx_msg void OnSpeedStep();
	afx_msg void OnHideOriginalClock();
	afx_msg void OnHideDate();
	afx_msg void OnSaveImage();
	afx_msg void OnPreviewText();
	afx_msg void OnCopyText();
	afx_msg void OnSaveText();
	afx_msg void OnQuickSaveImage();
	afx_msg void OnBiosInfo();
	afx_msg void OnTransmetaInfo();
	afx_msg void OnProcessorSerial();
	afx_msg void OnMsrEdit();
	afx_msg void OnRealTimeClock();
	afx_msg void OnAbout();
	afx_msg void OnLongHaul();
	afx_msg void OnCacheInfo();
	afx_msg void OnRefreshMmt();
	afx_msg void OnRefreshQpc();
	afx_msg void OnRefreshWt();
	afx_msg void OnRefreshNoload();
	afx_msg void OnDmiInfo();
	afx_msg void OnPciInfo();
	afx_msg void OnChipsetInfo();
	afx_msg void OnEnableCrystalNQuiet();
	afx_msg void OnCrystalNQuiet();
	afx_msg LONG OnRegMessage(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTaskbarCreated(WPARAM, LPARAM);
	afx_msg void OnWindowPosChanging(WINDOWPOS *lpwndpos);
	afx_msg void OnCrystalCpuidIni();
	afx_msg void OnMsrWalker();
	afx_msg void OnCpuMark2005();
	afx_msg void OnCpuInfoHistoryText();
	afx_msg void OnCrystalCpuidHistoryText();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CRYSTALCPUIDDLG_H__0EA235B8_265E_4CB3_8765_D4DC5A075721__INCLUDED_)
