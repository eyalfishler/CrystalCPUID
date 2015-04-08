/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CPUIDDLG_H__23C0088D_E6FB_42B6_9B53_BF543FCAF1A0__INCLUDED_)
#define AFX_CPUIDDLG_H__23C0088D_E6FB_42B6_9B53_BF543FCAF1A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCpuidDlg : public CDialog
{
public:
	CCpuidDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CCpuidDlg)
	enum { IDD = IDD_CPUID_DIALOG };
	CListCtrl	m_CpuidList;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCpuidDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

protected:
	int m_count;
	HICON m_hIcon;
	void InsertCpuidInfo(DWORD id,DWORD A,DWORD B,DWORD C,DWORD D);

	//{{AFX_MSG(CCpuidDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnCpuidCopy();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CPUIDDLG_H__23C0088D_E6FB_42B6_9B53_BF543FCAF1A0__INCLUDED_)
