# Microsoft Developer Studio Project File - Name="sdcppa" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=sdcppa - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sdcppa.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sdcppa.mak" CFG="sdcppa - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sdcppa - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "sdcppa - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "sdcppa - Win32 Release"

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

!ELSEIF  "$(CFG)" == "sdcppa - Win32 Debug"

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

# Name "sdcppa - Win32 Release"
# Name "sdcppa - Win32 Debug"
# Begin Source File

SOURCE=".\auto-host_vc_in.h"

!IF  "$(CFG)" == "sdcppa - Win32 Release"

# Begin Custom Build
InputPath=.\auto-host_vc_in.h

"auto-host.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy auto-host_vc_in.h auto-host.h > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "sdcppa - Win32 Debug"

# Begin Custom Build
InputPath=.\auto-host_vc_in.h

"auto-host.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy auto-host_vc_in.h auto-host.h > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\options_vc_in.c

!IF  "$(CFG)" == "sdcppa - Win32 Release"

# Begin Custom Build
InputPath=.\options_vc_in.c

"options.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy options_vc_in.c options.c > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "sdcppa - Win32 Debug"

# Begin Custom Build
InputPath=.\options_vc_in.c

"options.c" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy options_vc_in.c options.c > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\options_vc_in.h

!IF  "$(CFG)" == "sdcppa - Win32 Release"

# Begin Custom Build
InputPath=.\options_vc_in.h

"options.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy options_vc_in.h options.h > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "sdcppa - Win32 Debug"

# Begin Custom Build
InputPath=.\options_vc_in.h

"options.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy options_vc_in.h options.h > nul

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
