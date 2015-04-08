/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_LONGHAULDLG_H__E43DD49C_522D_4C28_BDF0_09B9ECD69CB7__INCLUDED_)
#define AFX_LONGHAULDLG_H__E43DD49C_522D_4C28_BDF0_09B9ECD69CB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLongHaulDlg 

class CLongHaulDlg : public CDialog
{
public:
	CLongHaulDlg(CWnd* pParent = NULL);
	void Refresh();
	BOOL Create();

	//{{AFX_DATA(CLongHaulDlg)
	enum { IDD = IDD_LONG_HAUL };
	CButton	m_LhConfirmedDialog;
	CComboBox	m_SetFID;
	CString	m_CurrentFID;
	CString	m_LhType;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CLongHaulDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	DWORD m_FlagLH;			// LongHaul 1, 2, 3
	DWORD m_FlagLHTable;	// LongHaulTable 1, 2, 3

	//{{AFX_MSG(CLongHaulDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLhSetFid();
	afx_msg void OnLhConfirmedDialog();
	afx_msg void OnLhCreateLink();
	afx_msg void OnLhHelp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_LONGHAULDLG_H__E43DD49C_522D_4C28_BDF0_09B9ECD69CB7__INCLUDED_)
