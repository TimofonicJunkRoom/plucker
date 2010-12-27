//----------------------------------------------------------------------------------------
// Name:        editor_dialog.cpp
// Purpose:     Simple HTML editor dialog
// Author:      Robert O'Connor
// Modified by:
// Created:     2001/10/20
// Copyright:   (c) Robert O'Connor ( rob@medicalmnemonics.com )
// Licence:     GPL
// RCS-ID:      $Id: editor_dialog.cpp,v 1.26 2004/01/04 00:57:49 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// GCC implementation
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma implementation "editor_dialog.h"
#endif

//----------------------------------------------------------------------------------------
// Setup information
//----------------------------------------------------------------------------------------

#include "setup.h"

//----------------------------------------------------------------------------------------
// Begin feature removal condition
//----------------------------------------------------------------------------------------

#if ( setupUSE_INTEGRATED_HTML_EDITOR )

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

#include "editor_dialog.h"

//----------------------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//----------------------------------------------------------------------------------------

#include "wx/notebook.h"            // wxNotebook (need its event table)
#include "wx/html/htmlwin.h"        // wxHtmlWindow    

// ---------------------------------------------------------------------------------------

#include "wx/xrc/xmlres.h"          // XRC XML resources
#if ( setupUSE_STYLED_TEXT_CONTROL )
    #include "wx/wfstream.h"        // FileStream for loading StyledTextControl data
#endif

// ---------------------------------------------------------------------------------------

#include "configuration.h"
#include "body_dialog.h"
#include "bookmark_dialog.h"
#include "email_dialog.h"
#include "font_dialog.h"
#include "hr_dialog.h"
#include "hyperlink_dialog.h"
#include "image_dialog.h"
#include "ol_dialog.h"
#include "popup_dialog.h"
#include "span_dialog.h"
#include "table_dialog.h"
#include "td_th_dialog.h"
#include "ul_dialog.h"
#include "image_list.h"
#include "utils_controls.h"
#include "help_controller.h"

//----------------------------------------------------------------------------------------
// Internal constants
//----------------------------------------------------------------------------------------

// Index numbers of tabs (aka notebook pages) in the notebook, as will find each tab by
// index and assign an image to each tab. Also for the notebook page changing function.
enum {
    EDITOR_NOTEBOOK_PAGE_INDEX_EDIT             = 0, 
    EDITOR_NOTEBOOK_PAGE_INDEX_PREVIEW
};

//----------------------------------------------------------------------------------------
// Event table: connect the events to the handler functions to process them
//----------------------------------------------------------------------------------------

BEGIN_EVENT_TABLE( editor_dialog, wxDialog )
    // The id to pass for this event is the the id of notebook, not the id of the
    // notebook page
    EVT_NOTEBOOK_PAGE_CHANGING( XRCID( "editor_dialog_notebook" ), editor_dialog::on_notebook_changing)
    // The basic toolbar panel
    EVT_BUTTON( XRCID( "editor_dialog_basic_panel_hyperlink_button" ), editor_dialog::on_toolbar_hyperlink_button )
    // The advanced toolbar panel
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_hyperlink_button" ), editor_dialog::on_toolbar_hyperlink_button )

    EVT_BUTTON( XRCID( "editor_dialog_toolbar_mailto_button" ), editor_dialog::on_toolbar_mailto_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_bookmark_button" ), editor_dialog::on_toolbar_bookmark_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_popup_button" ), editor_dialog::on_toolbar_popup_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_paragraph_button" ), editor_dialog::on_toolbar_paragraph_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_paragraph_left_button" ), editor_dialog::on_toolbar_paragraph_left_button )    
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_paragraph_center_button" ), editor_dialog::on_toolbar_paragraph_center_button ) 
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_paragraph_right_button" ), editor_dialog::on_toolbar_paragraph_right_button ) 
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_paragraph_full_button" ), editor_dialog::on_toolbar_paragraph_full_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_break_button" ), editor_dialog::on_toolbar_break_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_image_button" ), editor_dialog::on_toolbar_image_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_hr_button" ), editor_dialog::on_toolbar_hr_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_nbsp_button" ), editor_dialog::on_toolbar_nbsp_button )

    EVT_BUTTON( XRCID( "editor_dialog_toolbar_span_button" ), editor_dialog::on_toolbar_span_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_div_left_button" ), editor_dialog::on_toolbar_div_left_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_div_center_button" ), editor_dialog::on_toolbar_div_center_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_div_right_button" ), editor_dialog::on_toolbar_div_right_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_center_button" ), editor_dialog::on_toolbar_center_button )
   EVT_BUTTON( XRCID( "editor_dialog_toolbar_blockquote_button" ), editor_dialog::on_toolbar_blockquote_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_q_button" ), editor_dialog::on_toolbar_q_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_cite_button" ), editor_dialog::on_toolbar_cite_button )

    EVT_BUTTON( XRCID( "editor_dialog_toolbar_font_button" ), editor_dialog::on_toolbar_font_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_bold_button" ), editor_dialog::on_toolbar_bold_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_strong_button" ), editor_dialog::on_toolbar_strong_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_italic_button" ), editor_dialog::on_toolbar_italic_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_em_button" ), editor_dialog::on_toolbar_em_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_underline_button" ), editor_dialog::on_toolbar_underline_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_strike_button" ), editor_dialog::on_toolbar_strike_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_tt_button" ), editor_dialog::on_toolbar_tt_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_sup_button" ), editor_dialog::on_toolbar_sup_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_sub_button" ), editor_dialog::on_toolbar_sub_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_preformatted_button" ), editor_dialog::on_toolbar_preformatted_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_code_button" ), editor_dialog::on_toolbar_code_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h1_button" ), editor_dialog::on_toolbar_h1_button ) 
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h2_button" ), editor_dialog::on_toolbar_h2_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h3_button" ), editor_dialog::on_toolbar_h3_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h4_button" ), editor_dialog::on_toolbar_h4_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h5_button" ), editor_dialog::on_toolbar_h5_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_h6_button" ), editor_dialog::on_toolbar_h6_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_small_button" ), editor_dialog::on_toolbar_small_button )

    EVT_BUTTON( XRCID( "editor_dialog_toolbar_table_button" ), editor_dialog::on_toolbar_table_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_tr_button" ), editor_dialog::on_toolbar_tr_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_th_button" ), editor_dialog::on_toolbar_th_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_td_button" ), editor_dialog::on_toolbar_td_button )

    EVT_BUTTON( XRCID( "editor_dialog_toolbar_ordered_list_button" ), editor_dialog::on_toolbar_ordered_list_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_unordered_list_button" ), editor_dialog::on_toolbar_unordered_list_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_list_element_button" ), editor_dialog::on_toolbar_list_element_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_menu_button" ), editor_dialog::on_toolbar_menu_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_dt_button" ), editor_dialog::on_toolbar_dt_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_dd_button" ), editor_dialog::on_toolbar_dd_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_dl_button" ), editor_dialog::on_toolbar_dl_button )
    
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_html_button" ), editor_dialog::on_toolbar_html_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_meta_handheldfriendly_button" ), editor_dialog::on_toolbar_meta_handheldfriendly_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_body_button" ), editor_dialog::on_toolbar_body_button )
    EVT_BUTTON( XRCID( "editor_dialog_toolbar_comment_button" ), editor_dialog::on_toolbar_comment_button )

    EVT_BUTTON( wxID_OK, editor_dialog::OnOK )
    EVT_BUTTON( wxID_HELP_CONTEXT, editor_dialog::on_help_button )
END_EVENT_TABLE()

//----------------------------------------------------------------------------------------
// Public members
//----------------------------------------------------------------------------------------

// Constructor, including setting the dialog's m_editted_filename member to the
// incoming filename_to_load string.

editor_dialog::editor_dialog( wxWindow* parent, wxString filename_to_load )
                        : m_editted_filename( filename_to_load )
{
    wxXmlResource::Get()->LoadDialog( this, parent, "editor_dialog" );
            
    // Load either the simple or advanced toolbar into the <unknown> placeholder
    // in the XRC (similar to showcase_dialog.cpp--see there for comments).    
    bool use_advanced_toolbars = the_configuration->Read( "/PLUCKER_DESKTOP/editor_use_advanced_toolbars", 0L );
    
    if ( use_advanced_toolbars ) 
    {    
        wxXmlResource::Get()->AttachUnknownControl( wxT( "editor_dialog_toolbar_panel" ),
            wxXmlResource::Get()->LoadPanel( this, wxT( "editor_dialog_advanced_panel" ) ) );
    } 
    else 
    {
        wxXmlResource::Get()->AttachUnknownControl( wxT( "editor_dialog_toolbar_panel" ),
            wxXmlResource::Get()->LoadPanel( this, wxT( "editor_dialog_basic_panel" ) ) );
    }
    
    // Replaces the unknown placeholder in resource with a STC or TextCtrl
#if ( setupUSE_STYLED_TEXT_CONTROL )
    m_editor_stc = new wxStyledTextCtrl( this, -1 );
    wxXmlResource::Get()->AttachUnknownControl( wxT( "editor_dialog_stc_or_textctrl" ),
                                           m_editor_stc );                                           
#else       // setupUSE_STYLED_TEXT_CONTROL
    m_editor_textctrl = new wxTextCtrl( this, -1, wxEmptyString, wxDefaultPosition,
                                        wxSize( 0,0 ), wxTE_MULTILINE | wxHSCROLL );
    wxXmlResource::Get()->AttachUnknownControl( wxT( "editor_dialog_stc_or_textctrl" ),
                                            m_editor_textctrl );                                            
#endif      // setupUSE_STYLED_TEXT_CONTROL    

    // Initialize the settings for STC or TextCtrl
    stc_or_textctrl_init();
    
    // Give small icons to the 'Edit' and 'Preview' tabs, using images from the shareed 
    // wxImageList. First assign the imagelist to use to get images from, then tell what
    // index number to set the icons to. 
    XRCCTRL( *this, "editor_dialog_notebook", wxNotebook )
        ->SetImageList( image_list::get() );
    XRCCTRL( *this, "editor_dialog_notebook", wxNotebook )
        ->SetPageImage( EDITOR_NOTEBOOK_PAGE_INDEX_EDIT, SMALL_IMAGE_LIST_ID_EDITOR_DIALOG_EDIT_TAB );
    XRCCTRL( *this, "editor_dialog_notebook", wxNotebook )
        ->SetPageImage( EDITOR_NOTEBOOK_PAGE_INDEX_PREVIEW, SMALL_IMAGE_LIST_ID_EDITOR_DIALOG_PREVIEW_TAB );
    
    // Load the file into the STC or TextCtrl
    stc_or_textctrl_load_file( filename_to_load );
    
    // Set the size, position of the frame using our common function for this dialog
    // [utils_controls.cpp]
    utils_controls::read_dialog_position_and_size( this, "editor_dialog" );
}


// Destructor
editor_dialog::~editor_dialog()
{
    // Save the size and position of the dialog for next time
    // [utils_controls.cpp]
    utils_controls::write_dialog_position_and_size( this, "editor_dialog" );

    // Delete the added styled text control / textctrl so no memory leak.
    //! \todo SOURCE: Needed? Weren't they created as children so they should go anyways.
#if ( setupUSE_STYLED_TEXT_CONTROL )
    delete m_editor_stc; 
    m_editor_stc = NULL;
#else   // setupUSE_STYLED_TEXT_CONTROL
    delete m_editor_textctrl; 
    m_editor_textctrl = NULL;
#endif  // setupUSE_STYLED_TEXT_CONTROL
}

//----------------------------------------------------------------------------------------
// Private members: Non-event handlers
//----------------------------------------------------------------------------------------

// Initializes the STC or TextCtrl
void editor_dialog::stc_or_textctrl_init()
{
    // Make a default font for the STC or TextCtrl. Set the default point size up in 
    // plucker_defines.h, as it varies too much with the OS.
    wxFont default_font( plkrDEFAULT_TEXTCTRL_FONT_POINTSIZE,
                         wxFONTFAMILY_TELETYPE, 
                         wxFONTSTYLE_NORMAL,
                         wxFONTWEIGHT_NORMAL
//! \todo Remove the force to Courier after there is a font selector.
#ifdef __WXGTK__                         
                         ,
                         FALSE,
                         "Courier"
#endif                         
                         );  
                                
#if ( setupUSE_STYLED_TEXT_CONTROL )

    // Setup the default font, if it is OK
    if ( default_font.Ok() ) 
    {
        m_editor_stc->StyleSetFont( wxSTC_STYLE_DEFAULT, default_font );
    }
    // Clear any current styles.
    m_editor_stc->StyleClearAll();
    
    // The rest of these set up certain styles for HTML. See wx/stc/stc.h for the defines:   
    
    // wxSTC_H_DEFAULT: Normal text, not inside a <>: <tag>style</tag>style<nexttag>
    m_editor_stc->StyleSetForeground( wxSTC_H_DEFAULT, wxColour( 0x00, 0x00, 0x00 ) ); 
    
    // wxSTC_H_TAG: Tags: <style attribute="value"></style>
    m_editor_stc->StyleSetForeground( wxSTC_H_TAG,  wxColour( 0x99, 0x00, 0x99 ) );
    m_editor_stc->StyleSetBold( wxSTC_H_TAG, TRUE );
    
    // wxSTC_H_ATTRIBUTE: Attributes: <tag style="value">    
    m_editor_stc->StyleSetForeground( wxSTC_H_ATTRIBUTE, wxColour( 0xff, 0x00, 0x00 ) );
    m_editor_stc->StyleSetBold( wxSTC_H_ATTRIBUTE, TRUE );  

    // wxSTC_H_NUMBER: Unquoted attribute numerical values: <tag attribute=style>
    m_editor_stc->StyleSetForeground( wxSTC_H_NUMBER, wxColour( 0x00, 0x00, 0xff ) );
    
    // wxSTC_H_DOUBLESTRING: Doubled quoted attribute values: <tag attribute="style">
    m_editor_stc->StyleSetForeground( wxSTC_H_DOUBLESTRING, wxColour( 0x00, 0x00, 0xff ) );

    // wxSTC_H_SINGLESTRING: Single quoted attribute values: <tag attribute='style'>
    m_editor_stc->StyleSetForeground( wxSTC_H_SINGLESTRING,  wxColour( 0x00, 0x00, 0xff ) );

    // wxSTC_H_OTHER: Equal signs (=) between paramters and values: <tag attributestylevalue>
    m_editor_stc->StyleSetForeground( wxSTC_H_OTHER, wxColour( 0xff, 0x00, 0x00 ) );
    
    // wxSTC_H_COMMENT: HTML comments (also colors the <!--- and --->): <!--- style --->
    m_editor_stc->StyleSetForeground( wxSTC_H_COMMENT, wxColour( 0x99, 0x99, 0x99 ) );
    m_editor_stc->StyleSetItalic( wxSTC_H_COMMENT, TRUE );
    
    // wxSTC_H_ENTITY: Entities, such as &amp; or &nbsp;
    m_editor_stc->StyleSetForeground( wxSTC_H_ENTITY, wxColour( 0x00, 0x00, 0x00 ) );
    m_editor_stc->StyleSetItalic( wxSTC_H_ENTITY, TRUE );
    m_editor_stc->StyleSetBold( wxSTC_H_ENTITY, TRUE );  

    // Set the forecolor and backcolor for text currently wiped and selected 
    // with mouse. Fits in best with OS as a whole if use the OS system colors.
    m_editor_stc->SetSelForeground( TRUE, 
                  wxSystemSettings::GetSystemColour( wxSYS_COLOUR_HIGHLIGHTTEXT ) );
    m_editor_stc->SetSelBackground( TRUE, 
                  wxSystemSettings::GetSystemColour( wxSYS_COLOUR_HIGHLIGHT ) );

    // Put line numbers in the margin ( ie the gutter )
    m_editor_stc->SetMarginType( 0, wxSTC_MARGIN_NUMBER );    
  
    // Format the font of the line numbers, and the margin width    
#ifdef __WXMSW__
    m_editor_stc->StyleSetSpec( wxSTC_STYLE_LINENUMBER, "size:7,face:Arial,fore:#666666" );
    m_editor_stc->SetMarginWidth( 0, 22 );
#else
    m_editor_stc->StyleSetSpec( wxSTC_STYLE_LINENUMBER, "size:9,face:Helvetica" );
    m_editor_stc->SetMarginWidth( 0, 22 );
#endif

#else       // setupUSE_STYLED_TEXT_CONTROL

    m_editor_textctrl->SetFont( default_font );
        
#endif    // setupUSE_STYLED_TEXT_CONTROL
}


// Loads the file to edit into the STC or TextCtrl
void editor_dialog::stc_or_textctrl_load_file( wxString filename )
{
#if ( setupUSE_STYLED_TEXT_CONTROL )

    // Load up the text
    wxFile   file( filename );
    wxString st;

    char* buff = st.GetWriteBuf( file.Length() );
    file.Read( buff, file.Length() );
    st.UngetWriteBuf();

    //! \todo SOURCE: Need to destruct the wxFile object here?
    file.~wxFile();

    m_editor_stc->InsertText( 0, st );
    m_editor_stc->EmptyUndoBuffer();
    
    // STC has differed lexers to colorize different language syntaxes. This sets the 
    // lexer to HTML syntax 
    m_editor_stc->SetLexer( wxSTC_LEX_HTML );
    
#else      // setupUSE_STYLED_TEXT_CONTROL

    // wxTextCtrl has its own function to load directly from a file.
    m_editor_textctrl->LoadFile( filename );
        
#endif    // setupUSE_STYLED_TEXT_CONTROL
}


// Inserts 2 string of text at the current insertion point. It is split into 2 strings
// so that can (a) Highlight a swatch of text and the start and ending text gets put
// on either side of the selected text, or (b) if no text selected, then backspace the
// caret to right before the ending text, so that when add a <h1></h1>, the caret gets
// placed in a logical place: <h1>|</h1> 
void editor_dialog::stc_or_textctrl_insert_text
    ( 
    wxString starting_text_to_insert,   // Text to insert at start of selection
    wxString ending_text_to_insert,     // Text to insert at end of a selection
    bool     tag_can_insert_newline     // Whether prudent to ever insert a '\n'
    )                                                 
{
    wxString selected_text;      // Currently selected text in the control
   
    // Add a trailing linefeed to the ending text, if so requested by the user in the
    // editor preferences, and it is prudent to do so (ie a newline after a <i> </i> doesn't
    // make sense.
    bool insert_a_linefeed = the_configuration->Read( "/PLUCKER_DESKTOP/editor_tools_insert_linefeeds", 1L); 
    if ( insert_a_linefeed && tag_can_insert_newline ) 
    {
        ending_text_to_insert += wxT( "\n" );  
    }        
    
#if ( setupUSE_STYLED_TEXT_CONTROL )
    selected_text = m_editor_stc->GetSelectedText();
    
    if ( selected_text == "" )
    {
        // No text selected, just add the two strings at the insertion point
        int original_caret_position = m_editor_stc->GetCurrentPos();
        m_editor_stc->InsertText( original_caret_position, ending_text_to_insert );
        m_editor_stc->InsertText( original_caret_position, starting_text_to_insert );
        // Now set the cursor to between the starting and ending text. The previous
        // insertion text ends up at the the original caret position, so just fast-forward
        // the length of the starting text        
        int length_of_starting_text_to_insert = (int) starting_text_to_insert.Length();
        int new_caret_position = original_caret_position + length_of_starting_text_to_insert;
        m_editor_stc->SetSelectionStart( new_caret_position );
        //m_editor_stc->SetCurrentPos( new_caret_position );
    } 
    else 
    {
        // Some text is selected, so put the ending_text at the end of the selected text,
        // and the starting text before the selected text.
        // First, store the positions of start and end of selection.
        int selection_starting_caret_position = m_editor_stc->GetSelectionStart();
        int selection_ending_caret_position = m_editor_stc->GetSelectionEnd();
        // Now insert the ending text at the ending caret position (by inserting the 
        // ending text first, the starting position doesn't change).
        m_editor_stc->InsertText( selection_ending_caret_position , ending_text_to_insert );
        m_editor_stc->InsertText( selection_starting_caret_position, starting_text_to_insert );
    }   
    
    // Set the focus from the toolbar, back to the STC. 
    m_editor_stc->SetFocus();
    
#else      // setupUSE_STYLED_TEXT_CONTROL

    long selection_starting_caret_position;
    long selection_ending_caret_position;
    
    m_editor_textctrl->GetSelection( &selection_starting_caret_position,
                                     &selection_ending_caret_position );
                        
    // If selection starting caret position equals the end, that means was no selection
    if ( selection_starting_caret_position == selection_ending_caret_position ) 
    {       
        long original_caret_position = m_editor_textctrl->GetInsertionPoint();
        // No text selected, just add the two strings at the insertion point 
        m_editor_textctrl->WriteText( starting_text_to_insert );
        m_editor_textctrl->WriteText( ending_text_to_insert );
        // Now set the cursor to between the starting and ending text. The previous
        // insertion text ends up at the the original caret position, so just fast-forward
        // the length of the starting text
        long length_of_starting_text_to_insert = (long) starting_text_to_insert.Length();
        long new_caret_position = original_caret_position + length_of_starting_text_to_insert;
        m_editor_textctrl->SetInsertionPoint( new_caret_position );
    } 
    else
    {
        // Some text is selected, so put the ending_text at the end of the selected text,
        // and the starting text before the selected text.
        // Set the insertion point to selection end, then insert ending text
        m_editor_textctrl->SetInsertionPoint( selection_ending_caret_position );
        m_editor_textctrl->WriteText( ending_text_to_insert );
        // Set the insertion point to selection end, then insert ending text
        m_editor_textctrl->SetInsertionPoint( selection_starting_caret_position );
        m_editor_textctrl->WriteText( starting_text_to_insert );
    }
        
    // Set the focus from the toolbar, back to the TextCtrl. 
    m_editor_textctrl->SetFocus();
#endif    // setupUSE_STYLED_TEXT_CONTROL
}

void editor_dialog::stc_or_textctrl_send_text_to_preview()        
{
    wxString preview_text;
    
#if ( setupUSE_STYLED_TEXT_CONTROL )
    
    // Get all the text in the styled text control.
    preview_text = m_editor_stc->GetText();
    
#else      // setupUSE_STYLED_TEXT_CONTROL

    // Get all the text in the wxTextCtrl        
    preview_text = m_editor_textctrl->GetValue();

#endif    // setupUSE_STYLED_TEXT_CONTROL

    // Set the preview html window to the preview_text string
    XRCCTRL( *this, "editor_dialog_preview_tab_htmlwindow", wxHtmlWindow )
        ->SetPage( preview_text );
    wxLogDebug ( "Set the value of htmlwindow" );
}


// Saves context of STC or TextCtrl to a file ( no filename argument, since
// using the class member 'm_editted_filename').
void editor_dialog::stc_or_textctrl_save_file()      
{

#if ( setupUSE_STYLED_TEXT_CONTROL )
    
    // TODO: override OnCancel/On Close to give a warning box if cancelling on a text control
    //       that was modified.
    
    wxString stc_text;
    // Create a file object, and importantly, the second parameter is wxFile::write
    // mode so, that can write to the file.
    wxFile   file( m_editted_filename, wxFile::write );
    
    // Get all the text in the styled text control.
    stc_text = m_editor_stc->GetText();
    
    // Dump the entire string to the file.
    file.Write( stc_text );
    
    //TODO: Need to destroy the wxFile object?
    file.~wxFile();
    
#else      // setupUSE_STYLED_TEXT_CONTROL
    
    // TODO: override OnCancel/On Close to give a warning box if cancelling on a text control
    //       that was modified.
    
    // Only bother saving if the text inside the textbox has been modified.
    if ( m_editor_textctrl->IsModified() ) 
    {     
        // Save the HTML code from the text control, back into the file.
        // Recall "this->m_editted_filename" was the home.html file that we
        // squirreled away when we first opened this dialog.
    
        bool successful = m_editor_textctrl->SaveFile( this->m_editted_filename );        
        // Show a warning message if unable to save.
        if( ! successful )
        {
            wxLogError( _( "Unable to save home.html" ) );
        }
    }  
    
#endif    // setupUSE_STYLED_TEXT_CONTROL
}

//----------------------------------------------------------------------------------------
// Private members: event handlers
//----------------------------------------------------------------------------------------

// Loads the html source of edit textctrl into preview htmlwindow, so can render a preview.
void editor_dialog::on_notebook_changing( wxNotebookEvent &event )
{ 
    // There is a on_notebook_changing event sent when loading dialog, and this causes a 
    // crash. Therefore we prevent it executing until our notebook is displayed.
    if ( this->wxWindow::IsShown() ) 
    {
        wxLogDebug( "Entering on_notebook_changing_function" );
        wxLogDebug( "Notebook event.GetOldSelection=%d", event.GetOldSelection() ); 
        if ( event.GetOldSelection() == EDITOR_NOTEBOOK_PAGE_INDEX_EDIT ) {   
            wxLogDebug( "Switching from edit tab, so sending text to preview tab" );     
            stc_or_textctrl_send_text_to_preview();
        }        
    }   
}


void editor_dialog::on_toolbar_hyperlink_button( wxCommandEvent &event )
{
   hyperlink_dialog a_hyperlink_dialog( this );   
    
   if ( a_hyperlink_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_hyperlink_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    

}


void editor_dialog::on_toolbar_mailto_button( wxCommandEvent &event )
{
   email_dialog a_email_dialog( this );   
    
   if ( a_email_dialog.ShowModal() == wxID_OK ) 
   {
       wxString starting_text;
       wxString ending_text;
       a_email_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_bookmark_button( wxCommandEvent &event )
{
   bookmark_dialog a_bookmark_dialog( this );   
    
   if ( a_bookmark_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_bookmark_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_popup_button( wxCommandEvent &event )
{
   popup_dialog a_popup_dialog( this );   
    
   if ( a_popup_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_popup_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }     
}

void editor_dialog::on_toolbar_paragraph_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<p>", "</p>", TRUE );
}


void editor_dialog::on_toolbar_paragraph_left_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<p align=\"left\">", "</p>", TRUE );
}


void editor_dialog::on_toolbar_paragraph_center_button( wxCommandEvent &event ) 
{
    stc_or_textctrl_insert_text( "<p align=\"center\">", "</p>", TRUE );
}


void editor_dialog::on_toolbar_paragraph_right_button( wxCommandEvent &event ) 
{
    stc_or_textctrl_insert_text( "<p align=\"right\">", "</p>", TRUE );
}


void editor_dialog::on_toolbar_paragraph_full_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<p align=\"full\">", "</p>", TRUE );
}


void editor_dialog::on_toolbar_break_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<br>", "", TRUE );
}


void editor_dialog::on_toolbar_image_button( wxCommandEvent &event )
{
   image_dialog a_image_dialog( this );   
    
   if ( a_image_dialog.ShowModal() == wxID_OK ) 
   {
       wxString starting_text;
       wxString ending_text;
       a_image_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }      
}


void editor_dialog::on_toolbar_hr_button( wxCommandEvent &event )
{
   hr_dialog a_hr_dialog( this );   
    
   if ( a_hr_dialog.ShowModal() == wxID_OK ) 
   {
       wxString starting_text;
       wxString ending_text;
       a_hr_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_nbsp_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "&nbsp;", "", FALSE );
}


void editor_dialog::on_toolbar_span_button( wxCommandEvent &event )
{
   span_dialog a_span_dialog( this );   
    
   if ( a_span_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_span_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_div_left_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<div align=\"left\">", "</div>", TRUE );
}


void editor_dialog::on_toolbar_div_center_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<div align=\"center\">", "</div>", TRUE );
}


void editor_dialog::on_toolbar_div_right_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<div align=\"right\">", "</div>", TRUE );
}


void editor_dialog::on_toolbar_center_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<center>", "</center>", FALSE );
}


void editor_dialog::on_toolbar_blockquote_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<blockquote>", "</blockquote>", TRUE );
}


void editor_dialog::on_toolbar_q_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<q>", "</q>", TRUE );
}


void editor_dialog::on_toolbar_cite_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<cite>", "</cite>", TRUE );
}


void editor_dialog::on_toolbar_font_button( wxCommandEvent &event )
{
   font_dialog a_font_dialog( this );   
    
   if ( a_font_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_font_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_bold_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<b>", "</b>", FALSE );
}


void editor_dialog::on_toolbar_strong_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<strong>", "</strong>", FALSE );
}


void editor_dialog::on_toolbar_italic_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<i>", "</i>", FALSE );
}


void editor_dialog::on_toolbar_em_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<em>", "</em>", FALSE );
}


void editor_dialog::on_toolbar_underline_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<u>", "</u>", FALSE );
}


void editor_dialog::on_toolbar_strike_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<strike>", "</strike>", FALSE );
}


void editor_dialog::on_toolbar_tt_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<tt>", "</tt>", FALSE );
}


void editor_dialog::on_toolbar_sup_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<sup>", "</sup>", FALSE );
}


void editor_dialog::on_toolbar_sub_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<sub>", "</sub>", FALSE );
}


void editor_dialog::on_toolbar_preformatted_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<pre>", "</pre>", FALSE );
}


void editor_dialog::on_toolbar_code_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<code>", "</code>", TRUE);
}


void editor_dialog::on_toolbar_h1_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h1>", "</h1>", TRUE );
}


void editor_dialog::on_toolbar_h2_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h2>", "</h2>", TRUE );
}


void editor_dialog::on_toolbar_h3_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h3>", "</h3>", TRUE );
}


void editor_dialog::on_toolbar_h4_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h4>", "</h4>", TRUE );
}


void editor_dialog::on_toolbar_h5_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h5>", "</h5>", TRUE );
}


void editor_dialog::on_toolbar_h6_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<h6>", "</h6>", TRUE );
}


void editor_dialog::on_toolbar_small_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<small>", "</small>", TRUE );
}


void editor_dialog::on_toolbar_table_button( wxCommandEvent &event )
{
    table_dialog a_table_dialog( this );

    if ( a_table_dialog.ShowModal() == wxID_OK ) 
    {
        wxString starting_text;
        wxString ending_text;
        a_table_dialog.transfer_to( starting_text, ending_text );
        stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
    }
}


void editor_dialog::on_toolbar_tr_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<tr>", "</tr>", TRUE );
}


void editor_dialog::on_toolbar_th_button( wxCommandEvent &event )
{
    td_th_dialog a_td_th_dialog( this, optionTD_TH_DIALOG_IS_TH );

    if ( a_td_th_dialog.ShowModal() == wxID_OK )
    {
        wxString starting_text;
        wxString ending_text;
        a_td_th_dialog.transfer_to( starting_text, ending_text );
        stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
    }
}


void editor_dialog::on_toolbar_td_button( wxCommandEvent &event )
{
    td_th_dialog a_td_th_dialog( this, optionTD_TH_DIALOG_IS_TD );

    if ( a_td_th_dialog.ShowModal() == wxID_OK ) 
    {
        wxString starting_text;
        wxString ending_text;
        a_td_th_dialog.transfer_to( starting_text, ending_text );
        stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
    }
}


void editor_dialog::on_toolbar_ordered_list_button( wxCommandEvent &event )
{
    ol_dialog a_ol_dialog( this );

    if ( a_ol_dialog.ShowModal() == wxID_OK )
    {
        wxString starting_text;
        wxString ending_text;
        a_ol_dialog.transfer_to( starting_text, ending_text );
        stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
    }
}

void editor_dialog::on_toolbar_unordered_list_button( wxCommandEvent &event )
{
    ul_dialog a_ul_dialog( this );

    if ( a_ul_dialog.ShowModal() == wxID_OK ) 
    {
        wxString starting_text;
        wxString ending_text;
        a_ul_dialog.transfer_to( starting_text, ending_text );
        stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
    }
}

void editor_dialog::on_toolbar_list_element_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<li>", "</li>", TRUE );
}

void editor_dialog::on_toolbar_menu_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<menu>", "</menu>", TRUE );
}


void editor_dialog::on_toolbar_dt_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<dt>", "</dt>", TRUE );
}


void editor_dialog::on_toolbar_dd_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<dd>", "</dd>", TRUE );
}


void editor_dialog::on_toolbar_dl_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<dl>", "</dl>", TRUE );
}

    
void editor_dialog::on_toolbar_html_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<html>", "</html>", TRUE );
}


void editor_dialog::on_toolbar_meta_handheldfriendly_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( 
                       "<meta name=\"HandheldFriendly\" content=\"true\">", "", TRUE );
}


void editor_dialog::on_toolbar_body_button( wxCommandEvent &event )
{
   body_dialog a_body_dialog( this );   
    
   if ( a_body_dialog.ShowModal() == wxID_OK )
   {
       wxString starting_text;
       wxString ending_text;
       a_body_dialog.transfer_to( starting_text, ending_text );              
       stc_or_textctrl_insert_text( starting_text, ending_text, TRUE );
   }    
}


void editor_dialog::on_toolbar_comment_button( wxCommandEvent &event )
{
    stc_or_textctrl_insert_text( "<!--- ", " --->", FALSE );
}


// Override wxDialog's default behavior for clicking an OK button.
void editor_dialog::OnOK( wxCommandEvent& event )
{
    // Save to a file.
    stc_or_textctrl_save_file();
    
    // Get rid of the modal dialog. Not transferring any info from this modal's control
    // to a parent dialog, so don't have to bother with wxWindow::Validate or 
    // wxWindow::TransferDataFromWindow.    
    EndModal( wxID_OK );
}


void editor_dialog::on_help_button( wxCommandEvent &event )
{
#if ( setupUSE_ONLINE_HELP )
    help_controller::get()->show_help_topic( plkrHELP_ID_EDITOR_DIALOG );    
#endif 
}

//----------------------------------------------------------------------------------------
// End feature removal condition
//----------------------------------------------------------------------------------------

#endif // setupUSE_INTEGRATED_HTML_EDITOR 
