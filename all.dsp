# Microsoft Developer Studio Project File - Name="all" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=all - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "all.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "all.mak" CFG="all - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "all - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "all - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "all - Win32 Release"

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

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

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

# Name "all - Win32 Release"
# Name "all - Win32 Debug"
# Begin Source File

SOURCE=".\bin_vc\as-gbz80.exe"

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\as-gbz80.exe"

"bin\as-gbz80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\as-gbz80.exe bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\as-gbz80.exe"

"bin\as-gbz80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\as-gbz80.exe bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\bin_vc\as-z80.exe"

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\as-z80.exe"

"bin\as-z80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\as-z80.exe bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\as-z80.exe"

"bin\as-z80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\as-z80.exe bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bin_vc\aslink.exe

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\aslink.exe

"bin\aslink.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\aslink.exe bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\aslink.exe

"bin\aslink.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\aslink.exe bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bin_vc\asx8051.exe

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\asx8051.exe

"bin\asx8051.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\asx8051.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\asx8051.exe

"bin\asx8051.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\asx8051.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\bin_vc\link-gbz80.exe"

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\link-gbz80.exe"

".\bin\link-gbz80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\link-gbz80.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\link-gbz80.exe"

".\bin\link-gbz80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\link-gbz80.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=".\bin_vc\link-z80.exe"

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\link-z80.exe"

".\bin\link-z80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\link-z80.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=".\bin_vc\link-z80.exe"

".\bin\link-z80.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\link-z80.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bin_vc\packihx.exe

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\packihx.exe

"bin\packihx.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\packihx.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\packihx.exe

"bin\packihx.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\packihx.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bin_vc\sdcc.exe

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\sdcc.exe

"bin\sdcc.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\sdcc.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\sdcc.exe

"bin\sdcc.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\sdcc.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\bin_vc\sdcpp.exe

!IF  "$(CFG)" == "all - Win32 Release"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\sdcpp.exe

"bin\sdcpp.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\sdcpp.exe  bin /y > nul

# End Custom Build

!ELSEIF  "$(CFG)" == "all - Win32 Debug"

# PROP Ignore_Default_Tool 1
# Begin Custom Build
InputPath=.\bin_vc\sdcpp.exe

".bin\sdcpp.exe" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	copy bin_vc\sdcpp.exe  bin /y > nul

# End Custom Build

!ENDIF 

# End Source File
# End Target
# End Project
