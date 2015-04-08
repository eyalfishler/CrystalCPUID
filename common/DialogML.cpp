#include "stdafx.h"
#include "DialogML.h"

CDialogML::CDialogML()
{
}

CDialogML::~CDialogML()
{
}

BEGIN_MESSAGE_MAP(CDialogML, CDialog)
    //{{AFX_MSG_MAP(CDialogML)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDialogML::Create()
{
	BOOL bret = CDialog::Create( IDD, P );
	if( bret == TRUE){
		CenterWindow();
		ShowWindow(SW_SHOW);
	}
	return bret;
}

CDialogML::OnClose()
{
	CMenu *menu = P->GetMenu();
	menu->EnableMenuItem(IDM_REAL_TIME_CLOCK,MF_ENABLED);
	P->SetMenu(menu);
	P->ShowWindow(SW_SHOW);

	CDialog::OnClose();
	delete this;
	P->m_RealTimeClockDlg = NULL;
}

