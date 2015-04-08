/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CACHEINFODLG_H__A902E2A2_7A71_490E_B7D5_02CD9B607F44__INCLUDED_)
#define AFX_CACHEINFODLG_H__A902E2A2_7A71_490E_B7D5_02CD9B607F44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CCacheInfoDlg 
class CCacheInfoDlg : public CDialog
{
public:
	CCacheInfoDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CCacheInfoDlg)
	enum { IDD = IDD_CACHE_INFO };
	CListCtrl	m_CacheList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCacheInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CCacheInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CACHEINFODLG_H__A902E2A2_7A71_490E_B7D5_02CD9B607F44__INCLUDED_)
