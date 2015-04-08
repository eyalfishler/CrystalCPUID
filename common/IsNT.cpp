/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include <windows.h>
#include "IsNT.h"

BOOL IsNT()
{
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);

	return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}