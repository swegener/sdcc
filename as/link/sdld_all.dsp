# Microsoft Developer Studio Project File - Name="sdld_all" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=sdld_all - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdld_all.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdld_all.mak" CFG="sdld_all - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdld_all - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "sdld_all - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "sdld_all - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "sdld_all - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "sdld_all - Win32 Release"
# Name "sdld_all - Win32 Debug"
# Begin Source File

SOURCE=..\..\bin_vc\sdld.exe

!IF  "$(CFG)" == "sdld_all - Win32 Release"

# Begin Custom Build
InputPath=..\..\bin_vc\sdld.exe

"..\..\bin_vc\sdldz80.exe  ..\..\bin_vc\dldgb.exe ..\..\bin_vc\sdld6808.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\bin_vc\sdldz80.exe 
	copy $(InputPath) ..\..\bin_vc\sdldgb.exe 
	copy $(InputPath) ..\..\bin_vc\sdld6808.exe 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "sdld_all - Win32 Debug"

# Begin Custom Build
InputPath=..\..\bin_vc\sdld.exe

"..\..\bin_vc\sdldz80.exe  ..\..\bin_vc\dldgb.exe ..\..\bin_vc\sdld6808.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy $(InputPath) ..\..\bin_vc\sdldz80.exe 
	copy $(InputPath) ..\..\bin_vc\sdldgb.exe 
	copy $(InputPath) ..\..\bin_vc\sdld6808.exe 
	
# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
