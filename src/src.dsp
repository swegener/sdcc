# Microsoft Developer Studio Project File - Name="src" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=src - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "src.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "src.mak" CFG="src - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "src - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "src - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "src - Win32 Debug"

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
# ADD BASE CPP /nologo /MLd /W3 /Gm /GX /ZI /Od /I ".." /I "..\support\util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# ADD CPP /nologo /MLd /W3 /Gm /GX /ZI /Od /I ".." /I "..\support\util" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mcs51\Debug\port.lib z80\Debug\port.lib avr\Debug\port.lib ds390\Debug\port.lib pic\Debug\port.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\bin_vc\sdcc.exe" /pdbtype:sept
# ADD LINK32 mcs51\Debug\port.lib z80\Debug\port.lib avr\Debug\port.lib ds390\Debug\port.lib pic\Debug\port.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\bin_vc\sdcc.exe" /pdbtype:sept

!ELSEIF  "$(CFG)" == "src - Win32 Release"

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
# ADD BASE CPP /nologo /ML /W3 /GX /O2 /I ".." /I "..\support\util" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD CPP /nologo /ML /W3 /GX /O2 /I ".." /I "..\support\util" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 mcs51\Release\port.lib z80\Release\port.lib avr\Release\port.lib ds390\Release\port.lib pic\Release\port.lib /nologo /subsystem:console /pdb:none /machine:I386 /out:"..\bin_vc\sdcc.exe" /pdbtype:sept
# ADD LINK32 mcs51\Release\port.lib z80\Release\port.lib avr\Release\port.lib ds390\Release\port.lib pic\Release\port.lib /nologo /subsystem:console /pdb:none /machine:I386 /out:"..\bin_vc\sdcc.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "src - Win32 Debug"
# Name "src - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\asm.c
# End Source File
# Begin Source File

SOURCE=..\support\Util\NewAlloc.c
# End Source File
# Begin Source File

SOURCE=.\SDCC.lex

!IF  "$(CFG)" == "src - Win32 Debug"

USERDEP__SDCC_="sdccy.h"	
# Begin Custom Build
InputPath=.\SDCC.lex

"SDCClex.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex  -osdcclex.c sdcc.lex

# End Custom Build

!ELSEIF  "$(CFG)" == "src - Win32 Release"

USERDEP__SDCC_="sdccy.h"	
# Begin Custom Build
InputPath=.\SDCC.lex

"SDCClex.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex  -osdcclex.c sdcc.lex

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\SDCCast.c
# End Source File
# Begin Source File

SOURCE=.\SDCCBBlock.c
# End Source File
# Begin Source File

SOURCE=.\SDCCbitv.c
# End Source File
# Begin Source File

SOURCE=.\SDCCcflow.c
# End Source File
# Begin Source File

SOURCE=.\SDCCcse.c
# End Source File
# Begin Source File

SOURCE=.\SDCCdflow.c
# End Source File
# Begin Source File

SOURCE=..\support\Util\sdccerr.c
# End Source File
# Begin Source File

SOURCE=.\SDCCglue.c
# End Source File
# Begin Source File

SOURCE=.\SDCChasht.c
# End Source File
# Begin Source File

SOURCE=.\SDCCicode.c
# End Source File
# Begin Source File

SOURCE=.\SDCClabel.c
# End Source File
# Begin Source File

SOURCE=.\SDCClex.c
# End Source File
# Begin Source File

SOURCE=.\SDCCloop.c
# End Source File
# Begin Source File

SOURCE=.\SDCClrange.c
# End Source File
# Begin Source File

SOURCE=.\SDCCmacro.c
# End Source File
# Begin Source File

SOURCE=.\SDCCmain.c
# End Source File
# Begin Source File

SOURCE=.\SDCCmem.c
# End Source File
# Begin Source File

SOURCE=.\SDCCopt.c
# End Source File
# Begin Source File

SOURCE=.\SDCCpeeph.c
# End Source File
# Begin Source File

SOURCE=.\SDCCptropt.c
# End Source File
# Begin Source File

SOURCE=.\SDCCset.c
# End Source File
# Begin Source File

SOURCE=.\SDCCsymt.c
# End Source File
# Begin Source File

SOURCE=.\SDCCutil.c
# End Source File
# Begin Source File

SOURCE=.\SDCCval.c
# End Source File
# Begin Source File

SOURCE=.\sdccy.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\asm.h
# End Source File
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=..\support\Util\newalloc.h
# End Source File
# Begin Source File

SOURCE=.\port.h
# End Source File
# Begin Source File

SOURCE=..\sdcc_vc.h
# End Source File
# Begin Source File

SOURCE=.\SDCCast.h
# End Source File
# Begin Source File

SOURCE=.\SDCCBBlock.h
# End Source File
# Begin Source File

SOURCE=.\SDCCbitv.h
# End Source File
# Begin Source File

SOURCE=.\SDCCcflow.h
# End Source File
# Begin Source File

SOURCE=.\SDCCcse.h
# End Source File
# Begin Source File

SOURCE=.\SDCCdflow.h
# End Source File
# Begin Source File

SOURCE=.\SDCCglobl.h
# End Source File
# Begin Source File

SOURCE=.\SDCCglue.h
# End Source File
# Begin Source File

SOURCE=.\SDCChasht.h
# End Source File
# Begin Source File

SOURCE=.\SDCCicode.h
# End Source File
# Begin Source File

SOURCE=.\SDCClabel.h
# End Source File
# Begin Source File

SOURCE=.\SDCCloop.h
# End Source File
# Begin Source File

SOURCE=.\SDCClrange.h
# End Source File
# Begin Source File

SOURCE=.\SDCCmem.h
# End Source File
# Begin Source File

SOURCE=.\SDCCopt.h
# End Source File
# Begin Source File

SOURCE=.\SDCCpeeph.h
# End Source File
# Begin Source File

SOURCE=.\SDCCptropt.h
# End Source File
# Begin Source File

SOURCE=.\SDCCset.h
# End Source File
# Begin Source File

SOURCE=.\SDCCsymt.h
# End Source File
# Begin Source File

SOURCE=.\SDCCval.h
# End Source File
# Begin Source File

SOURCE=.\sdccy.h
# End Source File
# Begin Source File

SOURCE=.\spawn.h
# End Source File
# End Group
# Begin Group "Regression Test"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\regression\add.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\arrays.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\b.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\bool1.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\call1.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\compare.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\compare2.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\for.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\pointer1.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\struct1.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\sub.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\TempTest.c
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=.\regression\while.c
# PROP Exclude_From_Build 1
# End Source File
# End Group
# End Target
# End Project
