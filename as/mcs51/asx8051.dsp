# Microsoft Developer Studio Project File - Name="asx8051" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=asx8051 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "asx8051.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "asx8051.mak" CFG="asx8051 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "asx8051 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "asx8051 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "asx8051 - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FR /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin_vc\asx8051.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin_vc\asx8051.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "asx8051 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FD /c
# ADD CPP /nologo /ML /W3 /GX /O2 /I "." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\bin_vc\asx8051.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\bin_vc\asx8051.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "asx8051 - Win32 Debug"
# Name "asx8051 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\asdata.c
# End Source File
# Begin Source File

SOURCE=.\asexpr.c
# End Source File
# Begin Source File

SOURCE=.\aslex.c
# End Source File
# Begin Source File

SOURCE=.\aslist.c
# End Source File
# Begin Source File

SOURCE=.\asmain.c
# End Source File
# Begin Source File

SOURCE=.\asnoice.c
# End Source File
# Begin Source File

SOURCE=.\asout.c
# End Source File
# Begin Source File

SOURCE=.\assubr.c
# End Source File
# Begin Source File

SOURCE=..\asxxsrc\assym.c
# End Source File
# Begin Source File

SOURCE=.\i51adr.c
# End Source File
# Begin Source File

SOURCE=.\i51ext.c
# End Source File
# Begin Source File

SOURCE=.\i51mch.c
# End Source File
# Begin Source File

SOURCE=.\i51pst.c
# End Source File
# Begin Source File

SOURCE=..\asxxsrc\strcmpi.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\asm.h
# End Source File
# Begin Source File

SOURCE=.\i8051.h
# End Source File
# End Group
# End Target
# End Project
