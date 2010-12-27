//----------------------------------------------------------------------------------------
/*!
    \file       setup.h
    This file contains defines, that get included into all files. It is used to turn off
    certain features mostly.
    \modified
    \copyright  (c) Robert O'Connor ( rob@medicalmnemonics.com )
    \licence    GPL
    \brief      Plucker Desktop compilation settings
    \author     Robert O'Connor
    \date       2001/10/20
 */
// RCS-ID:          $Id: setup.h,v 1.48 2004/05/10 00:33:16 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#ifndef _SETUP_H_
#define _SETUP_H_

//----------------------------------------------------------------------------------------
// Restore the regular wxWindows defines for compilers, if not already there
//----------------------------------------------------------------------------------------

// Check this that it still works on Borland BCC, since __VISUALC__ is a test that we
// are using Visual C++

#if defined(_MSC_VER) && !defined(__VISUALC__)
    #define __VISUALC__ _MSC_VER
#endif

//----------------------------------------------------------------------------------------

/*!
    \addtogroup setup_defines
    \{
 */

//----------------------------------------------------------------------------------------
// Linked library inclusion defines
//----------------------------------------------------------------------------------------

//! Toggles inclusion of the Styled Text Control (STC),
/*!
    The Styled Text Control is located in a wxwindows distribution in the directory:
    \e /contrib/src/stc .
    There is also some sanity checks in the 'Sanity Checks' section down below.
 */
#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__) || defined(__WXMSW__) || defined(__WXMAC__)
    #define setupUSE_STYLED_TEXT_CONTROL                 1
#else
    #define setupUSE_STYLED_TEXT_CONTROL                 0
#endif

//----------------------------------------------------------------------------------------
// CPP feature inclusion/removal defines.
// These remove all calls to a certain plucker-desktop feature. To complete the removal,
// open up the makefile and remove its corresponding group of objects from the
// $(OBJECTS) list for example to get fid of internationalization, set
// setupUSE_INTERATIONALIZATION=0 in this setup.h file, then comment out
// $(INTERNATIONALIZATION_OBJECTS) from the makefile's list of OBJECTS=
//----------------------------------------------------------------------------------------

//! Whether to include the code for internationalization (using .po files)
#if ! defined(__GNUWIN32__)
    #define setupUSE_INTERNATIONALIZATION                1
#else
    #define setupUSE_INTERNATIONALIZATION                0
#endif

//! Whether to use a splashscreen.
/*!
    Program also has a configuration setting in preferences_dialog allowing end user
    to turn off displaying them, even if the compilation setting is 1 .
 */
#define setupUSE_SPLASHSCREEN                            1

//! Whether to use startup tips ("Did you know"...).
/*!
    Program also has a configuration setting in preferences_dialog allowing end user
    to turn off displaying them, even if the compilation setting is 1 .
 */
#define setupUSE_STARTUP_TIPS                            1

//! Whether to use a html enabled tip of the day dialog, instead of the default kind.
/*!
    If don't use it, tips will have visible HTML code though, and unreplaced defines
    like plkrAPPLICATION_SUITE_NAME.
 */
#define setupUSE_HTML_TIP_DIALOG                         1

//! Whether to use the online help (an html help controller, currently )
#define setupUSE_ONLINE_HELP                             1

//! Whether or not to use wizards
#define setupUSE_WIZARDS                                 1

//! Whether to use the progress dialog classes (otherwise console windows only)
#define setupUSE_ENHANCED_PROGRESS_DIALOG                1

//! Whether or not to use the integrated HTML editor
#define setupUSE_INTEGRATED_HTML_EDITOR                  1

//! Whether or not to use the integrated exclusion list editor
#define setupUSE_INTEGRATED_EXCLUSION_LIST_EDITOR        1

//! Whether or not to use the Plucker showcase
#define setupUSE_SHOWCASE                                1

//! Whether or not to show a live HTML preview of a channel in the showcase dialog.
/*!
    Requires wxWindows to be compiled with wxUSE_LIBJPEG=1 and wxUSE_GIF=1
    and wxUSE_FS_INET=1.
 */
#define setupUSE_SHOWCASE_PREVIEW_PANE                   0

//! Whether to use message dialogs with a checkbox that say "Don't remind me again".
/*!
    If set to 0, then some message dialogs will just not be shown, such as
    the message about if couldn't find the Palm Desktop software on MSW.
    This allows annoying reminder dialogs can stay hidden.
 */
#define setupUSE_CHECKBOX_MESSAGE_DIALOG                 1

//! Whether to use the art provider (replaces wxWindows icons). Set to 0 if version < 2.3.3
#define setupUSE_ART_PROVIDER                            1

//! Whether to use the enhanced about dialog
#define setupUSE_ENHANCED_ABOUT_DIALOG                   1

//! Whether to use the single instance checker code
#ifdef __WXMSW__
    #define setupUSE_SINGLE_INSTANCE_CHECKER             0
#else
    #define setupUSE_SINGLE_INSTANCE_CHECKER             0
#endif    

//! Whether to use the import controller (1 is recommended)
#define setupUSE_IMPORT_CONTROLLER                       1

//----------------------------------------------------------------------------------------
// Feature defines/mods, but not removing a cpp file object from the output
//----------------------------------------------------------------------------------------

//! Whether to use drag and drop and clipboard or not.
/*!
    Cygwin b20 doesn't have it. Later Cygwin versions probably do.
 */
#if ! defined(__GNUWIN32__)
    #define setupUSE_DRAG_AND_DROP                       1
#else
    #define setupUSE_DRAG_AND_DROP                       0
#endif

//! Whether to set CWD to pluckerhome before executing plucker-build.
/*! Don't know how it can pick up a pluckerdir of the Plucker Desktop cwd even if I
    specify a pluckerdir key in the channel section (env variable don't work either).
    This is required for MSW version to work successfully.
 */
#define setupSET_CWD_TO_PLUCKERHOME_BEFORE_BUILD         1

//! Catches the stdout instead of stderr from plucker-build.
/*!
    This is only set to 1 either testing, or if using a 1.1.13 version or older
    of the parser, as the old parser wrote all of its progress info to stdout not stderr.
 */
#define setupUSE_CATCH_STDOUT_FROM_PLUCKER_BUILD         0

//! Sends channel's output file to destination immediately vs. a group at the end.
/*!
    Often, user wants a set of channels to be updated. If set this switch to 1, then
    the output .pdb from each channel will be sent to its destinations as soon as it is
    created. On Linux, they will all be sent in a batch at the end. Linux needs to have
    the batch at the end to be user friendly, otherwise would have to sit and click the
    handheld's sync button multiple times (because of how pilot-link works). For others,
    they can be sent to destinations immediately, so set to 0.
 */
#if defined(__WXMSW__) || defined(__WXMAC__)
    #define setupSEND_CHANNEL_PDBS_TO_DESTINATIONS_IN_BATCH_AT_END_NOT_IMMEDIATELY  0
#else
    #define setupSEND_CHANNEL_PDBS_TO_DESTINATIONS_IN_BATCH_AT_END_NOT_IMMEDIATELY  1
#endif

//----------------------------------------------------------------------------------------
// OS-specific defines: MSW
//----------------------------------------------------------------------------------------

//! Looks up value of pluckerhome from the registry and saves it to the .ini during startup.
/*!
    Else it tries to use the a GetCwdDir() on startup, then going up a directory.
 */
#if defined(__WXMSW__)
    #define setupUSE_MSW_PLUCKERHOME_REGISTRY_CHECK      1
#else
    #define setupUSE_MSW_PLUCKERHOME_REGISTRY_CHECK      0
#endif

//! Improves process killing on MSW with some extra platform-specific code for killing.
/*!
    Will enable a function that can kill children of that process, and also kill better,
    by killing from within the process itself. MSW has an inadequate system of process
    killing, so this is pretty much required.
    Otherwise will just default to the wxKill() function used on all other systems.
 */
#if defined(__WXMSW__)
    #define setupUSE_MSW_ENHANCED_PROCESS_KILLING        1
#else
    #define setupUSE_MSW_ENHANCED_PROCESS_KILLING        0
#endif

//----------------------------------------------------------------------------------------
// OS-specific defines: OSX
//----------------------------------------------------------------------------------------

#if defined (__WXMAC__)
    #define setupUSE_MAC_PREVENT_SHOWING_STARTUP_TIPS   1
#else
    #define setupUSE_MAC_PREVENT_SHOWING_STARTUP_TIPS   0
#endif     

//----------------------------------------------------------------------------------------
// Hacks
//----------------------------------------------------------------------------------------

//! Whether to use an idle timer for generating idle events on the process progress dialog.
/*!
    Reason for this is that MSW anyways won't generate an idle event after the first one
    unless the mouse is wriggled. This only happens on the release build, not the debug
    build (might be Borland only too). This also happens on Linux/GTK at least Redhat 9 
    and 10 beta.
 */
#if defined(__WXMSW__)
    #define setupUSE_PROCESS_PROGRESS_DIALOG_IDLE_TIMER  1
#else
    #define setupUSE_PROCESS_PROGRESS_DIALOG_IDLE_TIMER  0
#endif

//! A hack around the fact that the wxDateTime::ParseFormat() as of wx2.3.2
/*!
    It won't handle a parse of a W3C formatting datetime string, and thus asserts on
    wxGTK. If put this to 0, it will return to using the wxDateTime::ParseFormat()
    function.
 */
#define setupUSE_DATETIME_MANUAL_PARSE_HACK              1

//! Similar to setupUSE_DATETIME_MANUAL_PARSE_HACK, but for time.
#define setupUSE_TIME_MANUAL_PARSE_HACK                  1

//! A hack to stop wxFileDialog from saving to plucker.ini while wxWindows awaits feature.
#define setupUSE_NO_CONTROL_PERSISTANCE_HACK             1

//! A hack to permit wxFileConfig to properly write last element so can be deleted.
/*!
    IMPORTANT NOTE: This was for wx2.3.3. wx2.4.0 seems to work properly now.
    This hack is found in plucker_controller.cpp, and what it does is
    when deleting a channel, does a quick finish_configuration and init_configuration .
    However, if it is set to 1, this causes an unknown crash, if reproduce this:
    (1) Add some suggested channels using the setup wizard
    (2) Delete them
    (3) Run the setup wizard again (dies somewhere in the message_wizardpage 
        constructor).
    Anyways, bottom line is to keep this around for a few months, in case it
    needs to be switched on, else take it out along with the associated code.
 */
#define setupUSE_FILECONFIG_CAN_DELETE_LAST_ELEMENT_HACK 0

//! A hack to force update UI events from derived listctrls to always be caught by parent
#ifdef __WXMSW__
    #define setupUSE_PARENT_WINDOW_CATCH_UPDATE_UI_EVENT_FROM_DERIVED_LISTCTRL_HACK 1
#else
    #define setupUSE_PARENT_WINDOW_CATCH_UPDATE_UI_EVENT_FROM_DERIVED_LISTCTRL_HACK 0
#endif

//! Hack to replace strings in XRC while we wait for runtime variable expansion in XRC
#define setupUSE_NO_XRC_VARIABLE_EXPANSION_YET_HACK      			   1

//! A Hack to deal with the fact that for some reason the satellite gets inserted (cached BCC headers?)
#define setupUSE_CHANNEL_LISTCTRL_BASE_FORCE_NEW_ITEM_IMAGE_HACK  	   1

//! Whether to force shutdown of commandline mode, MSW, progress dialog, autoclose dialog.
/*!
    Without this, on MSW, commandline mode, MSW, progress dialog, autoclose dialog, will
    never terminate, which will either cause many copies to float around, or will cause
    HotSync to hang if waiting for it to finish.
 */
#ifdef __WXMSW__
    #define setupUSE_MSW_FORCE_COMMANDLINE_AUTOCLOSE_PROGRESS_CLOSURE  1
#else
    #define setupUSE_MSW_FORCE_COMMANDLINE_AUTOCLOSE_PROGRESS_CLOSURE  0
#endif

//! On MSW, as of around wx 2.3.0, can't set en_US anymore. (May be fixed in wx2.5.0+)
#ifdef __WXMSW__
    #define setupUSE_MSW_EN_US_LOCALE_FAILURE_HACK                     1
#else
    #define setupUSE_MSW_EN_US_LOCALE_FAILURE_HACK                     0
#endif

//----------------------------------------------------------------------------------------
// Deprecated
//----------------------------------------------------------------------------------------

//! Deprecated using of 1000000 for always link, as 0 works also.
#define setupUSE_DEPRECATED_ALWAYS_LINK_MAXWIDTH_IS_ALSO_1000000_AS_WELL_AS_0 	1

//----------------------------------------------------------------------------------------

// Close out the mast list of grouping for Doxygen documentation generation:
/*!
    \}
 */

//----------------------------------------------------------------------------------------
// Sanity checks
//----------------------------------------------------------------------------------------

// Old Cygwin (beta20) doesn't support it either (new one does though).
#if defined(__WXMSW__) && defined(__GNUWIN32__) 
    #undef setupUSE_STYLED_TEXT_CONTROL
    #define setupUSE_STYLED_TEXT_CONTROL   0
#endif

// Can't seem to get the STC going yet as a Debug DLL on wxWin 2.4
#if defined(__VISUALC__) 
    #undef setupUSE_STYLED_TEXT_CONTROL
    #define setupUSE_STYLED_TEXT_CONTROL   0
#endif


//----------------------------------------------------------------------------------------
// End single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#endif      //_SETUP_H_
