/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                Copyright 2002- hiyohiyo All rights reserved.
/*---------------------------------------------------------------------------*/

#if !defined(AFX_STDAFX_H__BA6FA697_BE3C_4C3B_B820_F550FE08E37B__INCLUDED_)
#define AFX_STDAFX_H__BA6FA697_BE3C_4C3B_B820_F550FE08E37B__INCLUDED_

// #define _WIN32_WINNT 0x0500

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define _WIN32_WINNT 0x0400
#define _WIN32_DCOM

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。

#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdtctl.h>		// MFC の Internet Explorer 4 コモン コントロール サポート
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "../common/MessageBoxEJ.h"

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__BA6FA697_BE3C_4C3B_B820_F550FE08E37B__INCLUDED_)
