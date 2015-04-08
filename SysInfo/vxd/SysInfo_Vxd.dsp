# Microsoft Developer Studio Project File - Name="SysInfo_Vxd" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=SysInfo_Vxd - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "SysInfo_Vxd.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "SysInfo_Vxd.mak" CFG="SysInfo_Vxd - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "SysInfo_Vxd - Win32 Release" ("Win32 (x86) Console Application" 用)
!MESSAGE "SysInfo_Vxd - Win32 Debug" ("Win32 (x86) Console Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SysInfo_Vxd - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /O2 /I ".\\" /I "$(DDKROOT)\inc\win_me" /u /D "IS_32" /D "_WIN32" -Zdp -Gs /c
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# SUBTRACT BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 vxdwraps.clb kernel32.lib /pdb:none /machine:IX86 /nodefaultlib /out:"Release/SysInfo.vxd" /libpath:"$(DDKROOT)\lib\win_me" /vxd /EXETYPE:DYNAMIC /debug:none /IGNORE:4078
# Begin Special Build Tool
IntDir=.\Release
TargetPath=.\Release\SysInfo.vxd
SOURCE="$(InputPath)"
PostBuild_Cmds=$(DDKROOT)\bin\win_me\bin16\rc -r -x -Fo$(IntDir)\Pcidbg95.res Pcidbg95.rc	$(DDKROOT)\bin\win_me\bin\adrc2vxd $(TargetPath) $(IntDir)\Pcidbg95.res	copy  $(TargetPath)  ..\..\CrystalExec
# End Special Build Tool

!ELSEIF  "$(CFG)" == "SysInfo_Vxd - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /I ".\\" /I "$(DDKROOT)\inc\win_me" /D "IS_32" -Zdp -Gs /c
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 vxdwraps.clb kernel32.lib /pdb:none /machine:IX86 /nodefaultlib /def:"SysInfo.def" /out:"Debug/SysInfo.vxd" /libpath:"$(DDKROOT)\lib\win_me" /vxd /debug:none /IGNORE:4078
# SUBTRACT LINK32 /verbose
# Begin Special Build Tool
IntDir=.\Debug
TargetPath=.\Debug\SysInfo.vxd
SOURCE="$(InputPath)"
PostBuild_Cmds=$(DDKROOT)\bin\win_me\bin16\rc -r -x -Fo$(IntDir)\Pcidbg95.res Pcidbg95.rc	$(DDKROOT)\bin\win_me\bin\adrc2vxd $(TargetPath) $(IntDir)\Pcidbg95.res	copy  $(TargetPath)  ..\..\CrystalExec
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "SysInfo_Vxd - Win32 Release"
# Name "SysInfo_Vxd - Win32 Debug"
# Begin Group "Source File"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Ctrl.asm

!IF  "$(CFG)" == "SysInfo_Vxd - Win32 Release"

# Begin Custom Build
IntDir=.\Release
InputPath=.\Ctrl.asm
InputName=Ctrl

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -I$(DDKROOT)\inc\win_me  -Fo$(IntDir)\$(InputName).obj $(InputName).asm

# End Custom Build

!ELSEIF  "$(CFG)" == "SysInfo_Vxd - Win32 Debug"

# Begin Custom Build
IntDir=.\Debug
InputPath=.\Ctrl.asm
InputName=Ctrl

"$(IntDir)\$(InputName).obj" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	ml -coff -DBLD_COFF -DIS_32 -W2 -c -Cx -Zm -DMASM6 -I$(DDKROOT)\inc\win_me  -Fo$(IntDir)\$(InputName).obj $(InputName).asm

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\Pcidbg95.c
DEP_CPP_PCIDB=\
	"..\..\..\..\..\..\..\MSSDK\NTDDK\inc\win_me\basedef.h"\
	"..\..\..\..\..\..\..\MSSDK\NTDDK\inc\win_me\vmm.h"\
	"..\..\..\..\..\..\..\MSSDK\NTDDK\inc\win_me\vpicd.h"\
	"..\..\..\..\..\..\..\MSSDK\NTDDK\inc\win_me\vwin32.h"\
	"..\..\..\..\..\..\..\MSSDK\NTDDK\inc\win_me\vxdwraps.h"\
	"..\pcidef.h"\
	"..\pciioctl.h"\
	".\ntdefine.h"\
	
# End Source File
# Begin Source File

SOURCE=.\SysInfo.def
# End Source File
# End Group
# Begin Group "Resouce File"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\Pcidbg95.rc
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
