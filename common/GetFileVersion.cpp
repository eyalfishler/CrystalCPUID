/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "GetFileVersion.h"

#pragma comment(lib,"version.lib")

int GetFileVersion(const char *file,char *version)
{
	ULONG reserved = 0;	
	VS_FIXEDFILEINFO vffi;
	char *buf = NULL;
	int  Locale = 0;
	char str[256];
	str[0] = '\0';

	UINT size = GetFileVersionInfoSize((char*)file,&reserved);
	char *vbuf = new char[size];
	if (GetFileVersionInfo((char*)file, 0, size, vbuf))
	{
		VerQueryValue(vbuf,TEXT("\\"),(void**)&buf,&size);
		CopyMemory( &vffi, buf, sizeof(VS_FIXEDFILEINFO));

		VerQueryValue(vbuf, "\\VarFileInfo\\Translation", (void**)&buf, &size);
		CopyMemory(&Locale, buf, sizeof(int));
		wsprintf(str,
					"\\StringFileInfo\\%04X%04X\\%s", 
					LOWORD(Locale), HIWORD(Locale),
					"FileVersion"
				);
		VerQueryValue(vbuf,str,(void**)&buf,&size);

		strcpy(str,buf);
		if(version != NULL){
			strcpy(version,buf);
		}
	}
	delete [] vbuf;

	if(strcmp(str,"") != 0){
		return int(atof(str) * 100);
	}else{
		return 0;
	}
}
