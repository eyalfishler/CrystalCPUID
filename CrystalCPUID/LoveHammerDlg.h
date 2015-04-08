/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_LoveHammerDlg_H__F0DF2A0C_6D84_4056_8BB0_56E6621731D7__INCLUDED_)
#define AFX_LoveHammerDlg_H__F0DF2A0C_6D84_4056_8BB0_56E6621731D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLoveHammerDlg : public CDialog
{
public:
	CLoveHammerDlg(CWnd* pParent = NULL);
	void Refresh();
	BOOL Create();

	//{{AFX_DATA(CLoveHammerDlg)
	enum { IDD = IDD_LOVE_HAMMER };
	CButton	m_EnableVoltageC;
	CComboBox	m_SelectCpu;
	CButton	m_LhConfirmedDialog;
	CButton		m_Help;
	CComboBox	m_SetVID;
	CComboBox	m_SetFID;
	CString	m_CurrentFID;
	CString	m_CurrentVID;
	CString	m_MaxFID;
	CString	m_MaxVID;
	CString	m_StartupFID;
	CString	m_StartupVID;
	BOOL	m_EnableVoltage;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CLoveHammerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

protected:
	DWORD m_FlagLH;	// Love Hammer 7 => K7, 8 => K8
	BOOL m_FlagK8100MHzSteps;
	BOOL m_FlagK8Under1100V;
	double m_K8HyperTransport;
	//{{AFX_MSG(CLoveHammerDlg)
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLhSetFid();
	afx_msg void OnLhCreateLink();
	afx_msg void OnSelchangeLhSetVid();
	afx_msg void OnLhEnableVoltage();
	afx_msg void OnLhHelp();
	afx_msg void OnLhConfirmedDialog();
	afx_msg void OnSelchangeLhCpuSelect();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_LoveHammerDlg_H__F0DF2A0C_6D84_4056_8BB0_56E6621731D7__INCLUDED_)
