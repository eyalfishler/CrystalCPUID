/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_BIOSINFODLG_H__FB548084_BDDC_4ACD_8870_0D04BEA4EAC0__INCLUDED_)
#define AFX_BIOSINFODLG_H__FB548084_BDDC_4ACD_8870_0D04BEA4EAC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CBiosInfoDlg : public CDialog
{
public:
	CBiosInfoDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CBiosInfoDlg)
	enum { IDD = IDD_BIOS_INFO };
	CStatic	m_GetWmiMessage;
	CButton	m_GetWmi;
	CString	m_BiosCaption;
	CString	m_BiosDate;
	CString	m_BiosManufacturer;
	CString	m_BiosVersion;
	CString	m_MotherManufacturer;
	CString	m_MotherProduct;
	CString	m_MotherVersion;
	CString	m_SystemManufacturer;
	CString	m_SystemModel;
	CString	m_SmBiosCaption;
	CString	m_SmBiosVersion;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CBiosInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CBiosInfoDlg)
	virtual void OnCancel();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnGetWmi();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_BIOSINFODLG_H__FB548084_BDDC_4ACD_8870_0D04BEA4EAC0__INCLUDED_)
