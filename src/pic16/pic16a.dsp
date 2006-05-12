# Microsoft Developer Studio Project File - Name="pic16a" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=pic16a - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "pic16a.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "pic16a.mak" CFG="pic16a - Win32 Release"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "pic16a - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE "pic16a - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "pic16a - Win32 Debug"

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

!ELSEIF  "$(CFG)" == "pic16a - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""

!ENDIF

# Begin Target

# Name "pic16a - Win32 Debug"
# Name "pic16a - Win32 Release"
# Begin Source File

SOURCE=.\peeph.def

!IF  "$(CFG)" == "pic16a - Win32 Debug"

# Begin Custom Build
InputPath=.\peeph.def

"peeph.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph.rul

# End Custom Build

!ELSEIF  "$(CFG)" == "pic16a - Win32 Release"

# Begin Custom Build
InputPath=.\peeph.def

"peeph.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph.rul

# End Custom Build

!ENDIF

# End Source File
# End Target
# End Project
