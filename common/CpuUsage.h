/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : http://crystalmark.info/
//      License : The modified BSD license
//
//                                Copyright 2004 hiyohiyo, All rights reserved.
/*---------------------------------------------------------------------------*/

#ifndef __CPU_USAGE_H__
#define __CPU_USAGE_H__


#define MAX_CPU_SUPPORT              32

#define CPU_USAGE_AVERAGE			 -1
#define CPU_USAGE_MAX				 -2

#define SystemPerformanceInformation 2
#define SystemTimeInformation        3
#define SystemProcessorTimes         8



typedef struct
{
    LARGE_INTEGER   IdleTime;
    DWORD           Spare[76];
} SYSTEM_PERFORMANCE_INFORMATION;

typedef struct
{
    LARGE_INTEGER BootTime;
    LARGE_INTEGER SystemTime;
    LARGE_INTEGER ExpTimeZoneBias;
    ULONG         CurrentTimeZoneId;
    DWORD         Reserved;
} SYSTEM_TIME_INFORMATION;

typedef struct
{
	LARGE_INTEGER  IdleTime;
	LARGE_INTEGER  KernelTime;
	LARGE_INTEGER  UserTime;
	LARGE_INTEGER  DpcTime;
	LARGE_INTEGER  InterruptTime;
	ULONG    InterruptCount;
}SYSTEM_PROCESSOR_TIMES;

typedef struct
{
	int Average;
	int Max;
	int Unit[MAX_CPU_SUPPORT];
}CPU_USAGE;

void InitCpuUsage();
int GetCpuUsage(int type = CPU_USAGE_AVERAGE, CPU_USAGE* usage = NULL);
void CleanupCpuUsage();
#endif
