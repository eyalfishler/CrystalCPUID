# Microsoft Developer Studio Project File - Name="SysInfo_Sys" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SysInfo_Sys - Win32 Release
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SysInfo_Sys.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SysInfo_Sys.mak" CFG="SysInfo_Sys - Win32 Release"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SysInfo_Sys - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "SysInfo_Sys - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SysInfo_Sys - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /O2 /I "$(DDKROOT)\inc" /FI"$(DDKROOT)\inc\warning.h" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /FR /Yu"stddcls.h" /FD /Zel -cbstring /QIfdiv- /QI0f /GF /Oxs /c
# ADD CPP /Gz /W3 /O1 /Ob2 /I ".\\" /I "..\\" /I "$(DDKROOT)\inc" /I "$(DDKROOT)\inc\ddk" /I "$(DDKROOT)\inc\ddk\wdm" /FI"$(DDKROOT)\inc\warning.h" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _WIN32_WINNT=0x0500 /D WINVER=0x0500 /D _WIN32_IE=0x0501 /D WIN32_LEAN_AND_MEAN=1 /D DBG=0 /D DEVL=1 /D FPO=0 /D "NDEBUG" /D _DLL=1 /YX /Zel -cbstring /QIfdiv- /QI0f /GF /Oxs /c
# SUBTRACT CPP /nologo /Fr
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /D "NDEBUG" /win32
# SUBTRACT MTL /nologo /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib generic.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /machine:I386 /nodefaultlib /out:"Release\PciDebug.sys" /libpath:"$(DDKROOT)\libfre\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4039,4078 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,minimal
# ADD LINK32 ntoskrnl.lib hal.lib wmilib.lib /base:"0x10000" /version:5.0 /entry:"DriverEntry@8" /pdb:none /machine:I386 /nodefaultlib /out:"Release/SysInfo.sys" /libpath:"$(DDKROOT)\libfre\i386" /libpath:"Release" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -OPT:ICF -FORCE:MULTIPLE -RELEASE -FULLBUILD -osversion:5.00 -optidata -driver -align:0x20 -subsystem:native,5.00 -debug:NONE -IGNORE:4039,4078
# Begin Special Build Tool
TargetPath=.\Release\SysInfo.sys
SOURCE="$(InputPath)"
PostBuild_Cmds=copy  $(TargetPath)  ..\..\CrystalExec	REM copy  $(TargetPath)  ..\..\..
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SysInfo_Sys - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /Gz /W3 /Z7 /Oi /Gy /I "$(DDKROOT)\inc" /FI"$(DDKROOT)\inc\warning.h" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _NT1X_=100 /D WINNT=1 /D _WIN32_WINNT=0x0500 /D _WIN32_IE=0x0400 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D _DLL=1 /D "DRIVER" /D "_IDWBUILD" /D "RDRDBG" /D "SRVDBG" /FR /Yu"stddcls.h" /FD /GZ /Zel -cbstring /QIfdiv- /QI0f /GF /QIf /c
# ADD CPP /Gz /W3 /Z7 /Gy /I "C:\DDK\inc\ddk\w2k" /I "C:\DDK\inc\w2k" /I ".\\" /I "..\\" /I "$(DDKROOT)\inc" /I "$(DDKROOT)\inc\ddk" /I "$(DDKROOT)\inc\ddk\wdm" /FI"$(DDKROOT)\inc\warning.h" /D _X86_=1 /D i386=1 /D "STD_CALL" /D CONDITION_HANDLING=1 /D NT_UP=1 /D NT_INST=0 /D WIN32=100 /D _WIN32_WINNT=0x0500 /D WINVER=0x0500 /D _WIN32_IE=0x0501 /D WIN32_LEAN_AND_MEAN=1 /D DBG=1 /D DEVL=1 /D FPO=0 /D "_DEBUG" /D _DLL=1 -D_NT1X_=100-DWINNT=1 /Zel -cbstring /QIfdiv- /QIf /QI0f /GF /Oxs /c
# SUBTRACT CPP /nologo /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /D "_DEBUG" /win32
# SUBTRACT MTL /nologo /mktyplib203
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 wdm.lib generic.lib /nologo /base:"0x10000" /version:4.0 /entry:"DriverEntry@8" /subsystem:windows /pdb:none /debug /debugtype:both /machine:I386 /nodefaultlib /out:"Debug\PciDebug.sys" /libpath:"$(DDKROOT)\libchk\i386" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4039,4078 -osversion:4.00 -optidata -driver -align:0x20 -subsystem:native,4.00 -debug:notmapped,FULL
# ADD LINK32 ntoskrnl.lib hal.lib wmilib.lib /base:"0x10000" /version:5.0 /entry:"DriverEntry@8" /pdb:none /debug /machine:I386 /nodefaultlib /out:"Debug/SysInfo.sys" /libpath:"$(DDKROOT)\libchk\i386" /libpath:"Debug" -MERGE:_PAGE=PAGE -MERGE:_TEXT=.text -MERGE:.rdata=.text -SECTION:INIT,d -OPT:REF -OPT:ICF -FORCE:MULTIPLE -RELEASE -FULLBUILD -IGNORE:4039,4078 -osversion:5.00 -optidata -driver -align:0x20 -subsystem:native,5.00 -debug:FULL
# Begin Special Build Tool
TargetPath=.\Debug\SysInfo.sys
SOURCE="$(InputPath)"
PostBuild_Cmds=copy  $(TargetPath)  ..\..\CrystalExec	REM copy  $(TargetPath)  ..\..\..
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SysInfo_Sys - Win32 Release"
# Name "SysInfo_Sys - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Pcidebug.c
DEP_CPP_PCIDE=\
	"..\Pciioctl.h"\
	".\Pcidebug.h"\
	"C:\DDK\inc\basetsd.h"\
	"C:\DDK\inc\bugcodes.h"\
	"C:\DDK\inc\ddk\w2k\ia64reg.h"\
	"C:\DDK\inc\devioctl.h"\
	"C:\DDK\inc\guiddef.h"\
	"C:\DDK\inc\ntdef.h"\
	"C:\DDK\inc\ntiologc.h"\
	"C:\DDK\inc\ntstatus.h"\
	{$(INCLUDE)}"ntddk.h"\
	
NODEP_CPP_PCIDE=\
	"C:\DDK\inc\ddk\w2k\alpharef.h"\
	
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Pcidebug.h
# End Source File
# Begin Source File

SOURCE=..\pciioctl.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
