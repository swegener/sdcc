# Microsoft Developer Studio Project File - Name="z80" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=z80 - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "z80.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "z80.mak" CFG="z80 - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "z80 - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "z80 - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "z80 - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /G3 /Zp1 /W3 /GX /O2 /I ".." /I "." /I "..\.." /I "..\..\support" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /J /FD /Zm500 /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"port.lib"

!ELSEIF  "$(CFG)" == "z80 - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /G3 /Zp1 /ML /W3 /Gm /GX /ZI /Od /I ".." /I "." /I "..\.." /I "..\..\support" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /J /FD /GZ /Zm500 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"port.lib"

!ENDIF 

# Begin Target

# Name "z80 - Win32 Release"
# Name "z80 - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\gen.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=".\peeph-z80.def"

!IF  "$(CFG)" == "z80 - Win32 Release"

# Begin Custom Build
InputPath=.\peeph-z80.def

"peeph-z80.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph-z80.rul 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "z80 - Win32 Debug"

# Begin Custom Build
InputPath=.\peeph-z80.def

"peeph-z80.rul peeph.rul peeph-gbz80.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on 
	gawk -f ../SDCCpeeph.awk peeph-z80.def >peeph-z80.rul 
	gawk -f ../SDCCpeeph.awk peeph-gbz80.def >peeph-gbz80.rul 
	gawk -f ../SDCCpeeph.awk peeph.def >peeph.rul 
	
# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ralloc.c
# End Source File
# Begin Source File

SOURCE=.\support.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\gen.h
# End Source File
# Begin Source File

SOURCE=.\ralloc.h
# End Source File
# Begin Source File

SOURCE=.\support.h
# End Source File
# Begin Source File

SOURCE=.\z80.h
# End Source File
# End Group
# End Target
# End Project
