#if !defined(AFX_MSREDITDLG_H__47CC4357_2317_4BDE_9285_2C2EAEEC286A__INCLUDED_)
#define AFX_MSREDITDLG_H__47CC4357_2317_4BDE_9285_2C2EAEEC286A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//

/////////////////////////////////////////////////////////////////////////////

class CMsrEditDlg : public CDialog
{
public:
	CMsrEditDlg(CWnd* pParent = NULL); 
	BOOL Create();

	//{{AFX_DATA(CMsrEditDlg)
	enum { IDD = IDD_MSR_EDIT };
	CString	m_AccessMsr;
	CString	m_RdmsrEax;
	CString	m_RdmsrEdx;
	CString	m_WrmsrEax;
	CString	m_WrmsrEdx;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CMsrEditDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CMsrEditDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnRdmsr();
	afx_msg void OnWrmsr();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MSREDITDLG_H__47CC4357_2317_4BDE_9285_2C2EAEEC286A__INCLUDED_)
