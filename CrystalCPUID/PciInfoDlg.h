/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_PCIINFODLG_H__FEDCBDFE_262B_4223_8785_A83BCF9309B0__INCLUDED_)
#define AFX_PCIINFODLG_H__FEDCBDFE_262B_4223_8785_A83BCF9309B0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CPciInfoDlg Dialog

class CPciInfoDlg : public CDialog
{
public:
	CPciInfoDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CPciInfoDlg)
	enum { IDD = IDD_PCI_INFO };
	CListCtrl	m_PciList;
	CString	m_PciDeviceId;
	CString	m_PciClassId;
	CString	m_PciDeviceName;
	CString	m_PciRevision;
	CString	m_PciBus;
	CString	m_PciClassName;
	CString	m_PciDevice;
	CString	m_PciFunction;
	CString	m_PciSubSystemId;
	CString	m_PciSubSystemName;
	CString	m_PciVendorId;
	CString	m_PciVendorName;
	//}}AFX_DATA


	//{{AFX_VIRTUAL(CPciInfoDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); 
	//}}AFX_VIRTUAL

protected:
	DWORD m_NumberOfItem;
	DWORD m_hotItem;
	HACCEL m_hAccelerator;
	void TextCopy(DWORD type);
	void SetPciDetailInfo(DWORD hotItem);

	//{{AFX_MSG(CPciInfoDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnCopy();
	afx_msg void OnSaveText();
	afx_msg void OnExit();
	afx_msg void OnItemchangedPciList(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_PCIINFODLG_H__FEDCBDFE_262B_4223_8785_A83BCF9309B0__INCLUDED_)
