# sdcc.nsi - NSIS installer script for SDCC
#
# Copyright (c) 2003,2004 Borut Razem
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

# How to create WIN32 setup.exe
#
# - unpack WIN32 mingw daily snapshot sdcc-snapshot-i586-mingw32msvc-yyyymmdd.zip
#   to a clean directory (the option to create directories should be enambled).
#   A sub directory sdcc is created (referenced as PKGDIR in continuation).
# - copy files sdcc/support/scripts/sdcc.ico and sdcc/support/scripts/sdcc.nsi
#   (this file) from the sdcc CVS snapshot to the PKGDIR directory
# - copy file COPYING from the sdcc CVS snapshot to the PKGDIR directory,
#   rename it to COPYING.TXT and convert file COPYING to DOS format:
#   unix2dos COPYING.TXT
# - run NSIS installer from PKGDIR directory:
#   "c:\Program Files\NSIS\makensis.exe" sdcc.nsi
# - A setup file setup.exe is created in PKGDIR directory.
#   Rename it to sdcc_yyyymmdd_setup.exe and upload it
#   to sdcc download repository at sourceforge.net
#
#
# How to upload secc setup.exe tosourceforge.net
#
# Execute following commands from the cmd prompt:
# - sftp sdcc.sourceforge.net
# - cd /home/groups/s/sd/sdcc/htdocs/snapshots/i586-mingw32msvc-setup
# - put sdcc_yyyymmdd_setup.exe
# - quit


!include "MUI.nsh"

SetCompressor lzma

!define SDCC_ROOT "."

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
  ;Uninstall the old version, if present
  ReadRegStr $R0 HKLM \
  "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)" \
  "UninstallString"
  StrCmp $R0 "" inst

  MessageBox MB_OKCANCEL|MB_ICONEXCLAMATION \
  "$(^Name) is already installed. $\n$\nClick 'OK' to remove the \
  previous version or 'Cancel' to cancel this upgrade." \
  IDOK uninst
  Abort

uninst:
  ; Run the uninstaller
  ClearErrors
  ExecWait '$R0 _?=$INSTDIR' ;Do not copy the uninstaller to a temp file

  ;IfErrors no_remove_uninstaller
  ;  ;You can either use Delete /REBOOTOK in the uninstaller or add some code
  ;  ;here to remove to remove the uninstaller. Use a registry key to check
  ;  ;whether the user has chosen to uninstall. If you are using an uninstaller
  ;  ;components page, make sure all sections are uninstalled.
  ;no_remove_uninstaller:

  Goto done
inst:

  ; Install the new version
  MessageBox MB_YESNO|MB_ICONQUESTION "This will install $(^Name). Do you wish to continue?" IDYES +2
  Abort

done:
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

Section -AddToPath
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Do you want to add $INSTDIR\bin to the PATH?" IDNO +3
    Push "$INSTDIR\bin"
    Call AddToPath
SectionEnd


;;;; Uninstaller code ;;;;

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

  Push "$INSTDIR\bin"
  Call un.RemoveFromPath

  SetAutoClose true
SectionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Path Manipulation functions                                                 ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

!verbose 3
!include "WinMessages.NSH"
!verbose 4

; AddToPath - Adds the given dir to the search path.
;        Input - head of the stack
;        Note - Win9x systems requires reboot

Function AddToPath
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4

  ; don't add if the path doesn't exist
  IfFileExists $0 "" AddToPath_done

  Call IsNT
  Pop $4
  StrCmp $4 1 +3
    ; Not on NT: read PATH from environment variable
    ReadEnvStr $1 PATH
    Goto +2
    ; On NT: read PATH from registry
    ReadRegStr $1 HKCU "Environment" "PATH"
  Push "$1;"
  Push "$0;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$0\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  GetFullPathName /SHORT $3 $0
  Push "$1;"
  Push "$3;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done
  Push "$1;"
  Push "$3\;"
  Call StrStr
  Pop $2
  StrCmp $2 "" "" AddToPath_done

  StrCmp $4 1 AddToPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" a
    FileSeek $1 -1 END
    FileReadByte $1 $2
    IntCmp $2 26 0 +2 +2 ; DOS EOF
      FileSeek $1 -1 END ; write over EOF
    FileWrite $1 "$\r$\nSET PATH=%PATH%;$3$\r$\n"
    FileClose $1
    SetRebootFlag true
    Goto AddToPath_done

  AddToPath_NT:
    ReadRegStr $1 HKCU "Environment" "PATH"
    StrCpy $2 $1 1 -1  ; copy last char
    StrCmp $2 ";" 0 +2 ; if last char == ;
      StrCpy $1 $1 -1  ; remove last char
    StrCmp $1 "" AddToPath_NTdoIt
      StrCpy $0 "$1;$0"
    AddToPath_NTdoIt:
      WriteRegExpandStr HKCU "Environment" "PATH" $0
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

  AddToPath_done:
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

; RemoveFromPath - Remove a given dir from the path
;     Input: head of the stack

Function un.RemoveFromPath
  Exch $0
  Push $1
  Push $2
  Push $3
  Push $4
  Push $5
  Push $6

  IntFmt $6 "%c" 26 ; DOS EOF

  Call un.IsNT
  Pop $1
  StrCmp $1 1 unRemoveFromPath_NT
    ; Not on NT
    StrCpy $1 $WINDIR 2
    FileOpen $1 "$1\autoexec.bat" r
    GetTempFileName $4
    FileOpen $2 $4 w
    GetFullPathName /SHORT $0 $0
    StrCpy $0 "SET PATH=%PATH%;$0"
    Goto unRemoveFromPath_dosLoop

    unRemoveFromPath_dosLoop:
      FileRead $1 $3
      StrCpy $5 $3 1 -1 ; read last char
      StrCmp $5 $6 0 +2 ; if DOS EOF
        StrCpy $3 $3 -1 ; remove DOS EOF so we can compare
      StrCmp $3 "$0$\r$\n" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "$0$\n" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "$0" unRemoveFromPath_dosLoopRemoveLine
      StrCmp $3 "" unRemoveFromPath_dosLoopEnd
      FileWrite $2 $3
      Goto unRemoveFromPath_dosLoop
      unRemoveFromPath_dosLoopRemoveLine:
        SetRebootFlag true
        Goto unRemoveFromPath_dosLoop

    unRemoveFromPath_dosLoopEnd:
      FileClose $2
      FileClose $1
      StrCpy $1 $WINDIR 2
      Delete "$1\autoexec.bat"
      CopyFiles /SILENT $4 "$1\autoexec.bat"
      Delete $4
      Goto unRemoveFromPath_done

  unRemoveFromPath_NT:
    ReadRegStr $1 HKCU "Environment" "PATH"
    StrCpy $5 $1 1 -1 ; copy last char
    StrCmp $5 ";" +2  ; if last char != ;
      StrCpy $1 "$1;" ; append ;
    Push $1
    Push "$0;"
    Call un.StrStr    ; Find `$0;` in $1
    Pop $2            ; pos of our dir
    StrCmp $2 "" unRemoveFromPath_done
      ; else, it is in path
      ; $0 - path to add
      ; $1 - path var
      StrLen $3 "$0;"
      StrLen $4 $2
      StrCpy $5 $1 -$4   ; $5 is now the part before the path to remove
      StrCpy $6 $2 "" $3 ; $6 is now the part after the path to remove
      StrCpy $3 $5$6

      StrCpy $5 $3 1 -1  ; copy last char
      StrCmp $5 ";" 0 +2 ; if last char == ;
        StrCpy $3 $3 -1  ; remove last char

      StrCmp $3 "" +3
        ; New PATH not empty: update the registry
        WriteRegExpandStr HKCU "Environment" "PATH" $3
        Goto +2
        ; New PATH empty: remove from the registry
        DeleteRegValue HKCU "Environment" "PATH"
      SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

  unRemoveFromPath_done:
    Pop $6
    Pop $5
    Pop $4
    Pop $3
    Pop $2
    Pop $1
    Pop $0
FunctionEnd

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;            Utility Functions            ;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; IsNT
; no input
; output, top of the stack = 1 if NT or 0 if not
;
; Usage:
;   Call IsNT
;   Pop $R0
;  ($R0 at this point is 1 or 0)

!macro IsNT un
Function ${un}IsNT
  Push $0
  ReadRegStr $0 HKLM "SOFTWARE\Microsoft\Windows NT\CurrentVersion" CurrentVersion
  StrCmp $0 "" 0 IsNT_yes
  ; we are not NT.
  Pop $0
  Push 0
  Return

  IsNT_yes:
    ; NT!!!
    Pop $0
    Push 1
FunctionEnd
!macroend
!insertmacro IsNT ""
!insertmacro IsNT "un."

; StrStr
; input, top of stack = string to search for
;        top of stack-1 = string to search in
; output, top of stack (replaces with the portion of the string remaining)
; modifies no other variables.
;
; Usage:
;   Push "this is a long ass string"
;   Push "ass"
;   Call StrStr
;   Pop $R0
;  ($R0 at this point is "ass string")

!macro StrStr un
Function ${un}StrStr
Exch $R1   ; st=haystack,old$R1, $R1=needle
  Exch     ; st=old$R1,haystack
  Exch $R2 ; st=old$R1,old$R2, $R2=haystack
  Push $R3
  Push $R4
  Push $R5
  StrLen $R3 $R1
  StrCpy $R4 0
  ; $R1=needle
  ; $R2=haystack
  ; $R3=len(needle)
  ; $R4=cnt
  ; $R5=tmp
  loop:
    StrCpy $R5 $R2 $R3 $R4
    StrCmp $R5 $R1 done
    StrCmp $R5 "" done
    IntOp $R4 $R4 + 1
    Goto loop
done:
  StrCpy $R1 $R2 "" $R4
  Pop $R5
  Pop $R4
  Pop $R3
  Pop $R2
  Exch $R1
FunctionEnd
!macroend
!insertmacro StrStr ""
!insertmacro StrStr "un."
