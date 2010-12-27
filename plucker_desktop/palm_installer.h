//----------------------------------------------------------------------------------------
/*!
    \file       palm_installer.h        
    \modified                            
    \copyright  (c) Robert O'Connor ( rob@medicalmnemonics.com )    
    \licence    GPL
    \brief      Includes a correct platform-specific include file for palm_installer class    
    \author     Robert O'Connor
    \date       2002/10/20    
 */  
// RCS-ID:      $Id: palm_installer.h,v 1.6 2003/06/18 03:00:40 robertoconnor Exp $
//----------------------------------------------------------------------------------------

#ifndef _PALM_INSTALLER_H_
#define _PALM_INSTALLER_H_

#ifdef __WXMSW__ 
    #include "palm_installer_msw.h"
#endif

#ifdef __WXMAC__
	#include "palm_installer_mac.h"
#endif

#if defined(__WXGTK__) || defined(__WXX11__) || defined(__WXMOTIF__)
    #include "palm_installer_gtk.h"
#endif

#endif  //PALM_INSTALLER_H

