#include "Precompile.h"
#include "Frame.h"

#include <cctype>
#include <algorithm>
#include <wx/config.h>

#include "AppUtils/AppUtils.h"
#include "Common/Config.h"
#include "Common/Environment.h"
#include "Common/String/Utilities.h"


// Using
using namespace Luna;


// Static UI event table
BEGIN_EVENT_TABLE( Frame, wxFrame )
EVT_SET_FOCUS( Frame::OnSetFocus )
EVT_CLOSE( Frame::OnExiting )
END_EVENT_TABLE()


///////////////////////////////////////////////////////////////////////////////
// *R2 <[<title> - ]>[ <code>(<build>), assets: <assets> ]<[: <extra>>]
// *R2 Luna Asset Editor - [ tech(debug), assets: devel ]: x:/r2/assets/devel/entities/things/stuff/random.entity.irb
static const char* s_ProgrammerTitleBarFormat = "%s(p) %s[ %s%s, assets: %s ]"; //project, title, code, build, assets, extra

// *R2 <[<title> - ]>[ assets: <assets>, tech/tools: <code> ]<[: <extra>>]
// R2 Luna Asset Editor - [ assets: devel, tech/tools: devel ]: x:/r2/assets/devel/entities/things/stuff/random.entity.irb
static const char* s_UserTitleBarFormat = "%s %s[ assets: %s, tech/tools: %s ]"; //project, title, assets, code, extra



///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
Frame::Frame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name )
: wxFrame( parent, id, title, pos, size, style, name )
, m_CommandQueue( this )
{
  // tell wxAuiManager to manage this frame
  m_FrameManager.SetFrame( this ); 

  SetTitle( GetEShellTitle( title.c_str() ).c_str() );
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
// 
Frame::~Frame()
{
  m_FrameManager.UnInit();
}

///////////////////////////////////////////////////////////////////////////////
// Delays exectution of the specified command until the next time the message
// pump for the UI runs.  Useful for executing a command from within a UI 
// callback, when that command may cause the UI to be destroyed.
// 
void Frame::PostCommand( const Undo::CommandPtr& command )
{
  m_CommandQueue.Push( command );
}

///////////////////////////////////////////////////////////////////////////////
// Create the Luna frame title from the EvnShell settings
//
std::string Frame::GetEShellTitle( const std::string& title, const std::string& extra )
{
  std::string windowTitle;
  GetEShellTitle( windowTitle, title, extra );
  return windowTitle;
}

///////////////////////////////////////////////////////////////////////////////
// Create the Luna frame title from the EvnShell settings
//
void Frame::GetEShellTitle( std::string& windowTitle, const std::string& title, const std::string& extra )
{
  windowTitle.clear();

  if ( !title.empty() )
  {
    windowTitle += title;
    windowTitle += " - ";
  }

  std::string eshellTitle;
  Nocturnal::GetEnvVar( "ESHELL_TITLE", eshellTitle );
  windowTitle += eshellTitle;

  // if there is extra stuff tack it on
  if ( !extra.empty() )
  {
    windowTitle += ": ";
    windowTitle += extra;
  }
}

///////////////////////////////////////////////////////////////////////////////
// Adds a prefix to the key so that it is unique between editors/frames.
//
std::string Frame::PrefixPreferenceKey( const std::string& key ) const
{
  return GetPreferencePrefix() + std::string( ":" ) + key;
}

///////////////////////////////////////////////////////////////////////////////
// Creates a menu that enumerates all panes attached to the frame manager, as
// long as the pane has a caption.  The menu items will display checkmarks next
// to the name if the panel is currently visible.  Clicking on a menu item will
// toggle its visibility.  Provided as a convenience function so that the base
// classes can just drop this menu on their menu bar.
// 
u32 Frame::CreatePanelsMenu( wxMenu* menu )
{
  u32 numMenuOptionsAdded = 0;
  wxAuiPaneInfoArray& panes = m_FrameManager.GetAllPanes();
  const size_t numPanes = panes.Count();
  for ( size_t index = 0; index < numPanes; ++index )
  {
    wxAuiPaneInfo& pane = panes.Item( index );
    if ( !pane.caption.empty() )
    {
      wxMenuItem* item = menu->AppendCheckItem( wxID_ANY, pane.caption );
      Connect( item->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( Frame::OnShowPanel ), NULL, this );
      m_Panels.insert( M_MenuIdToPanel::value_type( item->GetId(), pane.window ) );
      numMenuOptionsAdded++;
    }
  }

  return numMenuOptionsAdded;
}

///////////////////////////////////////////////////////////////////////////////
// Convenience function for adding check marks next to menu items of panels that
// are currently shown.
// 
void Frame::UpdatePanelsMenu( wxMenu* menu )
{
  const wxAuiPaneInfoArray& panes = m_FrameManager.GetAllPanes();
  const size_t numPanes = panes.Count();
  for ( size_t index = 0; index < numPanes; ++index )
  {
    const wxAuiPaneInfo& pane = panes.Item( index );
    i32 itemId = menu->FindItem( pane.caption );
    if ( itemId >= 0 )
    {
      menu->Check( itemId, pane.IsShown() );
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Makes this editor the top level window when it becomes focused
// 
void Frame::OnSetFocus( wxFocusEvent& args )
{
  wxTheApp->SetTopWindow( this );
}

///////////////////////////////////////////////////////////////////////////////
// Called when the user clicks on an item in the "panels" menu.  Toggles the 
// visibility of the specified panel.
// 
void Frame::OnShowPanel( wxCommandEvent& args )
{
  M_MenuIdToPanel::const_iterator found = m_Panels.find( args.GetId() );
  if ( found != m_Panels.end() )
  {
    wxWindow* window = found->second;
    wxAuiPaneInfo& pane = m_FrameManager.GetPane( window );
    if ( pane.IsOk() )
    {
      pane.Show( !pane.IsShown() );
      m_FrameManager.Update();
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Called when existing the frame.  Saves the window state.
// 
void Frame::OnExiting( wxCloseEvent& args )
{
  SaveWindowState();
  args.Skip();
}
