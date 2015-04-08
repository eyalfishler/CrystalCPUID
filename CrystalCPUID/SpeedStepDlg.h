/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_SPEEDSTEPDLG_H__0393CCB0_F62A_4827_8C93_FB7FB82D9B51__INCLUDED_)
#define AFX_SPEEDSTEPDLG_H__0393CCB0_F62A_4827_8C93_FB7FB82D9B51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSpeedStepDlg Dialog

class CSpeedStepDlg : public CDialog
{
public:
	CSpeedStepDlg(CWnd* pParent = NULL);
	BOOL Create();
	void Refresh();

	//{{AFX_DATA(CSpeedStepDlg)
	enum { IDD = IDD_SPEED_STEP };
	CButton	m_EnableVoltageC;
	CComboBox	m_SelectCpu;
	CButton	m_SsConfirmedDialog;
	CButton		m_Help;
	CComboBox	m_SetVID;
	CComboBox	m_SetFID;
	CString	m_CurrentFID;
	CString	m_CurrentVID;
	CString	m_MaxFID;
	CString	m_MinVID;
	CString	m_MaxVID;
//	CString	m_StartupFID;
//	CString	m_StartupVID;
	BOOL	m_EnableVoltage;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CSpeedStepDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CSpeedStepDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeSsSetFid();
	afx_msg void OnSelchangeSsSetVid();
	afx_msg void OnSsConfirmedDialog();
	afx_msg void OnSsEnableVoltage();
	afx_msg void OnSsCreateLink();
	afx_msg void OnSsHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_SPEEDSTEPDLG_H__0393CCB0_F62A_4827_8C93_FB7FB82D9B51__INCLUDED_)
