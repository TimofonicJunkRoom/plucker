//----------------------------------------------------------------------------------------
/*!
    \file       bookmark_dialog.h        
    \modified                            
    \copyright  (c) Robert O'Connor ( rob@medicalmnemonics.com )    
    \licence    GPL
    \brief      Describes bookmark_dialog class    
    \author     Robert O'Connor
    \date       2002/01/03    
 */  
// RCS-ID:      $Id: bookmark_dialog.h,v 1.6 2003/03/17 17:43:19 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#ifndef _BOOKMARK_DIALOG_H_
#define _BOOKMARK_DIALOG_H_

//----------------------------------------------------------------------------------------
// GCC interface
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma interface "bookmark_dialog.h"
#endif

//----------------------------------------------------------------------------------------
// Shared defines
//----------------------------------------------------------------------------------------

#include "plucker_defines.h"

//----------------------------------------------------------------------------------------
// Begin feature removal condition
//----------------------------------------------------------------------------------------

#if ( setupUSE_INTEGRATED_HTML_EDITOR )

// ---------------------------------------------------------------------------------------
// Headers
// ---------------------------------------------------------------------------------------

#include "wx/dialog.h"

//----------------------------------------------------------------------------------------
// Class definition: bookmark_dialog
//----------------------------------------------------------------------------------------

//! A dialog for a "a name=" tag for the HTML editor.
class bookmark_dialog : public wxDialog
{

public:

    //! Constructor.
    /*!
        \param parent The parent window.
     */
    bookmark_dialog( wxWindow* parent );
    
    //!Destructor.
    ~bookmark_dialog();
   
    //! Gets the tag strings generated by the dialog, ready to insert by parent
    /*!
        \param starting_text The text to place at the start of parent's selected text.private: 
        \param ending_text The text to place at the end of parent's selected text.  
     */
    void transfer_to( wxString& starting_text,
                      wxString& ending_text
                    );

private:

    void on_help_button( wxCommandEvent& event );  
    //Override base class functions
    void OnOK( wxCommandEvent& event );
    
    wxString m_starting_text;
    wxString m_ending_text;

    DECLARE_EVENT_TABLE()
};

//----------------------------------------------------------------------------------------
// End feature removal condition
//----------------------------------------------------------------------------------------

#endif // setupUSE_INTEGRATED_HTML_EDITOR 

//----------------------------------------------------------------------------------------
// End single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#endif  //_BOOKMARK_DIALOG_H_

