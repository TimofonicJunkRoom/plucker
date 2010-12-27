/////////////////////////////////////////////////////////////////////////////
// Name:        winflip.h
// Purpose:     ListBox to flip among a set of managed windows
// Author:      Vaclav Slavik, Robert O'Connor
// RCS-ID:      $Id: winflip.h,v 1.1 2002/04/02 22:55:53 robertoconnor Exp $
// Copyright:   (c) Vaclav Slavik, Robert O'Connor
// Licence:     wxWindows licence
////////////////////////////////////////////////////////////////////////////


#ifndef __WX_WINFLIP_H__
#define __WX_WINFLIP_H__


#ifdef __GNUG__
	#pragma interface "winflip.h"
#endif

#include "wx/panel.h"
#include "wx/splitter.h"
#include "wx/dynarray.h"
#include "wx/imaglist.h"

class WXDLLEXPORT wxListCtrl;
class WXDLLEXPORT wxListEvent;


//----------------------------------------------------------------------------------------
// Define a new type
//----------------------------------------------------------------------------------------
#define wxWINFLIP_LISTCTRL                 0x0001
#define wxWINFLIP_TREECTRL                 0x0002
#define wxWINFLIP_CHOICE                   0x0004
#define wxWINFLIP_LEFT                     0x0008
#define wxWINFLIP_RIGHT                    0x0010
#define wxWINFLIP_TOP                      0x0020
#define wxWINFLIP_BOTTOM                   0x0040
//#define wxWINFLIP_                       0x0080
//#define wxWINFLIP_                       0x0100
#define wxWINFLIP_CONTROLLER_SCROLLBAR     0x0200
#define wxWINFLIP_LABEL                    0x0400
#define wxWINFLIP_DEFAULT                  ( wxWINFLIP_LISTCTRL | wxWINFLIP_LABEL | wxWINFLIP_LEFT ) 

//----------------------------------------------------------------------------------------
// Define a new type
//----------------------------------------------------------------------------------------
// TODO: Where should the array be defined, scope of global now. Should it be inside
// the class?

// A struct to hold info about each page in the winflip switching gizmo,
// used in the advanced panel.
struct winflip_page {
   wxPanel* panel;          // Pointer to the panel for the page
   wxString title;          // Panel's title to display in the sidebar listbox
};

// This WX macro declares a new array type, called winflip_page_array_type,
// which consists of a number of winflip_page items.
WX_DEFINE_ARRAY( winflip_page*, winflip_page_array_type );
               
// This class provides a listctrl of window names and a splitterwindow that
// shows the currently selected window, as specified by the listctrl

class WXDLLEXPORT wxWinFlip : public wxPanel
{
    // wx Macro to declare a dynamic class (one that can have new runtime instances)
	DECLARE_CLASS(wxWinFlip);

public:
    wxWinFlip(wxWindow *parent, wxWindowID id,
                      const wxString& label,
                      const wxPoint& pos = wxDefaultPosition,
                      const wxSize& size = wxDefaultSize,
                      const wxString& name = wxT("Choose a section:"),                      
                      const wxSize& controllerSize = wxDefaultSize,
                      long style = wxWINFLIP_DEFAULT
                      );
                      
    
    // TODO: Consider a GetPage(int) that returns a pointer to the panel    
    void                Init();
    void                AddXRCPanelPage(wxString title, wxString resourceName,
                                        int imageId = -1);
    int                 GetSelection();                                    
    void                SetSelection(int pageIndex);
    void                SetImageList(wxImageList* imageList);
    wxWindow*           GetPage(int pageIndex);
    winflip_page_array_type    m_page;
    
protected:
    wxListCtrl*                m_listCtrl;
    wxSplitterWindow*          m_splitterWindow;
    wxSize                     m_winFlipSize;
    int                        m_selectedPageIndex;
    
    void OnItemSelected(wxListEvent& event);
    DECLARE_EVENT_TABLE()
};

#endif
