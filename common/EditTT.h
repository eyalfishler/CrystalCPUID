//	Based on CButtonST by Davide Calabro http://www.softechsoftware.it

#ifndef __EDIT_TT_H__
#define __EDIT_TT_H__

/////////////////////////////////////////////////////////////////////////////
// CEditTT

class CEditTT : public CEdit
{
public:
	CEditTT();
	virtual ~CEditTT();

	//{{AFX_VIRTUAL(CEditTT)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

public:
	void SetTooltipText(int nText, BOOL bActivate = TRUE);
	void SetTooltipText(LPCTSTR lpszText, BOOL bActivate = TRUE);
	void ActivateTooltip(BOOL bEnable = TRUE);
	DWORD EnableBalloonTooltip();

	static short GetVersionI()		{return 10;}
	static LPCTSTR GetVersionC()	{return (LPCTSTR)_T("1.0");}

private:
	void InitToolTip();
	CToolTipCtrl	m_ToolTip;			// Tooltip
	DWORD			m_dwToolTipStyle;	// Style of tooltip control

	DECLARE_MESSAGE_MAP()
};

#endif // __EDIT_TT_H__
