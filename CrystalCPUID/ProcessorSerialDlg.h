/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_PROCESSORSERIAL_H__97AF2BED_50A2_408C_B5F3_41D176738486__INCLUDED_)
#define AFX_PROCESSORSERIAL_H__97AF2BED_50A2_408C_B5F3_41D176738486__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CProcessorSerialDlg Dialog

class CProcessorSerialDlg : public CDialog
{
public:
	CProcessorSerialDlg(CWnd* pParent = NULL);
	BOOL Create();

	//{{AFX_DATA(CProcessorSerialDlg)
	enum { IDD = IDD_PROCESSOR_SERIAL };
	CString	m_ProcessorSerial;
	//}}AFX_DATA

	//{{AFX_VIRTUAL(CProcessorSerialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CProcessorSerialDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_PROCESSORSERIAL_H__97AF2BED_50A2_408C_B5F3_41D176738486__INCLUDED_)
