/*
 * $Id: armlets.h,v 1.2 2003/06/14 04:19:06 adamm Exp $
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

#ifndef PLUCKER_ARMLETS_H
#define PLUCKER_ARMLETS_H

#include <PceNativeCall.h>
#include <PalmTypes.h>
#include <Standalone.h>

#include "../resourceids.h"

#define READUNALIGNED32(x) ( ( ( ( UInt8* )(x) ) [0] << 24 ) \
                            | ( ( ( UInt8* )(x) ) [1] << 16 ) \
                            | ( ( ( UInt8* )(x) ) [2] << 8 ) \
                            | ( ( ( UInt8* )(x) ) [3] ) )

#define SWAP16(x) ( ( (UInt16)(x) << 8 ) | ( (UInt16)(x) >> 8 ) )

#endif

