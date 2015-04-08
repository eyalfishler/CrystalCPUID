#if !defined(AFX_CPUMARK2005DLG_H__45BCA9E8_4BD2_4E16_9EC5_C9E8FF98EBA8__INCLUDED_)
#define AFX_CPUMARK2005DLG_H__45BCA9E8_4BD2_4E16_9EC5_C9E8FF98EBA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CpuMark2005Dlg.h : ヘッダー ファイル
//
#include "CrystalMater.h"

/////////////////////////////////////////////////////////////////////////////
// CCpuMark2005Dlg ダイアログ

class CCpuMark2005Dlg : public CDialog
{
// コンストラクション
public:
	CCpuMark2005Dlg(CWnd* pParent = NULL);   // 標準のコンストラクタ
	BOOL Create();

// ダイアログ データ
	//{{AFX_DATA(CCpuMark2005Dlg)
	enum { IDD = IDD_CPU_MARK_2005 };
	CStatic	m_MarkCpuClock;
	CButton	m_Mark64;
	CButton	m_Mark32;
	CStatic	m_MarkCpuInfo;
	CCrystalMater	m_MarkScore64;
	CCrystalMater	m_MarkScore32;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CCpuMark2005Dlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CCpuMark2005Dlg)
	afx_msg void OnMark32();
	afx_msg void OnMark64();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CPUMARK2005DLG_H__45BCA9E8_4BD2_4E16_9EC5_C9E8FF98EBA8__INCLUDED_)
