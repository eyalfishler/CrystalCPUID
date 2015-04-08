/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_DMIINFODLG_H__B3151432_FDFB_4F72_9326_E10496723380__INCLUDED_)
#define AFX_DMIINFODLG_H__B3151432_FDFB_4F72_9326_E10496723380__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDmiInfoDlg Dialog

class CDmiInfoDlg : public CDialog
{
public:
	CDmiInfoDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CDmiInfoDlg)
	enum { IDD = IDD_DMI_INFO };
	CString	m_DmiBiosReleaseDate;
	CString	m_DmiBiosRomSize;
	CString	m_DmiBiosVendor;
	CString	m_DmiBiosVersion;
	CString	m_DmiCpuCurrentClock;
	CString	m_DmiCpuExternalClock;
	CString	m_DmiCpuManufacturer;
	CString	m_DmiCpuMaxClock;
	CString	m_DmiCpuSocket;
	CString	m_DmiCpuVersion;
	CString	m_DmiVersion;
	CString	m_DmiMotherManufacturer;
	CString	m_DmiMotherProduct;
	CString	m_DmiMotherVersion;
	CString	m_QueryPerformance;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CDmiInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CDmiInfoDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_DMIINFODLG_H__B3151432_FDFB_4F72_9326_E10496723380__INCLUDED_)
