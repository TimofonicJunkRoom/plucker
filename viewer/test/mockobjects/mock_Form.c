/*
 * $Id: mock_Form.c,v 1.2 2004/01/05 15:58:37 nordstrom Exp $
 *
 * Viewer - a part of Plucker, the free off-line HTML viewer for PalmOS
 * Copyright (c) 1998-2003, Mark Ian Lillywhite and Michael Nordstrom
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

#include "mock_Form.h"

static UInt16 gotoFormId;


void mock_FrmGotoForm( UInt16 formId )
{
    gotoFormId = formId;
}



UInt16 mock_GetFrmGotoForm( void )
{
    UInt16 formID;

    formID      = gotoFormId;
    gotoFormId  = 0;

    return formID;
}

