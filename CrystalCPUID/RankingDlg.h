#if !defined(AFX_RANKINGDLG_H__FB43F13A_7FFA_4FA8_9902_519676F4E33D__INCLUDED_)
#define AFX_RANKINGDLG_H__FB43F13A_7FFA_4FA8_9902_519676F4E33D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RankingDlg.h : ヘッダー ファイル
//

#include "#include "CpuInfoID.h"
#include "../CrystalMarkShare/ISysInfo.h"
#include "../CrystalMarkShare/Share.h"

#include "CrystalCPUIDDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CRankingDlg ダイアログ

class CRankingDlg : public CDialog
{
// コンストラクション
public:
	CRankingDlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
// ダイアログ データ
	//{{AFX_DATA(CRankingDlg)
	enum { IDD = IDD_RANKING };
	CEdit	m_CPUNameC;
	CComboBox	m_AreaC;
	CEdit	m_MultiplierC;
	CButton	m_Fix;
	CEdit	m_ClockOriC;
	CEdit	m_SystemClockOriC;
	CEdit	m_MultiplierOriC;
	CString	m_UserName;
	CString	m_SystemClock;
	CString	m_Multiplier;
	CString	m_Mother;
	CString	m_Memory;
	CString	m_Mail;
	CString	m_VIO;
	CString	m_Homepage;
	CString	m_VCore;
	CString	m_Cool;
	CString	m_Comment;
	CString	m_Clock;
	CString	m_ChipSet;
	CString	m_CPUName;
	CString	m_ClockOri;
	CString	m_MultiplierOri;
	CString	m_SystemClockOri;
	CString	m_OverClock;
	CString	m_Area;
	CString	m_Password;
	CString	m_Machine;
	CString	m_OS;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CRankingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	ISysInfo* SysInfo;
	CCrystalCPUIDDlg *Parent;

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CRankingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnFix();
	afx_msg void OnChangeSystemClockOri();
	afx_msg void OnChangeMultiplierOri();
	afx_msg void OnChangeMultiplier();
	afx_msg void OnAfter();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg BOOL OnEntry();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_RANKINGDLG_H__FB43F13A_7FFA_4FA8_9902_519676F4E33D__INCLUDED_)
