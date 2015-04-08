/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                           Copyright 2002-2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "GetOSInfo.h"
#include "GetFileVersion.h"
#include "wbemcli.h"     // WMI interface declarations

#ifndef SM_TABLETPC
	#define SM_TABLETPC		86
#endif

#ifndef SM_MEDIACENTER
	#define SM_MEDIACENTER	87
#endif

// 関数定義
typedef BOOL (WINAPI *PFUNC_GetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);
PFUNC_GetProductInfo pGetProductInfo = NULL;
DWORD dwReturnedProductType = 0;

static BOOL OSNameWmi(CString& OsName);

void FakeCFileDialog(CFileDialog &dlg)
{
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx( &osvi );
	if( osvi.dwMajorVersion >= 5 ){
		dlg.m_ofn.lStructSize = sizeof(OPENFILENAME) + 12;
	}
}

void GetOSInfo(CString& OSName,CString& OSType,CString& OSCSD,CString& OSVersion,CString& OSBuild,CString& OSInfo)
{
	CString cstr;
	char path[MAX_PATH];
	OSVERSIONINFOEX osvi;
	BOOL bOsVersionInfoEx;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	if( !(bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi)) )
	{
		osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
		GetVersionEx( (OSVERSIONINFO *) &osvi);
	}

	switch(osvi.dwPlatformId)
	{
	case VER_PLATFORM_WIN32_WINDOWS:

		switch( osvi.dwMinorVersion ){
		case 0: // Windows 95
			if( LOWORD(osvi.dwBuildNumber) < 1000 ){
				OSName = "Windows 95";	
			}else if( LOWORD(osvi.dwBuildNumber) >= 1214 ){
				OSName = "Windows 95 OSR2.5";
			}else if( LOWORD(osvi.dwBuildNumber) >= 1212 ){
				OSName = "Windows 95 OSR2.1";
			}else if( LOWORD(osvi.dwBuildNumber) == 1111 ){
				OSName = "Windows 95 OSR2";
			}else{
				OSName = "Windows 95";	
			}
			break;
		case 10: // Windows 98
			if( LOWORD(osvi.dwBuildNumber) == 1998 ){
				OSName = "Windows 98";	
			}else if( LOWORD(osvi.dwBuildNumber) >= 2222 ){
				OSName = "Windows 98 SE";
			}else if( LOWORD(osvi.dwBuildNumber) >= 2000 ){
				OSName = "Windows 98 SP1";
			}else{
				OSName = "Windows 98";	
			}
			break;
		case 90:
			OSName = "Windows Me";
			break;
		default:
			OSName = "Windows 9x";
			break;
		}

		OSVersion.Format("%d.%d",osvi.dwMajorVersion,osvi.dwMinorVersion);
		OSBuild.Format("%d",LOWORD(osvi.dwBuildNumber)); 
		break;

	case VER_PLATFORM_WIN32_NT:
		if(osvi.dwMajorVersion == 3){
			OSName.Format("Windows NT3.%d",osvi.dwMinorVersion);
		}else if(osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0){
			OSName = "Windows NT4";
		}else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0){
			OSName = "Windows 2000";
		}else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1){
			OSName = "Windows XP";
		}else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2){
			OSName = "Windows Server 2003";
		}else if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0){
			if(osvi.wProductType != VER_NT_WORKSTATION)
			{
				OSName = _T("Windows Server 2008");
			}
			else
			{
				OSName = _T("Windows Vista");
			}
		}else{
			OSName.Format("Windows NT%d.%d",osvi.dwMajorVersion,osvi.dwMinorVersion);
		}

		if(osvi.dwMajorVersion >= 6){
			// GetProcAddressにより、Kernel32.dll内のGetProductInfoのアドレスを直接取得
			pGetProductInfo = (PFUNC_GetProductInfo)::GetProcAddress(GetModuleHandle(_T("kernel32.dll")), "GetProductInfo");

			if (pGetProductInfo) {
			   // GetProductInfoの呼出し
			   // 引数は、とりあえずVista or Server "Longhorn"(開発中)のv6.0で固定打ち
			   pGetProductInfo(6, 0, 0, 0, &dwReturnedProductType);

			   switch(dwReturnedProductType) {
				  case 0x00000006: //PRODUCT_BUSINESS
						   OSType = (_T("Business"));
						   break;
				  case 0x00000010: //PRODUCT_BUSINESS_N
						   OSType = (_T("[PRODUCT_BUSINESS_N]"));
						   break;
				  case 0x00000012: //PRODUCT_CLUSTER_SERVER
						   OSType = (_T("[PRODUCT_CLUSTER_SERVER]"));
						   break;
				  case 0x00000008: //PRODUCT_DATACENTER_SERVER
						   OSType = (_T("Datacenter Edition (Full installation)"));
						   break;
				  case 0x0000000C: //PRODUCT_DATACENTER_SERVER_CORE
						   OSType = (_T("Datacenter Edition (Server Core installation)"));
						   break;
				  case 0x00000004: //PRODUCT_ENTERPRISE
						   OSType = (_T("Enterprise"));
						   break;
				  case 0x0000001B: //PRODUCT_ENTERPRISE_N
						   OSType = (_T("Enterprise"));
						   break;
				  case 0x0000000A: //PRODUCT_ENTERPRISE_SERVER
						   OSType = (_T("Enterprise Edition (Full installation)"));
						   break;
				  case 0x0000000E: //PRODUCT_ENTERPRISE_SERVER_CORE
						   OSType = (_T("Enterprise Edition (Server Core installation)"));
						   break;
				  case 0x0000000F: //PRODUCT_ENTERPRISE_SERVER_IA64
						   OSType = (_T("Datacenter Enterprise Edition for Itanium-based Systems"));
						   break;
				  case 0x00000002: //PRODUCT_HOME_BASIC
						   OSType = (_T("Home Basic"));
						   break;
				  case 0x00000005: //PRODUCT_HOME_BASIC_N
						   OSType = (_T("Home Basic"));
						   break;
				  case 0x00000003: //PRODUCT_HOME_PREMIUM
						   OSType = (_T("Home Premium"));
						   break;
				  case 0x0000001A: //PRODUCT_HOME_PREMIUM_N
						   OSType = (_T("Home Premium"));
						   break;
				  case 0x00000013: //PRODUCT_HOME_SERVER
						   OSType = (_T("Home Server Edition"));
						   break;
				  case 0x00000018: //PRODUCT_SERVER_FOR_SMALLBUSINESS
						   OSType = (_T("Server for Small Business Edition"));
						   break;
				  case 0x00000009: //PRODUCT_SMALLBUSINESS_SERVER
						   OSType = (_T("Small Business Server"));
						   break;
				  case 0x00000019: //PRODUCT_SMALLBUSINESS_SERVER_PREMIUM
						   OSType = (_T("Small Business Server Premium Edition"));
						   break;
				  case 0x00000007: //PRODUCT_STANDARD_SERVER
						   OSType = (_T("Server Standard Edition (full installation)"));
						   break;
				  case 0x0000000D: //PRODUCT_STANDARD_SERVER_CORE
						   OSType = (_T("Server Standard Edition (core installation)"));
						   break;
				  case 0x0000000B: //PRODUCT_STARTER
						   OSType = (_T("Starter Edition"));
						   break;
				  case 0x00000017: //PRODUCT_STORAGE_ENTERPRISE_SERVER
						   OSType = (_T("Storage Server Enterprise Edition"));
						   break;
				  case 0x00000014: //PRODUCT_STORAGE_EXPRESS_SERVER
						   OSType = (_T("Storage Server Express Edition"));
						   break;
				  case 0x00000015: //PRODUCT_STORAGE_STANDARD_SERVER
						   OSType = (_T("Storage Server Standard Edition"));
						   break;
				  case 0x00000016: //PRODUCT_STORAGE_WORKGROUP_SERVER
						   OSType = (_T("Storage Server Workgroup Edition"));
						   break;
				  case 0x00000001: //PRODUCT_ULTIMATE
						   OSType = (_T("Ultimate Edition"));
						   break;
				  case 0x0000001C: //PRODUCT_ULTIMATE
						   OSType = (_T("Ultimate Edition"));
						   break;
				  case 0x00000011: //PRODUCT_WEB_SERVER
						   OSType = (_T("Web Server Edition"));
						   break;
			   }
			}
		}else if( bOsVersionInfoEx ){
			if( osvi.wProductType == VER_NT_WORKSTATION ){
				if( osvi.wSuiteMask & VER_SUITE_PERSONAL ){
					OSType = "Home";
				}else{
					if(osvi.dwMajorVersion == 5){OSType = "Professional";}
					else if(osvi.dwMajorVersion == 4){OSType = "Workstation";}
				}
			}else if( osvi.wProductType == VER_NT_SERVER ){
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER ){
					OSType = "DataCenter Server";
				}else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE ){
					OSType = "Advanced Server";
				}else{
					OSType = "Server";
				}
			}
			if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 ){
				// Meida Center & Tablet
				if( GetSystemMetrics(SM_MEDIACENTER) ){
					OSType = "Media Center ";
					GetWindowsDirectory(path, MAX_PATH);
					strcat(path,"\\ehome\\ehshell.exe");
					char str[256];
					GetFileVersion(path,str);
					cstr = str;
					cstr.Replace("5.1.","");
					double num = atof(cstr);
					if(num <= 2600.1200){
						OSType += "2002";
					}else if(num <= 2600.2500){
						OSType += "2004";
					}else{
						OSType += "2005";
					}
				}
				if( GetSystemMetrics(SM_TABLETPC) ){
					OSType = "Tablet PC";
				}
			}
		}else{// NT4 SP5
				HKEY hKey;
				char szProductType[80];
				DWORD dwBufLen;
				RegOpenKeyEx( HKEY_LOCAL_MACHINE,
					"SYSTEM\\CurrentControlSet\\Control\\ProductOptions",
					0, KEY_QUERY_VALUE, &hKey );
				RegQueryValueEx( hKey, "ProductType", NULL, NULL,
					(LPBYTE) szProductType, &dwBufLen);
				RegCloseKey( hKey );
				if( lstrcmpi( "WINNT", szProductType) == 0 ){OSType = "Workstation";}
				if( lstrcmpi( "LANMANNT", szProductType) == 0 ){OSType = "Server";}
				if( lstrcmpi( "SERVERNT", szProductType) == 0 ){OSType = "Advanced Server";}
		 }

		// for x86-64
		if( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 ){
			OSNameWmi(cstr);
			if( cstr.Find("XP") != -1 ){
				OSName = "Windows XP";
				OSType = "Professional";
			}
		}

		OSCSD = osvi.szCSDVersion;
		//OSCSD.Replace("Service Pack ","SP");

		OSVersion.Format("%d.%d",osvi.dwMajorVersion,osvi.dwMinorVersion);
		OSBuild.Format("%d",osvi.dwBuildNumber);
				 
		break;
	}
	if( ! OSCSD.IsEmpty() ){
		OSInfo = OSName + " " + OSCSD;
	}else{
		OSInfo = OSName;
	}
}


typedef HRESULT(WINAPI * _CoInitializeSecurity_)(PSECURITY_DESCRIPTOR pVoid,LONG cAuthSvc,SOLE_AUTHENTICATION_SERVICE * asAuthSvc,void * pReserved1,
DWORD dwAuthnLevel,DWORD dwImpLevel,SOLE_AUTHENTICATION_LIST * pAuthList,DWORD dwCapabilities,void * pReserved3);

static BOOL OSNameWmi(CString& OsName)
{
	try{
		HRESULT hRes = S_OK;
		DWORD uReturned = 1;

		IWbemLocator *pIWbemLocator = NULL;
		IWbemServices* pIWbemServices = NULL;
		IEnumWbemClassObject *pEnumCOMDevs = NULL;
		IWbemClassObject  *pCOMDev = NULL;

		CoInitialize( NULL );

		_CoInitializeSecurity_ CoInitializeSecurity = NULL;
		HINSTANCE hDLL = LoadLibrary( "ole32.dll" );
		if(  hDLL != NULL ){
			CoInitializeSecurity = (_CoInitializeSecurity_)GetProcAddress( hDLL, "CoInitializeSecurity" );
		}
		if(CoInitializeSecurity == NULL){
			FreeLibrary(hDLL);
			return FALSE;
		}
		CoInitializeSecurity(NULL, -1, NULL, NULL,
			RPC_C_AUTHN_LEVEL_PKT_PRIVACY,RPC_C_IMP_LEVEL_IMPERSONATE,
			NULL,EOAC_SECURE_REFS,NULL );
		FreeLibrary(hDLL);

		if( CoCreateInstance(CLSID_WbemLocator,NULL,
			CLSCTX_INPROC_SERVER,IID_IWbemLocator,
			(LPVOID *) &pIWbemLocator) != S_OK ){return FALSE;}
		if( pIWbemLocator->ConnectServer(SysAllocString(L"\\\\.\\root\\cimv2"),
			NULL,NULL,0L,0L,NULL,NULL,&pIWbemServices) != S_OK ){return FALSE;}
		pIWbemLocator->Release();

	/////////////////
	// OS
		pIWbemServices->ExecQuery(SysAllocString(L"WQL"),
		SysAllocString(L"select Caption,Version,CSDVersion,InstallDate from Win32_OperatingSystem"),0,NULL,&pEnumCOMDevs);
		if( SUCCEEDED( pEnumCOMDevs->Next(2000,1,&pCOMDev,&uReturned )) && uReturned == 1){
			VARIANT  pVal;VariantClear(&pVal);
			if( pCOMDev->Get(L"Caption",0L,&pVal,NULL,NULL) == WBEM_S_NO_ERROR){
				OsName = pVal.bstrVal;
				OsName.Replace("Microsoft ","");
			}
			pCOMDev->Release();
		}
		if( pEnumCOMDevs ){pEnumCOMDevs->Release();pEnumCOMDevs = NULL;}
		if(pIWbemServices){pIWbemServices->Release();pIWbemServices = NULL;}
	}catch(...){
		return FALSE;
	}
	return TRUE;
}