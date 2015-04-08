//	Based on CButtonST by Davide Calabro http://www.softechsoftware.it

#ifndef __BUTTON_TT_H__
#define __BUTTON_TT_H__

/////////////////////////////////////////////////////////////////////////////
// CButtonTT

class CButtonTT : public CButton
{
public:
	CButtonTT();
	virtual ~CButtonTT();

	//{{AFX_VIRTUAL(CButtonTT)
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

#endif // __BUTTON_TT_H__
