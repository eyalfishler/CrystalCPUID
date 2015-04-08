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
#include "LongHaulDlg.h"
#include "../SysInfo/MultiplierTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CCrystalCPUIDDlg* P;
extern int gLanguage;

/////////////////////////////////////////////////////////////////////////////
// CLongHaulDlg 


CLongHaulDlg::CLongHaulDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLongHaulDlg::IDD, pParent)
{
	P = (CCrystalCPUIDDlg*)pParent;
	//{{AFX_DATA_INIT(CLongHaulDlg)
	m_CurrentFID = _T("");
	m_LhType = _T("");
	//}}AFX_DATA_INIT
}


void CLongHaulDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLongHaulDlg)
	DDX_Control(pDX, IDC_LH_CONFIRMED_DIALOG, m_LhConfirmedDialog);
	DDX_Control(pDX, IDC_LH_SET_FID, m_SetFID);
	DDX_Text(pDX, IDC_LH_CURRENT_FID, m_CurrentFID);
	DDX_Text(pDX, IDC_LH_TYPE, m_LhType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLongHaulDlg, CDialog)
	//{{AFX_MSG_MAP(CLongHaulDlg)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_LH_SET_FID, OnSelchangeLhSetFid)
	ON_BN_CLICKED(IDC_LH_CONFIRMED_DIALOG, OnLhConfirmedDialog)
	ON_BN_CLICKED(IDC_LH_CREATE_LINK, OnLhCreateLink)
	ON_BN_CLICKED(IDC_LH_HELP, OnLhHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLongHaulDlg 
void CLongHaulDlg::OnOK() 
{	
//	CDialog::OnOK();
}

void CLongHaulDlg::OnCancel() 
{	
//	CDialog::OnCancel();
	OnClose();
}

BOOL CLongHaulDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString cstr;
	char str[256];
	int i;
	DWORD id = 0;

	// LongHaul Type
	m_LhType.Format("Type %d", P->m_LhType );

	// Init ComboBox
	int NumberOfTable = 0;

	switch( P->m_LhTableType )
	{
	case SAMUEL:
		for(i = 0; i < 16 ; i++ ){
			cstr.Format("%.1fx", LongHaulSamuel[i] / 2.0);
			m_SetFID.AddString(cstr);
		}
		break;
	case SAMUEL2:
		for(i = 0; i < 16 ; i++ ){
			cstr.Format("%.1fx", LongHaulSamuel2[i] / 2.0);
			m_SetFID.AddString(cstr);
		}
		break;
	case EZRA:
		for(i = 0; i < 16 ; i++ ){
			cstr.Format("%.1fx", LongHaulEzra[i] / 2.0);
			m_SetFID.AddString(cstr);
		}
		break;
	case EZRA_T:
		for(i = 0; i < 32 ; i++ ){
			cstr.Format("%.1fx", LongHaulEzraT[i] / 2.0);
			m_SetFID.AddString(cstr);
		}
		break;
	case NEHEMIAH:
		for(i = 0; i < 32 ; i++ ){
			cstr.Format("%.1fx", LongHaulNehemiah[i] / 2.0);
			m_SetFID.AddString(cstr);
		}
		break;
	default:
		break;
	}

	m_CurrentFID.Format("%.1fx", atof( P->m_Multiplier ) );
	cstr.Format("%sx", m_CurrentFID );
	m_SetFID.SetCurSel( m_SetFID.FindString(0, cstr) );

	GetPrivateProfileString("Setting","ShowConfirmedDialog","0",str,256,P->m_ini);
	if( atoi(str) >= 1 ){
		m_LhConfirmedDialog.SetCheck(1);
	}

	UpdateData(FALSE);
	CenterWindow();
	ShowWindow(SW_SHOW);

	return TRUE; 
}

void CLongHaulDlg::Refresh()
{
	char str[256];
	m_CurrentFID.Format("%.1fx", atof( P->SysInfo->GetInfo(CPU_MULTIPLIER,str) ) );
	UpdateData(FALSE);
	UpdateWindow();
}

void CLongHaulDlg::OnSelchangeLhSetFid() 
{
	if( P->m_FlagCrystalNQuiet ){
		MessageBoxEJ(gLanguage,
			IDS_STOP_CRYSTAL_N_QUIET_EN,
			IDS_STOP_CRYSTAL_N_QUIET_JP);
		return ;
	}

	if( ! m_LhConfirmedDialog.GetCheck()
	||  MessageBoxEJ(gLanguage,
			IDS_SET_NEW_FID_EN,
			IDS_SET_NEW_FID_JP,
			MB_YESNO) == IDYES  
	){
		char str[256];
		m_SetFID.GetLBText(m_SetFID.GetCurSel(), str);
		if( atoi(str) > 0 ){
			if(P->m_RealTimeClockDlg != NULL){
				P->m_RealTimeClockDlg->EndTimer();
			}
			P->SysInfo->SetData(LH_SET_FID, m_SetFID.GetCurSel());
			P->ForceHideOriginalClock();
			Refresh();
			P->Refresh(0);
			if(P->m_RealTimeClockDlg != NULL){
				P->m_RealTimeClockDlg->StartTimer();
			}
		}
	}
}

void CLongHaulDlg::OnLhConfirmedDialog() 
{
	CString cstr;
	cstr.Format("%d", m_LhConfirmedDialog.GetCheck() );
	WritePrivateProfileString("Setting","ShowConfirmedDialog",cstr,P->m_ini);
}

void CLongHaulDlg::OnLhCreateLink() 
{
	UpdateData( TRUE );
	CString cstr,cstr1,cstr2;
	CString option;
	CString strobj;
	CString message;
	TCHAR buf[MAX_PATH];

	m_SetFID.GetLBText(m_SetFID.GetCurSel(),cstr1);

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

	option.Format("/F%d /P%d /E", m_SetFID.GetCurSel(), P->m_Mask + 1 );
	strobj.Format("%s\\%s.lnk", buf, cstr1);

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
						MessageBoxEJ(gLanguage,
							IDS_CAN_NOT_CREATE_SHORTCUT_EN,
							IDS_CAN_NOT_CREATE_SHORTCUT_JP);
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

void CLongHaulDlg::OnLhHelp() 
{
	MessageBoxEJ(gLanguage,
		IDS_LH_COMMAND_LINE_EN,
		IDS_LH_COMMAND_LINE_JP);
}

BOOL CLongHaulDlg::Create() 
{
	return CDialog::Create( IDD, P );
}

void CLongHaulDlg::OnClose() 
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_LONG_HAUL,MF_ENABLED);
	P->SetMenu(menu);

	CDialog::OnClose();
	delete this;
	P->m_LongHaulDlg = NULL;
}
