/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_REALTIMECLOCKDLG_H__B0DDB207_5000_44A7_8D84_617C82376814__INCLUDED_)
#define AFX_REALTIMECLOCKDLG_H__B0DDB207_5000_44A7_8D84_617C82376814__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MatrixStatic.h"

class CRealTimeClockDlg : public CDialog
{
public:
	CRealTimeClockDlg(CWnd* pParent = NULL);
	BOOL Create();
	CWnd* m_pParent;
	void ChangeIntervalTime(int time);
	void ChangeMethodType(int method);
	void ChangeTimerType(int timer);
	void StartTimer();
	void EndTimer();
	void ChangeFontSize(int type);
	//{{AFX_DATA(CRealTimeClockDlg)
	enum { IDD = IDD_REAL_TIME_CLOCK };
	CMatrixStatic	m_Clock;
	int m_IntervalTime;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CRealTimeClockDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

protected:
	DWORD m_MethodType;	// 0 = No Load, 1 = Load
	DWORD m_TimerType;	// 0 = QPC + MMT, 1 = WT, 2 = MMT, 3 = QPC
	DWORD m_FlagEistCorrect;
	double m_multiplierori;
	double MultiplierOri;
	double SystemClockOri;

	//{{AFX_MSG(CRealTimeClockDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnRtcClock();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_REALTIMECLOCKDLG_H__B0DDB207_5000_44A7_8D84_617C82376814__INCLUDED_)
