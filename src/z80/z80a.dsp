# Microsoft Developer Studio Project File - Name="z80a" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=z80a - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE
!MESSAGE NMAKE /f "z80a.mak".
!MESSAGE
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE
!MESSAGE NMAKE /f "z80a.mak" CFG="z80a - Win32 Debug"
!MESSAGE
!MESSAGE Possible choices for configuration are:
!MESSAGE
!MESSAGE "z80a - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe
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
# Begin Target

# Name "z80a - Win32 Debug"

# Begin Source File

SOURCE=".\peeph-gbz80.def"
# Begin Custom Build
InputPath=".\peeph-gbz80.def"

"peeph-gbz80.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph-gbz80.rul

# End Custom Build
# End Source File
# Begin Source File

SOURCE=".\peeph-z80.def"
# Begin Custom Build
InputPath=".\peeph-z80.def"

"peeph-z80.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph-z80.rul

# End Custom Build
# End Source File
# Begin Source File

SOURCE=".\peeph.def"
# Begin Custom Build
InputPath=".\peeph.def"

"peeph.rul" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	echo on
	gawk -f ../SDCCpeeph.awk $(InputPath) >peeph.rul

# End Custom Build
# End Source File
# End Target
# End Project
