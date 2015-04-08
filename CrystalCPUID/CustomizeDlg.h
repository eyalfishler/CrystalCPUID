/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CUSTOMIZEDLG_H__649B37F5_0996_4917_BA61_DFFD7FE3035E__INCLUDED_)
#define AFX_CUSTOMIZEDLG_H__649B37F5_0996_4917_BA61_DFFD7FE3035E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../common/DibSection.h"
#include "MatrixStatic.h"

class CCustomizeDlg : public CDialog
{
public:
	CCustomizeDlg(CWnd* pParent = NULL);
	BOOL Create();

	void SelectLcdColor(COLORREF &color, CString &ctrl, CString section);
	void SetLcdColor(COLORREF &color, CString &ctrl, CString section, DWORD type = 0);
	void SetupLcdSampleColor( COLORREF grid, COLORREF on, COLORREF off );

	CDibSection	Dib;
	//{{AFX_DATA(CCustomizeDlg)
	enum { IDD = IDD_CUSTOMIZE };
	CStatic	m_TtiNormalIcon;
	CStatic	m_TtiMinIcon;
	CStatic	m_TtiMidIcon;
	CStatic	m_TtiMaxIcon;
	CComboBox	m_RtcFontSize;
	CMatrixStatic	m_LcdSample11;
	CComboBox	m_MeasureTime;
	CButton	m_RtcTopMost;
	CMatrixStatic	m_LcdSample9;
	CMatrixStatic	m_LcdSample8;
	CMatrixStatic	m_LcdSample7;
	CMatrixStatic	m_LcdSample6;
	CMatrixStatic	m_LcdSample5;
	CMatrixStatic	m_LcdSample4;
	CMatrixStatic	m_LcdSample3;
	CMatrixStatic	m_LcdSample2;
	CMatrixStatic	m_LcdSample10;
	CMatrixStatic	m_LcdSample1;
	CComboBox	m_IntervalTime;
	CMatrixStatic	m_LcdSample;
	CButton	m_GrayC;
	CButton	m_EnableAlpha;
	CString	m_FileName;
	CString	m_WindowSize;
	CString	m_ImageSize;
	CString	m_TextColor;
	BOOL	m_Gray;
	COLORREF m_LcdOnRgb;
	COLORREF m_LcdOffRgb;
	COLORREF m_LcdGridRgb;
	COLORREF m_LcdOnUserRgb;
	COLORREF m_LcdOffUserRgb;
	COLORREF m_LcdGridUserRgb;
	CString	m_LcdGrid;
	CString	m_LcdOff;
	CString	m_LcdOn;
	CString	m_FontInfoMain;
	CString	m_FontInfoOther;
	BOOL	m_TtiDisable;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CCustomizeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV Support
	//}}AFX_VIRTUAL

protected:
	char m_ini[MAX_PATH];
	RECT m_rect;

	//{{AFX_MSG(CCustomizeDlg)
	afx_msg void OnClose();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnSelectFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectColor();
	afx_msg void OnClear();
	afx_msg void OnGray();
	afx_msg void OnSelectLcdOn();
	afx_msg void OnSelectLcdOff();
	afx_msg void OnSelectLcdGrid();
	afx_msg void OnRtcHelp();
	afx_msg void OnSelchangeIntervalTime();
	afx_msg void OnRtcNoLoad();
	afx_msg void OnRtcWithLoad();
	afx_msg void OnLcdSample1();
	afx_msg void OnLcdSample2();
	afx_msg void OnLcdSample3();
	afx_msg void OnLcdSample4();
	afx_msg void OnLcdSample5();
	afx_msg void OnLcdSample6();
	afx_msg void OnLcdSample7();
	afx_msg void OnLcdSample8();
	afx_msg void OnLcdSample9();
	afx_msg void OnLcdSample10();
	afx_msg void OnRtcTopmost();
	afx_msg void OnEditchangeMeasureTime();
	afx_msg void OnRtcNewType();
	afx_msg void OnRtcNoLoadMmt();
	afx_msg void OnRtcNoLoadQpc();
	afx_msg void OnLcdSample11();
	afx_msg void OnSelectFontMain();
	afx_msg void OnSelectFontOther();
	afx_msg void OnSelchangeRtcDialogFont();
	afx_msg void OnTtiMax();
	afx_msg void OnTtiNormal();
	afx_msg void OnTtiMid();
	afx_msg void OnTtiMin();
	afx_msg void OnTtiDefault();
	afx_msg void OnTtiDisable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CUSTOMIZEDLG_H__649B37F5_0996_4917_BA61_DFFD7FE3035E__INCLUDED_)
