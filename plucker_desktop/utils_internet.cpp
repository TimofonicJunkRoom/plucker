//----------------------------------------------------------------------------------------
// Name:        utils_internet.cpp
// Purpose:     Utils for Internet
// Author:      Robert O'Connor
// Modified by:
// Created:     2001/10/20
// Copyright:   (c) Robert O'Connor ( rob@medicalmnemonics.com )
// Licence:     GPL
// RCS-ID:      $Id: utils_internet.cpp,v 1.10 2004/01/04 03:07:25 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// GCC implementation
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma implementation "utils_internet.h"
#endif

//----------------------------------------------------------------------------------------
// Setup information
//----------------------------------------------------------------------------------------

#include "setup.h"

//----------------------------------------------------------------------------------------
// Begin feature removal condition
//----------------------------------------------------------------------------------------

// This util is only used in the showcase preview pane, currently.
#if ( setupUSE_SHOWCASE_PREVIEW_PANE )

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

#include "utils_internet.h"

//----------------------------------------------------------------------------------------
// Remaining headers: Needed wx headers, then wx/contrib headers, then application headers
//----------------------------------------------------------------------------------------

#include "wx/wfstream.h"            // wxFFileOutputStream 
#include "wx/fs_inet.h"             // wxInternetFSHandler [wxHtmlWindow->LoadPage(http:)]

//----------------------------------------------------------------------------------------
// Non-event handler functions
//----------------------------------------------------------------------------------------

// Downloads a url to a local file. Returns true if successful, false if error
bool utils_internet::download_url_to_local_file(
     wxString   remote_url,       // URL of the file to download
     wxString   local_filename,   // Local filename to install to. Can be a temp file     
     wxString   proxy             // What proxy to use <hostname>:<portnumber>. 
                                  // If wxEmptyString, then ignored                                    
     )     
{         
    
    // Make a URL object from the location and abort if invalid
    wxURL url( remote_url );
    if ( url.GetError() != wxURL_NOERR )
    {
        wxLogDebug( "Error: couldn't parse URL" );
        return FALSE;
    }    
    
    // Set the default proxy for all future URLs, if the proxy argument isn't empty
    if ( ! proxy.IsEmpty() ) 
    {
        wxLogDebug( "About to set proxy" );  
        // Note SetDefaultProxy() is a static. SetProxy() isn't though      
        wxURL::SetDefaultProxy( proxy );
        wxLogDebug( "SetDefaultProxy=" + proxy );
    }
      
    wxYield();
    
    // Try to get the input stream (connects to the given URL)
    wxLogDebug( "Trying to establish connection..." );  
    wxInputStream *remote_datastream = url.GetInputStream();
    if ( ! remote_datastream )
    {
        wxLogDebug( "Error: couldn't read from URL" );
        // Get rid of the input stream object, since docs say an inputstream made
        // by wxURL.GetInputStream()  must be removed manually.
        delete remote_datastream;
        return FALSE;
    }    
    
    wxYield();     
    
    // Create a fileoutput stream to manage the stream, since Internet streams are
    // difficult since don't know the size of it, etc.
    wxFileOutputStream local_filestream( local_filename );
    if ( ! local_filestream.Ok() ) 
    {
        wxLogDebug( "Error: couldn't open file for output" );
        delete remote_datastream;
        return FALSE;
    }
    
    // This is a compact command that just dumps the input string to the file
    remote_datastream->Read( local_filestream );
    wxLogDebug( "Successfully wrote out data" );
    return TRUE;
}


// Takes a string of text and converts the urls within the tag parameter to absolute URLs
wxString utils_internet::to_absolute_urls
    (
    wxString content_string,    // The input string that needs replacements
    wxString tag_parameter,     // Parameter to look for, eg either href or src
    wxString original_url       // The url of the displayed document needing conversion
    )
{
    // Declaration of shortcuts to components of the original_url.
    wxString protocol;          // Example: "http:" [includes the ':' at the end]
    wxString full_server_path;  // Example: "http://www.plkr.org/subdir"
    wxString root_server_path;  // Example: "http://www.plkr.org"
    wxString host;              // Example: "www.plkr.org"        
    
    // Declaration of items to handle ../ replacements
    long subdirectory_depth = 0; // Number of subdirectories down from root in full server path 
    wxString cdup_string;        // Will hold a substring like "../../" or "../../../", etc
    long cdup_counter;           // A counter for cdup replacements.
    wxString subdirectory_to_replace; // Starts as full_server_path and is shaved during replacements

    // Declaration of items to do main replacement loop ( addresses both src='' and src="" )   
    wxArrayString  parameter_delimiters;                        // Array of delimiters, either ' or "  
    wxString delimiter;                                         // The delimiter to currently process
    wxString parameter_uppercase = tag_parameter;               // Uppercase = not processed yet
    wxString parameter_lowercase = tag_parameter;               // Lowercase = processed
    long main_counter;                                          // A counter to loop ' and " replacements

    // Initialize the shortcuts to what they should be
    protocol    = original_url.BeforeFirst( '/' );              // protocol is now http:
    host        = original_url.AfterFirst( '/' );               // host is now /www.plkr.org/news/about.htm
    host        = host.AfterFirst( '/' );                       // host is now www.plkr.org/news/about.htm
    // If location was a full path to a file
    if ( host.Contains( wxT( '/' ) ) ) 
    {          
        full_server_path        = host.BeforeLast( '/' );       // full_server_path is now www.plkr.org/news 
        host                    = host.BeforeFirst( '/' );      // host is now www.plkr.org
    // Else location was just a domain name, without trailing slash.
    } 
    else 
    {
        full_server_path = host;                                // addresses case of a top level dir.
    }
    subdirectory_to_replace = full_server_path;                 // subdirectory_to_replace is now www.plkr.org/news
    subdirectory_depth      = full_server_path.Freq('/');       // depth is 1 directory down from root 
    
    root_server_path = protocol + '/' + '/' + host;             // root_server_path is now http://wwww.plkr.org
    full_server_path = protocol + '/' + '/' + full_server_path; // full_server_path is now http://www.plkr.org/news
    
    // Initialize parameter_uppercase and paramter_lowercase to proper cases.
    parameter_uppercase.UpperCase(); // Uppercase = not processed yet
    parameter_lowercase.LowerCase(); // Lowercase = processed     
      
    // Initialize our our list of delimiters, which are ' and "
    parameter_delimiters.Add( wxT( '\'' ) );
    parameter_delimiters.Add( wxT( '\"' ) );    
    
    // Tell us how things got set up
    wxLogDebug( "About to parse tag_parameter of '%s'", tag_parameter.c_str() ); 
    wxLogDebug( "Initialization of full_server_path='%s'", full_server_path.c_str() ); 
    wxLogDebug( "Initialization of root_server_path='%s'", root_server_path.c_str() ); 
    wxLogDebug( "Initialization of protocol='%s'", protocol.c_str() ); 
    wxLogDebug( "Initialization of host='%s'", host.c_str() ); 
    wxLogDebug( "Initialization of subdirectory_to_replace='%s'", subdirectory_to_replace.c_str() );     
    wxLogDebug( "Initialization of subdirectory_depth='%i'", subdirectory_depth ); 
    
    wxLogDebug( "parameter_delimiters.GetCount=%l", (long) parameter_delimiters.GetCount() ); 
    // Loop through the replacements for each of our delimiters
    for ( main_counter = 0;
          main_counter < (long) parameter_delimiters.GetCount();
          main_counter++ ) 
    {        
        delimiter = parameter_delimiters.Item( main_counter );
        wxLogDebug( "Entered main_loop.delimiter=' %s '", delimiter.c_str() ); 
        
        // No sense continuing if the delimiter not in the line 
        if ( ! content_string.Contains( delimiter ) ) 
        {
            wxLogDebug( "No delimiter in string so aborting main loop" ); 
            continue;
        }
        
        // First, put all src to uppercase, and of a common format so can replace them easier. 
        // Also, could have multiple src tags in the line we are dealing with, so lowercase
        // marking is a way to skip over the cases already dealt with.
        // We are leaving out the cases of: (1) giant spaces between src and its param
        // and (2) a src and param that are on different lines (3) case mixing like 'sRc'
        content_string.Replace( wxT( parameter_lowercase ), 
            wxT( parameter_uppercase ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() ); 
        
         // No sense continuing if there is now no parameter in uppercase in this line
        if ( ! content_string.Contains( parameter_uppercase ) ) 
        {
            wxLogDebug( "No uppercase '%s' tag in string so aborting main loop", parameter_uppercase.c_str() );
            continue;
        }
        
        content_string.Replace( wxT( parameter_uppercase + " =" ),
            wxT( parameter_uppercase + '=' ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() );
         
        content_string.Replace( wxT( parameter_uppercase + "= " ),
            wxT( parameter_uppercase + '=' ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() ); 
        
        // Replace src="//images.slashdot.org" with a src="http://images.slashdot.org
        content_string.Replace( wxT( parameter_uppercase + '=' + delimiter + "//" ),
            wxT( parameter_lowercase + '=' + delimiter + protocol + "//" ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() );
        
        // Replace src="/the_file" with src="http://www.plkr.org/the_file        
        content_string.Replace( wxT( parameter_uppercase + '=' + delimiter + '/' ), 
            wxT( parameter_lowercase + '=' + delimiter + root_server_path + '/' ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() ); 
        
        // Mark the http ones as done, as they don't need changing, since 
        // already of the format src="http://www.plkr.org/the_file"
        content_string.Replace( wxT( parameter_uppercase + '=' + delimiter + "http" ),
            wxT( parameter_lowercase + '=' + delimiter + "http" ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() ); 
        
        // To deal with ../ and ../../ replacements we will loop through the number of 
        // depths the current file is down off root, so that all possiblities are covered,
        // but not looking more than actually possible. First make a cdup_string the right
        // length so we can shave, since want to replace ../../ first, then ../ after.
        // Will be shaving the directories as climb, to do the replacements. 
        cdup_string = "../";        
        for ( cdup_counter = 1; cdup_counter < subdirectory_depth; cdup_counter++ ) 
        {
            cdup_string += "../";
        }
        wxLogDebug( "cdup_string initially='%s'", cdup_string.c_str() ); 
        for ( cdup_counter = 1; cdup_counter <= subdirectory_depth; cdup_counter++ ) 
        {
            content_string.Replace( wxT( parameter_uppercase + '=' + delimiter + cdup_string ),
            wxT( parameter_lowercase + '=' + delimiter + protocol + "//" + subdirectory_to_replace + '/' ), TRUE );
            // Shave a directory (../) of the string to replace, and a directory off the 
            // subdirectory_to_replace string, to be ready for the next replacement.  
            cdup_string = cdup_string.AfterFirst( wxT( '/' ) );
            wxLogDebug( "subdirectory_to_replace='%s'", cdup_string.c_str() );
            wxLogDebug( "cdup_string in loop='%s'", cdup_string.c_str() ); 
            subdirectory_to_replace = subdirectory_to_replace.BeforeLast( wxT( '/' ) );
        }
        wxLogDebug( "content_string='%s'", content_string.c_str() ); 
                
        // Now just subdir files and paths in the same directory that file was on.
        // The former didn't work, later did. Instead of telling apart, replace them all,
        // Need to replace with the full server path to the file.
        content_string.Replace( wxT( parameter_uppercase + '=' + delimiter ), 
            wxT( parameter_lowercase + '=' + delimiter + full_server_path + '/' ), TRUE );
        wxLogDebug( "content_string='%s'", content_string.c_str() );
    
    }
    
    // Clear memory occupied by array
    parameter_delimiters.Clear(); 
    return content_string;
}


//----------------------------------------------------------------------------------------
// End feature removal condition
//----------------------------------------------------------------------------------------

#endif  // setupUSE_SHOWCASE_PREVIEW_PANE
