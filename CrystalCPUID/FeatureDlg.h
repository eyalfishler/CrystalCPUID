/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_FEATUREDIALOG_H__088B3A13_CDD5_4CC4_BF6C_557C7D8AE933__INCLUDED_)
#define AFX_FEATUREDIALOG_H__088B3A13_CDD5_4CC4_BF6C_557C7D8AE933__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CFeatureDialog Dialog

class CFeatureDlg : public CDialog
{
public:
	CFeatureDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CFeatureDlg)
	enum { IDD = IDD_FEATURE_DIALOG };
	CString	m_FeatureInitial0;
	CString	m_FeatureInitial1;
	CString	m_FeatureInitial2;
	CString	m_FeatureInitial3;
	CString	m_FeatureInitial4;
	CString	m_FeatureInitial5;
	CString	m_FeatureInitial6;
	CString	m_FeatureInitial7;
	CString	m_FeatureInitial8;
	CString	m_FeatureInitial9;
	CString	m_FeatureInitial10;
	CString	m_FeatureInitial11;
	CString	m_FeatureInitial12;
	CString	m_FeatureInitial13;
	CString	m_FeatureInitial14;
	CString	m_FeatureInitial15;
	CString	m_FeatureInitial16;
	CString	m_FeatureInitial17;
	CString	m_FeatureInitial18;
	CString	m_FeatureInitial19;
	CString	m_FeatureInitial20;
	CString	m_FeatureInitial21;
	CString	m_FeatureInitial22;
	CString	m_FeatureInitial23;
	CString	m_FeatureInitial24;
	CString	m_FeatureInitial25;
	CString	m_FeatureInitial26;
	CString	m_FeatureInitial27;
	CString	m_FeatureInitial28;
	CString	m_FeatureInitial29;
	CString	m_FeatureInitial30;
	CString	m_FeatureInitial31;

	CString	m_FeatureName0;
	CString	m_FeatureName1;
	CString	m_FeatureName2;
	CString	m_FeatureName3;
	CString	m_FeatureName4;
	CString	m_FeatureName5;
	CString	m_FeatureName6;
	CString	m_FeatureName7;
	CString	m_FeatureName8;
	CString	m_FeatureName9;
	CString	m_FeatureName10;
	CString	m_FeatureName11;
	CString	m_FeatureName12;
	CString	m_FeatureName13;
	CString	m_FeatureName14;
	CString	m_FeatureName15;
	CString	m_FeatureName16;
	CString	m_FeatureName17;
	CString	m_FeatureName18;
	CString	m_FeatureName19;
	CString	m_FeatureName20;
	CString	m_FeatureName21;
	CString	m_FeatureName22;
	CString	m_FeatureName23;
	CString	m_FeatureName24;
	CString	m_FeatureName25;
	CString	m_FeatureName26;
	CString	m_FeatureName27;
	CString	m_FeatureName28;
	CString	m_FeatureName29;
	CString	m_FeatureName30;
	CString	m_FeatureName31;

	CString	m_FeatureSupport0;
	CString	m_FeatureSupport1;
	CString	m_FeatureSupport2;
	CString	m_FeatureSupport3;
	CString	m_FeatureSupport4;
	CString	m_FeatureSupport5;
	CString	m_FeatureSupport6;
	CString	m_FeatureSupport7;
	CString	m_FeatureSupport8;
	CString	m_FeatureSupport9;
	CString	m_FeatureSupport10;
	CString	m_FeatureSupport11;
	CString	m_FeatureSupport12;
	CString	m_FeatureSupport13;
	CString	m_FeatureSupport14;
	CString	m_FeatureSupport15;
	CString	m_FeatureSupport16;
	CString	m_FeatureSupport17;
	CString	m_FeatureSupport18;
	CString	m_FeatureSupport19;
	CString	m_FeatureSupport20;
	CString	m_FeatureSupport21;
	CString	m_FeatureSupport22;
	CString	m_FeatureSupport23;
	CString	m_FeatureSupport24;
	CString	m_FeatureSupport25;
	CString	m_FeatureSupport26;
	CString	m_FeatureSupport27;
	CString	m_FeatureSupport28;
	CString	m_FeatureSupport29;
	CString	m_FeatureSupport30;
	CString	m_FeatureSupport31;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CFeatureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

protected:
	DWORD m_Feature;
	DWORD m_FeatureEcx;
	DWORD m_FeatureEx;
	DWORD m_FeatureExEcx;
	DWORD m_FeatureVia;
	DWORD m_FeatureTransmeta;
	DWORD m_FeaturePM;
	char m_VendorName[16];
	
	//{{AFX_MSG(CFeatureDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnFeatureFlagStandard();
	afx_msg void OnFeatureFlagStandardEcx();
	afx_msg void OnFeatureFlagEx();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnFeatureFlagVia();
	afx_msg void OnFeatureFlagTransmeta();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnFeatureFlagExEcx();
	afx_msg void OnFeatureFlagPowerManagement();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_FEATUREDIALOG_H__088B3A13_CDD5_4CC4_BF6C_557C7D8AE933__INCLUDED_)
