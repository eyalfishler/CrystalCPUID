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
#include "DummyDlg.h"
#include "CrystalCPUIDDlg.h"
#include "../common/DialogTemplateEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int gDialogID;
extern char gDefaultFontFace[128];
extern int gDefaultFontSize;

/////////////////////////////////////////////////////////////////////////////
// CDummyDlg Dialog


CDummyDlg::CDummyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDummyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDummyDlg)
	//}}AFX_DATA_INIT
}


void CDummyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDummyDlg)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDummyDlg, CDialog)
	//{{AFX_MSG_MAP(CDummyDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDummyDlg Message Handler

BOOL CDummyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	ModifyStyleEx(0,WS_EX_TOOLWINDOW);

	char str[256];

	for(;;){
		CCrystalCPUIDDlg* dlg = new CCrystalCPUIDDlg(this);
		CDialogTemplateEx tml;
		tml.Load(MAKEINTRESOURCE(gDialogID));
		
		GetPrivateProfileString("Setting","MainFontFace", gDefaultFontFace,str,256,AfxGetApp()->m_pszProfileName);
		CString face = str;
		UINT size = GetPrivateProfileInt("Setting","MainFontSize",gDefaultFontSize,AfxGetApp()->m_pszProfileName);
		if( size <= 0 ){size = 9;}
		tml.SetFont(face,size);
		dlg->InitModalIndirect((HGLOBAL)tml.Detach());
		AfxGetApp()->m_pActiveWnd = dlg;
	
		if( dlg->DoModal() != 999 )
		{
			delete dlg;
			break;
		}
		delete dlg;
	}

	EndDialog(0);
	
	return TRUE;
}


