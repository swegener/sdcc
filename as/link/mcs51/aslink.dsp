# Microsoft Developer Studio Project File - Name="aslink" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=aslink - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "aslink.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "aslink.mak" CFG="aslink - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "aslink - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "aslink - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "aslink - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "mcs51\Debug"
# PROP BASE Intermediate_Dir "mcs51\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /FR /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin_vc\aslink.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\bin_vc\sdld.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "aslink - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "mcs51\Release"
# PROP BASE Intermediate_Dir "mcs51\Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin_vc\aslink.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\..\bin_vc\sdld.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "aslink - Win32 Debug"
# Name "aslink - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lk_readnl.c
# End Source File
# Begin Source File

SOURCE=..\lkaomf51.c
# End Source File
# Begin Source File

SOURCE=..\lkar.c
# End Source File
# Begin Source File

SOURCE=.\lkarea.c
# End Source File
# Begin Source File

SOURCE=..\lkdata.c
# End Source File
# Begin Source File

SOURCE=..\lkeval.c
# End Source File
# Begin Source File

SOURCE=..\lkhead.c
# End Source File
# Begin Source File

SOURCE=.\lkihx.c
# End Source File
# Begin Source File

SOURCE=..\lklex.c
# End Source File
# Begin Source File

SOURCE=..\lklib.c
# End Source File
# Begin Source File

SOURCE=..\lklibr.c
# End Source File
# Begin Source File

SOURCE=..\lklist.c
# End Source File
# Begin Source File

SOURCE=..\lkmain.c
# End Source File
# Begin Source File

SOURCE=..\lkmem.c
# End Source File
# Begin Source File

SOURCE=..\lknoice.c
# End Source File
# Begin Source File

SOURCE=..\lkrel.c
# End Source File
# Begin Source File

SOURCE=..\lkrloc.c
# End Source File
# Begin Source File

SOURCE=..\lks19.c
# End Source File
# Begin Source File

SOURCE=..\lksdcclib.c
# End Source File
# Begin Source File

SOURCE=..\lksym.c
# End Source File
# Begin Source File

SOURCE=..\sdld.c
# End Source File
# Begin Source File

SOURCE=..\..\asxxsrc\strcmpi.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\aslink.h
# End Source File
# Begin Source File

SOURCE=..\asxxxx_config.h
# End Source File
# Begin Source File

SOURCE=..\getline.h
# End Source File
# Begin Source File

SOURCE=..\lk_readnl.h
# End Source File
# Begin Source File

SOURCE=..\lkar.h
# End Source File
# Begin Source File

SOURCE=..\lklibr.h
# End Source File
# Begin Source File

SOURCE=..\lkrel.h
# End Source File
# Begin Source File

SOURCE=..\sdld.h
# End Source File
# End Group
# End Target
# End Project
