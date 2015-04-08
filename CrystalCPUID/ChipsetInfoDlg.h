/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CHIPSETINFODLG_H__A2E3F1D4_DB78_47C3_B33D_F1B9C2546AAF__INCLUDED_)
#define AFX_CHIPSETINFODLG_H__A2E3F1D4_DB78_47C3_B33D_F1B9C2546AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CChipsetInfoDlg Dialog

class CChipsetInfoDlg : public CDialog
{
public:
	CChipsetInfoDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CChipsetInfoDlg)
	enum { IDD = IDD_CHIPSET_INFO };
	CString	m_Chipset;
	CString	m_NorthBridge;
	CString	m_SouthBridge;
	CString	m_VideoChip;
	CString	m_VideoChipId;
	CString	m_NorthBridgeId;
	CString	m_SouthBridgeId;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CChipsetInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CChipsetInfoDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CHIPSETINFODLG_H__A2E3F1D4_DB78_47C3_B33D_F1B9C2546AAF__INCLUDED_)
