# Microsoft Developer Studio Project File - Name="as_gbz80" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=as_gbz80 - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "as_gbz80.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "as_gbz80.mak" CFG="as_gbz80 - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "as_gbz80 - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "as_gbz80 - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "as_gbz80 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug_gbz80"
# PROP BASE Intermediate_Dir "Debug_gbz80"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_gbz80"
# PROP Intermediate_Dir "Debug_gbz80"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FR /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "." /I "../../support/Util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin_vc\as-z80.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /out:"..\..\bin_vc\as-gbz80.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "as_gbz80 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release_gbz80"
# PROP BASE Intermediate_Dir "Release_gbz80"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release_gbz80"
# PROP Intermediate_Dir "Release_gbz80"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FD /c
# ADD CPP /nologo /ML /W3 /GX /O2 /I "." /I "../../support/Util" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "INDEXLIB" /D "MLH_MAP" /D "SDK" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\bin_vc\as-z80.exe" /pdbtype:sept
# ADD LINK32 /nologo /subsystem:console /machine:I386 /out:"..\..\bin_vc\as-gbz80.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "as_gbz80 - Win32 Debug"
# Name "as_gbz80 - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\asdata.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\asexpr.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=..\asxxsrc\aslex.c
# End Source File
# Begin Source File

SOURCE=.\aslist.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\asmain.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\asout.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\assubr.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=..\asxxsrc\assym.c
# End Source File
# Begin Source File

SOURCE=..\..\support\Util\dbuf.c
# End Source File
# Begin Source File

SOURCE=..\..\support\Util\dbuf_string.c
# End Source File
# Begin Source File

SOURCE=..\asxxsrc\strcmpi.c
# End Source File
# Begin Source File

SOURCE=.\z80adr.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\z80ext.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\z80mch.c
# ADD CPP /D "GAMEBOY"
# End Source File
# Begin Source File

SOURCE=.\z80pst.c
# ADD CPP /D "GAMEBOY"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\alloc.h
# End Source File
# Begin Source File

SOURCE=.\asm.h
# End Source File
# Begin Source File

SOURCE=..\..\support\Util\dbuf.h
# End Source File
# Begin Source File

SOURCE=..\..\support\Util\dbuf_string.h
# End Source File
# Begin Source File

SOURCE=.\string.h
# End Source File
# Begin Source File

SOURCE=.\z80.h
# End Source File
# End Group
# End Target
# End Project
