/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "FeatureDlg.h"
#include "CrystalCPUIDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;

/////////////////////////////////////////////////////////////////////////////
// CFeatureDialog Dialog

CFeatureDlg::CFeatureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFeatureDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;

	m_Feature = 0;
	m_FeatureEcx = 0;
	m_FeatureEx = 0;
	m_FeatureExEcx = 0;
	m_FeatureVia = 0;
	m_FeatureTransmeta = 0;
	m_FeaturePM = 0;

	P->SysInfo->GetData(CPU_FEATURE,&m_Feature);
	P->SysInfo->GetData(CPU_FEATURE_ECX,&m_FeatureEcx);
	P->SysInfo->GetData(CPU_FEATURE_EX,&m_FeatureEx);
	P->SysInfo->GetData(CPU_FEATURE_EX_ECX,&m_FeatureExEcx);
	P->SysInfo->GetData(CPU_FEATURE_VIA,&m_FeatureVia);
	P->SysInfo->GetData(CPU_FEATURE_TRANSMETA,&m_FeatureTransmeta);
	P->SysInfo->GetData(CPU_FEATURE_PM,&m_FeaturePM);
	P->SysInfo->GetString(CPU_VENDOR_NAME,m_VendorName);

	//{{AFX_DATA_INIT(CFeatureDlg)
	m_FeatureInitial0 = _T("");
	m_FeatureInitial1 = _T("");
	m_FeatureInitial2 = _T("");
	m_FeatureInitial3 = _T("");
	m_FeatureInitial4 = _T("");
	m_FeatureInitial5 = _T("");
	m_FeatureInitial6 = _T("");
	m_FeatureInitial7 = _T("");
	m_FeatureInitial8 = _T("");
	m_FeatureInitial9 = _T("");
	m_FeatureInitial10 = _T("");
	m_FeatureInitial11 = _T("");
	m_FeatureInitial12 = _T("");
	m_FeatureInitial13 = _T("");
	m_FeatureInitial14 = _T("");
	m_FeatureInitial15 = _T("");
	m_FeatureInitial16 = _T("");
	m_FeatureInitial17 = _T("");
	m_FeatureInitial18 = _T("");
	m_FeatureInitial19 = _T("");
	m_FeatureInitial20 = _T("");
	m_FeatureInitial21 = _T("");
	m_FeatureInitial22 = _T("");
	m_FeatureInitial23 = _T("");
	m_FeatureInitial24 = _T("");
	m_FeatureInitial25 = _T("");
	m_FeatureInitial26 = _T("");
	m_FeatureInitial27 = _T("");
	m_FeatureInitial28 = _T("");
	m_FeatureInitial29 = _T("");
	m_FeatureInitial30 = _T("");
	m_FeatureInitial31 = _T("");

	m_FeatureName0 = _T("");
	m_FeatureName1 = _T("");
	m_FeatureName2 = _T("");
	m_FeatureName3 = _T("");
	m_FeatureName4 = _T("");
	m_FeatureName5 = _T("");
	m_FeatureName6 = _T("");
	m_FeatureName7 = _T("");
	m_FeatureName8 = _T("");
	m_FeatureName9 = _T("");
	m_FeatureName10 = _T("");
	m_FeatureName11 = _T("");
	m_FeatureName12 = _T("");
	m_FeatureName13 = _T("");
	m_FeatureName14 = _T("");
	m_FeatureName15 = _T("");
	m_FeatureName16 = _T("");
	m_FeatureName17 = _T("");
	m_FeatureName18 = _T("");
	m_FeatureName19 = _T("");
	m_FeatureName20 = _T("");
	m_FeatureName21 = _T("");
	m_FeatureName22 = _T("");
	m_FeatureName23 = _T("");
	m_FeatureName24 = _T("");
	m_FeatureName25 = _T("");
	m_FeatureName26 = _T("");
	m_FeatureName27 = _T("");
	m_FeatureName28 = _T("");
	m_FeatureName29 = _T("");
	m_FeatureName30 = _T("");
	m_FeatureName31 = _T("");

	m_FeatureSupport0 = _T("");
	m_FeatureSupport1 = _T("");
	m_FeatureSupport2 = _T("");
	m_FeatureSupport3 = _T("");
	m_FeatureSupport4 = _T("");
	m_FeatureSupport5 = _T("");
	m_FeatureSupport6 = _T("");
	m_FeatureSupport7 = _T("");
	m_FeatureSupport8 = _T("");
	m_FeatureSupport9 = _T("");
	m_FeatureSupport10 = _T("");
	m_FeatureSupport11 = _T("");
	m_FeatureSupport12 = _T("");
	m_FeatureSupport13 = _T("");
	m_FeatureSupport14 = _T("");
	m_FeatureSupport15 = _T("");
	m_FeatureSupport16 = _T("");
	m_FeatureSupport17 = _T("");
	m_FeatureSupport18 = _T("");
	m_FeatureSupport19 = _T("");
	m_FeatureSupport20 = _T("");
	m_FeatureSupport21 = _T("");
	m_FeatureSupport22 = _T("");
	m_FeatureSupport23 = _T("");
	m_FeatureSupport24 = _T("");
	m_FeatureSupport25 = _T("");
	m_FeatureSupport26 = _T("");
	m_FeatureSupport27 = _T("");
	m_FeatureSupport28 = _T("");
	m_FeatureSupport29 = _T("");
	m_FeatureSupport30 = _T("");
	m_FeatureSupport31 = _T("");
	//}}AFX_DATA_INIT
}


void CFeatureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFeatureDlg)
	DDX_Text(pDX, IDC_FEATURE_INITIAL_0, m_FeatureInitial0);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_1, m_FeatureInitial1);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_2, m_FeatureInitial2);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_3, m_FeatureInitial3);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_4, m_FeatureInitial4);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_5, m_FeatureInitial5);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_6, m_FeatureInitial6);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_7, m_FeatureInitial7);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_8, m_FeatureInitial8);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_9, m_FeatureInitial9);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_10, m_FeatureInitial10);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_11, m_FeatureInitial11);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_12, m_FeatureInitial12);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_13, m_FeatureInitial13);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_14, m_FeatureInitial14);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_15, m_FeatureInitial15);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_16, m_FeatureInitial16);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_17, m_FeatureInitial17);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_18, m_FeatureInitial18);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_19, m_FeatureInitial19);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_20, m_FeatureInitial20);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_21, m_FeatureInitial21);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_22, m_FeatureInitial22);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_23, m_FeatureInitial23);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_24, m_FeatureInitial24);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_25, m_FeatureInitial25);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_26, m_FeatureInitial26);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_27, m_FeatureInitial27);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_28, m_FeatureInitial28);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_29, m_FeatureInitial29);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_30, m_FeatureInitial30);
	DDX_Text(pDX, IDC_FEATURE_INITIAL_31, m_FeatureInitial31);

	DDX_Text(pDX, IDC_FEATURE_NAME_0, m_FeatureName0);
	DDX_Text(pDX, IDC_FEATURE_NAME_1, m_FeatureName1);
	DDX_Text(pDX, IDC_FEATURE_NAME_2, m_FeatureName2);
	DDX_Text(pDX, IDC_FEATURE_NAME_3, m_FeatureName3);
	DDX_Text(pDX, IDC_FEATURE_NAME_4, m_FeatureName4);
	DDX_Text(pDX, IDC_FEATURE_NAME_5, m_FeatureName5);
	DDX_Text(pDX, IDC_FEATURE_NAME_6, m_FeatureName6);
	DDX_Text(pDX, IDC_FEATURE_NAME_7, m_FeatureName7);
	DDX_Text(pDX, IDC_FEATURE_NAME_8, m_FeatureName8);
	DDX_Text(pDX, IDC_FEATURE_NAME_9, m_FeatureName9);
	DDX_Text(pDX, IDC_FEATURE_NAME_10, m_FeatureName10);
	DDX_Text(pDX, IDC_FEATURE_NAME_11, m_FeatureName11);
	DDX_Text(pDX, IDC_FEATURE_NAME_12, m_FeatureName12);
	DDX_Text(pDX, IDC_FEATURE_NAME_13, m_FeatureName13);
	DDX_Text(pDX, IDC_FEATURE_NAME_14, m_FeatureName14);
	DDX_Text(pDX, IDC_FEATURE_NAME_15, m_FeatureName15);
	DDX_Text(pDX, IDC_FEATURE_NAME_16, m_FeatureName16);
	DDX_Text(pDX, IDC_FEATURE_NAME_17, m_FeatureName17);
	DDX_Text(pDX, IDC_FEATURE_NAME_18, m_FeatureName18);
	DDX_Text(pDX, IDC_FEATURE_NAME_19, m_FeatureName19);
	DDX_Text(pDX, IDC_FEATURE_NAME_20, m_FeatureName20);
	DDX_Text(pDX, IDC_FEATURE_NAME_21, m_FeatureName21);
	DDX_Text(pDX, IDC_FEATURE_NAME_22, m_FeatureName22);
	DDX_Text(pDX, IDC_FEATURE_NAME_23, m_FeatureName23);
	DDX_Text(pDX, IDC_FEATURE_NAME_24, m_FeatureName24);
	DDX_Text(pDX, IDC_FEATURE_NAME_25, m_FeatureName25);
	DDX_Text(pDX, IDC_FEATURE_NAME_26, m_FeatureName26);
	DDX_Text(pDX, IDC_FEATURE_NAME_27, m_FeatureName27);
	DDX_Text(pDX, IDC_FEATURE_NAME_28, m_FeatureName28);
	DDX_Text(pDX, IDC_FEATURE_NAME_29, m_FeatureName29);
	DDX_Text(pDX, IDC_FEATURE_NAME_30, m_FeatureName30);
	DDX_Text(pDX, IDC_FEATURE_NAME_31, m_FeatureName31);

	DDX_Text(pDX, IDC_FEATURE_SUPPORT_0, m_FeatureSupport0);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_1, m_FeatureSupport1);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_2, m_FeatureSupport2);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_3, m_FeatureSupport3);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_4, m_FeatureSupport4);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_5, m_FeatureSupport5);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_6, m_FeatureSupport6);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_7, m_FeatureSupport7);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_8, m_FeatureSupport8);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_9, m_FeatureSupport9);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_10, m_FeatureSupport10);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_11, m_FeatureSupport11);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_12, m_FeatureSupport12);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_13, m_FeatureSupport13);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_14, m_FeatureSupport14);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_15, m_FeatureSupport15);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_16, m_FeatureSupport16);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_17, m_FeatureSupport17);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_18, m_FeatureSupport18);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_19, m_FeatureSupport19);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_20, m_FeatureSupport20);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_21, m_FeatureSupport21);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_22, m_FeatureSupport22);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_23, m_FeatureSupport23);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_24, m_FeatureSupport24);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_25, m_FeatureSupport25);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_26, m_FeatureSupport26);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_27, m_FeatureSupport27);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_28, m_FeatureSupport28);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_29, m_FeatureSupport29);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_30, m_FeatureSupport30);
	DDX_Text(pDX, IDC_FEATURE_SUPPORT_31, m_FeatureSupport31);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFeatureDlg, CDialog)
	//{{AFX_MSG_MAP(CFeatureDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_FEATURE_FLAG_STANDARD, OnFeatureFlagStandard)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_STANDARD_ECX, OnFeatureFlagStandardEcx)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_EX, OnFeatureFlagEx)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_VIA, OnFeatureFlagVia)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_TRANSMETA, OnFeatureFlagTransmeta)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_EX_ECX, OnFeatureFlagExEcx)
	ON_BN_CLICKED(IDC_FEATURE_FLAG_POWER_MANAGEMENT, OnFeatureFlagPowerManagement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFeatureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	OnFeatureFlagStandard();
	CenterWindow();
	ShowWindow(SW_SHOW);
	
//	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), TRUE);

	return TRUE;
}

void CFeatureDlg::OnFeatureFlagStandard() 
{
	m_FeatureInitial0 = "FPU";
	m_FeatureInitial1 = "VME";
	m_FeatureInitial2 = "DE";
	m_FeatureInitial3 = "PSE";
	m_FeatureInitial4 = "TSC";
	m_FeatureInitial5 = "MSR";
	m_FeatureInitial6 = "PAE";
	m_FeatureInitial7 = "MCE";
	m_FeatureInitial8 = "CX8";
	m_FeatureInitial9 = "APIC";
	m_FeatureInitial10 = "";
	m_FeatureInitial11 = "SEP";
	m_FeatureInitial12 = "MTRR";
	m_FeatureInitial13 = "PGE";
	m_FeatureInitial14 = "MCA";
	m_FeatureInitial15 = "CMOV";
	m_FeatureInitial16 = "PAT";
	m_FeatureInitial17 = "PSE-36";
	m_FeatureInitial18 = "PSN";
	m_FeatureInitial19 = "CLFSH";
	m_FeatureInitial20 = "";
	m_FeatureInitial21 = "DS";
	m_FeatureInitial22 = "ACPI";
	m_FeatureInitial23 = "MMX";
	m_FeatureInitial24 = "FXSR";
	m_FeatureInitial25 = "SSE";
	m_FeatureInitial26 = "SSE2";
	m_FeatureInitial27 = "SS";
	m_FeatureInitial28 = "HTT";
	m_FeatureInitial29 = "TM";
	m_FeatureInitial30 = "IA-64";
	m_FeatureInitial31 = "PBE";

	m_FeatureName0 = "Floating-Point Unit";
	m_FeatureName1 = "Virtual Mode Extensions";
	m_FeatureName2 = "Debugging Extensions";
	m_FeatureName3 = "Page Size Extensions";
	m_FeatureName4 = "Time Stamp Counter";
	m_FeatureName5 = "Model Specific Registers";
	m_FeatureName6 = "Page Address Extensions";
	m_FeatureName7 = "Machine Check Exception";
	m_FeatureName8 = "CMPXCHG8B Instruction";
	m_FeatureName9 = "APIC";
	m_FeatureName10 = "Reserved";
	m_FeatureName11 = "SYSENTER/SYSEXIT Instructions";
	m_FeatureName12 = "Memory Type Range Registers";
	m_FeatureName13 = "Global Paging Extension";
	m_FeatureName14 = "Machine Check Architecture";
	m_FeatureName15 = "Conditional Move Instructions";

	m_FeatureName16 = "Page Attribute Table";
	m_FeatureName17 = "36-bit Page Size Extension";
	m_FeatureName18 = "Processor Serial Number";
	m_FeatureName19 = "CLFLUSH Instruction";
	m_FeatureName20 = "Reserved";
	m_FeatureName21 = "Debug Store";
	m_FeatureName22 = "ACPI";
	m_FeatureName23 = "MMX Instructions";
	m_FeatureName24 = "FXSAVE/FXRSTOR Instructions";
	m_FeatureName25 = "Streaming SIMD Extensions";
	m_FeatureName26 = "Streaming SIMD Extensions 2";
	m_FeatureName27 = "Self-Snoop";
	m_FeatureName28 = "Hyper-Threading Technology";
	m_FeatureName29 = "Thermal Monitor";
	m_FeatureName30 = "IA-64 Processor Capable";
	m_FeatureName31 = "Pending Break Enable";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_Feature;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}
	
	UpdateData(FALSE);	
}

void CFeatureDlg::OnFeatureFlagStandardEcx() 
{
	m_FeatureInitial0 = "SSE3";
	m_FeatureInitial1 = "";
	m_FeatureInitial2 = "DTES64";
	m_FeatureInitial3 = "MONITOR";
	m_FeatureInitial4 = "DS-CPL";
	m_FeatureInitial5 = "VMX";
	m_FeatureInitial6 = "SMX";
	m_FeatureInitial7 = "EST";
	m_FeatureInitial8 = "TM2";
	m_FeatureInitial9 = "SSSE3";
	m_FeatureInitial10 = "CID";
	m_FeatureInitial11 = "";
	m_FeatureInitial12 = "FMA";
	m_FeatureInitial13 = "CX16";
	m_FeatureInitial14 = "xTPR";
	m_FeatureInitial15 = "PDCM";
	m_FeatureInitial16 = "";
	m_FeatureInitial17 = "";
	m_FeatureInitial18 = "DCA";
	m_FeatureInitial19 = "SSE4.1";
	m_FeatureInitial20 = "SSE4.2";
	m_FeatureInitial21 = "x2APIC";
	m_FeatureInitial22 = "MOVBE";
	m_FeatureInitial23 = "POPCNT";
	m_FeatureInitial24 = "";
	m_FeatureInitial25 = "AES";
	m_FeatureInitial26 = "XSAVE";
	m_FeatureInitial27 = "OSXSAVE";
	m_FeatureInitial28 = "AVX";
	m_FeatureInitial29 = "";
	m_FeatureInitial30 = "";
	m_FeatureInitial31 = "";

	m_FeatureName0 = "Streaming SIMD Extensions 3";
	m_FeatureName1 = "Reserved";
	m_FeatureName2 = "64-Bit Debug Store";
	m_FeatureName3 = "MONITOR/MWAIT Instructions";
	m_FeatureName4 = "CPL Qualified Debug Store";
	m_FeatureName5 = "Virtual Machine Extentions";
	m_FeatureName6 = "Reserved";
	m_FeatureName7 = "Enhanced SpeedStep Technology";
	m_FeatureName8 = "Thermal Monitor 2";
	m_FeatureName9 = "Supplemental SSE3";
	m_FeatureName10 = "Context ID";
	m_FeatureName11 = "Reserved";
	m_FeatureName12 = "Fused Multiply Add";
	m_FeatureName13 = "CMPXCHG16B";
	m_FeatureName14 = "Send Task Priority Messages";
	m_FeatureName15 = "Performance Capabilities MSR";

	m_FeatureName16 = "Reserved";
	m_FeatureName17 = "Reserved";
	m_FeatureName18 = "Direct Cache Access";
	m_FeatureName19 = "Streaming SIMD Extensions 4.1";
	m_FeatureName20 = "Streaming SIMD Extensions 4.2";
	m_FeatureName21 = "x2APIC";
	m_FeatureName22 = "Reserved";
	m_FeatureName23 = "Population Count Instruction";
	m_FeatureName24 = "Reserved";
	m_FeatureName25 = "Advanced Encryption Standard";
	m_FeatureName26 = "XSAVE";
	m_FeatureName27 = "OSXSAVE";
	m_FeatureName28 = "Advanced Vector Extensions";
	m_FeatureName29 = "Reserved";
	m_FeatureName30 = "Reserved";
	m_FeatureName31 = "Reserved";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeatureEcx;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}
	
	UpdateData(FALSE);	
}


void CFeatureDlg::OnFeatureFlagEx() 
{
	m_FeatureInitial0 = "FPU";
	m_FeatureInitial1 = "VME";
	m_FeatureInitial2 = "DE";
	m_FeatureInitial3 = "PSE";
	m_FeatureInitial4 = "TSC";
	m_FeatureInitial5 = "MSR";
	m_FeatureInitial6 = "PAE";
	m_FeatureInitial7 = "MCE";
	m_FeatureInitial8 = "CX8";
	m_FeatureInitial9 = "APIC";
	m_FeatureInitial10 = "";
	m_FeatureInitial11 = "SEP";
	m_FeatureInitial12 = "MTRR";
	m_FeatureInitial13 = "PGE";
	m_FeatureInitial14 = "MCA";
	m_FeatureInitial15 = "CMOV";
	m_FeatureInitial16 = "PAT";
	m_FeatureInitial17 = "PSE-36";
	m_FeatureInitial18 = "";
	m_FeatureInitial19 = "MP";
	m_FeatureInitial20 = "NX/XD";
	m_FeatureInitial21 = "";
	m_FeatureInitial22 = "MMX+";
	m_FeatureInitial23 = "MMX";
	m_FeatureInitial24 = "FXSR";
	m_FeatureInitial25 = "FFXSR";
	m_FeatureInitial26 = "P1GB";
	m_FeatureInitial27 = "RDTSCP";
	m_FeatureInitial28 = "";
	m_FeatureInitial29 = "x64";
	m_FeatureInitial30 = "3DNow!+";
	m_FeatureInitial31 = "3DNow!";

	m_FeatureName0 = "Floating-Point Unit";
	m_FeatureName1 = "Virtual Mode Extensions";
	m_FeatureName2 = "Debugging Extensions";
	m_FeatureName3 = "Page Size Extensions";
	m_FeatureName4 = "Time Stamp Counter";
	m_FeatureName5 = "Model Specific Registers";
	m_FeatureName6 = "Page Address Extensions";
	m_FeatureName7 = "Machine Check Exception";
	m_FeatureName8 = "CMPXCHG8B Instruction";
	m_FeatureName9 = "On-chip APIC Hardware";
	m_FeatureName10 = "Reserved";
	m_FeatureName11 = "SYSCALL/SYSRET Instructions";
	m_FeatureName12 = "Memory Type Range Registers";
	m_FeatureName13 = "Page Global Enable";
	m_FeatureName14 = "Machine Check Architecture";
	m_FeatureName15 = "Conditional Move Instruction";
	m_FeatureName16 = "Page Attribute Table";
	m_FeatureName17 = "36-bit Page Size Extension";
	m_FeatureName18 = "Reserved";
	m_FeatureName19 = "Multiprocessing Capable";
	m_FeatureName20 = "No Execute/Execute Disable";
	m_FeatureName21 = "Reserved";
	m_FeatureName22 = "MMX Instruction Extensions";
	m_FeatureName23 = "MMX Instructions";
	m_FeatureName24 = "FXSAVE/FXRSTOR Instructions";
	m_FeatureName25 = "FXSAVE/FXRSTOR optimizations";
	m_FeatureName26 = "1GB Page Size";
	m_FeatureName27 = "RDTSCP instructions";
	m_FeatureName28 = "Reserved";
	m_FeatureName29 = "AMD64/Intel 64";
	m_FeatureName30 = "3DNow! Instruction Extensions";
	m_FeatureName31 = "3DNow! Instructions";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeatureEx;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}
	
	UpdateData(FALSE);	
}

void CFeatureDlg::OnFeatureFlagExEcx() 
{
	m_FeatureInitial0 = "LAHF";
	m_FeatureInitial1 = "CMPL";
	m_FeatureInitial2 = "AMD-V";
	m_FeatureInitial3 = "EAS";
	m_FeatureInitial4 = "AMC8";
	m_FeatureInitial5 = "ABM";
	m_FeatureInitial6 = "SSE4A";
	m_FeatureInitial7 = "MASSE";
	m_FeatureInitial8 = "PREFETCH";
	m_FeatureInitial9 = "OSVW";
	m_FeatureInitial10 = "IBS";
	m_FeatureInitial11 = "SSE5";
	m_FeatureInitial12 = "SKINIT";
	m_FeatureInitial13 = "WDT";
	m_FeatureInitial14 = "";
	m_FeatureInitial15 = "";
	m_FeatureInitial16 = "";
	m_FeatureInitial17 = "";
	m_FeatureInitial18 = "";
	m_FeatureInitial19 = "";
	m_FeatureInitial20 = "";
	m_FeatureInitial21 = "";
	m_FeatureInitial22 = "";
	m_FeatureInitial23 = "";
	m_FeatureInitial24 = "";
	m_FeatureInitial25 = "";
	m_FeatureInitial26 = "";
	m_FeatureInitial27 = "";
	m_FeatureInitial28 = "";
	m_FeatureInitial29 = "";
	m_FeatureInitial30 = "";
	m_FeatureInitial31 = "";

	m_FeatureName0 = "LAHF/SAHF Support";
	m_FeatureName1 = "Core MultiProcessing legacy";
	m_FeatureName2 = "SVM : Secure virtual machine";
	m_FeatureName3 = "Extended APIC";
	m_FeatureName4 = "LOCK MOV CR0 means MOV CR8";
	m_FeatureName5 = "Advanced Bit Manipulation";
	m_FeatureName6 = "Streaming SIMD Extensions 4 AMD";
	m_FeatureName7 = "Misaligned SSE Modus";
	m_FeatureName8 = "3DNow! Prefetch";
	m_FeatureName9 = "OS Visible Workaround";
	m_FeatureName10 = "Instruction Based Sampling";
	m_FeatureName11 = "Streaming SIMD Extensions 5";
	m_FeatureName12 = "SKINIT/STGI/DEV Support";
	m_FeatureName13 = "Watchdog timer";
	m_FeatureName14 = "Reserved";
	m_FeatureName15 = "Reserved";
	m_FeatureName16 = "Reserved";
	m_FeatureName17 = "Reserved";
	m_FeatureName18 = "Reserved";
	m_FeatureName19 = "Reserved";
	m_FeatureName20 = "Reserved";
	m_FeatureName21 = "Reserved";
	m_FeatureName22 = "Reserved";
	m_FeatureName23 = "Reserved";
	m_FeatureName24 = "Reserved";
	m_FeatureName25 = "Reserved";
	m_FeatureName26 = "Reserved";
	m_FeatureName27 = "Reserved";
	m_FeatureName28 = "Reserved";
	m_FeatureName29 = "Reserved";
	m_FeatureName30 = "Reserved";
	m_FeatureName31 = "Reserved";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeatureExEcx;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}
	
	UpdateData(FALSE);		
}


void CFeatureDlg::OnFeatureFlagVia() 
{
	m_FeatureInitial0 = "AIS";
	m_FeatureInitial1 = "AIS-E";
	m_FeatureInitial2 = "RNG";
	m_FeatureInitial3 = "RNG-E";
	m_FeatureInitial4 = "LH";
	m_FeatureInitial5 = "FEMMS";
	m_FeatureInitial6 = "ACE";
	m_FeatureInitial7 = "ACE-E";
	m_FeatureInitial8 = "MM";
	m_FeatureInitial9 = "MM-E";
	m_FeatureInitial10 = "";
	m_FeatureInitial11 = "";
	m_FeatureInitial12 = "";
	m_FeatureInitial13 = "";
	m_FeatureInitial14 = "";
	m_FeatureInitial15 = "";
	m_FeatureInitial16 = "";
	m_FeatureInitial17 = "";
	m_FeatureInitial18 = "";
	m_FeatureInitial19 = "";
	m_FeatureInitial20 = "";
	m_FeatureInitial21 = "";
	m_FeatureInitial22 = "";
	m_FeatureInitial23 = "";
	m_FeatureInitial24 = "";
	m_FeatureInitial25 = "";
	m_FeatureInitial26 = "";
	m_FeatureInitial27 = "";
	m_FeatureInitial28 = "";
	m_FeatureInitial29 = "";
	m_FeatureInitial30 = "";
	m_FeatureInitial31 = "";

	m_FeatureName0 = "Alternate Instruction Set";
	m_FeatureName1 = "AIS enabled";
	m_FeatureName2 = "Random Number Generator";
	m_FeatureName3 = "RNG enabled";
	m_FeatureName4 = "LongHaul MSR 0000_110Ah";
	m_FeatureName5 = "FEMMS";
	m_FeatureName6 = "Advanced Cryptography Engine";
	m_FeatureName7 = "ACE enabled";
	m_FeatureName8 = "Montgomery Multiplier";
	m_FeatureName9 = "Montgomery Multiplier enabled";
	m_FeatureName10 = "Reserved";
	m_FeatureName11 = "Reserved";
	m_FeatureName12 = "Reserved";
	m_FeatureName13 = "Reserved";
	m_FeatureName14 = "Reserved";
	m_FeatureName15 = "Reserved";
	m_FeatureName16 = "Reserved";
	m_FeatureName17 = "Reserved";
	m_FeatureName18 = "Reserved";
	m_FeatureName19 = "Reserved";
	m_FeatureName20 = "Reserved";
	m_FeatureName21 = "Reserved";
	m_FeatureName22 = "Reserved";
	m_FeatureName23 = "Reserved";
	m_FeatureName24 = "Reserved";
	m_FeatureName25 = "Reserved";
	m_FeatureName26 = "Reserved";
	m_FeatureName27 = "Reserved";
	m_FeatureName28 = "Reserved";
	m_FeatureName29 = "Reserved";
	m_FeatureName30 = "Reserved";
	m_FeatureName31 = "Reserved";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeatureVia;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}
	
	UpdateData(FALSE);		
}

void CFeatureDlg::OnFeatureFlagTransmeta() 
{
	m_FeatureInitial0 = "BAD";
	m_FeatureInitial1 = "LR";
	m_FeatureInitial2 = "";
	m_FeatureInitial3 = "LRTI";
	m_FeatureInitial4 = "";
	m_FeatureInitial5 = "";
	m_FeatureInitial6 = "";
	m_FeatureInitial7 = "";
	m_FeatureInitial8 = "";
	m_FeatureInitial9 = "";
	m_FeatureInitial10 = "";
	m_FeatureInitial11 = "";
	m_FeatureInitial12 = "";
	m_FeatureInitial13 = "";
	m_FeatureInitial14 = "";
	m_FeatureInitial15 = "";
	m_FeatureInitial16 = "";
	m_FeatureInitial17 = "";
	m_FeatureInitial18 = "";
	m_FeatureInitial19 = "";
	m_FeatureInitial20 = "";
	m_FeatureInitial21 = "";
	m_FeatureInitial22 = "";
	m_FeatureInitial23 = "";
	m_FeatureInitial24 = "";
	m_FeatureInitial25 = "";
	m_FeatureInitial26 = "";
	m_FeatureInitial27 = "";
	m_FeatureInitial28 = "";
	m_FeatureInitial29 = "";
	m_FeatureInitial30 = "";
	m_FeatureInitial31 = "";

	m_FeatureName0 = "recovery CMS active";
	m_FeatureName1 = "LongRun";
	m_FeatureName2 = "Reserved";
	m_FeatureName3 = "LongRun Table Interface";
	m_FeatureName4 = "Reserved";
	m_FeatureName5 = "Reserved";
	m_FeatureName6 = "Reserved";
	m_FeatureName7 = "Reserved";
	m_FeatureName8 = "Reserved";
	m_FeatureName9 = "Reserved";
	m_FeatureName10 = "Reserved";
	m_FeatureName11 = "Reserved";
	m_FeatureName12 = "Reserved";
	m_FeatureName13 = "Reserved";
	m_FeatureName14 = "Reserved";
	m_FeatureName15 = "Reserved";
	m_FeatureName16 = "Reserved";
	m_FeatureName17 = "Reserved";
	m_FeatureName18 = "Reserved";
	m_FeatureName19 = "Reserved";
	m_FeatureName20 = "Reserved";
	m_FeatureName21 = "Reserved";
	m_FeatureName22 = "Reserved";
	m_FeatureName23 = "Reserved";
	m_FeatureName24 = "Reserved";
	m_FeatureName25 = "Reserved";
	m_FeatureName26 = "Reserved";
	m_FeatureName27 = "Reserved";
	m_FeatureName28 = "Reserved";
	m_FeatureName29 = "Reserved";
	m_FeatureName30 = "Reserved";
	m_FeatureName31 = "Reserved";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeatureTransmeta;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}

	UpdateData(FALSE);		
}

void CFeatureDlg::OnFeatureFlagPowerManagement() 
{
	m_FeatureInitial0 = "TS";
	m_FeatureInitial1 = "FID";
	m_FeatureInitial2 = "VID";
	m_FeatureInitial3 = "TTP";
	m_FeatureInitial4 = "TM";
	m_FeatureInitial5 = "STC";
	m_FeatureInitial6 = "MUL";
	m_FeatureInitial7 = "HWPS";
	m_FeatureInitial8 = "ITSC";
	m_FeatureInitial9 = "";
	m_FeatureInitial10 = "";
	m_FeatureInitial11 = "";
	m_FeatureInitial12 = "";
	m_FeatureInitial13 = "";
	m_FeatureInitial14 = "";
	m_FeatureInitial15 = "";
	m_FeatureInitial16 = "";
	m_FeatureInitial17 = "";
	m_FeatureInitial18 = "";
	m_FeatureInitial19 = "";
	m_FeatureInitial20 = "";
	m_FeatureInitial21 = "";
	m_FeatureInitial22 = "";
	m_FeatureInitial23 = "";
	m_FeatureInitial24 = "";
	m_FeatureInitial25 = "";
	m_FeatureInitial26 = "";
	m_FeatureInitial27 = "";
	m_FeatureInitial28 = "";
	m_FeatureInitial29 = "";
	m_FeatureInitial30 = "";
	m_FeatureInitial31 = "";

	m_FeatureName0 = "Temperature Sensor";
	m_FeatureName1 = "Frequency ID Control";
	m_FeatureName2 = "Voltage ID Control";
	m_FeatureName3 = "Thermal Trip";
	m_FeatureName4 = "Thermal Monitoring";
	m_FeatureName5 = "Software Thermanl Control";
	m_FeatureName6 = "100MHz Multiplier Steps";
	m_FeatureName7 = "Hardware P-State Control";
	m_FeatureName8 = "Invariant TSC";
	m_FeatureName9 = "Reserved";
	m_FeatureName10 = "Reserved";
	m_FeatureName11 = "Reserved";
	m_FeatureName12 = "Reserved";
	m_FeatureName13 = "Reserved";
	m_FeatureName14 = "Reserved";
	m_FeatureName15 = "Reserved";
	m_FeatureName16 = "Reserved";
	m_FeatureName17 = "Reserved";
	m_FeatureName18 = "Reserved";
	m_FeatureName19 = "Reserved";
	m_FeatureName20 = "Reserved";
	m_FeatureName21 = "Reserved";
	m_FeatureName22 = "Reserved";
	m_FeatureName23 = "Reserved";
	m_FeatureName24 = "Reserved";
	m_FeatureName25 = "Reserved";
	m_FeatureName26 = "Reserved";
	m_FeatureName27 = "Reserved";
	m_FeatureName28 = "Reserved";
	m_FeatureName29 = "Reserved";
	m_FeatureName30 = "Reserved";
	m_FeatureName31 = "Reserved";

	m_FeatureSupport0 = "NO";
	m_FeatureSupport1 = "NO";
	m_FeatureSupport2 = "NO";
	m_FeatureSupport3 = "NO";
	m_FeatureSupport4 = "NO";
	m_FeatureSupport5 = "NO";
	m_FeatureSupport6 = "NO";
	m_FeatureSupport7 = "NO";
	m_FeatureSupport8 = "NO";
	m_FeatureSupport9 = "NO";
	m_FeatureSupport10 = "NO";
	m_FeatureSupport11 = "NO";
	m_FeatureSupport12 = "NO";
	m_FeatureSupport13 = "NO";
	m_FeatureSupport14 = "NO";
	m_FeatureSupport15 = "NO";
	m_FeatureSupport16 = "NO";
	m_FeatureSupport17 = "NO";
	m_FeatureSupport18 = "NO";
	m_FeatureSupport19 = "NO";
	m_FeatureSupport20 = "NO";
	m_FeatureSupport21 = "NO";
	m_FeatureSupport22 = "NO";
	m_FeatureSupport23 = "NO";
	m_FeatureSupport24 = "NO";
	m_FeatureSupport25 = "NO";
	m_FeatureSupport26 = "NO";
	m_FeatureSupport27 = "NO";
	m_FeatureSupport28 = "NO";
	m_FeatureSupport29 = "NO";
	m_FeatureSupport30 = "NO";
	m_FeatureSupport31 = "NO";

	DWORD data = m_FeaturePM;
	if(data & 0x00000001){m_FeatureSupport0 = "YES";}
	if(data & 0x00000002){m_FeatureSupport1 = "YES";}
	if(data & 0x00000004){m_FeatureSupport2 = "YES";}
	if(data & 0x00000008){m_FeatureSupport3 = "YES";}
	if(data & 0x00000010){m_FeatureSupport4 = "YES";}
	if(data & 0x00000020){m_FeatureSupport5 = "YES";}
	if(data & 0x00000040){m_FeatureSupport6 = "YES";}
	if(data & 0x00000080){m_FeatureSupport7 = "YES";}
	if(data & 0x00000100){m_FeatureSupport8 = "YES";}
	if(data & 0x00000200){m_FeatureSupport9 = "YES";}
	if(data & 0x00000400){m_FeatureSupport10 = "YES";}
	if(data & 0x00000800){m_FeatureSupport11 = "YES";}
	if(data & 0x00001000){m_FeatureSupport12 = "YES";}
	if(data & 0x00002000){m_FeatureSupport13 = "YES";}
	if(data & 0x00004000){m_FeatureSupport14 = "YES";}
	if(data & 0x00008000){m_FeatureSupport15 = "YES";}
	if(data & 0x00010000){m_FeatureSupport16 = "YES";}
	if(data & 0x00020000){m_FeatureSupport17 = "YES";}
	if(data & 0x00040000){m_FeatureSupport18 = "YES";}
	if(data & 0x00080000){m_FeatureSupport19 = "YES";}
	if(data & 0x00100000){m_FeatureSupport20 = "YES";}
	if(data & 0x00200000){m_FeatureSupport21 = "YES";}
	if(data & 0x00400000){m_FeatureSupport22 = "YES";}
	if(data & 0x00800000){m_FeatureSupport23 = "YES";}
	if(data & 0x01000000){m_FeatureSupport24 = "YES";}
	if(data & 0x02000000){m_FeatureSupport25 = "YES";}
	if(data & 0x04000000){m_FeatureSupport26 = "YES";}
	if(data & 0x08000000){m_FeatureSupport27 = "YES";}
	if(data & 0x10000000){m_FeatureSupport28 = "YES";}
	if(data & 0x20000000){m_FeatureSupport29 = "YES";}
	if(data & 0x40000000){m_FeatureSupport30 = "YES";}
	if(data & 0x80000000){m_FeatureSupport31 = "YES";}

	UpdateData(FALSE);	
}

BOOL CFeatureDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CFeatureDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_FEATURE_FLAGS,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_FeatureDlg = NULL;
}

void CFeatureDlg::OnOK() 
{	
//	CDialog::OnOK();
}

void CFeatureDlg::OnCancel() 
{
//	CDialog::OnCancel();
	OnClose();
}
