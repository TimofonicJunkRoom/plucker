//----------------------------------------------------------------------------------------
/*!
    \file       channel_type.h        
    \modified                            
    \copyright  (c) Robert O'Connor ( rob@medicalmnemonics.com )    
    \licence    GPL
    \brief      Describes channel_type class    
    \author     Robert O'Connor
    \date       2002/10/20    
 */  
// RCS-ID:      $Id: channel_type.h,v 1.7 2003/03/17 17:43:19 robertoconnor Exp $
//----------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------
// Begin single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#ifndef _CHANNEL_TYPE_H_
#define _CHANNEL_TYPE_H_

//----------------------------------------------------------------------------------------
// GCC interface
//----------------------------------------------------------------------------------------

#if defined(__GNUG__) && ! defined(__APPLE__)
    #pragma interface "channel_type.h"
#endif

//----------------------------------------------------------------------------------------
// Shared defines
//----------------------------------------------------------------------------------------

#include "plucker_defines.h"

//----------------------------------------------------------------------------------------
// Headers
//----------------------------------------------------------------------------------------

#include "wx/datetime.h"

//----------------------------------------------------------------------------------------
// Class definition: channel_type
//----------------------------------------------------------------------------------------

//! A class to hold a description of a channel.
/*!
    Members name and role match up with commandline plucker and the plucker
    configuration file names, where applicable.

    All public members, so can just get/set one whenever desired.

    \note Bools are longs, so that they can store a -1 value.
    \todo Add anchor_color parameter, and any others.
 */
class channel_type  :  public wxObject
{
    DECLARE_DYNAMIC_CLASS( channel_type )

public:

    //! Constructor.
    /*!
        \param initialize_to_illegal_values TRUE to put an illegal value in each
            member (such a -1 in a switch that is usually 0 or 1. This allows us to
            track whether each item was actually set by something to 0 or 1--
            otherwise wouldn't know if it started at 0, or something specifically
            said that is should be 0.
     */
    channel_type( bool initialize_to_illegal_values = TRUE );

    //! Destructor
    ~channel_type();


    //! The channels section in the section in config file stored.
    /*!
        Will also be the directory name in the /channels directory that the channel is
        stored in.
     */
    wxString    configuration_section;

    //! Name of the channel, as appearing in main_listctrl and the viewer's library.
    wxString    doc_name;

    //! The starting page of the channel.
    wxString    home_url;

    //! TRUE to spider depth-first instead of breadth first.
    long        depth_first;

    //! Amount of messages generated by the parser.
    long        verbosity;

    // Number of characters in a URL status line in the stderr progress, before truncated.
    long        status_line_length;

    //! Command executed before parsing this channel.
    wxString    command_before;

    //! Command execute after parsing this channel.
    wxString    command_after;

    //! Maximum depth to spider. 1=starting page only.
    long        home_maxdepth;

    //! TRUE is to not leave the hostname of the starting page.
    long        home_stayonhost;

    //! TRUE is to not leave the second level domain of the starting page.
    long        home_stayondomain;

    //! A regex pattern that must be matched for the page/image to be retreived.
    wxString    home_url_pattern;

    //! Character set of the channel.
    wxString    charset;

    //! TRUE if paragraphs should be indented
    long        indent_paragraphs;

    //! Image depth for images. 0=no images.
    long        bpp;

    //! TRUE to include alt tags for images not included in the output.
    long        alt_text;

    //! Mamimum height of an inline image (if higher than this, optionally linked to fullsize.
    long        maxheight;

    //! Mamimum width of an inline image (if higher than this, optionally linked to fullsize.
    long        maxwidth;

    //! Mamimum height of an fullsize image that is linked from its thumbnail.
    long        alt_maxheight;

    //! Mamimum width of an fullsize image that is linked from its thumbnail.
    long        alt_maxwidth;

    //! Image file size must exceed this number of bytes for it to warrant oompression.
    long        image_compression_limit;

    //! Images must be <60000 bytes to fit in viewer. TRUE to reduce bpp to fit under 60000 bytes.
    long        try_reduce_bpp;

    //! Images must be <60000 bytes to fit in viewer. TRUE to reduce dimensions to fit 60000 bytes.
    /*!
        Can only use try_reduce_bpp or try_reduce_dimension. If specify both, only one will be
        used.
     */
    long        try_reduce_dimension;

    //! Type of compression, either doc, or zlib.
    wxString    compression;

    //! Delimited string array of categories for output file to be placed, in handheld viewer.
    wxString    category;

    //! TRUE to remove the url info from each page for security and/or smaller size.
    long        no_urlinfo;

    //! A security measure for documents, owner_id_build must match the viewer's username.
    wxString    owner_id_build;

    //! TRUE to prevent beaming of the output among handhelds.
    long        copyprevention_bit;

    //! TRUE to archive a copy of the output from viewer onto the desktop computer.
    long        backup_bit;

    //! TRUE to make the output launchable from the viewer's Launcher screen.
    long        launchable_bit;

    //! Fullname to an icon file to overrride the default large icon in Launcher.
    /*!
        Only noticable if launchable_bit=TRUE. Incorrectly sized bitamps
        will be resized as needed.
     */
    wxString    big_icon;

    //! Fullname to an icon file to overrride the default large icon in Launcher.
    /*!
        Only noticable if launchable_bit=TRUE. Incorrectly sized bitamps
        will be resized as needed.
     */
    wxString    small_icon;

    //! Fullname of the output file.
    /*!
        May include a plucker: protocol prefix to specify relative location to
        pluckerhome.

        Currently autogenerated by Plucker Desktop as to be the same as the section
        name, to prevent clashes of similarly named files when try to put two files
        of the same name onto the handheld, since on MSW/Mac they are queued into
        a certain directory.
     */
    wxString    doc_file;

    //! Handheld username to install the output to during a sync (if OS supports it).
    wxString    user;

    //! Directory to copy output to.
    wxString    copy_to_dir;
                     
    //! The number of periods between each update. Example: the '3' in '3 days'
    long        update_frequency;
    
    //! The period of updates. Example: the 'days' in '3 days'.
    wxString    update_period;
    
    //! A base datetime at which start counting update_periods from, to calculate if due.
    wxDateTime  update_base;
    
    //! TRUE if updates are enabled for this channel.
    long        update_enabled;
    
};

//----------------------------------------------------------------------------------------
// End single inclusion of this .h file condition
//----------------------------------------------------------------------------------------

#endif  //_CHANNEL_TYPE_H_
