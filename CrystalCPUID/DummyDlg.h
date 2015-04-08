/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_DUMMYDLG_H__89013F1D_240F_4985_B236_70DB0C42C84B__INCLUDED_)
#define AFX_DUMMYDLG_H__89013F1D_240F_4985_B236_70DB0C42C84B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDummyDlg Dialog

class CDummyDlg : public CDialog
{
public:
	CDummyDlg(CWnd* pParent = NULL); 

	//{{AFX_DATA(CDummyDlg)
	enum { IDD = IDD_DUMMY };
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDummyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CDummyDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DUMMYDLG_H__89013F1D_240F_4985_B236_70DB0C42C84B__INCLUDED_)
