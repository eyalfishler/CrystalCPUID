/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_ABOUTDLG_H__45431584_60DA_4FF5_A116_2BA72C8A8D3C__INCLUDED_)
#define AFX_ABOUTDLG_H__45431584_60DA_4FF5_A116_2BA72C8A8D3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg 
#include "MatrixStatic.h"

class CAboutDlg : public CDialog
{
public:
	CAboutDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUT };
	CMatrixStatic	m_Main;
	CMatrixStatic	m_Sub;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CAboutDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_ABOUTDLG_H__45431584_60DA_4FF5_A116_2BA72C8A8D3C__INCLUDED_)
