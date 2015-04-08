/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CRYSTALNQUIET_H__8F57B749_513C_4369_926A_D956A688BC75__INCLUDED_)
#define AFX_CRYSTALNQUIET_H__8F57B749_513C_4369_926A_D956A688BC75__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CrystalNQuietDlg.h : Header
//

#include "../common/CpuUsage.h"

/////////////////////////////////////////////////////////////////////////////
// CCrystalNQuietDlg Dialog

class CCrystalNQuietDlg : public CDialog
{
public:
	CCrystalNQuietDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CCrystalNQuietDlg)
	enum { IDD = IDD_CRYSTAL_N_QUIET };
	CComboBox	m_TrigerType;
	CComboBox	m_DualWaitTime;
	CComboBox	m_WaitTime;
	CComboBox	m_ExitMode;
	CComboBox	m_MaxThresholdDown;
	CComboBox	m_MidThresholdUp;
	CComboBox	m_MidThresholdDown;
	CComboBox	m_MinThresholdUp;
	CComboBox	m_MaxIntervalTime;
	CComboBox	m_MidIntervalTime;
	CComboBox	m_MinIntervalTime;
	CButton	m_EnableVoltageC;
	CComboBox	m_MinVID;
	CComboBox	m_MinFID;
	CComboBox	m_MidVID;
	CComboBox	m_MidFID;
	CComboBox	m_MaxVID;
	CComboBox	m_MaxFID;
	BOOL	m_EnableVoltage;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CCrystalNQuietDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CCrystalNQuietDlg)
	afx_msg void OnCqEnableVoltage();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCqApply();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CRYSTALNQUIET_H__8F57B749_513C_4369_926A_D956A688BC75__INCLUDED_)
