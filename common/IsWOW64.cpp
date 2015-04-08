/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                Copyright 2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
//#include <windows.h>
#include "IsWOW64.h"

BOOL IsWOW64()
{
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);
	BOOL flag = FALSE;

	/*
	if( OSVersionInfo.dwMajorVersion >= 5 ){
		typedef BOOL (*_IsWow64Process) (HANDLE, PBOOL);
		HINSTANCE hLib = LoadLibrary("kernel32.dll");
		if(hLib != NULL){
			_IsWow64Process pIsWow64Process = (_IsWow64Process) GetProcAddress (hLib,"IsWow64Process");
			if( pIsWow64Process != NULL ){
				pIsWow64Process(GetCurrentProcess(), &flag);
			}
		}
		FreeLibrary(hLib);
	}
	*/
	return flag;
}