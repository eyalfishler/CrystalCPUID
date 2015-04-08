/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                Copyright 2002- hiyohiyo All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "MessageBoxEx.h"

int MessageBoxEx(int Lang, int EnCode, int JaCode, unsigned int Type)
{
	CString cstr;
	if( Lang == 1 /* JAPANESE */ ){
		cstr.LoadString(JaCode);
	}else{
		cstr.LoadString(EnCode);
	}
	return MessageBox(NULL, cstr, "CrystalCPUID", Type);
}
