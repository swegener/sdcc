# Microsoft Developer Studio Project File - Name="pic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=pic - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "pic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "pic.mak" CFG="pic - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "pic - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE "pic - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "pic - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "." /I "..\.." /I "..\..\support\util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /Zm500 /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I ".." /I "." /I "..\.." /I "..\..\support\util" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /FD /GZ /Zm1000 /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Debug\port.lib"
# ADD LIB32 /nologo /out:"Debug\port.lib"

!ELSEIF  "$(CFG)" == "pic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /ML /W3 /GX /O2 /I ".." /I "." /I "..\.." /I "..\..\support\util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /Zm500 /c
# ADD CPP /nologo /ML /W3 /GX /O2 /I ".." /I "." /I "..\.." /I "..\..\support\util" /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /Zm1000 /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo /out:"Release\port.lib"
# ADD LIB32 /nologo /out:"Release\port.lib"

!ENDIF 

# Begin Target

# Name "pic - Win32 Debug"
# Name "pic - Win32 Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\device.c
# End Source File
# Begin Source File

SOURCE=.\gen.c
# End Source File
# Begin Source File

SOURCE=.\genarith.c
# End Source File
# Begin Source File

SOURCE=.\glue.c
# End Source File
# Begin Source File

SOURCE=.\main.c
# End Source File
# Begin Source File

SOURCE=.\pcode.c
# End Source File
# Begin Source File

SOURCE=.\pcodeflow.c
# End Source File
# Begin Source File

SOURCE=.\pcodepeep.c
# End Source File
# Begin Source File

SOURCE=.\pcoderegs.c
# End Source File
# Begin Source File

SOURCE=.\ralloc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\device.h
# End Source File
# Begin Source File

SOURCE=.\gen.h
# End Source File
# Begin Source File

SOURCE=.\glue.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\pcode.h
# End Source File
# Begin Source File

SOURCE=.\pcodeflow.h
# End Source File
# Begin Source File

SOURCE=.\pcoderegs.h
# End Source File
# Begin Source File

SOURCE=.\ralloc.h
# End Source File
# End Group
# End Target
# End Project
