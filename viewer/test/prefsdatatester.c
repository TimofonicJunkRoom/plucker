/*
 * $Id: prefsdatatester.c,v 1.4 2004/01/01 08:53:08 nordstrom Exp $
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

#include "TestDB.h"
#include "prefsdata.h"
#include "const.h"
#include "util.h"

#include "prefsdatatester.h"

#define ViewerPrefID                4
#define ViewerOldPrefID             2

#define ViewerVersion               3
#define ViewerVersionV2             2
#define ViewerVersionV1             1


typedef struct {
    Char                docName[ dmDBNameLength ];
    UInt16              cardNo;
    UInt16              location;
    UInt16              categories;
    UInt16              lastForm;
    UInt16              screenDepth;
    UInt16              searchEntries;
    UInt16              toolbarButton;
    UInt16              autoscrollInterval;
    UInt16              autoscrollLastScrollTime;
    UInt16              autoscrollJump;
    Boolean             autoscrollEnabled;
    Boolean             autoscrollStayOn;
    Boolean             strikethrough;
    Boolean             linkClick;
    Boolean             multipleSelect;
    Boolean             underlineMode;
    SearchModeType      searchMode;
    UInt8               searchFlags;
    Boolean             hardKeys;
    Boolean             gestures;
    SyncPolicyType      syncPolicy;
    AutoscrollDirType   autoscrollDir;
    AutoscrollModeType  autoscrollMode;
    ColumnFlags         column;
    FilterType          filterMode;
    FontModeType        fontModeMain;
    ModeType            controlMode;
    ScrollbarType       scrollbar;
    SortOrderType       sortOrder;
    SortType            sortType;
    ToolbarType         toolbar;
    SelectType          hwMode[ NUM_OF_HW_BUTTONS ];
    SelectType          gestMode[ NUM_OF_GESTURES ];
    SelectType          select[ NUM_OF_CONTROL_MODES ][ NUM_OF_CONTROL_LISTS ];
} Preferences_V1;


typedef struct {
    Char                docName[ dmDBNameLength ];
    UInt16              cardNo;
    UInt16              location;
    UInt16              categories;
    UInt16              lastForm;
    UInt16              screenDepth;
    UInt16              searchEntries;
    UInt16              toolbarButton;
    UInt16              autoscrollInterval;
    UInt16              autoscrollLastScrollTime;
    UInt16              autoscrollJump;
    Boolean             autoscrollEnabled;
    Boolean             autoscrollStayOn;
    Boolean             strikethrough;
    Boolean             linkClick;
    Boolean             multipleSelect;
    Boolean             underlineMode;
    SearchModeType      searchMode;
    UInt8               searchFlags;
    Boolean             hardKeys;
    Boolean             gestures;
    SyncPolicyType      syncPolicy;
    AutoscrollDirType   autoscrollDir;
    AutoscrollModeType  autoscrollMode;
    ColumnFlags         column;
    FilterType          filterMode;
    FontModeType        fontModeMain;
    ModeType            controlMode;
    ScrollbarType       scrollbar;
    SortOrderType       sortOrder;
    SortType            sortType;
    ToolbarType         toolbar;
    SelectType          hwMode[ NUM_OF_HW_BUTTONS ];
    SelectType          gestMode[ NUM_OF_GESTURES ];
    SelectType          select[ NUM_OF_CONTROL_MODES ][ NUM_OF_CONTROL_LISTS ];
/* new items in V2 */
    SelectType          jogMode[ NUM_OF_JOGEVENTS ];
    Boolean             jogEnabled;
    SilkScreenStatus    previousSilkStatus;
    FontModeType        fontModeLibrary;
    CategoryStyleType   categoryStyle;
    Boolean             enableSoftHyphens;
    HardcopyActionType  hardcopyAction;
    HardcopyRangeType   hardcopyRange;
    HardcopyLinkType    hardcopyLink;
    Int16               lineSpacing;
    Int16               paragraphSpacing;
    SelectType          arrowMode[ NUM_OF_ARROW_BUTTONS ];
    Boolean             arrowKeys;
    Boolean             dynamicScrollbar;
    Boolean             visualAid;
} Preferences_V2;



static Boolean      hasStoredPrefs;
static Preferences  originalPref;


/* Remove preferences from the Preferences database */
static void RemovePref
    (
    UInt16 prefID   /* preference to remove */
    )
{
    DmOpenRef           ref;
    DmSearchStateType   state;
    LocalID             dbID;
    UInt16              cardNo;
    Int16               index;

    DmGetNextDatabaseByTypeCreator( true, &state, 'sprf', 'psys', false,
        &cardNo, &dbID );
    ref = DmOpenDatabase( cardNo, dbID, dmModeReadWrite );
    if ( ref == NULL )
        return;

    index = DmFindResource( ref, ViewerAppID, prefID, NULL );
    if ( index != -1 )
        DmRemoveResource( ref, index );

     DmCloseDatabase( ref );
}



int PrefsDataSuiteSetUp( void )
{
    Int16   version;
    UInt16  prefSize;

    /* Store original preference structure */
    prefSize    = 0;
    version     = PrefGetAppPreferences( (UInt32) ViewerAppID,
                    (UInt16) ViewerPrefID, NULL, &prefSize, true);

    if ( version != noPreferenceFound && prefSize == sizeof( Preferences ) ) {
        PrefGetAppPreferences( (UInt32) ViewerAppID,
            (UInt16) ViewerPrefID, &originalPref, &prefSize, true);
        hasStoredPrefs = true;
    }
    else {
        hasStoredPrefs = false;
    }

    return 0;
}



int PrefsDataSuiteTearDown( void )
{
    /* Restore original preference structure */
    if ( hasStoredPrefs )
    {
        PrefSetAppPreferences( (UInt32) ViewerAppID, (UInt16) ViewerPrefID,
            (Int16) ViewerVersion, &originalPref, sizeof( Preferences ), true );
    }
    MemSet( &originalPref, sizeof( Preferences ), 0 );

    return 0;
}



int PrefsDataInitTest( void )
{
    RemovePref( ViewerPrefID );
    return 0;
}



int PrefsDataCleanupTest( void )
{
    return 0;
}



void TestPrefsDataInit( void )
{
    /* Change a known item to make sure it is updated */
    Prefs()->controlMode = MODE3;

    /* Since we don't have any preference structure calling ReadPrefs
       should create a new structure, i.e. the controlMode item should
       be set to its default value (MODE1) */
    ReadPrefs();

    ASSERT( Prefs()->controlMode == MODE1 );
}



void TestPrefsDataRead( void )
{
    Preferences testPref;

    /* Add known preference structure */
    MemSet( &testPref, sizeof( Preferences ), 0 );
    testPref.controlMode = MODE2;
    PrefSetAppPreferences( (UInt32) ViewerAppID, (UInt16) ViewerPrefID,
        (Int16) ViewerVersion, &testPref, sizeof( Preferences ), true );

    /* Read it back and check that the item has the correct value */
    ReadPrefs();
    ASSERT( Prefs()->controlMode == MODE2 );
}



void TestPrefsDataReadOld( void )
{
    OldPreferences  oldPref;
    Int16           version;
    UInt16          prefSize;

    /* Add known old preference structure */
    MemSet( &oldPref, sizeof( OldPreferences ), 0 );
    oldPref.showDate        = true;
    oldPref.controlMode     = MODE2;
    oldPref.doManualSync    = true;
    PrefSetAppPreferences( (UInt32) ViewerAppID, (UInt16) ViewerOldPrefID,
        (Int16) ViewerVersion, &oldPref, sizeof( OldPreferences ), true );

    /* When the old preference structure is found it should be converted
       to the new format and then the old structure should be removed */
    ReadPrefs();
    ASSERT( Prefs()->column.date == SHOW );
    ASSERT( Prefs()->controlMode == MODE2 );
    ASSERT( Prefs()->syncPolicy == SYNC_MANUAL );

    /* Check that the old preference structure is removed */
    prefSize    = 0;
    version     = PrefGetAppPreferences( (UInt32) ViewerAppID,
                    (UInt16) ViewerOldPrefID, NULL, &prefSize, true);

    ASSERT( version == noPreferenceFound );
}



void TestPrefsDataReadV1( void )
{
    Preferences_V1  prefV1;
    Int16           version;
    UInt16          prefSize;

    /* Add known preference structure of version 1 format */
    MemSet( &prefV1, sizeof( Preferences_V1 ), 0 );
    StrCopy( prefV1.docName, "UnitTest" );
    prefV1.screenDepth  = 8;
    prefV1.select[ MODE2 ][ 2 ] = SELECT_PREFS;
    PrefSetAppPreferences( (UInt32) ViewerAppID, (UInt16) ViewerPrefID,
        (Int16) ViewerVersionV1, &prefV1, sizeof( Preferences_V1 ), true );

    /* When the older version of the preference structure is found default
       values should be added for the new items */
    ReadPrefs();

    /* these items should be left intact since they are included in
       version 1 */
    ASSERT( STREQ( Prefs()->docName, "UnitTest" ) );
    ASSERT( Prefs()->screenDepth == 8 );
    ASSERT( Prefs()->select[ MODE2 ][ 2 ] == SELECT_PREFS );

    /* check new items in version 2 */
    ASSERT( Prefs()->column.type == SHOW );
    ASSERT( Prefs()->categoryStyle == CATEGORY_ADVANCED );
    ASSERT( Prefs()->visualAid );
}



void TestPrefsDataReadV2( void )
{
    Preferences_V2  prefV2;
    Int16           version;
    UInt16          prefSize;

    /* Add known preference structure of version 2 format */
    MemSet( &prefV2, sizeof( Preferences_V2 ), 0 );
    prefV2.column.type    = HIDE;
    prefV2.categoryStyle  = CATEGORY_CLASSIC;
    prefV2.visualAid      = false;
    PrefSetAppPreferences( (UInt32) ViewerAppID, (UInt16) ViewerPrefID,
        (Int16) ViewerVersionV2, &prefV2, sizeof( Preferences_V2 ), true );

    /* When the older version of the preference structure is found default
       values should be added for the new items */
    ReadPrefs();

    /* these items should be left intact since they are included in
       version 2 */
    ASSERT( Prefs()->column.type == HIDE );
    ASSERT( Prefs()->categoryStyle == CATEGORY_CLASSIC );
    ASSERT( ! Prefs()->visualAid );

    /* check new items in version 3 */
    ASSERT( ! Prefs()->indicateOpened );
    ASSERT( Prefs()->forceAlign == FORCE_ALIGN_NONE );
    ASSERT( Prefs()->defaultRotate == ROTATE_ZERO );
}



void TestPrefsDataWrite( void )
{
    Preferences testPref;
    Int16       version;
    UInt16      prefSize;

    /* Write known preference structure */
    MemSet( Prefs(), sizeof( Preferences ), 0 );
    Prefs()->controlMode = MODE2;
    WritePrefs();

    /* Check that the correct preference structure was stored */
    prefSize    = 0;
    version     = PrefGetAppPreferences( (UInt32) ViewerAppID,
                    (UInt16) ViewerPrefID, NULL, &prefSize, true);

    if ( version != noPreferenceFound && prefSize == sizeof( Preferences ) ) {
        PrefGetAppPreferences( (UInt32) ViewerAppID,
            (UInt16) ViewerPrefID, &testPref, &prefSize, true);
    }

    ASSERT( testPref.controlMode == MODE2 );
}



void InitializePrefsDataSuite( void )
{
    TestGroup* group;

    /* Preference Data Tester */
    group = add_test_group( "PrefsData", PrefsDataSuiteSetUp,
                PrefsDataSuiteTearDown, PrefsDataInitTest,
                PrefsDataCleanupTest );
    add_test_case( group, "Init", TestPrefsDataInit );
    add_test_case( group, "Read", TestPrefsDataRead );
    add_test_case( group, "Read (V1)", TestPrefsDataReadV1 );
    add_test_case( group, "Read (V2)", TestPrefsDataReadV2 );
    add_test_case( group, "Read (old)", TestPrefsDataReadOld );
    add_test_case( group, "Write", TestPrefsDataWrite );
}



void TerminatePrefsDataSuite( void )
{
}

