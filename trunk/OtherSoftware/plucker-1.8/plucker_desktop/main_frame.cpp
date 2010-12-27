//----------------------------------------------------------------------------------------
// Name:        main_frame.cpp
// Purpose:     Main dialog
// Author:      Robert O'Connor
// Modified by:
// Created:     2001/10/20
// Copyright:   (c) Robert O'Connor ( rob@medicalmnemonics.com )
// Licence:     GPL
// RCS-ID:      $Id: main_frame.cpp,v 1.13 2004/01/04 00:57:49 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// GCC implementation
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma implementation "main_frame.h"
#endif

//----------------------------------------------------------------------------------------
// Standard wxWindows headers
//----------------------------------------------------------------------------------------

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// For all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers)
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

//----------------------------------------------------------------------------------------
// Header of this .cpp file
//----------------------------------------------------------------------------------------

#include "main_frame.h"  

//----------------------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//----------------------------------------------------------------------------------------

#include "wx/image.h"
#include "wx/radiobut.h"                // wxRadioButton
#include "wx/dir.h"                     // wxDir
#include "wx/tooltip.h"                 // wxToolTip
#include "wx/toolbar.h"                 // wxToolBar
#if ( setupUSE_DRAG_AND_DROP )
    #include "wx/clipbrd.h"             // wxClipBoard [so know when to disable menuitems]
#endif

// ---------------------------------------------------------------------------------------

#include "wx/xrc/xmlres.h"              // XRC XML resouces

// ---------------------------------------------------------------------------------------
  
#include "configuration.h"
#include "channel_dialog.h"
#include "preferences_dialog.h"
#include "about_dialog.h"
#include "image_list.h"
#include "plucker_controller.h"
#include "utils_datetime.h"
#include "utils_string.h"
#include "utils_internet.h"
#include "utils_controls.h"
#include "plucker_wizard_wrappers.h"
#include "startup_tips.h"
#include "help_controller.h"

//----------------------------------------------------------------------------------------
// Internal constants: control IDs
//----------------------------------------------------------------------------------------

// Required, so that we don't file update UI events for every control on the frame.
enum {
    ID_MAIN_LISTCTRL = wxID_HIGHEST + 500,
    ID_MAIN_TIMER
};

//----------------------------------------------------------------------------------------
// Event table: connect the events to the handler functions to process them
//----------------------------------------------------------------------------------------

BEGIN_EVENT_TABLE( main_frame, wxFrame )
    // NOTE: EVT_MENU and EVT_TOOL are the same thing, so that toolbar and menu events are
    // handled by the same event handler, Thus, both a "menu" xml resouce control and
    // a "tool" resource control named "main_dialog_new_channel_tool_or_menuitem" will
    // generate the exact same event, which then can be looked here in this table. The
    // tools and menuitems are thus named "tool_or_menuitem".
    EVT_MENU( XRCID( "main_frame_add_channel_wizard_tool_or_menuitem" ), main_frame::on_add_channel_wizard_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_add_channel_tool_or_menuitem" ), main_frame::on_add_channel_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_configure_channel_tool_or_menuitem" ), main_frame::on_configure_channel_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_exit_tool_or_menuitem" ), main_frame::on_exit_tool_or_menuitem )

    EVT_MENU( XRCID( "main_frame_paste_url_as_new_channel_tool_or_menuitem" ), main_frame::on_paste_url_as_new_channel_tool_or_menuitem )
    // Disable clipboard menu items if they aren't on the clipboard
    EVT_UPDATE_UI( XRCID( "main_frame_paste_url_as_new_channel_tool_or_menuitem" ), main_frame::on_update_ui_paste_url_as_new_channel_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_paste_fullnames_as_new_separate_channels_tool_or_menuitem" ), main_frame::on_paste_fullnames_as_new_separate_channels_tool_or_menuitem )
    // Disable clipboard menu items if they aren't on the clipboard
    EVT_UPDATE_UI( XRCID( "main_frame_paste_fullnames_as_new_separate_channels_tool_or_menuitem" ), main_frame::on_update_ui_paste_fullnames_as_new_separate_channels_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_delete_channels_tool_or_menuitem" ), main_frame::on_delete_channels_tool_or_menuitem )

    EVT_MENU( XRCID( "main_frame_update_selected_tool_or_menuitem" ), main_frame::on_update_selected_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_update_due_tool_or_menuitem" ), main_frame::on_update_due_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_update_all_tool_or_menuitem" ), main_frame::on_update_all_tool_or_menuitem )

    EVT_MENU( XRCID( "main_frame_preferences_tool_or_menuitem" ), main_frame::on_preferences_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_channel_defaults_tool_or_menuitem" ), main_frame::on_channel_defaults_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_install_software_tool_or_menuitem" ), main_frame::on_install_software_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_setup_wizard_tool_or_menuitem" ), main_frame::on_setup_wizard_tool_or_menuitem )

    EVT_MENU( XRCID( "main_frame_help_contents_tool_or_menuitem" ), main_frame::on_help_contents_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_help_on_this_dialog_tool_or_menuitem" ), main_frame::on_help_on_this_dialog_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_search_help_tool_or_menuitem" ), main_frame::on_search_help_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_tip_of_the_day_tool_or_menuitem" ), main_frame::on_tip_of_the_day_tool_or_menuitem )
    EVT_MENU( XRCID( "main_frame_about_tool_or_menuitem" ), main_frame::on_about_tool_or_menuitem )

    // When the listctrl has an update UI event (finished clicking, selecting, etc,
    // call a function to disable certain tools and menuitems if no no rows are selected
    // in the listctrl).
    // NOTE: This *should* have been ID_MAIN_LISTCTRL instead of -1, but for some reason
    // BCC won't ever catch the event (on builds with everything switched on in setup.h)!
    // So a hack used.
#if ( setupUSE_PARENT_WINDOW_CATCH_UPDATE_UI_EVENT_FROM_DERIVED_LISTCTRL_HACK )
    EVT_UPDATE_UI( -1, main_frame::on_update_ui_channels_tab_listctrl )
#else
    EVT_UPDATE_UI( ID_MAIN_LISTCTRL, main_frame::on_update_ui_channels_tab_listctrl )
#endif

    // The autoupdate timer
    EVT_TIMER( ID_MAIN_TIMER, main_frame::on_timer )

    // Do something when closing main dialog.
    EVT_CLOSE( main_frame::on_close )
END_EVENT_TABLE()

//----------------------------------------------------------------------------------------
// Public methods
//----------------------------------------------------------------------------------------

// Constructor
main_frame::main_frame( wxWindow* parent )
{
    // Load up this frame from XRC. [Note, instead of making a class's
    // constructor take a wxWindow* parent with a default value of NULL,
    // we could have just had designed MyFrame class with an empty
    // constructor and then written here:
    // wxXmlResource::Get()->LoadFrame(this, (wxWindow* )NULL, "main_frame");
    // since this frame will always be the top window, and thus parentless.
    // However, the current approach has source code that can be recycled
    // in case code to moves to having an invisible frame as the top level window.
    wxXmlResource::Get()->LoadFrame( this, parent, "main_frame" );

    //----Add the frame icon------------------------------------------------------------

    // Set an iconbundle for the frame (this should be in XmlResources eventually
    // once that capacity becomes available).
    wxIconBundle a_icon_bundle;

    a_icon_bundle.AddIcon( get_plucker_directory( RESOURCES ) << "/icons_application" <<
                           "/" << "application_32x32.xpm", wxBITMAP_TYPE_XPM );

    a_icon_bundle.AddIcon( get_plucker_directory( RESOURCES ) << "/icons_application" <<
                           "/" << "application_16x16.xpm", wxBITMAP_TYPE_XPM );

    a_icon_bundle.AddIcon( get_plucker_directory( RESOURCES ) << "/icons_application" <<
                           "/" << "application_48x48.xpm", wxBITMAP_TYPE_XPM );

    a_icon_bundle.AddIcon( get_plucker_directory( RESOURCES ) << "/icons_application" <<
                           "/" << "application_64x64.xpm", wxBITMAP_TYPE_XPM );

    SetIcons( a_icon_bundle );

    //----Add the menu------------------------------------------------------------------

    SetMenuBar( wxXmlResource::Get()->LoadMenuBar( "main_menubar" ) );

    //----Menu: branding modifications--------------------------------------------------

#if ( setupUSE_NO_XRC_VARIABLE_EXPANSION_YET_HACK )
    // This should really be a runtime variable expansion in the XRC file, but
    // the feature isn't there yet, so do it via normal code.

    // Set the title
    SetTitle( plkrAPPLICATION_DESKTOP_NAME );

    // Set the text of the about menu entry (this obviously needs to be done only after
    // the menu was added above.)
    wxString about_menu_label_string ;
    // As a side note, the accelerator on the About will be stripped on a Mac.
    about_menu_label_string << _( "&About" ) << wxT( " " ) << plkrAPPLICATION_DESKTOP_NAME;
    GetMenuBar()->SetLabel( XRCID( "main_frame_about_tool_or_menuitem" ), about_menu_label_string );
#endif

    //----Menu: Relocate menu items on a Mac-------------------------------------------

#ifdef __WXMAC__
    // Macintosh applications have the "About", "Preferences" and "Quit" in a special
    // bolded <application name> menu, to the right of the right-corner apple logo.
    // If we were creating the menuitems with C++ code, we would just create the About
    // menuitem with ID of wxID_ABOUT, and that's the end of the story, wxWindows 
    // will take care of the rest. 
    // However, since it was an XRC-managed ID, we can't set the ID, so we use this 
    // property instead, to do the proper shifting of the About menuitem.
    wxApp::s_macAboutMenuItemId = XRCID( "main_frame_about_tool_or_menuitem" );
    // Ditto for the Preferences menu item... [Preferences and Quit handling went into wxWindows
    // for Mac on March 10, 2003.]
    wxApp::s_macPreferencesMenuItemId = XRCID( "main_frame_preferences_tool_or_menuitem" );
    // ...and the Quit menu item.     
    wxApp::s_macExitMenuItemId = XRCID( "main_frame_exit_tool_or_menuitem" );
#endif

    //----Add the toolbar---------------------------------------------------------------

    // If the user has specified that they want a toolbar (the 1L means default is 1)...
    if ( the_configuration->Read( wxT( "/PLUCKER_DESKTOP/main_frame_toolbar_enabled" ), 1L ) ) 
    {
        // Calculate the name of the toolbar in the xrc. User can specify an extension
        // to the toolbar set and it will load that one, with some different icons (or styles).
        wxString toolbar_full_xrc_string = wxT( "main_toolbar" );
        wxString theme;

        // We will set the default based on the user's color settings. If they are on
        // a high color system, the default is "high". Otherwise will stay "", just to
        // load the regular resource with no extension to the name "main_toolbar".
        // Default starts with "" for everyone...
        wxString default_theme;
        if ( 16 <= ::wxDisplayDepth() ) 
        {
            // ...then default theme is "high",
            default_theme = wxT( "high" );
        }

        // Load theme from configrution file, using our generated default, if no key.
        theme = the_configuration->Read( wxT( "/PLUCKER_DESKTOP/theme" ), default_theme );        
    
        // If key isn't "", then add a '_' and the theme onto the end of full xrc string.
        if ( theme != wxT( "" ) ) 
        {
            toolbar_full_xrc_string << wxT( "_" ) << theme;
        }
         
        // Now load a toolbar by that name from the xrc.
        SetToolBar( wxXmlResource::Get()->LoadToolBar( this, toolbar_full_xrc_string ) );
        wxLogDebug( "Loading toolbar called " + toolbar_full_xrc_string );
                        
        // Write and flush the results of our detection so that have them for 
        // preferences dialog and for next execution.
        the_configuration->Write( wxT( "/PLUCKER_DESKTOP/theme" ), theme );
        the_configuration->Flush();    
    }
    
    //----Insert a main_listctrl to its "unknown" placeholder in the XRC file----------       
    // Construct an instance of the main_listctrl custom class
    m_main_listctrl = new main_listctrl( this, ID_MAIN_LISTCTRL, 
                                         wxDefaultPosition, wxDefaultSize,
                                         wxLC_REPORT );
   
    wxXmlResource::Get()->AttachUnknownControl( "main_frame_main_listctrl",
                                                m_main_listctrl ); 
    
    //----Add the statusbar-------------------------------------------------------------

    // If the user has specified that they want a statusar (the IL means default is 1)...
    if ( the_configuration->Read( "/PLUCKER_DESKTOP/main_frame_statusbar_enabled", 1L ) )
    {
        // Add a statusbar with one field [will automatically put a "gripsizer" 
        // indicator on the lower right corner if the OS usually does that with 
        // resizable frames].
        CreateStatusBar( 1 );

    }

    //----Load the saved position and size of the frame--------------------------------

    // ..and now set the size, position of the frame using our common function for 
    // this frame [utils_controls.cpp]
    utils_controls::read_dialog_position_and_size( this, "main_frame", 
                                                   wxDefaultPosition,
                                                   wxSize( 350,320 ) );
                                                   
    //----Initialize and start the autoupdate timer------------------------------------
     
    // NOTE: for Borland C++ anyway, (and Visual C I think gives a warning), can't do
    // a m_timer( this ) in the member constructor list. Borland C++ causes wxWindows
    // memory pointer trashing on program close. Therefore, just let it use its default
    // constructor, and then call SetOwner to set it to 'this'.
    m_timer.SetOwner( this, ID_MAIN_TIMER );
    
    // Start the timer to call the event every time a certain number of milliseconds has
    // elapsed. Use our precision preference to get the number of minutes to loop (then 
    // multiply by 100 * 60 to convert the minutes to milliseconds.
    long autoupdate_precision = the_configuration->Read( "/PLUCKER_DAEMON/autoupdate_precision", 10L );
    m_timer.Start( 1000 * 60 * autoupdate_precision );
}


main_frame::~main_frame()
{    
    // Save the size and position of the frame for next time
    // [utils_controls.cpp]
    utils_controls::write_dialog_position_and_size( this, "main_frame" );
}


// This had to be pulled from the constructor, so that can create the main frame,
// then show splash right after, then load rest of XML stuff, then show main frame,
// then setup rest of main_frame's stuff.
void main_frame::frame_startup_init()
{     
    // Set up the listctrl
    m_main_listctrl->init();          
    
    // Disable online help menuitems if was compiled out of the application    
#if ( ! setupUSE_ONLINE_HELP )
    GetMenuBar()->Enable( XRCID( "main_frame_help_contents_tool_or_menuitem" ), FALSE );
    GetMenuBar()->Enable( XRCID( "main_frame_help_on_this_dialog_tool_or_menuitem" ), FALSE );
    GetMenuBar()->Enable( XRCID( "main_frame_search_help_tool_or_menuitem" ), FALSE );
#endif

    // Disable clipboard menuitems if was compiled out of the application
#if ( ! setupUSE_DRAG_AND_DROP )
    GetMenuBar()->Enable( XRCID( "main_frame_paste_url_as_new_channel_tool_or_menuitem" ), FALSE );
    GetMenuBar()->Enable( XRCID( "main_frame_paste_fullnames_as_new_separate_channels_tool_or_menuitem" ), FALSE );
#endif

    // Disable wizard menuitems if was compiled out of the application
#if ( ! setupUSE_WIZARDS )
    GetMenuBar()->Enable( XRCID( "main_frame_install_software_tool_or_menuitem" ), FALSE );
    GetMenuBar()->Enable( XRCID( "main_frame_setup_wizard_tool_or_menuitem" ), FALSE );
    GetMenuBar()->Enable( XRCID( "main_frame_add_channel_wizard_tool_or_menuitem" ), FALSE );
    // Have to check that user chose to have a toolbar before trying to disable 
    // a tool on it.
    if ( GetToolBar() ) {
        GetToolBar()->EnableTool( XRCID( "main_frame_add_channel_wizard_tool_or_menuitem" ), FALSE );
    }
#endif

    // Disable startup tips menuitems if was compiled out of the application
#if ( ! setupUSE_STARTUP_TIPS )
    EVT_MENU( XRCID( "main_frame_tip_of_the_day_tool_or_menuitem" ), main_frame::on_tip_of_the_day_tool_or_menuitem )
#endif

    //----Menu: Clean out excess menuitem separators------------------------------------

    // After the About, Preferences, and Quit menuitems have been moved on the Mac, and 
    // we have deleted the menuitems of the compiled-out features, we can be left in 
    // the menu with some remaining separators at the top of a menu, at the bottom of 
    // a menu, or have >1 separators in a row. Therefore, call a function which will 
    // go through the menus of the menubar, removing these excess separators.
    utils_controls::strip_excess_separators_from_menubar( GetMenuBar() );

}

//----------------------------------------------------------------------------------------
// Private methods: event handlers
//----------------------------------------------------------------------------------------

void main_frame::on_add_channel_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_add_channel();
}


void main_frame::on_add_channel_wizard_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_add_channel_wizard();
}


void main_frame::on_configure_channel_tool_or_menuitem( wxCommandEvent &event )
{    
    m_main_listctrl->gui_configure_channel();
}


void main_frame::on_delete_channels_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_delete_channels();
}


void main_frame::on_exit_tool_or_menuitem( wxCommandEvent &event )
{
    // wxFrame::Close( TRUE ) forces the frame to close
    Close( TRUE );
}

void main_frame::on_paste_url_as_new_channel_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->on_paste_url_as_new_channel_menuitem();  
}


void main_frame::on_update_ui_paste_url_as_new_channel_tool_or_menuitem( wxUpdateUIEvent& event )
{
#if ( setupUSE_DRAG_AND_DROP )
    // See comments in main_listctrl.cpp
    bool is_text_on_clipboard = wxTheClipboard->IsSupported( wxDF_TEXT );
    event.Enable( is_text_on_clipboard );
#endif
}


void main_frame::on_paste_fullnames_as_new_separate_channels_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->on_paste_fullnames_as_new_separate_channels_menuitem();
}


void main_frame::on_update_ui_paste_fullnames_as_new_separate_channels_tool_or_menuitem( wxUpdateUIEvent& event )
{
#if ( setupUSE_DRAG_AND_DROP )
    // See comments in main_listctrl.cpp
    bool is_filenames_on_clipboard = wxTheClipboard->IsSupported( wxDF_FILENAME ); 
    event.Enable( is_filenames_on_clipboard );
#endif
}


void main_frame::on_update_selected_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_update_selected_channels();
}


void main_frame::on_update_due_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_update_due_channels();
}


void main_frame::on_update_all_tool_or_menuitem( wxCommandEvent &event )
{
    m_main_listctrl->gui_update_all_channels();
}


void main_frame::on_preferences_tool_or_menuitem( wxCommandEvent &event )
{    
    wxLogDebug( "Entering main_frame::on_preferences_tool_or_menuitem()" );
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );
    // Launch a preferences dialog
    preferences_dialog a_preferences_dialog( this );
    a_preferences_dialog.ShowModal();

    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );
    wxLogDebug( "Finished main_frame::on_preferences_tool_or_menuitem()" );
}


void main_frame::on_channel_defaults_tool_or_menuitem( wxCommandEvent &event )
{    
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );
    
    // Launch the channel dialog, but this time telling section is 
    // plkrNEW_CHANNEL_TEMPLATE_SECTION, not a channel directory
    channel_dialog a_channel_dialog( this, plkrNEW_CHANNEL_TEMPLATE_SECTION );
    a_channel_dialog.ShowModal();
    
    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );
}


void main_frame::on_install_software_tool_or_menuitem( wxCommandEvent &event )
{ 
#if ( setupUSE_WIZARDS )
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );

    // [plucker_desktop.cpp wrapper functions]
    plucker_wizard_wrappers::run_install_viewer_wizard( this );
    
    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );
#endif
}


void main_frame::on_setup_wizard_tool_or_menuitem( wxCommandEvent &event )
{
#if ( setupUSE_WIZARDS )
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );

    // [plucker_desktop.cpp wrapper functions]
    // If they clicked "Finish" button...
    if ( plucker_wizard_wrappers::run_setup_wizard( this ) ) 
    {
        // ...then reload the rows to show the possbile newly added channels.
        m_main_listctrl->reload_rows();
    }
    
    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );
#endif
}


void main_frame::on_help_contents_tool_or_menuitem( wxCommandEvent &event )
{
#if ( setupUSE_ONLINE_HELP )
    help_controller::get()->show_help();
#endif
}


void main_frame::on_help_on_this_dialog_tool_or_menuitem( wxCommandEvent &event )
{
#if ( setupUSE_ONLINE_HELP )
    help_controller::get()->show_help_topic( plkrHELP_ID_MAIN_DIALOG );
#endif
}


void main_frame::on_search_help_tool_or_menuitem( wxCommandEvent &event )
{
#if ( setupUSE_ONLINE_HELP )
    help_controller::get()->search_help( this );
#endif
}


void main_frame::on_tip_of_the_day_tool_or_menuitem( wxCommandEvent &event )
{
#if ( setupUSE_STARTUP_TIPS )
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );

    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );

    startup_tips::show_startup_tips( this, TRUE );
#endif
}


void main_frame::on_about_tool_or_menuitem( wxCommandEvent &event )
{
    plucker_controller::get()->set_is_new_update_execution_allowed( FALSE );

    wxString branding_about_dialog_style = the_configuration->Read( "/PLUCKER_DESKTOP/branding_about_dialog_style", wxT( "enhanced" ) );

    wxString about_title;
    about_title << _( "About" ) << wxT( " " ) << plkrAPPLICATION_DESKTOP_NAME;

    if ( branding_about_dialog_style == wxT( "simple" ) )
    {
        wxDialog a_about_simple_dialog;
        wxXmlResource::Get()->LoadDialog( &a_about_simple_dialog, this, wxT( "about_simple_dialog" ) );
#if ( setupUSE_NO_XRC_VARIABLE_EXPANSION_YET_HACK )
        a_about_simple_dialog.SetTitle( about_title );
#endif
        a_about_simple_dialog.ShowModal();
#if ( setupUSE_ENHANCED_ABOUT_DIALOG )
    // It is set up this way with the #if, so that there is just a fall through
    // if the enhanced about dialog is compiled out.
    } 
    else if ( branding_about_dialog_style == wxT( "enhanced" ) ||
                branding_about_dialog_style == wxT( ""  ) ) 
    {
        // Launch the nice about dialog
        about_dialog a_about_dialog( this );
        a_about_dialog.ShowModal();
#endif
    } 
    else 
    {
        // Just launch a Joe-Blow one.
        wxString about_message;
        about_message << plkrAPPLICATION_DESKTOP_NAME << wxT( " " ) 
                      << _( "version" ) << wxT( " " )
                      << _T( plkrPLUCKER_DESKTOP_VERSION_STRING );
        wxMessageBox( about_message, about_title, wxOK | wxICON_INFORMATION, this );
    }

    plucker_controller::get()->set_is_new_update_execution_allowed( TRUE );
}


// Disables menuitems and toolbar tools that need to have an item selected in the
// listctrl first
void main_frame::on_update_ui_channels_tab_listctrl( wxUpdateUIEvent &event )
{
    wxLogDebug( "Entering main_frame::on_update_ui_channels_tab_listctrl()" );
    // True if there is some items selected in the listctrl.
    bool enabled = (bool)m_main_listctrl->GetSelectedItemCount();

    wxLogDebug( "Number of items selected in listctrl =%d", m_main_listctrl->GetSelectedItemCount() );

    // Now set the tools and menuitems to that either true or false.
    // The GetToolBar() and GetMenu() are the handy ways to get a pointer to the frame's
    // toolbar and menubar. Do the menu first:

    GetMenuBar()->Enable( XRCID( "main_frame_configure_channel_tool_or_menuitem" ), enabled );
    GetMenuBar()->Enable( XRCID( "main_frame_delete_channels_tool_or_menuitem" ), enabled );
    GetMenuBar()->Enable( XRCID( "main_frame_update_selected_tool_or_menuitem" ), enabled );

    // But it is possible that the user turned off the toolbar, so check to see if it there
    // it a pointer by querying whether a call to wxFrame::GetToolBar() returns anything.
    // Note we can't use the configuration setting to check it, since that value is
    // written to config file when options dialog is closed.
    if ( GetToolBar() ) 
    {
        GetToolBar()->EnableTool( XRCID( "main_frame_configure_channel_tool_or_menuitem" ), enabled );
        GetToolBar()->EnableTool( XRCID( "main_frame_delete_channels_tool_or_menuitem" ), enabled );
        GetToolBar()->EnableTool( XRCID( "main_frame_update_selected_tool_or_menuitem" ), enabled );
    }
}


void main_frame::on_timer( wxTimerEvent& event )
{
    wxLogDebug( "Entering main_frame::on_timer" );

    // Check the 3 criteria needed for an autoupdate event to start:
    // (1) Is it allowed, via plucker_controller::set_is_new_update_execution_allowed() status
    // (2) Are we in an autoupdate mode that permits autoupdate behaviour
    // (3) Are any channels due
    if ( plucker_controller::get()->is_all_criteria_met_to_execute_an_autoupdate() ) 
    {
         // Update the due channels
        m_main_listctrl->gui_update_due_channels();
    }
}


void main_frame::on_close( wxCloseEvent& event )
{
    wxLogDebug( "Entering main_frame::on_close()" );
    // If the timer is running...
    if ( m_timer.IsRunning() )
    {
        // ...stop it
        wxLogDebug( "About to stop main_frame's timer" );
        m_timer.Stop();
    }

#if ( setupUSE_ONLINE_HELP )
    // Shut down the help controller frame if it exists and is still running.
    // It is put here and not wxApp::OnExit(), in order for it to be destroyed when
    // shut down the application. If it wasn't, then help will continue to run the
    // program, since wxApp::OnExit() only fires when all the frames are gone,
    // and the help controller is a frame.
    // We call get() with FALSE, so that don't make a frame if there wasn't any
    // at this point (would prevent close until another click on GTK).
    wxLogDebug( "Since main_frame is closing, will make help close too, so app will exit." );
    help_controller::get( FALSE )->close();
#endif

    // Absolutely must call event.Skip() so that can skip upward to the next
    // valid handler, which will do the rest of the things that should close the frame.
    event.Skip();
}
