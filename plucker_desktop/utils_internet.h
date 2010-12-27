//----------------------------------------------------------------------------------------
/*!
    \file       utils_internet.h        
    \modified                            
    \copyright  (c) Robert O'Connor ( rob@medicalmnemonics.com )    
    \licence    GPL
    \brief      Describes utils_internet class    
    \author     Robert O'Connor
    \date       2002/10/20    
 */  
// RCS-ID:      $Id: utils_internet.h,v 1.8 2003/03/17 17:43:21 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#ifndef _ADD_CHANNEL_WIZARD_H_
#define _ADD_CHANNEL_WIZARD_H_

//----------------------------------------------------------------------------------------
// GCC interface
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma interface "utils_internet.h"
#endif

//----------------------------------------------------------------------------------------
// Shared defines
//----------------------------------------------------------------------------------------

#include "plucker_defines.h"

//----------------------------------------------------------------------------------------
// Begin feature removal condition
//----------------------------------------------------------------------------------------

// This util is only used in the showcase preview pane, currently.
#if ( setupUSE_SHOWCASE_PREVIEW_PANE )

//----------------------------------------------------------------------------------------
// Static-only class
//----------------------------------------------------------------------------------------

//! A static-only class for commonly used functions of Internet access
class utils_internet
{

public:

    //! Downloads a url to a local file. 
    /*! 
        \param remote_url URL of the file to download.
        \param Local filename to install to. Can be a temp file.
        \param What proxy to use/ Syntas is <hostname>:<portnumber>. If wxEmptyString, 
            then ignored--don't use a proxy.
        \return TRUE if download was successful.
     */
    static bool     download_url_to_local_file( wxString remote_url,
                                                wxString local_filename,
                                                wxString proxy = wxEmptyString
                                              );
    
    //! Takes a string, and converts urls inside one tag parameter to absolute URLs.
    /*!
        \param content_string The input string that needs replacements.
        \param tag_parameter Parameter to look for, eg either href or src.
        \param original_url The url of the displayed document needing conversion.
     */ 
    static wxString to_absolute_urls( wxString content_string, 
                                      wxString tag_parameter,
                                      wxString original_url
                                    );                                      

};

//----------------------------------------------------------------------------------------
// End feature removal condition
//----------------------------------------------------------------------------------------

#endif  // setupUSE_SHOWCASE_PREVIEW_PANE

//----------------------------------------------------------------------------------------
// End single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#endif  //_UTILS_INTERNET_H_

