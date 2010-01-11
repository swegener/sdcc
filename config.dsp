# Microsoft Developer Studio Project File - Name="config" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=config - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "config.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "config.mak" CFG="config - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "config - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "config - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "config - Win32 Release"

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

!ELSEIF  "$(CFG)" == "config - Win32 Debug"

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

# Name "config - Win32 Release"
# Name "config - Win32 Debug"
# Begin Source File

SOURCE=.\sdas\linksrc\asxxxx_config.h.in

!IF  "$(CFG)" == "config - Win32 Release"

# Begin Custom Build
InputPath=.\sdas\linksrc\asxxxx_config.h.in

"sdas/linksrc/asxxxx_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f configure_vc.awk sdas/linksrc/asxxxx_config.h.in > sdas/linksrc/asxxxx_config.h

# End Custom Build

!ELSEIF  "$(CFG)" == "config - Win32 Debug"

# Begin Custom Build
InputPath=.\sdas\linksrc\asxxxx_config.h.in

"sdas/linksrc/asxxxx_config.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f configure_vc.awk sdas/linksrc/asxxxx_config.h.in > sdas/linksrc/asxxxx_config.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\configure_vc.awk
# End Source File
# Begin Source File

SOURCE=.\sdcc_vc_in.h

!IF  "$(CFG)" == "config - Win32 Release"

USERDEP__SDCC_=".version"	"configure_vc.awk"	
# Begin Custom Build
InputPath=.\sdcc_vc_in.h

"sdcc_vc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f configure_vc.awk sdcc_vc_in.h > sdcc_vc.h

# End Custom Build

!ELSEIF  "$(CFG)" == "config - Win32 Debug"

USERDEP__SDCC_=".version"	"configure_vc.awk"	
# Begin Custom Build
InputPath=.\sdcc_vc_in.h

"sdcc_vc.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f configure_vc.awk sdcc_vc_in.h > sdcc_vc.h

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\version.awk

!IF  "$(CFG)" == "config - Win32 Release"

# PROP Ignore_Default_Tool 1
USERDEP__VERSI="ChangeLog"	
# Begin Custom Build
InputPath=.\src\version.awk

".\src\version.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f $(InputPath) ChangeLog  > .\src\version.h

# End Custom Build

!ELSEIF  "$(CFG)" == "config - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__VERSI="ChangeLog"	
# Begin Custom Build
InputPath=.\src\version.awk

".\src\version.h" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f $(InputPath) ChangeLog  > .\src\version.h

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
