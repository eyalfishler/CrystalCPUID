/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                 Copyright 2004 hiyohiyo All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "DebugInfo.h"

void DebugInfo(char* mes)
{
	static int flag = TRUE;
	static char file[MAX_PATH];

//	MessageBox(NULL,mes,mes,MB_OK);

	if(flag){
		char* ptrEnd;
		::GetModuleFileName(NULL,file, MAX_PATH);
		if ( (ptrEnd = strrchr(file, '.')) != NULL ){
			*ptrEnd = '\0';
			strcat(file, ".log");
		}
		DeleteFile(file);
		flag = FALSE;
	}

	FILE *fp;
	fp = fopen(file,"a");
	fprintf(fp,"%s\n",mes);
	fclose(fp);
}