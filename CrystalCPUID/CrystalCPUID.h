/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2005 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_CRYSTALCPUID_H__307C6BFD_555E_40C5_BAD6_8A914D0FA55C__INCLUDED_)
#define AFX_CRYSTALCPUID_H__307C6BFD_555E_40C5_BAD6_8A914D0FA55C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "../SysInfo/ItemID.h"
#include "../SysInfo/ISysInfo.h"

typedef ISysInfo* (*_CreateSysInfo) (DWORD);
typedef void (*_DestroySysInfo) (ISysInfo*);
typedef ULONG (*_ReadMSR) (ULONG address, ULONG* pEAX,ULONG* pEDX);
typedef ULONG (*_WriteMSR) (ULONG address, ULONG* pEAX,ULONG* pEDX);

class CCrystalCPUIDApp : public CWinApp
{
public:
	CCrystalCPUIDApp();
	
	// SysInfo
	ISysInfo* SysInfo;

	_TCHAR m_path[MAX_PATH];
	_TCHAR m_ini[MAX_PATH];
	_TCHAR m_profile[MAX_PATH];

private:
	HINSTANCE hSysInfoLib;
	BOOL ConnectSysInfo(DWORD Mode);
	BOOL DisconnectSysInfo();

	//{{AFX_VIRTUAL(CCrystalCPUIDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCrystalCPUIDApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_CRYSTALCPUID_H__307C6BFD_555E_40C5_BAD6_8A914D0FA55C__INCLUDED_)
