///////////////////////////////////////////////////////////////////////////////
// Name:        fileconf.cpp
// Purpose:     implementation of wxFileConfig derivation of wxConfig
// Author:      Vadim Zeitlin
// Modified by:
// Created:     07.04.98 (adapted from appconf.cpp)
// RCS-ID:      $Id: fileconf.cpp,v 1.1 2002/07/16 20:46:57 robertoconnor Exp $
// Copyright:   (c) 1997 Karsten Ball�der   &  Vadim Zeitlin
//                       Ballueder@usa.net     <zeitlin@dptmaths.ens-cachan.fr>
// Licence:     wxWindows license
///////////////////////////////////////////////////////////////////////////////

#ifdef __GNUG__
#pragma implementation "fileconf.h"
#endif

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include  "wx/wxprec.h"

#ifdef    __BORLANDC__
  #pragma hdrstop
#endif  //__BORLANDC__

#if wxUSE_CONFIG

#ifndef   WX_PRECOMP
  #include  "wx/string.h"
  #include  "wx/intl.h"
#endif  //WX_PRECOMP

#include  "wx/app.h"
#include  "wx/dynarray.h"
#include  "wx/file.h"
#include  "wx/log.h"
#include  "wx/textfile.h"
#include  "wx/config.h"
#include  "wx/fileconf.h"

#include  "wx/utils.h"    // for wxGetHomeDir

// _WINDOWS_ is defined when windows.h is included,
// __WXMSW__ is defined for MS Windows compilation
#if       defined(__WXMSW__) && !defined(_WINDOWS_)
  #include  <windows.h>
#endif  //windows.h
#if defined(__WXPM__)
  #define INCL_DOS
  #include <os2.h>
#endif

#include  <stdlib.h>
#include  <ctype.h>

// headers needed for umask()
#ifdef __UNIX__
    #include <sys/types.h>
    #include <sys/stat.h>
#endif // __UNIX__

// ----------------------------------------------------------------------------
// macros
// ----------------------------------------------------------------------------
#define CONST_CAST ((wxFileConfig *)this)->

// ----------------------------------------------------------------------------
// constants
// ----------------------------------------------------------------------------
#ifndef MAX_PATH
  #define MAX_PATH 512
#endif

// ----------------------------------------------------------------------------
// global functions declarations
// ----------------------------------------------------------------------------

// compare functions for sorting the arrays
static int LINKAGEMODE CompareEntries(ConfigEntry *p1, ConfigEntry *p2);
static int LINKAGEMODE CompareGroups(ConfigGroup *p1, ConfigGroup *p2);

// filter strings
static wxString FilterInValue(const wxString& str);
static wxString FilterOutValue(const wxString& str);

static wxString FilterInEntryName(const wxString& str);
static wxString FilterOutEntryName(const wxString& str);

// get the name to use in wxFileConfig ctor
static wxString GetAppName(const wxString& appname);

// ============================================================================
// implementation
// ============================================================================

// ----------------------------------------------------------------------------
// static functions
// ----------------------------------------------------------------------------
wxString wxFileConfig::GetGlobalDir()
{
  wxString strDir;

  #ifdef __VMS__ // Note if __VMS is defined __UNIX is also defined
    strDir = wxT("sys$manager:");
  #elif defined( __UNIX__ )
    strDir = wxT("/etc/");
  #elif defined(__WXPM__)
    ULONG                           aulSysInfo[QSV_MAX] = {0};
    UINT                            drive;
    APIRET                          rc;

    rc = DosQuerySysInfo( 1L, QSV_MAX, (PVOID)aulSysInfo, sizeof(ULONG)*QSV_MAX);
    if (rc == 0)
    {
        drive = aulSysInfo[QSV_BOOT_DRIVE - 1];
        switch(drive)
        {
           case 1:
              strDir = "A:\\OS2\\";
              break;
           case 2:
              strDir = "B:\\OS2\\";
              break;
           case 3:
              strDir = "C:\\OS2\\";
              break;
           case 4:
              strDir = "D:\\OS2\\";
              break;
           case 5:
              strDir = "E:\\OS2\\";
              break;
           case 6:
              strDir = "F:\\OS2\\";
              break;
           case 7:
              strDir = "G:\\OS2\\";
              break;
           case 8:
              strDir = "H:\\OS2\\";
              break;
           case 9:
              strDir = "I:\\OS2\\";
              break;
           case 10:
              strDir = "J:\\OS2\\";
              break;
           case 11:
              strDir = "K:\\OS2\\";
              break;
           case 12:
              strDir = "L:\\OS2\\";
              break;
           case 13:
              strDir = "M:\\OS2\\";
              break;
           case 14:
              strDir = "N:\\OS2\\";
              break;
           case 15:
              strDir = "O:\\OS2\\";
              break;
           case 16:
              strDir = "P:\\OS2\\";
              break;
           case 17:
              strDir = "Q:\\OS2\\";
              break;
           case 18:
              strDir = "R:\\OS2\\";
              break;
           case 19:
              strDir = "S:\\OS2\\";
              break;
           case 20:
              strDir = "T:\\OS2\\";
              break;
           case 21:
              strDir = "U:\\OS2\\";
              break;
           case 22:
              strDir = "V:\\OS2\\";
              break;
           case 23:
              strDir = "W:\\OS2\\";
              break;
           case 24:
              strDir = "X:\\OS2\\";
              break;
           case 25:
              strDir = "Y:\\OS2\\";
              break;
           case 26:
              strDir = "Z:\\OS2\\";
              break;
        }
    }
  #elif defined(__WXSTUBS__)
    wxASSERT_MSG( FALSE, wxT("TODO") ) ;
  #elif defined(__WXMAC__)
	strDir = wxMacFindFolder(  (short) kOnSystemDisk, kPreferencesFolderType, kDontCreateFolder ) ;
  #else // Windows
    wxChar szWinDir[MAX_PATH];
    ::GetWindowsDirectory(szWinDir, MAX_PATH);

    strDir = szWinDir;
    strDir << wxT('\\');
  #endif // Unix/Windows

  return strDir;
}

wxString wxFileConfig::GetLocalDir()
{
  wxString strDir;

#ifndef __WXMAC__
  wxGetHomeDir(&strDir);

#ifdef  __UNIX__
#ifdef __VMS
   if (strDir.Last() != wxT(']'))
#endif
   if (strDir.Last() != wxT('/')) strDir << wxT('/');
#else
  if (strDir.Last() != wxT('\\')) strDir << wxT('\\');
#endif
#else
	// no local dir concept on mac
	return GetGlobalDir() ;
#endif

  return strDir;
}

wxString wxFileConfig::GetGlobalFileName(const wxChar *szFile)
{
  wxString str = GetGlobalDir();
  str << szFile;

  if ( wxStrchr(szFile, wxT('.')) == NULL )
  #ifdef  __UNIX__
    str << wxT(".conf");
  #elif defined( __WXMAC__ )
     str << " Preferences";
  #else   // Windows
    str << wxT(".ini");
  #endif  // UNIX/Win

  return str;
}

wxString wxFileConfig::GetLocalFileName(const wxChar *szFile)
{
#ifdef __VMS__ // On VMS I saw the problem that the home directory was appended
   // twice for the configuration file. Does that also happen for other
   // platforms?
   wxString str = wxT( '.' ); 
#else
   wxString str = GetLocalDir();
#endif
   
  #if defined( __UNIX__ ) && !defined( __VMS )
    str << wxT('.');
  #endif

  str << szFile;

  #ifdef __WXMSW__
    if ( wxStrchr(szFile, wxT('.')) == NULL )
      str << wxT(".ini");
  #endif


  #ifdef __WXMAC__
     str << " Preferences";
  #endif
  return str;
}

// ----------------------------------------------------------------------------
// ctor
// ----------------------------------------------------------------------------

void wxFileConfig::Init()
{
  m_pCurrentGroup =
  m_pRootGroup    = new ConfigGroup(NULL, "", this);

  m_linesHead =
  m_linesTail = NULL;

  // it's not an error if (one of the) file(s) doesn't exist

  // parse the global file
  if ( !m_strGlobalFile.IsEmpty() && wxFile::Exists(m_strGlobalFile) ) {
    wxTextFile fileGlobal(m_strGlobalFile);

    if ( fileGlobal.Open() ) {
      Parse(fileGlobal, FALSE /* global */);
      SetRootPath();
    }
    else
      wxLogWarning(_("can't open global configuration file '%s'."),
                   m_strGlobalFile.c_str());
  }

  // parse the local file
  if ( !m_strLocalFile.IsEmpty() && wxFile::Exists(m_strLocalFile) ) {
    wxTextFile fileLocal(m_strLocalFile);
    if ( fileLocal.Open() ) {
      Parse(fileLocal, TRUE /* local */);
      SetRootPath();
    }
    else
      wxLogWarning(_("can't open user configuration file '%s'."),
                   m_strLocalFile.c_str());
  }
}

// constructor supports creation of wxFileConfig objects of any type
wxFileConfig::wxFileConfig(const wxString& appName, const wxString& vendorName,
                           const wxString& strLocal, const wxString& strGlobal,
                           long style)
            : wxConfigBase(::GetAppName(appName), vendorName,
                           strLocal, strGlobal,
                           style),
              m_strLocalFile(strLocal), m_strGlobalFile(strGlobal)
{
  // Make up names for files if empty
  if ( m_strLocalFile.IsEmpty() && (style & wxCONFIG_USE_LOCAL_FILE) )
  {
    m_strLocalFile = GetLocalFileName(GetAppName());
  }

  if ( m_strGlobalFile.IsEmpty() && (style & wxCONFIG_USE_GLOBAL_FILE) )
  {
    m_strGlobalFile = GetGlobalFileName(GetAppName());
  }

  // Check if styles are not supplied, but filenames are, in which case
  // add the correct styles.
  if ( !m_strLocalFile.IsEmpty() )
    SetStyle(GetStyle() | wxCONFIG_USE_LOCAL_FILE);

  if ( !m_strGlobalFile.IsEmpty() )
    SetStyle(GetStyle() | wxCONFIG_USE_GLOBAL_FILE);

  // if the path is not absolute, prepend the standard directory to it
  // UNLESS wxCONFIG_USE_RELATIVE_PATH style is set
  if ( !(style & wxCONFIG_USE_RELATIVE_PATH) )
  {
      if ( !m_strLocalFile.IsEmpty() && !wxIsAbsolutePath(m_strLocalFile) )
      {
          wxString strLocal = m_strLocalFile;
          m_strLocalFile = GetLocalDir();
          m_strLocalFile << strLocal;
      }

      if ( !m_strGlobalFile.IsEmpty() && !wxIsAbsolutePath(m_strGlobalFile) )
      {
          wxString strGlobal = m_strGlobalFile;
          m_strGlobalFile = GetGlobalDir();
          m_strGlobalFile << strGlobal;
      }
  }

  SetUmask(-1);

  Init();
}

void wxFileConfig::CleanUp()
{
  delete m_pRootGroup;

  LineList *pCur = m_linesHead;
  while ( pCur != NULL ) {
    LineList *pNext = pCur->Next();
    delete pCur;
    pCur = pNext;
  }
}

wxFileConfig::~wxFileConfig()
{
  Flush();

  CleanUp();
}

// ----------------------------------------------------------------------------
// parse a config file
// ----------------------------------------------------------------------------

void wxFileConfig::Parse(wxTextFile& file, bool bLocal)
{
  const wxChar *pStart;
  const wxChar *pEnd;
  wxString strLine;

  size_t nLineCount = file.GetLineCount();
  for ( size_t n = 0; n < nLineCount; n++ ) {
    strLine = file[n];

    // add the line to linked list
    if ( bLocal )
      LineListAppend(strLine);

    // skip leading spaces
    for ( pStart = strLine; wxIsspace(*pStart); pStart++ )
      ;

    // skip blank/comment lines
    if ( *pStart == wxT('\0')|| *pStart == wxT(';') || *pStart == wxT('#') )
      continue;

    if ( *pStart == wxT('[') ) {          // a new group
      pEnd = pStart;

      while ( *++pEnd != wxT(']') ) {
        if ( *pEnd == wxT('\\') ) {
            // the next char is escaped, so skip it even if it is ']'
            pEnd++;
        }

        if ( *pEnd == wxT('\n') || *pEnd == wxT('\0') ) {
            // we reached the end of line, break out of the loop
            break;
        }
      }

      if ( *pEnd != wxT(']') ) {
        wxLogError(_("file '%s': unexpected character %c at line %d."),
                   file.GetName(), *pEnd, n + 1);
        continue; // skip this line
      }

      // group name here is always considered as abs path
      wxString strGroup;
      pStart++;
      strGroup << wxCONFIG_PATH_SEPARATOR
               << FilterInEntryName(wxString(pStart, pEnd - pStart));

      // will create it if doesn't yet exist
      SetPath(strGroup);

      if ( bLocal )
        m_pCurrentGroup->SetLine(m_linesTail);

      // check that there is nothing except comments left on this line
      bool bCont = TRUE;
      while ( *++pEnd != wxT('\0') && bCont ) {
        switch ( *pEnd ) {
          case wxT('#'):
          case wxT(';'):
            bCont = FALSE;
            break;

          case wxT(' '):
          case wxT('\t'):
            // ignore whitespace ('\n' impossible here)
            break;

          default:
            wxLogWarning(_("file '%s', line %d: '%s' ignored after group header."),
                         file.GetName(), n + 1, pEnd);
            bCont = FALSE;
        }
      }
    }
    else {                        // a key
      const wxChar *pEnd = pStart;
      while ( *pEnd && *pEnd != wxT('=') && !wxIsspace(*pEnd) ) {
        if ( *pEnd == wxT('\\') ) {
          // next character may be space or not - still take it because it's
          // quoted (unless there is nothing)
          pEnd++;
          if ( !*pEnd ) {
            // the error message will be given below anyhow
            break;
          }
        }

        pEnd++;
      }

      wxString strKey(FilterInEntryName(wxString(pStart, pEnd)));

      // skip whitespace
      while ( wxIsspace(*pEnd) )
        pEnd++;

      if ( *pEnd++ != wxT('=') ) {
        wxLogError(_("file '%s', line %d: '=' expected."),
                   file.GetName(), n + 1);
      }
      else {
        ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(strKey);

        if ( pEntry == NULL ) {
          // new entry
          pEntry = m_pCurrentGroup->AddEntry(strKey, n);

          if ( bLocal )
            pEntry->SetLine(m_linesTail);
        }
        else {
          if ( bLocal && pEntry->IsImmutable() ) {
            // immutable keys can't be changed by user
            wxLogWarning(_("file '%s', line %d: value for immutable key '%s' ignored."),
                         file.GetName(), n + 1, strKey.c_str());
            continue;
          }
          // the condition below catches the cases (a) and (b) but not (c):
          //  (a) global key found second time in global file
          //  (b) key found second (or more) time in local file
          //  (c) key from global file now found in local one
          // which is exactly what we want.
          else if ( !bLocal || pEntry->IsLocal() ) {
            wxLogWarning(_("file '%s', line %d: key '%s' was first found at line %d."),
                         file.GetName(), n + 1, strKey.c_str(), pEntry->Line());

            if ( bLocal )
              pEntry->SetLine(m_linesTail);
          }
        }

        // skip whitespace
        while ( wxIsspace(*pEnd) )
          pEnd++;
        
        if ( GetStyle() & wxCONFIG_USE_NO_ESCAPE_CHARACTERS )
            pEntry->SetValue(pEnd, FALSE /* read from file, without escapes */);
        else
            pEntry->SetValue(FilterInValue(pEnd), FALSE /* read from file, with escapes */);
        
        //pEntry->SetValue(FilterInValue(pEnd), FALSE /* read from file */);
      }
    }
  }
}

// ----------------------------------------------------------------------------
// set/retrieve path
// ----------------------------------------------------------------------------

void wxFileConfig::SetRootPath()
{
  m_strPath.Empty();
  m_pCurrentGroup = m_pRootGroup;
}

void wxFileConfig::SetPath(const wxString& strPath)
{
  wxArrayString aParts;

  if ( strPath.IsEmpty() ) {
    SetRootPath();
    return;
  }

  if ( strPath[0] == wxCONFIG_PATH_SEPARATOR ) {
    // absolute path
    wxSplitPath(aParts, strPath);
  }
  else {
    // relative path, combine with current one
    wxString strFullPath = m_strPath;
    strFullPath << wxCONFIG_PATH_SEPARATOR << strPath;
    wxSplitPath(aParts, strFullPath);
  }

  // change current group
  size_t n;
  m_pCurrentGroup = m_pRootGroup;
  for ( n = 0; n < aParts.Count(); n++ ) {
    ConfigGroup *pNextGroup = m_pCurrentGroup->FindSubgroup(aParts[n]);
    if ( pNextGroup == NULL )
      pNextGroup = m_pCurrentGroup->AddSubgroup(aParts[n]);
    m_pCurrentGroup = pNextGroup;
  }

  // recombine path parts in one variable
  m_strPath.Empty();
  for ( n = 0; n < aParts.Count(); n++ ) {
    m_strPath << wxCONFIG_PATH_SEPARATOR << aParts[n];
  }
}

// ----------------------------------------------------------------------------
// enumeration
// ----------------------------------------------------------------------------

bool wxFileConfig::GetFirstGroup(wxString& str, long& lIndex) const
{
  lIndex = 0;
  return GetNextGroup(str, lIndex);
}

bool wxFileConfig::GetNextGroup (wxString& str, long& lIndex) const
{
  if ( size_t(lIndex) < m_pCurrentGroup->Groups().Count() ) {
    str = m_pCurrentGroup->Groups()[(size_t)lIndex++]->Name();
    return TRUE;
  }
  else
    return FALSE;
}

bool wxFileConfig::GetFirstEntry(wxString& str, long& lIndex) const
{
  lIndex = 0;
  return GetNextEntry(str, lIndex);
}

bool wxFileConfig::GetNextEntry (wxString& str, long& lIndex) const
{
  if ( size_t(lIndex) < m_pCurrentGroup->Entries().Count() ) {
    str = m_pCurrentGroup->Entries()[(size_t)lIndex++]->Name();
    return TRUE;
  }
  else
    return FALSE;
}

size_t wxFileConfig::GetNumberOfEntries(bool bRecursive) const
{
  size_t n = m_pCurrentGroup->Entries().Count();
  if ( bRecursive ) {
    ConfigGroup *pOldCurrentGroup = m_pCurrentGroup;
    size_t nSubgroups = m_pCurrentGroup->Groups().Count();
    for ( size_t nGroup = 0; nGroup < nSubgroups; nGroup++ ) {
      CONST_CAST m_pCurrentGroup = m_pCurrentGroup->Groups()[nGroup];
      n += GetNumberOfEntries(TRUE);
      CONST_CAST m_pCurrentGroup = pOldCurrentGroup;
    }
  }

  return n;
}

size_t wxFileConfig::GetNumberOfGroups(bool bRecursive) const
{
  size_t n = m_pCurrentGroup->Groups().Count();
  if ( bRecursive ) {
    ConfigGroup *pOldCurrentGroup = m_pCurrentGroup;
    size_t nSubgroups = m_pCurrentGroup->Groups().Count();
    for ( size_t nGroup = 0; nGroup < nSubgroups; nGroup++ ) {
      CONST_CAST m_pCurrentGroup = m_pCurrentGroup->Groups()[nGroup];
      n += GetNumberOfGroups(TRUE);
      CONST_CAST m_pCurrentGroup = pOldCurrentGroup;
    }
  }

  return n;
}

// ----------------------------------------------------------------------------
// tests for existence
// ----------------------------------------------------------------------------

bool wxFileConfig::HasGroup(const wxString& strName) const
{
  wxConfigPathChanger path(this, strName);

  ConfigGroup *pGroup = m_pCurrentGroup->FindSubgroup(path.Name());
  return pGroup != NULL;
}

bool wxFileConfig::HasEntry(const wxString& strName) const
{
  wxConfigPathChanger path(this, strName);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  return pEntry != NULL;
}

// ----------------------------------------------------------------------------
// read/write values
// ----------------------------------------------------------------------------

bool wxFileConfig::Read(const wxString& key,
                        wxString* pStr) const
{
  wxConfigPathChanger path(this, key);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  if (pEntry == NULL) {
    return FALSE;
  }

  *pStr = ExpandEnvVars(pEntry->Value());
  return TRUE;
}

bool wxFileConfig::Read(const wxString& key,
                        wxString* pStr, const wxString& defVal) const
{
  wxConfigPathChanger path(this, key);

  ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(path.Name());
  bool ok;
  if (pEntry == NULL) {
    if( IsRecordingDefaults() )
      ((wxFileConfig *)this)->Write(key,defVal);
    *pStr = ExpandEnvVars(defVal);
    ok = FALSE;
  }
  else {
    *pStr = ExpandEnvVars(pEntry->Value());
    ok = TRUE;
  }

  return ok;
}

bool wxFileConfig::Read(const wxString& key, long *pl) const
{
  wxString str;
  if ( !Read(key, & str) )
  {
    return FALSE;
  }

  *pl = wxAtol(str);
  return TRUE;
}

bool wxFileConfig::Write(const wxString& key, const wxString& szValue)
{
  wxConfigPathChanger path(this, key);

  wxString strName = path.Name();
  if ( strName.IsEmpty() ) {
    // setting the value of a group is an error
    wxASSERT_MSG( wxIsEmpty(szValue), wxT("can't set value of a group!") );

    // ... except if it's empty in which case it's a way to force it's creation
    m_pCurrentGroup->SetDirty();

    // this will add a line for this group if it didn't have it before
    (void)m_pCurrentGroup->GetGroupLine();
  }
  else {
    // writing an entry

    // check that the name is reasonable
    if ( strName[0u] == wxCONFIG_IMMUTABLE_PREFIX ) {
      wxLogError(_("Config entry name cannot start with '%c'."),
                 wxCONFIG_IMMUTABLE_PREFIX);
      return FALSE;
    }

    ConfigEntry *pEntry = m_pCurrentGroup->FindEntry(strName);
    if ( pEntry == NULL )
      pEntry = m_pCurrentGroup->AddEntry(strName);

    pEntry->SetValue(szValue);
  }

  return TRUE;
}

bool wxFileConfig::Write(const wxString& key, long lValue)
{
  // ltoa() is not ANSI :-(
  wxString buf;
  buf.Printf(wxT("%ld"), lValue);
  return Write(key, buf);
}

bool wxFileConfig::Flush(bool /* bCurrentOnly */)
{
  if ( LineListIsEmpty() || !m_pRootGroup->IsDirty() || !m_strLocalFile )
    return TRUE;

#ifdef __UNIX__
  // set the umask if needed
  mode_t umaskOld = 0;
  if ( m_umask != -1 )
  {
      umaskOld = umask((mode_t)m_umask);
  }
#endif // __UNIX__

  wxTempFile file(m_strLocalFile);

  if ( !file.IsOpened() ) {
    wxLogError(_("can't open user configuration file."));
    return FALSE;
  }

  // write all strings to file
  for ( LineList *p = m_linesHead; p != NULL; p = p->Next() ) {
    if ( !file.Write(p->Text() + wxTextFile::GetEOL()) ) {
      wxLogError(_("can't write user configuration file."));
      return FALSE;
    }
  }

  bool ret = file.Commit();

#if defined(__WXMAC__) && !defined(__UNIX__)
  if ( ret )
  {
  	FSSpec spec ;
  	
  	wxMacFilename2FSSpec( m_strLocalFile , &spec ) ;
  	FInfo finfo ;
  	if ( FSpGetFInfo( &spec , &finfo ) == noErr )
  	{
  		finfo.fdType = 'TEXT' ;
  		finfo.fdCreator = 'ttxt' ;
  		FSpSetFInfo( &spec , &finfo ) ;
  	}
  }
#endif // __WXMAC__ && !__UNIX__

#ifdef __UNIX__
  // restore the old umask if we changed it
  if ( m_umask != -1 )
  {
      (void)umask(umaskOld);
  }
#endif // __UNIX__

  return ret;
}

// ----------------------------------------------------------------------------
// renaming groups/entries
// ----------------------------------------------------------------------------

bool wxFileConfig::RenameEntry(const wxString& oldName,
                               const wxString& newName)
{
    // check that the entry exists
    ConfigEntry *oldEntry = m_pCurrentGroup->FindEntry(oldName);
    if ( !oldEntry )
        return FALSE;

    // check that the new entry doesn't already exist
    if ( m_pCurrentGroup->FindEntry(newName) )
        return FALSE;

    // delete the old entry, create the new one
    wxString value = oldEntry->Value();
    if ( !m_pCurrentGroup->DeleteEntry(oldName) )
        return FALSE;

    ConfigEntry *newEntry = m_pCurrentGroup->AddEntry(newName);
    newEntry->SetValue(value);

    return TRUE;
}

bool wxFileConfig::RenameGroup(const wxString& oldName,
                               const wxString& newName)
{
    // check that the group exists
    ConfigGroup *group = m_pCurrentGroup->FindSubgroup(oldName);
    if ( !group )
        return FALSE;

    // check that the new group doesn't already exist
    if ( m_pCurrentGroup->FindSubgroup(newName) )
        return FALSE;

    group->Rename(newName);

    return TRUE;
}

// ----------------------------------------------------------------------------
// delete groups/entries
// ----------------------------------------------------------------------------

bool wxFileConfig::DeleteEntry(const wxString& key, bool bGroupIfEmptyAlso)
{
  wxConfigPathChanger path(this, key);

  if ( !m_pCurrentGroup->DeleteEntry(path.Name()) )
    return FALSE;

  if ( bGroupIfEmptyAlso && m_pCurrentGroup->IsEmpty() ) {
    if ( m_pCurrentGroup != m_pRootGroup ) {
      ConfigGroup *pGroup = m_pCurrentGroup;
      SetPath(wxT(".."));  // changes m_pCurrentGroup!
      m_pCurrentGroup->DeleteSubgroupByName(pGroup->Name());
    }
    //else: never delete the root group
  }

  return TRUE;
}

bool wxFileConfig::DeleteGroup(const wxString& key)
{
  wxConfigPathChanger path(this, key);

  return m_pCurrentGroup->DeleteSubgroupByName(path.Name());
}

bool wxFileConfig::DeleteAll()
{
  CleanUp();

  if ( wxRemove(m_strLocalFile) == -1 )
    wxLogSysError(_("can't delete user configuration file '%s'"), m_strLocalFile.c_str());

  m_strLocalFile = m_strGlobalFile = wxT("");
  Init();

  return TRUE;
}

// ----------------------------------------------------------------------------
// linked list functions
// ----------------------------------------------------------------------------

// append a new line to the end of the list
LineList *wxFileConfig::LineListAppend(const wxString& str)
{
  LineList *pLine = new LineList(str);

  if ( m_linesTail == NULL ) {
    // list is empty
    m_linesHead = pLine;
  }
  else {
    // adjust pointers
    m_linesTail->SetNext(pLine);
    pLine->SetPrev(m_linesTail);
  }

  m_linesTail = pLine;
  return m_linesTail;
}

// insert a new line after the given one or in the very beginning if !pLine
LineList *wxFileConfig::LineListInsert(const wxString& str,
                                                     LineList *pLine)
{
  if ( pLine == m_linesTail )
    return LineListAppend(str);

  LineList *pNewLine = new LineList(str);
  if ( pLine == NULL ) {
    // prepend to the list
    pNewLine->SetNext(m_linesHead);
    m_linesHead->SetPrev(pNewLine);
    m_linesHead = pNewLine;
  }
  else {
    // insert before pLine
    LineList *pNext = pLine->Next();
    pNewLine->SetNext(pNext);
    pNewLine->SetPrev(pLine);
    pNext->SetPrev(pNewLine);
    pLine->SetNext(pNewLine);
  }

  return pNewLine;
}

void wxFileConfig::LineListRemove(LineList *pLine)
{
  LineList *pPrev = pLine->Prev(),
           *pNext = pLine->Next();

  // first entry?
  if ( pPrev == NULL )
    m_linesHead = pNext;
  else
    pPrev->SetNext(pNext);

  // last entry?
  if ( pNext == NULL )
    m_linesTail = pPrev;
  else
    pNext->SetPrev(pPrev);

  delete pLine;
}

bool wxFileConfig::LineListIsEmpty()
{
  return m_linesHead == NULL;
}

// ============================================================================
// wxFileConfig::ConfigGroup
// ============================================================================

// ----------------------------------------------------------------------------
// ctor/dtor
// ----------------------------------------------------------------------------

// ctor
ConfigGroup::ConfigGroup(ConfigGroup *pParent,
                                       const wxString& strName,
                                       wxFileConfig *pConfig)
                         : m_aEntries(CompareEntries),
                           m_aSubgroups(CompareGroups),
                           m_strName(strName)
{
  m_pConfig = pConfig;
  m_pParent = pParent;
  m_bDirty  = FALSE;
  m_pLine   = NULL;

  m_pLastEntry = NULL;
  m_pLastGroup = NULL;
}

// dtor deletes all children
ConfigGroup::~ConfigGroup()
{
  // entries
  size_t n, nCount = m_aEntries.Count();
  for ( n = 0; n < nCount; n++ )
    delete m_aEntries[n];

  // subgroups
  nCount = m_aSubgroups.Count();
  for ( n = 0; n < nCount; n++ )
    delete m_aSubgroups[n];
}

// ----------------------------------------------------------------------------
// line
// ----------------------------------------------------------------------------

void ConfigGroup::SetLine(LineList *pLine)
{
  wxASSERT( m_pLine == NULL ); // shouldn't be called twice

  m_pLine = pLine;
}

/*
  This is a bit complicated, so let me explain it in details. All lines that
  were read from the local file (the only one we will ever modify) are stored
  in a (doubly) linked list. Our problem is to know at which position in this
  list should we insert the new entries/subgroups. To solve it we keep three
  variables for each group: m_pLine, m_pLastEntry and m_pLastGroup.

  m_pLine points to the line containing "[group_name]"
  m_pLastEntry points to the last entry of this group in the local file.
  m_pLastGroup                   subgroup

  Initially, they're NULL all three. When the group (an entry/subgroup) is read
  from the local file, the corresponding variable is set. However, if the group
  was read from the global file and then modified or created by the application
  these variables are still NULL and we need to create the corresponding lines.
  See the following functions (and comments preceding them) for the details of
  how we do it.

  Also, when our last entry/group are deleted we need to find the new last
  element - the code in DeleteEntry/Subgroup does this by backtracking the list
  of lines until it either founds an entry/subgroup (and this is the new last
  element) or the m_pLine of the group, in which case there are no more entries
  (or subgroups) left and m_pLast<element> becomes NULL.

  NB: This last problem could be avoided for entries if we added new entries
      immediately after m_pLine, but in this case the entries would appear
      backwards in the config file (OTOH, it's not that important) and as we
      would still need to do it for the subgroups the code wouldn't have been
      significantly less complicated.
*/

// Return the line which contains "[our name]". If we're still not in the list,
// add our line to it immediately after the last line of our parent group if we
// have it or in the very beginning if we're the root group.
LineList *ConfigGroup::GetGroupLine()
{
  if ( m_pLine == NULL ) {
    ConfigGroup *pParent = Parent();

    // this group wasn't present in local config file, add it now
    if ( pParent != NULL ) {
      wxString strFullName;
      strFullName << wxT("[")
                  // +1: no '/'
                  << FilterOutEntryName(GetFullName().c_str() + 1)
                  << wxT("]");
      m_pLine = m_pConfig->LineListInsert(strFullName,
                                          pParent->GetLastGroupLine());
      pParent->SetLastGroup(this);  // we're surely after all the others
    }
    else {
      // we return NULL, so that LineListInsert() will insert us in the
      // very beginning
    }
  }

  return m_pLine;
}

// Return the last line belonging to the subgroups of this group (after which
// we can add a new subgroup), if we don't have any subgroups or entries our
// last line is the group line (m_pLine) itself.
LineList *ConfigGroup::GetLastGroupLine()
{
  // if we have any subgroups, our last line is the last line of the last
  // subgroup
  if ( m_pLastGroup != NULL ) {
    LineList *pLine = m_pLastGroup->GetLastGroupLine();

    wxASSERT( pLine != NULL );  // last group must have !NULL associated line
    return pLine;
  }

  // no subgroups, so the last line is the line of thelast entry (if any)
  return GetLastEntryLine();
}

// return the last line belonging to the entries of this group (after which
// we can add a new entry), if we don't have any entries we will add the new
// one immediately after the group line itself.
LineList *ConfigGroup::GetLastEntryLine()
{
  if ( m_pLastEntry != NULL ) {
    LineList *pLine = m_pLastEntry->GetLine();

    wxASSERT( pLine != NULL );  // last entry must have !NULL associated line
    return pLine;
  }

  // no entries: insert after the group header
  return GetGroupLine();
}

// ----------------------------------------------------------------------------
// group name
// ----------------------------------------------------------------------------

void ConfigGroup::Rename(const wxString& newName)
{
    m_strName = newName;

    LineList *line = GetGroupLine();
    wxString strFullName;
    strFullName << wxT("[") << (GetFullName().c_str() + 1) << wxT("]"); // +1: no '/'
    line->SetText(strFullName);

    SetDirty();
}

wxString ConfigGroup::GetFullName() const
{
  if ( Parent() )
    return Parent()->GetFullName() + wxCONFIG_PATH_SEPARATOR + Name();
  else
    return wxT("");
}

// ----------------------------------------------------------------------------
// find an item
// ----------------------------------------------------------------------------

// use binary search because the array is sorted
ConfigEntry *
ConfigGroup::FindEntry(const wxChar *szName) const
{
  size_t i,
       lo = 0,
       hi = m_aEntries.Count();
  int res;
  ConfigEntry *pEntry;

  while ( lo < hi ) {
    i = (lo + hi)/2;
    pEntry = m_aEntries[i];

    #if wxCONFIG_CASE_SENSITIVE
      res = wxStrcmp(pEntry->Name(), szName);
    #else
      res = wxStricmp(pEntry->Name(), szName);
    #endif

    if ( res > 0 )
      hi = i;
    else if ( res < 0 )
      lo = i + 1;
    else
      return pEntry;
  }

  return NULL;
}

ConfigGroup *
ConfigGroup::FindSubgroup(const wxChar *szName) const
{
  size_t i,
       lo = 0,
       hi = m_aSubgroups.Count();
  int res;
  ConfigGroup *pGroup;

  while ( lo < hi ) {
    i = (lo + hi)/2;
    pGroup = m_aSubgroups[i];

    #if wxCONFIG_CASE_SENSITIVE
      res = wxStrcmp(pGroup->Name(), szName);
    #else
      res = wxStricmp(pGroup->Name(), szName);
    #endif

    if ( res > 0 )
      hi = i;
    else if ( res < 0 )
      lo = i + 1;
    else
      return pGroup;
  }

  return NULL;
}

// ----------------------------------------------------------------------------
// create a new item
// ----------------------------------------------------------------------------

// create a new entry and add it to the current group
ConfigEntry *
ConfigGroup::AddEntry(const wxString& strName, int nLine)
{
  wxASSERT( FindEntry(strName) == NULL );

  ConfigEntry *pEntry = new ConfigEntry(this, strName, nLine);
  m_aEntries.Add(pEntry);

  return pEntry;
}

// create a new group and add it to the current group
ConfigGroup *
ConfigGroup::AddSubgroup(const wxString& strName)
{
  wxASSERT( FindSubgroup(strName) == NULL );

  ConfigGroup *pGroup = new ConfigGroup(this, strName, m_pConfig);
  m_aSubgroups.Add(pGroup);

  return pGroup;
}

// ----------------------------------------------------------------------------
// delete an item
// ----------------------------------------------------------------------------

/*
  The delete operations are _very_ slow if we delete the last item of this
  group (see comments before GetXXXLineXXX functions for more details),
  so it's much better to start with the first entry/group if we want to
  delete several of them.
 */

bool ConfigGroup::DeleteSubgroupByName(const wxChar *szName)
{
  return DeleteSubgroup(FindSubgroup(szName));
}

// doesn't delete the subgroup itself, but does remove references to it from
// all other data structures (and normally the returned pointer should be
// deleted a.s.a.p. because there is nothing much to be done with it anyhow)
bool ConfigGroup::DeleteSubgroup(ConfigGroup *pGroup)
{
  wxCHECK( pGroup != NULL, FALSE ); // deleting non existing group?

  // delete all entries
  size_t nCount = pGroup->m_aEntries.Count();
  for ( size_t nEntry = 0; nEntry < nCount; nEntry++ ) {
    LineList *pLine = pGroup->m_aEntries[nEntry]->GetLine();
    if ( pLine != NULL )
      m_pConfig->LineListRemove(pLine);
  }

  // and subgroups of this sungroup
  nCount = pGroup->m_aSubgroups.Count();
  for ( size_t nGroup = 0; nGroup < nCount; nGroup++ ) {
    pGroup->DeleteSubgroup(pGroup->m_aSubgroups[0]);
  }

  LineList *pLine = pGroup->m_pLine;
  if ( pLine != NULL ) {
    // notice that we may do this test inside the previous "if" because the
    // last entry's line is surely !NULL
    if ( pGroup == m_pLastGroup ) {
      // our last entry is being deleted - find the last one which stays
      wxASSERT( m_pLine != NULL );  // we have a subgroup with !NULL pLine...

      // go back until we find a subgroup or reach the group's line
      ConfigGroup *pNewLast = NULL;
      size_t n, nSubgroups = m_aSubgroups.Count();
      LineList *pl;
      for ( pl = pLine->Prev(); pl != m_pLine; pl = pl->Prev() ) {
        // is it our subgroup?
        for ( n = 0; (pNewLast == NULL) && (n < nSubgroups); n++ ) {
          // do _not_ call GetGroupLine! we don't want to add it to the local
          // file if it's not already there
          if ( m_aSubgroups[n]->m_pLine == m_pLine )
            pNewLast = m_aSubgroups[n];
        }

        if ( pNewLast != NULL ) // found?
          break;
      }

      if ( pl == m_pLine ) {
        wxASSERT( !pNewLast );  // how comes it has the same line as we?

        // we've reached the group line without finding any subgroups
        m_pLastGroup = NULL;
      }
      else
        m_pLastGroup = pNewLast;
    }

    m_pConfig->LineListRemove(pLine);
  }

  SetDirty();

  m_aSubgroups.Remove(pGroup);
  delete pGroup;

  return TRUE;
}

bool ConfigGroup::DeleteEntry(const wxChar *szName)
{
  ConfigEntry *pEntry = FindEntry(szName);
  wxCHECK( pEntry != NULL, FALSE );  // deleting non existing item?

  LineList *pLine = pEntry->GetLine();
  if ( pLine != NULL ) {
    // notice that we may do this test inside the previous "if" because the
    // last entry's line is surely !NULL
    if ( pEntry == m_pLastEntry ) {
      // our last entry is being deleted - find the last one which stays
      wxASSERT( m_pLine != NULL );  // if we have an entry with !NULL pLine...

      // go back until we find another entry or reach the group's line
      ConfigEntry *pNewLast = NULL;
      size_t n, nEntries = m_aEntries.Count();
      LineList *pl;
      for ( pl = pLine->Prev(); pl != m_pLine; pl = pl->Prev() ) {
        // is it our subgroup?
        for ( n = 0; (pNewLast == NULL) && (n < nEntries); n++ ) {
          if ( m_aEntries[n]->GetLine() == m_pLine )
            pNewLast = m_aEntries[n];
        }

        if ( pNewLast != NULL ) // found?
          break;
      }

      if ( pl == m_pLine ) {
        wxASSERT( !pNewLast );  // how comes it has the same line as we?

        // we've reached the group line without finding any subgroups
        m_pLastEntry = NULL;
      }
      else
        m_pLastEntry = pNewLast;
    }

    m_pConfig->LineListRemove(pLine);
  }

  // we must be written back for the changes to be saved
  SetDirty();

  m_aEntries.Remove(pEntry);
  delete pEntry;

  return TRUE;
}

// ----------------------------------------------------------------------------
//
// ----------------------------------------------------------------------------
void ConfigGroup::SetDirty()
{
  m_bDirty = TRUE;
  if ( Parent() != NULL )             // propagate upwards
    Parent()->SetDirty();
}

// ============================================================================
// wxFileConfig::ConfigEntry
// ============================================================================

// ----------------------------------------------------------------------------
// ctor
// ----------------------------------------------------------------------------
ConfigEntry::ConfigEntry(ConfigGroup *pParent,
                                       const wxString& strName,
                                       int nLine)
                         : m_strName(strName)
{
  wxASSERT( !strName.IsEmpty() );

  m_pParent = pParent;
  m_nLine   = nLine;
  m_pLine   = NULL;

  m_bDirty  = FALSE;

  m_bImmutable = strName[0] == wxCONFIG_IMMUTABLE_PREFIX;
  if ( m_bImmutable )
    m_strName.erase(0, 1);  // remove first character
}

// ----------------------------------------------------------------------------
// set value
// ----------------------------------------------------------------------------

void ConfigEntry::SetLine(LineList *pLine)
{
  if ( m_pLine != NULL ) {
    wxLogWarning(_("entry '%s' appears more than once in group '%s'"),
                 Name().c_str(), m_pParent->GetFullName().c_str());
  }

  m_pLine = pLine;
  Group()->SetLastEntry(this);
}

// second parameter is FALSE if we read the value from file and prevents the
// entry from being marked as 'dirty'
void ConfigEntry::SetValue(const wxString& strValue, bool bUser)
{
  if ( bUser && IsImmutable() ) {
    wxLogWarning(_("attempt to change immutable key '%s' ignored."),
                 Name().c_str());
    return;
  }

  // do nothing if it's the same value
  if ( strValue == m_strValue )
    return;

  m_strValue = strValue;
  
  if ( bUser ) {
    wxString strVal;
    if ( Group()->Config()->GetStyle() & wxCONFIG_USE_NO_ESCAPE_CHARACTERS ) {
        strVal = strValue;
    } else {
        strVal = FilterOutValue(strValue);
    }    
    wxString strLine;
    strLine << FilterOutEntryName(m_strName) << wxT('=') << strVal;

    if ( m_pLine != NULL ) {
      // entry was read from the local config file, just modify the line
      m_pLine->SetText(strLine);
    }
    else {
      // add a new line to the file
      wxASSERT( m_nLine == wxNOT_FOUND );   // consistency check

      m_pLine = Group()->Config()->LineListInsert(strLine,
                                                  Group()->GetLastEntryLine());
      Group()->SetLastEntry(this);
    }

    SetDirty();
  }
}

void ConfigEntry::SetDirty()
{
  m_bDirty = TRUE;
  Group()->SetDirty();
}

// ============================================================================
// global functions
// ============================================================================

// ----------------------------------------------------------------------------
// compare functions for array sorting
// ----------------------------------------------------------------------------

int CompareEntries(ConfigEntry *p1,
                   ConfigEntry *p2)
{
  #if wxCONFIG_CASE_SENSITIVE
    return wxStrcmp(p1->Name(), p2->Name());
  #else
    return wxStricmp(p1->Name(), p2->Name());
  #endif
}

int CompareGroups(ConfigGroup *p1,
                  ConfigGroup *p2)
{
  #if wxCONFIG_CASE_SENSITIVE
    return wxStrcmp(p1->Name(), p2->Name());
  #else
    return wxStricmp(p1->Name(), p2->Name());
  #endif
}

// ----------------------------------------------------------------------------
// filter functions
// ----------------------------------------------------------------------------

// undo FilterOutValue
static wxString FilterInValue(const wxString& str)
{
  wxString strResult;
  strResult.Alloc(str.Len());

  bool bQuoted = !str.IsEmpty() && str[0] == '"';

  for ( size_t n = bQuoted ? 1 : 0; n < str.Len(); n++ ) {
    if ( str[n] == wxT('\\') ) {
      switch ( str[++n] ) {
        case wxT('n'):
          strResult += wxT('\n');
          break;

        case wxT('r'):
          strResult += wxT('\r');
          break;

        case wxT('t'):
          strResult += wxT('\t');
          break;

        case wxT('\\'):
          strResult += wxT('\\');
          break;

        case wxT('"'):
          strResult += wxT('"');
          break;
      }
    }
    else {
      if ( str[n] != wxT('"') || !bQuoted )
        strResult += str[n];
      else if ( n != str.Len() - 1 ) {
        wxLogWarning(_("unexpected \" at position %d in '%s'."),
                     n, str.c_str());
      }
      //else: it's the last quote of a quoted string, ok
    }
  }

  return strResult;
}

// quote the string before writing it to file
static wxString FilterOutValue(const wxString& str)
{
   if ( !str )
      return str;

  wxString strResult;
  strResult.Alloc(str.Len());

  // quoting is necessary to preserve spaces in the beginning of the string
  bool bQuote = wxIsspace(str[0]) || str[0] == wxT('"');

  if ( bQuote )
    strResult += wxT('"');

  wxChar c;
  for ( size_t n = 0; n < str.Len(); n++ ) {
    switch ( str[n] ) {
      case wxT('\n'):
        c = wxT('n');
        break;

      case wxT('\r'):
        c = wxT('r');
        break;

      case wxT('\t'):
        c = wxT('t');
        break;

      case wxT('\\'):
        c = wxT('\\');
        break;

      case wxT('"'):
        if ( bQuote ) {
          c = wxT('"');
          break;
        }
        //else: fall through

      default:
        strResult += str[n];
        continue;   // nothing special to do
    }

    // we get here only for special characters
    strResult << wxT('\\') << c;
  }

  if ( bQuote )
    strResult += wxT('"');

  return strResult;
}

// undo FilterOutEntryName
static wxString FilterInEntryName(const wxString& str)
{
  wxString strResult;
  strResult.Alloc(str.Len());

  for ( const wxChar *pc = str.c_str(); *pc != '\0'; pc++ ) {
    if ( *pc == wxT('\\') )
      pc++;

    strResult += *pc;
  }

  return strResult;
}

// sanitize entry or group name: insert '\\' before any special characters
static wxString FilterOutEntryName(const wxString& str)
{
  wxString strResult;
  strResult.Alloc(str.Len());

  for ( const wxChar *pc = str.c_str(); *pc != wxT('\0'); pc++ ) {
    wxChar c = *pc;

    // we explicitly allow some of "safe" chars and 8bit ASCII characters
    // which will probably never have special meaning
    // NB: note that wxCONFIG_IMMUTABLE_PREFIX and wxCONFIG_PATH_SEPARATOR
    //     should *not* be quoted
    if ( !wxIsalnum(c) && !wxStrchr(wxT("@_/-!.*%"), c) && ((c & 0x80) == 0) )
      strResult += wxT('\\');

    strResult += c;
  }

  return strResult;
}

// we can't put ?: in the ctor initializer list because it confuses some
// broken compilers (Borland C++)
static wxString GetAppName(const wxString& appName)
{
    if ( !appName && wxTheApp )
        return wxTheApp->GetAppName();
    else
        return appName;
}

#endif // wxUSE_CONFIG

