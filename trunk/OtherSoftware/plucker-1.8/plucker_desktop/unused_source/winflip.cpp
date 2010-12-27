/////////////////////////////////////////////////////////////////////////////
// Name:        winflip.cpp
// Purpose:     ListBox to flip among a set of managed windows
// Author:      Vaclav Slavik, Robert O'Connor
// RCS-ID:      $Id: winflip.cpp,v 1.1 2002/04/02 22:55:53 robertoconnor Exp $
// Copyright:   (c) Vaclav Slavik, Robert O'Connor
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
    #pragma implementation "winflip.h"
#endif

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

// NOTE: Vaclav's gizmo had the header in gizmos
//#include "wx/gizmos/winflip.h"
#include "winflip.h"
#include "wx/sizer.h"
#include "wx/listctrl.h"

// ---------------------------------------------------------------------------------------
#include "wx/xrc/xmlres.h"              // XRC XML resouces
// ---------------------------------------------------------------------------------------

// List control with a single column that auto-resizes its column to the 
// width of the control, minus some right hand space for the scrollbar:
class CleverListCtrl : public wxListCtrl
{
public:
   CleverListCtrl(wxWindow *parent,
                  wxWindowID id = -1,
                  const wxPoint &pos = wxDefaultPosition,
                  const wxSize &size = wxDefaultSize,
                  long listCtrlStyle = wxLC_ICON,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString &name = "listctrl",
                  long winFlipStyle = wxWINFLIP_DEFAULT )
         : wxListCtrl(parent, id, pos, size, listCtrlStyle, validator, name)
    {
        m_leaveScrollbarSpacing = ( winFlipStyle & wxWINFLIP_CONTROLLER_SCROLLBAR );        
        CreateColumns();
    }

    void CreateColumns()
    {
        InsertColumn(0, _T("item"));
        SizeColumns();
    }

    void SizeColumns()
    {
         wxLogDebug("About to get win_flip's listctrl width" );   
         int w = GetSize().x;
         wxLogDebug("Got win_flip's listctrl width" );  
            w -= 6;
         if ( m_leaveScrollbarSpacing )
            w -= wxSystemSettings::GetSystemMetric(wxSYS_VSCROLL_X);
         SetColumnWidth(0, w);
    }

private:
    DECLARE_EVENT_TABLE()
    
    bool m_leaveScrollbarSpacing;
    void OnSize(wxSizeEvent& event)
    {
        SizeColumns();
    }
};

BEGIN_EVENT_TABLE(CleverListCtrl, wxListCtrl)
   EVT_SIZE(CleverListCtrl::OnSize)
END_EVENT_TABLE()

// wx Marcro to implement a dynamic class (one that can have new instances at runtime).
IMPLEMENT_CLASS(wxWinFlip, wxPanel)

enum 
{
    wxID_WINFLIP_LISTCTRL = wxID_HIGHEST + 1,
    wxID_WINFLIP_SPLITTERWINDOW    
};

BEGIN_EVENT_TABLE(wxWinFlip, wxPanel)
    EVT_LIST_ITEM_SELECTED(wxID_WINFLIP_LISTCTRL, wxWinFlip::OnItemSelected)
END_EVENT_TABLE()

//----------------------------------------------------------------------------------------
// Interface
//----------------------------------------------------------------------------------------

// Constructor
wxWinFlip::wxWinFlip(wxWindow *parent, wxWindowID id,
                          const wxString& label,
                          const wxPoint& pos, const wxSize& size,
                          const wxString& name,
                          const wxSize& controllerSize,
                          long style)
   : wxPanel(parent, id, pos, size, wxTAB_TRAVERSAL, name)
{        
    
    // The panel is laid out like this, using a FlexGridSizer. 
    //
    // +---------------------------------------------------------------+
    // |    Static Message      |     (a blank spacer placeholder )    |
    // +------------------------+--------------------------------------+
    // |                        |                                      |
    // |       ListCtrl         |         SplitterWindow               |
    // |                        |                                      |
    // +------------------------+--------------------------------------+
    // 
    // The listctrl and splitterwindow expand to fill the panel.
    // Mechanism of wxFlexGridSizer is just to specify its number of 
    // columns (in this case 2), and then just start adding the items
    // in order, starting from upper left: first the static message,
    // then an empty spacer placeholder, then the listctrl, then the 
    // splitterwindow. 
    // It is thus evident why a blank placeholder is needed to make 
    // the shape, because need something to occupy the slot when
    // counting from upperleft corner to lower right corner, so the
    // the cells are in their desired places.
     
    // Make a new flexgrid sizer ( format # of rows, # of columns, then hor/vert gaps)
    
    int numberFlexRows;
    
    if ( style & wxWINFLIP_LABEL ) {
        numberFlexRows = 2;
    } else {
        numberFlexRows = 1;
    }
    
    wxFlexGridSizer *flexsizer = new wxFlexGridSizer( numberFlexRows, 0, 0, 0 );

    // If label, set its growable columns to 1, and growable rows to 1 (on a zero index 
    // numbering system)
    flexsizer->AddGrowableCol( numberFlexRows - 1 );
    flexsizer->AddGrowableRow( numberFlexRows - 1 );
    
    if ( style & wxWINFLIP_LABEL ) {
        wxLogDebug( "About to add a static label to the winflip widget's constructor" );
        // Make a new statictext, -1 ID since don't do anything with them anyways
        // "label" is the parameter we called in the constructor.
        wxStaticText *statictext = new wxStaticText( this, -1, label,
                                                     wxDefaultPosition, wxDefaultSize, 0 );
        // Put the static text into the sizer                                        
        flexsizer->Add( statictext, 0, wxALIGN_CENTRE|wxLEFT|wxRIGHT|wxTOP, 5 ); 
        
            // Put an empty placeholder into the sizer for the second item
        flexsizer->Add( 5, 5, 0, wxALIGN_CENTRE, 5 );
        wxLogDebug( "Finishined adding a static label to the winflip widget's constructor" );
    }
    
    // Make a listctrl
    m_listCtrl = new CleverListCtrl( this, wxID_WINFLIP_LISTCTRL,
                                     wxDefaultPosition,                                     
                                     controllerSize,
                                     wxLC_REPORT|wxLC_NO_HEADER|
                                     wxLC_SINGLE_SEL|wxSUNKEN_BORDER,
                                     wxDefaultValidator,
                                     "listctrl",
                                     style );

    // Add it to sizer                                    
    flexsizer->Add( m_listCtrl, 0, wxGROW|wxALIGN_CENTER_VERTICAL|wxALL, 5 );


    // Make the splitterwindow
    m_splitterWindow = new wxSplitterWindow( this, wxID_WINFLIP_SPLITTERWINDOW,
                                             wxDefaultPosition, wxDefaultSize,
                                             wxCLIP_CHILDREN );
                                             
    // Strip the splitterwindow's borders
    m_splitterWindow->SetBorderSize( 0 ); 
    wxLogDebug( "Set splitterwindow border to zero" );
                                             
    // Add it to sizer
    flexsizer->Add( m_splitterWindow, 0, 
                    wxGROW|wxALIGN_CENTER_VERTICAL|wxRIGHT|wxTOP|wxBOTTOM, 5 );

    // Now that everything is added into the flexgrid sizer, send some commands to 
    // tell it calculate all the control sizes nicely to fill up the available space.
    SetAutoLayout( TRUE );
    SetSizer( flexsizer );
    flexsizer->Fit( this );
    flexsizer->SetSizeHints( this );
    
    // TODO: Don't know whether this Layout() is needed
	//Layout();
}


void wxWinFlip::OnItemSelected(wxListEvent& event)
{    
    int pageRequested = event.GetIndex();

    // Only switch pages, if the index clicked isn't the one that is already selected  
    // (since it will otherwise cause it to switch to a blank panel ).
    if ( m_selectedPageIndex != pageRequested ) {
        // It is a new page. Execute the winflip widget's SetSelection() function.
        SetSelection(pageRequested);
    }
}


void wxWinFlip::AddXRCPanelPage( wxString title, wxString resourceName, int imageId )
{  
    // Make a new element for the array and load up each bit of it with the proper
    // properties, then add it to the end of the array.
    winflip_page *new_page    = new winflip_page();    
    new_page->title           = title; 
    m_page.Add( new_page );
    wxLogDebug( "Added winflip_page. title=%s, resource_name=%s",
                 title.c_str(), resourceName.c_str() );            
    
    // Get the index of the element now.
    int newestPageIndex = m_page.GetCount() - 1;    
    wxLogDebug( "winflip_array: newestPageIndex=%i", newestPageIndex );
    
    // Create it's panel and load from the resource.
    m_page[ newestPageIndex ]->panel = new wxPanel;    
    wxTheXmlResource->LoadPanel( m_page[ newestPageIndex ]->panel,
                                 m_splitterWindow, resourceName );                                 
    wxLogDebug( "Built a winflip_panel" );
    
    // Set the panel's size, so that it expands to fill the full size. TODO: can this
    // be replaced with a SetSizer or something so that don't need to write a separate 
    // on size function. 
    wxLogDebug("About to get winflip's m_splitterWindows's width" );    
    m_winFlipSize = m_splitterWindow->GetSize();
    wxLogDebug("Got winflip's m_splitterWindows's width" );   
    m_page[ newestPageIndex ]->panel->SetSize( m_winFlipSize );
    
    // Initially hide this panel. so won't peek through the active window
    // when clicking away on the active window.
    m_page[ newestPageIndex ]->panel->Show( FALSE ); 
    wxLogDebug( "Hid panel" );
    
    // Append the panel's title to the sidebar listctrl    
    m_listCtrl->InsertItem( newestPageIndex, title, imageId ); 
    wxLogDebug( "Appended title '%s' to listctrl", title.c_str() );    
}


void wxWinFlip::Init()
{   
    // Set the splitterwindow to initially display the topmost panel (0), and similarly
    // set the listbox sidebar controller to topmost.    
    m_splitterWindow->Initialize( m_page[ 0 ]->panel );    
    wxLogDebug( "winflip: set the initial panel to array number 0" );  
        
    m_page[ 0 ]->panel->Show( TRUE );
    wxLogDebug( "winflip: showing panel" );  
    
    m_selectedPageIndex = 0;        
    wxLogDebug( "winflip: displayed_pageIndex is 0" );  
    // Highlight the top one in the listctrl initially
    m_listCtrl->SetItemState( 0, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED );
}


int wxWinFlip::GetSelection()
{
    return m_selectedPageIndex;
}


void wxWinFlip::SetSelection(int pageIndex)
{
    // Get a pointer to the old window that is about to be replaced. Using GetWindow1 for
    // splitters with just 1 managed window, returns that only managed window.
    wxWindow* oldWindow;    
    oldWindow = m_splitterWindow->GetWindow1();
    
    // Now swap the oldWindow with the new window
    m_splitterWindow->ReplaceWindow( oldWindow, m_page[ pageIndex ]->panel );
                                 
    // Show the new window, hide the old one;
    m_page[ pageIndex ]->panel->Show( TRUE );    
    oldWindow->Show( FALSE );
    
    m_selectedPageIndex = pageIndex;
}


void wxWinFlip::SetImageList(wxImageList* imageList)
{
    m_listCtrl->SetImageList(imageList, wxIMAGE_LIST_SMALL);
}


// Could typedef this to return a wxWinFlipPage
wxWindow* wxWinFlip::GetPage(int pageIndex)
{
    return m_page[ pageIndex ]->panel;
}