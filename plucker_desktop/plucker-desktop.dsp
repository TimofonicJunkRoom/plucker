# Microsoft Developer Studio Project File - Name="PluckerDesktop" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=PluckerDesktop - Win32 Debug DLL
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "plucker-desktop.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "plucker-desktop.mak" CFG="PluckerDesktop - Win32 Debug DLL"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PluckerDesktop - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PluckerDesktop - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "PluckerDesktop - Win32 Debug DLL" (based on "Win32 (x86) Console Application")
!MESSAGE "PluckerDesktop - Win32 Release DLL" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PluckerDesktop - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "D:\wx242\include" /I "D:\wx242\contrib\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WXMSW__" /D "__WIN95__" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "D:\wx242\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:\wx242\lib\wxmsw.lib D:\wx242\lib\wxxrc.lib D:\wx242\lib\stc.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PluckerDesktop - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "D:\wx242\include" /I "D:\wx242\src\zlib" /I "d:\wx242\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WXMSW__" /D "__WIN95__" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "d:\wx242\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:\wx242\lib\wxmswd.lib D:\wx242\lib\wxxrc.lib D:\wx242\lib\stc.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /pdbtype:sept /FORCE:MULTIPLE
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PluckerDesktop - Win32 Debug DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "PluckerDesktop___Win32_Debug_DLL"
# PROP BASE Intermediate_Dir "PluckerDesktop___Win32_Debug_DLL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugDll"
# PROP Intermediate_Dir "DebugDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "D:\wx242\include" /I "D:\wx242\src\zlib" /I "d:\wx242\contrib\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WXMSW__" /D "__WIN95__" /YX /FD /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /Zi /Od /I "D:\wx242\include" /I "D:\wx242\src\zlib" /I "d:\wx242\contrib\include" /I "D:\wx242\lib\mswdlld" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D WINVER=0x400 /D "_MT" /D wxUSE_GUI=1 /D "__WXDEBUG__" /D "__WINDOWS__" /D "__WXMSW__" /D WXDEBUG=1 /D "WXUSINGDLL" /YX /FD /c
# ADD BASE RSC /l 0x409 /i "d:\wx242\include" /d "_DEBUG"
# ADD RSC /l 0x409 /i "d:\wx242\include" /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:\wx242\lib\wxmswd.lib D:\wx242\lib\wxxrc.lib D:\wx242\lib\stc.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /pdbtype:sept /FORCE:MULTIPLE
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib d:\wx242\lib\wxmsw24d.lib d:\wx242\lib\wxxrcd.lib d:\wx242\lib\stcdlld.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libcd.lib" /nodefaultlib:"libcid.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libc.lib" /pdbtype:sept /FORCE:MULTIPLE
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PluckerDesktop - Win32 Release DLL"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PluckerDesktop___Win32_Release_DLL"
# PROP BASE Intermediate_Dir "PluckerDesktop___Win32_Release_DLL"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "DebugDll"
# PROP Intermediate_Dir "DebugDll"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "D:\wx242\include" /I "D:\wx242\contrib\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WXMSW__" /D "__WIN95__" /FD /c
# SUBTRACT BASE CPP /YX /Yc /Yu
# ADD CPP /nologo /MD /W3 /GX /O2 /I "d:\wx242\include" /I "D:\wx242\contrib\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "__WXMSW__" /D "__WIN95__" /D "WXUSINGDLL" /FD /c
# ADD BASE RSC /l 0x409 /i "D:\wx242\include" /d "NDEBUG"
# ADD RSC /l 0x409 /i "d:\wx242\include" /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:\wx242\lib\wxmsw.lib D:\wx242\lib\wxxrc.lib D:\wx242\lib\stc.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib D:\wx242\lib\wxmsw.lib D:\wx242\lib\wxxrc.lib D:\wx242\lib\stc.lib comctl32.lib rpcrt4.lib wsock32.lib /nologo /subsystem:windows /machine:I386
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "PluckerDesktop - Win32 Release"
# Name "PluckerDesktop - Win32 Debug"
# Name "PluckerDesktop - Win32 Debug DLL"
# Name "PluckerDesktop - Win32 Release DLL"
# Begin Source File

SOURCE=.\about_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\about_dialog.h
# End Source File
# Begin Source File

SOURCE=.\add_channel_wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\add_channel_wizard.h
# End Source File
# Begin Source File

SOURCE=.\resource\icons_application\application.ico
# End Source File
# Begin Source File

SOURCE=.\art_provider.cpp
# End Source File
# Begin Source File

SOURCE=.\art_provider.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\blank.cur
# End Source File
# Begin Source File

SOURCE=.\blocked_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\blocked_dialog.h
# End Source File
# Begin Source File

SOURCE=.\body_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\body_dialog.h
# End Source File
# Begin Source File

SOURCE=.\bookmark_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\bookmark_dialog.h
# End Source File
# Begin Source File

SOURCE=.\build_progress_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\build_progress_dialog.h
# End Source File
# Begin Source File

SOURCE=.\build_progress_wrappers.cpp
# End Source File
# Begin Source File

SOURCE=.\build_progress_wrappers.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\bullseye.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\cdrom.ico
# End Source File
# Begin Source File

SOURCE=.\channel_check_listctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\channel_check_listctrl.h
# End Source File
# Begin Source File

SOURCE=.\channel_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\channel_dialog.h
# End Source File
# Begin Source File

SOURCE=.\channel_list_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\channel_list_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\channel_listctrl_base.cpp
# End Source File
# Begin Source File

SOURCE=.\channel_listctrl_base.h
# End Source File
# Begin Source File

SOURCE=.\channel_type.cpp
# End Source File
# Begin Source File

SOURCE=.\channel_type.h
# End Source File
# Begin Source File

SOURCE=.\checkbox_message_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\checkbox_message_dialog.h
# End Source File
# Begin Source File

SOURCE=.\checkbox_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\checkbox_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\commandline_parser.cpp
# End Source File
# Begin Source File

SOURCE=.\commandline_parser.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\computer.ico
# End Source File
# Begin Source File

SOURCE=.\configuration.cpp
# End Source File
# Begin Source File

SOURCE=.\configuration.h
# End Source File
# Begin Source File

SOURCE=.\destinations_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\destinations_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\details_progress_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\details_progress_dialog.h
# End Source File
# Begin Source File

SOURCE=.\details_progress_listbox.cpp
# End Source File
# Begin Source File

SOURCE=.\details_progress_listbox.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\drive.ico
# End Source File
# Begin Source File

SOURCE=.\editor_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\editor_dialog.h
# End Source File
# Begin Source File

SOURCE=.\email_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\email_dialog.h
# End Source File
# Begin Source File

SOURCE=.\exclusion_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\exclusion_dialog.h
# End Source File
# Begin Source File

SOURCE=.\exclusion_listctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\exclusion_listctrl.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\file1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\floppy.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder1.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\folder2.ico
# End Source File
# Begin Source File

SOURCE=.\font_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\font_dialog.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\hand.cur
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_dialog.h
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_type.cpp
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_type.h
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_wrappers.cpp
# End Source File
# Begin Source File

SOURCE=.\handheld_dest_wrappers.h
# End Source File
# Begin Source File

SOURCE=.\help_controller.cpp
# End Source File
# Begin Source File

SOURCE=.\help_controller.h
# End Source File
# Begin Source File

SOURCE=.\hhictrl.cpp
# End Source File
# Begin Source File

SOURCE=.\hhictrl.h
# End Source File
# Begin Source File

SOURCE=.\hr_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\hr_dialog.h
# End Source File
# Begin Source File

SOURCE=.\html_tip_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\html_tip_dialog.h
# End Source File
# Begin Source File

SOURCE=.\hyperlink_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\hyperlink_dialog.h
# End Source File
# Begin Source File

SOURCE=.\image_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\image_dialog.h
# End Source File
# Begin Source File

SOURCE=.\image_list.cpp
# End Source File
# Begin Source File

SOURCE=.\image_list.h
# End Source File
# Begin Source File

SOURCE=.\import_controller.cpp
# End Source File
# Begin Source File

SOURCE=.\import_controller.h
# End Source File
# Begin Source File

SOURCE=.\install_viewer_wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\install_viewer_wizard.h
# End Source File
# Begin Source File

SOURCE=.\internationalization.cpp
# End Source File
# Begin Source File

SOURCE=.\internationalization.h
# End Source File
# Begin Source File

SOURCE=.\limits_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\limits_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\magnif1.cur
# End Source File
# Begin Source File

SOURCE=.\main_frame.cpp
# End Source File
# Begin Source File

SOURCE=.\main_frame.h
# End Source File
# Begin Source File

SOURCE=.\main_listctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\main_listctrl.h
# End Source File
# Begin Source File

SOURCE=.\message_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\message_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\noentry.cur
# End Source File
# Begin Source File

SOURCE=.\ol_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ol_dialog.h
# End Source File
# Begin Source File

SOURCE=.\os_msw.cpp
# End Source File
# Begin Source File

SOURCE=.\os_msw.h
# End Source File
# Begin Source File

SOURCE=.\palm_installer.h
# End Source File
# Begin Source File

SOURCE=.\palm_installer_base.cpp
# End Source File
# Begin Source File

SOURCE=.\palm_installer_base.h
# End Source File
# Begin Source File

SOURCE=.\palm_installer_gtk.cpp
# End Source File
# Begin Source File

SOURCE=.\palm_installer_gtk.h
# End Source File
# Begin Source File

SOURCE=.\palm_installer_mac.cpp
# End Source File
# Begin Source File

SOURCE=.\palm_installer_mac.h
# End Source File
# Begin Source File

SOURCE=.\palm_installer_msw.cpp
# End Source File
# Begin Source File

SOURCE=.\palm_installer_msw.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pbrush.cur
# End Source File
# Begin Source File

SOURCE=.\pda_listctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\pda_listctrl.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pencil.cur
# End Source File
# Begin Source File

SOURCE=.\piped_process.cpp
# End Source File
# Begin Source File

SOURCE=.\piped_process.h
# End Source File
# Begin Source File

SOURCE=".\plucker-desktop.cpp"
# End Source File
# Begin Source File

SOURCE=".\plucker-desktop.h"
# End Source File
# Begin Source File

SOURCE=".\plucker-desktop.rc"
# End Source File
# Begin Source File

SOURCE=.\plucker_controller.cpp
# End Source File
# Begin Source File

SOURCE=.\plucker_controller.h
# End Source File
# Begin Source File

SOURCE=.\plucker_defines.h
# End Source File
# Begin Source File

SOURCE=.\plucker_wizard_base.cpp
# End Source File
# Begin Source File

SOURCE=.\plucker_wizard_base.h
# End Source File
# Begin Source File

SOURCE=.\plucker_wizard_wrappers.cpp
# End Source File
# Begin Source File

SOURCE=.\plucker_wizard_wrappers.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntleft.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\pntright.cur
# End Source File
# Begin Source File

SOURCE=.\popup_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\popup_dialog.h
# End Source File
# Begin Source File

SOURCE=.\preferences_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\preferences_dialog.h
# End Source File
# Begin Source File

SOURCE=.\process_progress_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\process_progress_dialog.h
# End Source File
# Begin Source File

SOURCE=.\processed_tip_provider.cpp
# End Source File
# Begin Source File

SOURCE=.\processed_tip_provider.h
# End Source File
# Begin Source File

SOURCE=.\proxy_prompt.cpp
# End Source File
# Begin Source File

SOURCE=.\proxy_prompt.h
# End Source File
# Begin Source File

SOURCE=.\proxy_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\proxy_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\query.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\removble.ico
# End Source File
# Begin Source File

SOURCE=.\wx\msw\rightarr.cur
# End Source File
# Begin Source File

SOURCE=.\wx\msw\roller.cur
# End Source File
# Begin Source File

SOURCE=.\setup.h
# End Source File
# Begin Source File

SOURCE=.\setup_wizard.cpp
# End Source File
# Begin Source File

SOURCE=.\setup_wizard.h
# End Source File
# Begin Source File

SOURCE=.\showcase_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\showcase_dialog.h
# End Source File
# Begin Source File

SOURCE=.\showcase_listctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\showcase_listctrl.h
# End Source File
# Begin Source File

SOURCE=.\showcase_splitterwindow.cpp
# End Source File
# Begin Source File

SOURCE=.\showcase_splitterwindow.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\size.cur
# End Source File
# Begin Source File

SOURCE=.\span_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\span_dialog.h
# End Source File
# Begin Source File

SOURCE=.\splashscreen.cpp
# End Source File
# Begin Source File

SOURCE=.\splashscreen.h
# End Source File
# Begin Source File

SOURCE=.\starting_page_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\starting_page_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\startup_tips.cpp
# End Source File
# Begin Source File

SOURCE=.\startup_tips.h
# End Source File
# Begin Source File

SOURCE=.\table_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\table_dialog.h
# End Source File
# Begin Source File

SOURCE=.\td_th_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\td_th_dialog.h
# End Source File
# Begin Source File

SOURCE=.\textctrl_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\textctrl_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\time_control.cpp
# End Source File
# Begin Source File

SOURCE=.\time_control.h
# End Source File
# Begin Source File

SOURCE=.\ul_dialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ul_dialog.h
# End Source File
# Begin Source File

SOURCE=.\utils_clipboard.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_clipboard.h
# End Source File
# Begin Source File

SOURCE=.\utils_controls.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_controls.h
# End Source File
# Begin Source File

SOURCE=.\utils_datetime.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_datetime.h
# End Source File
# Begin Source File

SOURCE=.\utils_execute.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_execute.h
# End Source File
# Begin Source File

SOURCE=.\utils_internet.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_internet.h
# End Source File
# Begin Source File

SOURCE=.\utils_string.cpp
# End Source File
# Begin Source File

SOURCE=.\utils_string.h
# End Source File
# Begin Source File

SOURCE=.\viewer_wizardpage.cpp
# End Source File
# Begin Source File

SOURCE=.\viewer_wizardpage.h
# End Source File
# Begin Source File

SOURCE=.\wx\msw\watch1.cur
# End Source File
# End Target
# End Project
