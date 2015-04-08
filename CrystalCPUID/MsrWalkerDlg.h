#if !defined(AFX_MSRWALKERDLG_H__C502B04E_1877_49D2_AF04_B3D58075CAA8__INCLUDED_)
#define AFX_MSRWALKERDLG_H__C502B04E_1877_49D2_AF04_B3D58075CAA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CMsrWalkerDlg ダイアログ

class CMsrWalkerDlg : public CDialog
{
public:
	CMsrWalkerDlg(CWnd* pParent = NULL);
	BOOL Create();

	HANDLE m_hThread;
	DWORD m_Start;
	DWORD m_End;
	DWORD m_TimeStart;
	BOOL m_MsrUpdateFlag;

	//{{AFX_DATA(CMsrWalkerDlg)
	enum { IDD = IDD_MSR_WALKER };
	CButton	m_MsrUpdateResult;
	CEdit	m_MsrResultC;
	CButton	m_MsrStop;
	CStatic	m_MsrTimeC;
	CEdit	m_MsrEndC;
	CEdit	m_MsrStartC;
	CButton	m_MsrSearch;
	CString	m_MsrStart;
	CString	m_MsrEnd;
	CString	m_MsrTime;
	CString	m_MsrResult;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CMsrWalkerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CMsrWalkerDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnMsrSearch();
	afx_msg void OnMsrStop();
	afx_msg void OnMsrUpdateResult();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_MSRWALKERDLG_H__C502B04E_1877_49D2_AF04_B3D58075CAA8__INCLUDED_)
