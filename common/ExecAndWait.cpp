/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include <windows.h>
#include "ExecAndWait.h"

DWORD ExecAndWait(char *pszCmd, char* option, BOOL bNoWindow)
{
	DWORD Code;
	BOOL bSuccess;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);

	char tmp[MAX_PATH];
	char path[MAX_PATH];
	char *ptrEnd;
	GetModuleFileName(NULL,tmp, MAX_PATH);
	if ( (ptrEnd = strrchr(tmp, '\\')) != NULL ) {*ptrEnd = '\0';}
	wsprintf(path,"\"%s\\%s\" %s",tmp,pszCmd,option);
//	MessageBox(NULL,path,path,MB_OK);

	if (bNoWindow) {
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
	}

	bSuccess = CreateProcess(
		NULL,	// lpApplicationName
		path,	// lpCommandLine
		NULL,	// lpProcessAttributes
		NULL,	// lpThreadAttributes
		FALSE,	// bInheritHandle
		0,		// dwCreationFlag
		NULL,	// lpEnvironment
		NULL,	// lpCurrentDirectory
		&si,	// lpStartupInfo
		&pi		// lpProcessInformation
	);
	if (bSuccess != TRUE) return 0;

	WaitForInputIdle(pi.hProcess, INFINITE);
	WaitForSingleObject(pi.hProcess, INFINITE);

	GetExitCodeProcess(pi.hProcess, &Code);

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);

//	wsprintf(str,"%s\n%08X",path, Code);
//	DebugInfo(str);
//	MessageBox(NULL,str,str,MB_OK);

	return Code;
}