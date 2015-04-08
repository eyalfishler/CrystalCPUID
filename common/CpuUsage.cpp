/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                Copyright 2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#include "stdafx.h"
#include "CpuUsage.h"

static int GetCpuUsageNT(int number);
static int GetCpuUsage9x();
static BOOL FlagNT = FALSE;
static BOOL FlagWinTopVxd = FALSE;
static BOOL IsNT();

typedef LONG (WINAPI *__NTQSI__)(UINT,PVOID,ULONG,PULONG);

static __NTQSI__ NTQSI = NULL;
static HMODULE hModule = NULL;
static DWORD NumberOfProcessors = 1;
static SYSTEM_PROCESSOR_TIMES *spt;

static HKEY hKey, hDummy;
static DWORD dummy, dummysize=sizeof(DWORD);
static DWORD CpuUsage, size=sizeof(DWORD);

static void InitCpuUsageNT();
static int GetCpuUsageNT(int type, CPU_USAGE* usage);
static void CleanupCpuUsageNT();

static int InitCpuUsageWinTopVxd();
static int GetCpuUsageWinTopVxd();

static void InitCpuUsage9x();
static int GetCpuUsage9x();
static void CleanupCpuUsage9x();

void InitCpuUsage()
{
	FlagNT = IsNT();
	if(FlagNT){
		InitCpuUsageNT();
	}else{
		FlagWinTopVxd = InitCpuUsageWinTopVxd();
		if(FlagWinTopVxd){
		}else{
			InitCpuUsage9x();
		}
	}
}

int GetCpuUsage(int type, CPU_USAGE* usage)
{
	if( FlagNT ){
		return GetCpuUsageNT(type, usage);
	}else if(FlagWinTopVxd){
		return GetCpuUsageWinTopVxd();
	}else{
		return GetCpuUsage9x();
	}
}

void CleanupCpuUsage()
{
	if( FlagNT ){
		CleanupCpuUsageNT();
	}else if(FlagWinTopVxd){
	}else{
		CleanupCpuUsage9x();
	}
}

static void InitCpuUsageNT()
{
	spt = NULL;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	NumberOfProcessors = si.dwNumberOfProcessors;
	hModule = GetModuleHandle("ntdll");
	NTQSI = (__NTQSI__)GetProcAddress(
				hModule,
				"NtQuerySystemInformation"
				);

	spt = (SYSTEM_PROCESSOR_TIMES *)GlobalAlloc(GPTR,
		  sizeof(SYSTEM_PROCESSOR_TIMES) * NumberOfProcessors);
}

static int GetCpuUsageNT(int type, CPU_USAGE* usage)
{
	if( NTQSI == NULL){
		return -1;
	}

	SYSTEM_PERFORMANCE_INFORMATION SysPerfInfo;
	SYSTEM_TIME_INFORMATION        SysTimeInfo;
    double                         IdleTime;
    double                         SystemTime;
    LONG                           status;
    static LARGE_INTEGER           OldIdleTime   = {0, 0};
    static LARGE_INTEGER           OldSystemTime = {0, 0};
	static LARGE_INTEGER		   OldIdleTimeUnit[MAX_CPU_SUPPORT];
	double						   IdleTimeUnit[MAX_CPU_SUPPORT];

	DWORD i;

	CPU_USAGE cu;
	int CpuUsage = 0;
	int CpuUsageUnit[MAX_CPU_SUPPORT];

	status = NTQSI( SystemTimeInformation,
					&SysTimeInfo,
					sizeof(SysTimeInfo),
					NULL);
	if (status != NO_ERROR){
		return -1;
	}

	status = NTQSI( SystemPerformanceInformation,
					&SysPerfInfo,
					sizeof(SysPerfInfo),
					NULL);
	if (status != NO_ERROR){
		return -1;
	}

// by Unit //
	status = NTQSI(	SystemProcessorTimes,
					spt,
					sizeof(SYSTEM_PROCESSOR_TIMES) * NumberOfProcessors,
					NULL);
	if (status != NO_ERROR){
		return -1;
	}
	
	if (OldIdleTime.QuadPart != 0)
	{
		IdleTime = (double)(SysPerfInfo.IdleTime.QuadPart - OldIdleTime.QuadPart);
		SystemTime = (double)(SysTimeInfo.SystemTime.QuadPart - OldSystemTime.QuadPart);
		IdleTime = IdleTime / SystemTime;
		CpuUsage = (int)(100.0 - IdleTime * 100.0 / NumberOfProcessors + 0.5);
		if(CpuUsage >= 100){CpuUsage = 100;}

		for(i = 0; i < NumberOfProcessors; i++){
			IdleTimeUnit[i] = (double)(spt[i].IdleTime.QuadPart - OldIdleTimeUnit[i].QuadPart);
			IdleTimeUnit[i] = IdleTimeUnit[i] / SystemTime;
			CpuUsageUnit[i] = (int)(100.0 - IdleTimeUnit[i] * 100.0 + 0.5);
			if(CpuUsageUnit[i] >= 100){CpuUsageUnit[i] = 100;}
		}
	}
	
	OldIdleTime = SysPerfInfo.IdleTime;
	OldSystemTime = SysTimeInfo.SystemTime;
	
	for(i = 0; i < NumberOfProcessors; i++){
		OldIdleTimeUnit[i].QuadPart = spt[i].IdleTime.QuadPart;
	}

	cu.Max = 0;
	cu.Average = CpuUsage;
	for(i = 0; i < NumberOfProcessors; i++){
		cu.Unit[i] = CpuUsageUnit[i];
		if(CpuUsageUnit[i] > cu.Max){
			cu.Max = CpuUsageUnit[i];
		}
	}

	// Fill CpuUsage;
	if(usage != NULL){
		usage->Average = cu.Average;
		usage->Max = cu.Max;
		for(i = 0; i < NumberOfProcessors; i++){
			usage->Unit[i] = cu.Unit[i];
		}
	}

	if(type == CPU_USAGE_MAX){
		return cu.Max;
	}else if(type == CPU_USAGE_AVERAGE){
		return cu.Average;
	}else if(type < (int)NumberOfProcessors){
		return cu.Unit[type];
	}else{
		return CpuUsage;
	}
}

static void CleanupCpuUsageNT()
{
	if(spt != NULL){
		GlobalFree(spt);
		spt = NULL;
	}
}

static void InitCpuUsage9x()
{
	RegOpenKey (HKEY_DYN_DATA, "PerfStats\\StatData", &hKey);

	RegOpenKey (HKEY_DYN_DATA, "PerfStats\\StartStat", &hDummy); 
	RegQueryValueEx (hDummy, "KERNEL\\CPUUsage", NULL, NULL, (LPBYTE)&dummy, &dummysize); 
	RegCloseKey (hDummy);
}

static int GetCpuUsage9x()
{
	RegQueryValueEx(hKey,"KERNEL\\CPUUsage",NULL,NULL,(LPBYTE)&CpuUsage,&size);
	return (int)CpuUsage;
}

static void CleanupCpuUsage9x()
{
	RegOpenKeyA(HKEY_DYN_DATA, "PerfStats\\StopStat", &hDummy); 
	RegQueryValueEx(hDummy, "KERNEL\\CPUUsage", NULL, NULL, (LPBYTE)&dummy, &dummysize); 
	RegCloseKey(hDummy);
	
	RegCloseKey(hKey);
}

static int InitCpuUsageWinTopVxd()
{
	HANDLE hVxd = NULL;
	BOOL result = FALSE;
	DWORD dd;
	DWORD data[0x1000];

	hVxd = CreateFile("\\\\.\\WINTOP.VXD", 0, 0, NULL, 0, GENERIC_ALL, NULL);
	if(hVxd != INVALID_HANDLE_VALUE){
		result = DeviceIoControl(hVxd, 1, 0, 0, data, 0x3FFC, &dd, 0);
		CloseHandle(hVxd);
		if(result != 0){
			return TRUE;
		}
	}
	return FALSE;
}

static int GetCpuUsageWinTopVxd()
{
	static int CpuUsage = 0;
	static DWORD CurrentIdleTime = 0;
	static DWORD CurrentTime = 0;
	static DWORD OldIdleTime = 0;
	static DWORD OldTime = 0;

	HANDLE hVxd;
	DWORD dd;
	DWORD data[0x1000];
	DWORD i;

	hVxd = CreateFile("\\\\.\\WINTOP.VXD", 0, 0, NULL, 0, GENERIC_ALL, NULL);
	DeviceIoControl(hVxd, 1, 0, 0, data, 0x3FFC, &dd, 0);
	CloseHandle(hVxd);

	for(i = 3;i < dd;i += 4){
		if(data[i] & 2){
			break;
		}
	}
	CurrentIdleTime = data[i - 1];
	CurrentTime = GetTickCount();

	if(CurrentTime != OldTime && OldTime != 0){
		CpuUsage = (int)(100.000 - 100.00 * (double)(CurrentIdleTime - OldIdleTime) / (double)(CurrentTime - OldTime) + 0.5);
		if(CpuUsage >= 100){CpuUsage = 100;}
		if(CpuUsage <= 0){CpuUsage = 0;}
	}
	OldTime = CurrentTime;
	OldIdleTime = CurrentIdleTime;

	return CpuUsage;
}


static BOOL IsNT()
{
	OSVERSIONINFO OSVersionInfo;
	OSVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSVersionInfo);

	return OSVersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT;
}
