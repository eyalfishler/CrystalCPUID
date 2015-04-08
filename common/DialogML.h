#ifndef __DIALOG_ML_H__
#define __DIALOG_ML_H__

/////////////////////////////////////////////////////////////////////////////
// CDialogML

class CDialogML : public CDialog
{
public:
	CDialogML(CWnd* pParent = NULL);
	virtual ~CDialogML();

	BOOL Create();


	//{{AFX_VIRTUAL(CDialogML)
	//}}AFX_VIRTUAL
	DECLARE_MESSAGE_MAP()

protected:

	//{{AFX_MSG(CDialogML)
	afx_msg void OnClose();
	//}}AFX_MSG

};
#endif // __DIALOG_ML_H__
