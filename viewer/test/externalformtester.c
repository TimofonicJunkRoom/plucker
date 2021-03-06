/*
 * $Id: externalformtester.c,v 1.13 2004/02/21 16:31:33 chrish Exp $
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
#include "genericfile.h"
#include "externalform.h"
#include "resourceids.h"
#include "util.h"
#include "testutil.h"
#include "mock_Form.h"
#include "mock_Device.h"
#include "mock_Database.h"
#include "prefsdata.h"
#include "prefsdatatester.h"

#include "externalformtester.h"


static DocumentInfo docInfo;


/* Expected data for first record in MemoDB. We start from position 29 since
   we don't know the exact format of the header (it depends on the date and
   time) */
static UInt16 Memo1Size = 31;
static UInt8* Memo1Data = "
Test message 1
Test message 2
 ";



int ExternalFormSuiteSetUp( void )
{
    /* Check that we have access to the test documents we require */
    if ( DmFindDatabase( 0, "UnitTest" ) == 0 )
        return 1;

    return OpenTestDocument( "UnitTest", &docInfo );
}



int ExternalFormSuiteTearDown( void )
{
    CloseTestDocument( "UnitTest" );

    return 0;
}



int ExternalFormInitTest( void )
{
    return 0;
}



int ExternalFormCleanupTest( void )
{
    return 0;
}



void TestExternalFormInit( void )
{
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );
}



void TestExternalFormLinksBackButton( void )
{
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );
    FrmSetEventHandler( externalForm, ExternalLinksFormHandleEvent );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType                     = ctlSelectEvent;
    event.data.ctlEnter.controlID   = frmExternalLinksBack;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );
    ASSERT_GOTOERR( mock_GetFrmGotoForm() == GetMainFormId() );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );
}



void TestExternalFormLinksGesturesBack( void )
{
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    /* Store original preference structure */
    PrefsDataSuiteSetUp();

    /* Set backspace gesture to Go Back */
    ReadPrefs();
    Prefs()->gestures                   = true;
    Prefs()->gestMode[ GESTURES_LEFT ]  = SELECT_GO_BACK; 
    WritePrefs();

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );
    FrmSetEventHandler( externalForm, ExternalLinksFormHandleEvent );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType             = keyDownEvent;
    event.data.keyDown.chr  = backspaceChr;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );
    ASSERT_GOTOERR( mock_GetFrmGotoForm() == GetMainFormId() );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );

    /* Restore preference structure */
    PrefsDataSuiteTearDown();
}



void TestExternalFormLinksJogDialBack( void )
{
#ifdef HAVE_JOGDIAL
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );
    FrmSetEventHandler( externalForm, ExternalLinksFormHandleEvent );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType             = keyDownEvent;
    event.data.keyDown.chr  = vchrJogBack;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );
    ASSERT_GOTOERR( mock_GetFrmGotoForm() == GetMainFormId() );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );
#endif
}



void TestExternalFormLinksFiveWayBack( void )
{
#ifdef HAVE_FIVEWAY_SDK
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    /* Store original preference structure */
    PrefsDataSuiteSetUp();

    /* Set left "arrow" to Go Back */
    ReadPrefs();
    Prefs()->arrowKeys                  = true;
    Prefs()->arrowMode[ LEFT_ARROW ]    = SELECT_GO_BACK; 
    WritePrefs();

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );
    FrmSetEventHandler( externalForm, ExternalLinksFormHandleEvent );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType                     = keyDownEvent;
    event.data.keyDown.chr          = vchrRockerLeft;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ));
    ASSERT_GOTOERR( mock_GetFrmGotoForm() == GetMainFormId() );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );

    /* Restore preference structure */
    PrefsDataSuiteTearDown();
#endif
}



void TestExternalFormLinksCopy( void )
{
    FormType* oldForm;
    FormType* externalForm;
    EventType event;

    oldForm = FrmGetActiveForm();

    externalForm = FrmInitForm( frmExternalLinks );
    FrmSetActiveForm( externalForm );
    FrmSetEventHandler( externalForm, ExternalLinksFormHandleEvent );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType = frmOpenEvent;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );

    MemSet( &event, sizeof( EventType ), 0 );
    event.eType                     = ctlSelectEvent;
    event.data.ctlEnter.controlID   = frmExternalLinksCopy;
    ASSERT_GOTOERR( ExternalLinksFormHandleEvent( &event ) );
    ASSERT_GOTOERR( mock_GetFrmGotoForm() == GetMainFormId() );

ERR:
    FrmSetActiveForm( oldForm );
    FrmEraseForm( externalForm );
    FrmDeleteForm( externalForm );
}



void TestExternalFormWriteText( void )
{
    Char*       msg1    = "Test message 1";
    Char*       msg2    = "Test message 2";
    Char*       data    = NULL;
    MemHandle   handle  = NULL;
    DmOpenRef   dbRef   = NULL;
    Err         err;

    /* Create a 'fake' MemoDB for the test */
    err = CreateFakeMemoDB();
    if ( err != errNone ) {
        DeleteFakeMemoDB();
        err = CreateFakeMemoDB();
        ASSERT_UINT16_EQUAL_MSG( "Couldn't create fake MemoDB: ",
            errNone, err );
    }

    /* Create new internal MemoDB */
    CreateInternalMemoDB();

    WriteTextToMemo( msg1 );
    WriteTextToMemo( msg2 );

    /* Simulate end of session to force the writing of the internal
       memo data to the 'fake' MemoDB */
    DeleteInternalMemoDB();

    /* Check that only one memo record has been created */
    ASSERT_UINT16_EQUAL_MSG_GOTOERR( "Wrong number of records: ", 1,
        mock_CheckNumOfRecordsInFakeMemoDB() );

    /* Check that the correct data has been written to the memo record */
    dbRef   = OpenFakeMemoDB();
    ASSERT_MSG_GOTOERR( "Failed to open MemoDB", dbRef != NULL );
    handle  = DmQueryRecord( dbRef, 0 );
    ASSERT_MSG_GOTOERR( "No data found in MemoDB", handle != NULL );
    data = MemHandleLock( handle );
    ASSERT_MSG_GOTOERR( "Wrong data: ",
        CheckRecordData( data + 29, Memo1Data, Memo1Size ) );

ERR:
    if ( handle != NULL ) {
        MemHandleUnlock( handle );
    }

    DmCloseDatabase( dbRef );

    /* Delete the 'fake' MemoDB */
    DeleteFakeMemoDB();
}



void InitializeExternalFormSuite( void )
{
    TestGroup* group;

    /* ExternalForm Tester */
    group = add_test_group( "ExternalForm", ExternalFormSuiteSetUp,
                ExternalFormSuiteTearDown, ExternalFormInitTest,
                ExternalFormCleanupTest );
    add_test_case( group, "Init", TestExternalFormInit );
    add_test_case( group, "Back (button)", TestExternalFormLinksBackButton );
    add_test_case( group, "Back (jogdial)", TestExternalFormLinksJogDialBack );
    add_test_case( group, "Back (fiveway)", TestExternalFormLinksFiveWayBack );
    add_test_case( group, "Back (gestures)", TestExternalFormLinksGesturesBack );
    add_test_case( group, "Copy button", TestExternalFormLinksCopy );
    add_test_case( group, "Write text", TestExternalFormWriteText );
}



void TerminateExternalFormSuite( void )
{
}

