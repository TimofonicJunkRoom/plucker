/*
 * $Id: fiveway.c,v 1.15 2004/05/08 08:57:55 nordstrom Exp $
 *
 * Viewer - a part of Plucker, the free off-line HTML viewer for PalmOS
 * Copyright (c) 1998-2002, Mark Ian Lillywhite and Michael Nordstrom
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 */

#include "os.h"
#include "control.h"
#include "debug.h"
#include "prefsdata.h"
#include "util.h"
#include "rotate.h"
#include "libraryform.h"

#include "fiveway.h"

/***********************************************************************
 *
 *      Private variables
 *
 ***********************************************************************/



/* Handler to process any FiveWay requests in the library form */
Boolean FiveWayLibraryHandler
    (
    EventType* event  /* pointer to an EventType structure */
    )
{
    if ( FiveWayCenterPressed( event ) )
        return LibrarySelectorHandler( librarySelectorGo, 0 );
    if ( FiveWayDirectionPressed( event, Up ) )
        return LibrarySelectorHandler( librarySelectorUp, 0 );
    if ( FiveWayDirectionPressed( event, Down ) )
        return LibrarySelectorHandler( librarySelectorDown, 0 );
    if ( FiveWayDirectionPressed( event, Left ) )
        return LibrarySelectorHandler( librarySelectorLeft, 0 );
    if ( FiveWayDirectionPressed( event, Right ) )
        return LibrarySelectorHandler( librarySelectorRight, 0 );

    return false; 
}



/* Handler to process any FiveWay requests in the main form */
Boolean FiveWayMainHandler
    (
    EventType* event
    )
{
    Boolean handled;
    Boolean didLeft;
    Boolean didRight;
    Boolean didUp;
    Boolean didDown;

    handled = false;

    if ( ! Prefs()->arrowKeys || ! HaveFiveWay() || ! IsFiveWayEvent( event ))
    {
        return handled;
    }

    didLeft  = FiveWayKeyPressed( event, Left );
    didRight = FiveWayKeyPressed( event, Right );
    didUp    = FiveWayKeyPressed( event, Up );
    didDown  = FiveWayKeyPressed( event, Down );

    if ( RotSelect( didLeft, didDown, didUp ) &&
         Prefs()->arrowMode[ LEFT_ARROW ] != SELECT_NONE ) {
        DoSelectTypeAction( Prefs()->arrowMode[ LEFT_ARROW ] );
        handled = true;
    }
    else if ( RotSelect( didRight, didUp, didDown ) &&
         Prefs()->arrowMode[ RIGHT_ARROW ] != SELECT_NONE ) {
        DoSelectTypeAction( Prefs()->arrowMode[ RIGHT_ARROW ] );
        handled = true;
    }
    else if ( RotSelect( didUp, didLeft, didRight ) &&
         Prefs()->arrowMode[ UP_ARROW ] != SELECT_NONE ) {
        DoSelectTypeAction( Prefs()->arrowMode[ UP_ARROW ] );
        handled = true;
    }
    else if ( RotSelect( didDown, didRight, didLeft ) &&
         Prefs()->arrowMode[ DOWN_ARROW ] != SELECT_NONE ) {
        DoSelectTypeAction( Prefs()->arrowMode[ DOWN_ARROW ] );
        handled = true;
    }
    else if ( FiveWayCenterPressed( event ) &&
         Prefs()->arrowMode[ SELECT_ARROW ] != SELECT_NONE ) {
        DoSelectTypeAction( Prefs()->arrowMode[ SELECT_ARROW ]);
        handled = true;
    }

    return handled;
}

