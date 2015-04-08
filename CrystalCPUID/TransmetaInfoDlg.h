/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_TRANSMETAINFODLG_H__319CF71A_3E74_4B4B_94B0_3637094AC37A__INCLUDED_)
#define AFX_TRANSMETAINFODLG_H__319CF71A_3E74_4B4B_94B0_3637094AC37A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTransmetaInfoDlg : public CDialog
{
public:
	CTransmetaInfoDlg(CWnd* pParent = NULL);
	BOOL Create();
	//{{AFX_DATA(CTransmetaInfoDlg)
	enum { IDD = IDD_TRANSMETA_INFO };
	CString	m_TmClock;
	CString	m_TmNominalClock;
	CString	m_TmNameString;
	CString	m_TmCurrentGateDelay;
	CString	m_TmCurrentPerformance;
	CString	m_TmCurrentVoltage;
	CString	m_TmHardwareVersion;
	CString	m_TmSoftwareVersion;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CTransmetaInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CTransmetaInfoDlg)
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnTmRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_TRANSMETAINFODLG_H__319CF71A_3E74_4B4B_94B0_3637094AC37A__INCLUDED_)
