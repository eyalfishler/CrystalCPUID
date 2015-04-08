# Microsoft Developer Studio Project File - Name="CrystalCPUID" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CrystalCPUID - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "CrystalCPUID.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "CrystalCPUID.mak" CFG="CrystalCPUID - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "CrystalCPUID - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "CrystalCPUID - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE "CrystalCPUID - Win32 Release64" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CrystalCPUID - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G6 /MT /W3 /GX /Od /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 wbemuuid.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcd.lib" /out:"..\CrystalExec\CrystalCPUID.exe"
# SUBTRACT LINK32 /pdb:none /nodefaultlib

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Debug"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wbemuuid.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libc.lib" /nodefaultlib:"libcmtd.lib" /out:"../build/Debug/CrystalCPUID.exe" /pdbtype:sept /ignore:4098
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Release64"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "CrystalCPUID___Win32_Release64"
# PROP BASE Intermediate_Dir "CrystalCPUID___Win32_Release64"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release64"
# PROP Intermediate_Dir "Release64"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /Ox /Og /Os /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /D "_X86_64" /D "USE_GDI_PLUS" /D "_WIN32" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /Yu"stdafx.h" /FD /EHsc /GS- /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wbemuuid.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"libc.lib" /out:"../CrystalExec/CrystalCPUID.exe"
# SUBTRACT BASE LINK32 /pdb:none /nodefaultlib
# ADD LINK32 cpuid.obj wbemuuid.lib /nologo /subsystem:windows /machine:IX86 /out:"../CrystalExec/CrystalCPUIDx64.exe" /machine:AMD64
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "CrystalCPUID - Win32 Release"
# Name "CrystalCPUID - Win32 Debug"
# Name "CrystalCPUID - Win32 Release64"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AboutDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BiosInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CacheInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChipsetInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CpuidDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalCPUID.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalCPUIDDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalNQuietDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CustomizeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DmiInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DummyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FeatureDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LongHaulDlg.cpp

!IF  "$(CFG)" == "CrystalCPUID - Win32 Release"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Debug"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Release64"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\LoveHammerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsrEditDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MsrWalkerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PciInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProcessorSerialDlg.cpp

!IF  "$(CFG)" == "CrystalCPUID - Win32 Release"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Debug"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Release64"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\RealTimeClockDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SpeedStepDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TransmetaInfoDlg.cpp

!IF  "$(CFG)" == "CrystalCPUID - Win32 Release"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Debug"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Release64"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AboutDlg.h
# End Source File
# Begin Source File

SOURCE=.\BiosInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\CacheInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ChipsetInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\CpuidDlg.h
# End Source File
# Begin Source File

SOURCE=..\SysInfo\CpuInfoID.h
# End Source File
# Begin Source File

SOURCE=.\CrystalCPUID.h
# End Source File
# Begin Source File

SOURCE=.\CrystalCPUIDDlg.h
# End Source File
# Begin Source File

SOURCE=.\CrystalNQuietDlg.h
# End Source File
# Begin Source File

SOURCE=.\CustomizeDlg.h
# End Source File
# Begin Source File

SOURCE=.\DmiInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\DummyDlg.h
# End Source File
# Begin Source File

SOURCE=.\FeatureDlg.h
# End Source File
# Begin Source File

SOURCE=..\SysInfo\ItemID.h
# End Source File
# Begin Source File

SOURCE=.\LcdSetting.h
# End Source File
# Begin Source File

SOURCE=.\LongHaulDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoveHammerDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsrEditDlg.h
# End Source File
# Begin Source File

SOURCE=.\MsrWalkerDlg.h
# End Source File
# Begin Source File

SOURCE=.\PciInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProcessorSerialDlg.h
# End Source File
# Begin Source File

SOURCE=.\RealTimeClockDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SpeedStepDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TransmetaInfoDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\CrystalCPUID.ico
# End Source File
# Begin Source File

SOURCE=.\CrystalCPUID.rc

!IF  "$(CFG)" == "CrystalCPUID - Win32 Release"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Debug"

!ELSEIF  "$(CFG)" == "CrystalCPUID - Win32 Release64"

# ADD BASE RSC /l 0x411
# ADD RSC /l 0x411 /d "_X86_64"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\res\CrystalCPUID.rc2
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsetsmallblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\matrixsettinyblue.bmp
# End Source File
# Begin Source File

SOURCE=.\res\max.ico
# End Source File
# Begin Source File

SOURCE=.\res\mid.ico
# End Source File
# Begin Source File

SOURCE=.\res\min.ico
# End Source File
# Begin Source File

SOURCE=.\res\normal.ico
# End Source File
# Begin Source File

SOURCE=.\XPStyle.xml
# End Source File
# End Group
# Begin Group "common"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\CpuUsage.cpp
# End Source File
# Begin Source File

SOURCE=..\common\CpuUsage.h
# End Source File
# Begin Source File

SOURCE=.\CrystalMater.cpp
# End Source File
# Begin Source File

SOURCE=.\CrystalMater.h
# End Source File
# Begin Source File

SOURCE=..\common\DebugInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\common\DebugInfo.h
# End Source File
# Begin Source File

SOURCE=..\common\DialogTemplateEx.cpp
# End Source File
# Begin Source File

SOURCE=..\common\DialogTemplateEx.h
# End Source File
# Begin Source File

SOURCE=..\common\GetFileVersion.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GetFileVersion.h
# End Source File
# Begin Source File

SOURCE=..\common\GetOsInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\common\GetOSInfo.h
# End Source File
# Begin Source File

SOURCE=..\common\MessageBoxEJ.cpp
# End Source File
# Begin Source File

SOURCE=..\common\MessageBoxEJ.h
# End Source File
# End Group
# Begin Group "MatrixStatic"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\MatrixStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MatrixStatic.h
# End Source File
# End Group
# Begin Group "CDibSection"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\common\DibSection.cpp
# End Source File
# Begin Source File

SOURCE=..\common\DibSection.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\manifest.xml
# End Source File
# End Target
# End Project
