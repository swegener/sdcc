# sdcc.nsi - NSIS installer script for SDCC
#
# Copyright (c) 2003 Borut Razem
#
# This file is part of sdcc.
#
#  This software is provided 'as-is', without any express or implied
#  warranty.  In no event will the authors be held liable for any damages
#  arising from the use of this software.
#
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it
#  freely, subject to the following restrictions:
#
#  1. The origin of this software must not be misrepresented; you must not
#     claim that you wrote the original software. If you use this software
#     in a product, an acknowledgment in the product documentation would be
#     appreciated but is not required.
#  2. Altered source versions must be plainly marked as such, and must not be
#     misrepresented as being the original software.
#  3. This notice may not be removed or altered from any source distribution.
#
#  Borut Razem
#  borut.razem@siol.net

!include "MUI.nsh"

SetCompressor lzma

!define SDCC_ROOT "..\.."

!define FROM_SNAPSHOT

!ifdef FROM_SNAPSHOT
!system "dir ${SDCC_ROOT}\device > NUL" <> 0
!define DEV_ROOT "${SDCC_ROOT}"
!else
!system "dir ${SDCC_ROOT}\device > NUL" = 0
!define DEV_ROOT "${SDCC_ROOT}\device"
!endif

!system "unix2dos ${SDCC_ROOT}\doc\ChangeLog_head.txt" = 0
!system "unix2dos ${SDCC_ROOT}\doc\README.TXT" = 0

InstType "Full (Bin, Doc, Lib, Src)"
InstType "Medium (Bin, Doc, Lib)"
InstType "Compact (Bin, Doc)"

;--------------------------------
;Variables

Var MUI_STARTMENUPAGE_VARIABLE

;--------------------------------
!define MUI_ABORTWARNING
!insertmacro MUI_PAGE_WELCOME
!define MUI_LICENSEPAGE_RADIOBUTTONS
!insertmacro MUI_PAGE_LICENSE "${SDCC_ROOT}\COPYING.TXT"
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "SDCC"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "HKLM"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "NSIS:StartMenuDir"
!define MUI_STARTMENUPAGE_NODISABLE
!insertmacro MUI_PAGE_STARTMENU Application $MUI_STARTMENUPAGE_VARIABLE
!insertmacro MUI_PAGE_COMPONENTS
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH
!insertmacro MUI_UNPAGE_INSTFILES
!insertmacro MUI_LANGUAGE "English"

Name "SDCC"
BrandingText ""
OutFile "setup.exe"
InstallDir "$PROGRAMFILES\SDCC"
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  MessageBox MB_YESNO|MB_ICONQUESTION "This will install $(^Name). Do you wish to continue?" IDYES +2
  Abort
FunctionEnd

Section -Common
  SetOutPath "$INSTDIR"
  File ".\sdcc.ico"
  File "${SDCC_ROOT}\COPYING.TXT"
SectionEnd

Section "SDCC application files"
  SectionIn 1 2 3 RO
  SetOutPath "$INSTDIR\bin"
  File "${SDCC_ROOT}\bin\*.exe"
SectionEnd

Section "SDCC documentation"
  SectionIn 1 2 3
  SetOutPath "$INSTDIR\doc"
  File "${SDCC_ROOT}\doc\ChangeLog_head.txt"
  File "${SDCC_ROOT}\doc\README.TXT"
SectionEnd

Section "SDCC include files"
  SectionIn 1 2
  SetOutPath "$INSTDIR\include\asm\default"
  File "${DEV_ROOT}\include\asm\default\features.h"
  SetOutPath "$INSTDIR\include\asm\gbz80"
  File "${DEV_ROOT}\include\asm\gbz80\features.h"
  SetOutPath "$INSTDIR\include\asm\z80"
  File "${DEV_ROOT}\include\asm\z80\features.h"
  SetOutPath "$INSTDIR\include"
  File "${DEV_ROOT}\include\*.h"
SectionEnd

Section "SDCC DS390 library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\ds390"
  File "${DEV_ROOT}\lib\ds390\*.asm"
  File "${DEV_ROOT}\lib\ds390\*.rel"
  File "${DEV_ROOT}\lib\ds390\*.lib"
SectionEnd

Section "SDCC DS400 library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\ds400"
  File "${DEV_ROOT}\lib\ds400\*.asm"
  File "${DEV_ROOT}\lib\ds400\*.rel"
  File "${DEV_ROOT}\lib\ds400\*.lib"
SectionEnd

Section "SDCC GBZ80 library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\gbz80"
  File "${DEV_ROOT}\lib\gbz80\*.asm"
  File "${DEV_ROOT}\lib\gbz80\*.o"
  File "${DEV_ROOT}\lib\gbz80\*.lib"
SectionEnd

Section "SDCC Z80 library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\z80"
  File "${DEV_ROOT}\lib\z80\*.asm"
  File "${DEV_ROOT}\lib\z80\*.o"
  File "${DEV_ROOT}\lib\z80\*.lib"
SectionEnd

Section "SDCC large model library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\large"
  File "${DEV_ROOT}\lib\large\*.asm"
  File "${DEV_ROOT}\lib\large\*.rel"
  File "${DEV_ROOT}\lib\large\*.lib"
SectionEnd

Section "SDCC small model library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\small"
  File "${DEV_ROOT}\lib\small\*.asm"
  File "${DEV_ROOT}\lib\small\*.rel"
  File "${DEV_ROOT}\lib\small\*.lib"
SectionEnd

Section "SDCC HC08 library"
  SectionIn 1 2
  SetOutPath "$INSTDIR\lib\hc08"
  File "${DEV_ROOT}\lib\hc08\*.asm"
  File "${DEV_ROOT}\lib\hc08\*.rel"
  File "${DEV_ROOT}\lib\hc08\*.lib"
SectionEnd

Section "SDCC library sources"
  SectionIn 1
  SetOutPath "$INSTDIR\lib\src\ds390\examples"
  File "${DEV_ROOT}\lib\src\ds390\examples\MOVED"

  SetOutPath "$INSTDIR\lib\src\ds390"
  File "${DEV_ROOT}\lib\src\ds390\*.c"
  File "${DEV_ROOT}\lib\src\ds390\libds390.lib"
  File "${DEV_ROOT}\lib\src\ds390\Makefile"
  File "${DEV_ROOT}\lib\src\ds390\Makefile.dep"

  SetOutPath "$INSTDIR\lib\src\ds400"
  File "${DEV_ROOT}\lib\src\ds400\*.c"
  File "${DEV_ROOT}\lib\src\ds400\libds400.lib"
  File "${DEV_ROOT}\lib\src\ds400\Makefile"
  File "${DEV_ROOT}\lib\src\ds400\Makefile.dep"

  SetOutPath "$INSTDIR\lib\src\gbz80"
  File "${DEV_ROOT}\lib\src\gbz80\*.c"
  File "${DEV_ROOT}\lib\src\gbz80\*.s"
  File "${DEV_ROOT}\lib\src\gbz80\gbz80.lib"
  File "${DEV_ROOT}\lib\src\gbz80\Makefile"
  File "${DEV_ROOT}\lib\src\gbz80\README"

  SetOutPath "$INSTDIR\lib\src\z80"
  File "${DEV_ROOT}\lib\src\z80\*.c"
  File "${DEV_ROOT}\lib\src\z80\*.s"
  File "${DEV_ROOT}\lib\src\z80\Makefile"
  File "${DEV_ROOT}\lib\src\z80\README"
  File "${DEV_ROOT}\lib\src\z80\z80.lib"

  SetOutPath "$INSTDIR\lib\src\hc08"
  File "${DEV_ROOT}\lib\src\hc08\*.c"
  File "${DEV_ROOT}\lib\src\hc08\Makefile"
  File "${DEV_ROOT}\lib\src\hc08\hc08.lib"

  SetOutPath "$INSTDIR\lib\src"
  File "${DEV_ROOT}\lib\src\*.c"
SectionEnd

Section -Icons
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
  CreateDirectory "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE"
  CreateShortCut "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\SDCC on the Web.lnk" "$INSTDIR\sdcc.url" 
  CreateShortCut "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Uninstall SDCC.lnk" "$INSTDIR\uninstall.exe" 
  CreateShortCut "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Documentation.lnk" "$INSTDIR\doc\README.TXT" "" "$INSTDIR\sdcc.ico" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Change Log.lnk" "$INSTDIR\doc\ChangeLog_head.txt" "" "$INSTDIR\sdcc.ico" "" "" "" ""
  CreateShortCut "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\GPL 2 License.lnk" "$INSTDIR\COPYING.TXT" 
!insertmacro MUI_STARTMENU_WRITE_END
SectionEnd

Section -INI
  WriteIniStr "$INSTDIR\sdcc.url" "InternetShortcut" "URL" "http://sdcc.sourceforge.net/"
SectionEnd

Section -PostInstall
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "DisplayName" "SDCC"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "UninstallString" "$INSTDIR\uninstall.exe"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "Publisher" "sdcc.sourceforge.net"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "URLInfoAbout" "http://sdcc.sourceforge.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "HelpLink" "http://sdcc.sourceforge.net/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "URLUpdateInfo" "http://sdcc.sourceforge.net/"
  WriteUninstaller "$INSTDIR\uninstall.exe"
SectionEnd


#### Uninstaller code ####

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove $(^Name) and all of its components?" IDYES +2
  Abort
FunctionEnd

Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) was successfully removed from your computer."
FunctionEnd

Section Uninstall
  !insertmacro MUI_STARTMENU_GETFOLDER Application $MUI_STARTMENUPAGE_VARIABLE

  Delete "$INSTDIR\sdcc.url"

  DeleteRegValue HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC" "NSIS:StartMenuDir"
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\SDCC"

  Delete "$INSTDIR\uninstall.exe"
  Delete "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\GPL 2 License.lnk"
  Delete "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Change Log.lnk"
  Delete "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Documentation.lnk"
  Delete "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\Uninstall SDCC.lnk"
  Delete "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE\SDCC on the Web.lnk"

  Delete "$INSTDIR\lib\src\hc08\*.c"
  Delete "$INSTDIR\lib\src\hc08\hc08.lib"
  Delete "$INSTDIR\lib\src\hc08\Makefile"

  Delete "$INSTDIR\lib\src\z80\*.c"
  Delete "$INSTDIR\lib\src\z80\*.s"
  Delete "$INSTDIR\lib\src\z80\z80.lib"
  Delete "$INSTDIR\lib\src\z80\README"
  Delete "$INSTDIR\lib\src\z80\Makefile"

  Delete "$INSTDIR\lib\src\gbz80\*.c"
  Delete "$INSTDIR\lib\src\gbz80\*.s"
  Delete "$INSTDIR\lib\src\gbz80\gbz80.lib"
  Delete "$INSTDIR\lib\src\gbz80\README"
  Delete "$INSTDIR\lib\src\gbz80\Makefile"

  Delete "$INSTDIR\lib\src\ds390\*.c"
  Delete "$INSTDIR\lib\src\ds390\libds390.lib"
  Delete "$INSTDIR\lib\src\ds390\Makefile.dep"
  Delete "$INSTDIR\lib\src\ds390\Makefile"
  Delete "$INSTDIR\lib\src\ds390\examples\MOVED"

  Delete "$INSTDIR\lib\src\ds400\*.c"
  Delete "$INSTDIR\lib\src\ds400\libds400.lib"
  Delete "$INSTDIR\lib\src\ds400\Makefile.dep"
  Delete "$INSTDIR\lib\src\ds400\Makefile"

  Delete "$INSTDIR\lib\src\*.c"

  Delete "$INSTDIR\lib\hc08\*.rel"
  Delete "$INSTDIR\lib\hc08\*.asm"
  Delete "$INSTDIR\lib\hc08\*.lib"

  Delete "$INSTDIR\lib\z80\*.o"
  Delete "$INSTDIR\lib\z80\*.asm"
  Delete "$INSTDIR\lib\z80\*.lib"

  Delete "$INSTDIR\lib\small\*.asm"
  Delete "$INSTDIR\lib\small\*.rel"
  Delete "$INSTDIR\lib\small\*.lib"

  Delete "$INSTDIR\lib\large\*.asm"
  Delete "$INSTDIR\lib\large\*.rel"
  Delete "$INSTDIR\lib\large\*.lib"

  Delete "$INSTDIR\lib\gbz80\*.asm"
  Delete "$INSTDIR\lib\gbz80\*.o"
  Delete "$INSTDIR\lib\gbz80\*.lib"

  Delete "$INSTDIR\lib\ds390\*.asm"
  Delete "$INSTDIR\lib\ds390\*.rel"
  Delete "$INSTDIR\lib\ds390\*.lib"

  Delete "$INSTDIR\lib\ds400\*.asm"
  Delete "$INSTDIR\lib\ds400\*.rel"
  Delete "$INSTDIR\lib\ds400\*.lib"

  Delete "$INSTDIR\include\asm\z80\*.h"
  Delete "$INSTDIR\include\asm\gbz80\*.h"
  Delete "$INSTDIR\include\asm\default\*.h"
  Delete "$INSTDIR\include\*.h"

  Delete "$INSTDIR\doc\README.TXT"
  Delete "$INSTDIR\doc\ChangeLog_head.txt"

  Delete "$INSTDIR\bin\*.exe"

  Delete "$INSTDIR\COPYING.TXT"
  Delete "$INSTDIR\sdcc.ico"

  RMDir "$SMPROGRAMS\$MUI_STARTMENUPAGE_VARIABLE"

  RMDir "$INSTDIR\lib\src\z80"
  RMDir "$INSTDIR\lib\src\gbz80"
  RMDir "$INSTDIR\lib\src\ds390\examples"
  RMDir "$INSTDIR\lib\src\ds390"
  RMDir "$INSTDIR\lib\src\ds400"
  RMDir "$INSTDIR\lib\src\hc08"
  RMDir "$INSTDIR\lib\src"

  RMDir "$INSTDIR\lib\z80"
  RMDir "$INSTDIR\lib\small"
  RMDir "$INSTDIR\lib\large"
  RMDir "$INSTDIR\lib\gbz80"
  RMDir "$INSTDIR\lib\ds390"
  RMDir "$INSTDIR\lib\ds400"
  RMDir "$INSTDIR\lib\hc08"
  RMDir "$INSTDIR\lib"

  RMDir "$INSTDIR\include\asm\z80"
  RMDir "$INSTDIR\include\asm\gbz80"
  RMDir "$INSTDIR\include\asm\default"
  RMDir "$INSTDIR\include\asm"
  RMDir "$INSTDIR\include"

  RMDir "$INSTDIR\doc"

  RMDir "$INSTDIR\bin"

  RMDir "$INSTDIR"

  SetAutoClose true
SectionEnd

