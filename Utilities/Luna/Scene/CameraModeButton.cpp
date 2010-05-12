#include "Precompile.h"
#include "CameraModeButton.h"

#include "SceneEditor.h"

#include "UIToolKit/ImageManager.h"

using namespace Luna;

BEGIN_EVENT_TABLE( CameraModeButton, UIToolKit::MenuButton )
END_EVENT_TABLE()

CameraModeButton::CameraModeButton( wxWindow* parent, SceneEditor* sceneEditor, wxWindowID id )
: UIToolKit::MenuButton( parent, id, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW )
, m_CurrentMode( CameraModes::Orbit )
, m_SceneEditor( sceneEditor )
{
  SetMargins( 4 );
  const Reflect::Enumeration* cameraModeEnum = Reflect::Registry::GetInstance()->GetEnumeration( Reflect::GetType< CameraModes::CameraMode >() );

  m_Bitmaps[ CameraModes::Orbit ] = UIToolKit::GlobalImageManager().GetBitmap( "camera_perspective_32.png" );
  m_Bitmaps[ CameraModes::Front ] = UIToolKit::GlobalImageManager().GetBitmap( "camera_front_32.png" );
  m_Bitmaps[ CameraModes::Side ] = UIToolKit::GlobalImageManager().GetBitmap( "camera_side_32.png" );
  m_Bitmaps[ CameraModes::Top ] = UIToolKit::GlobalImageManager().GetBitmap( "camera_top_32.png" );

  std::string label;
  wxMenu* menu = new wxMenu();

  View* view = m_SceneEditor->GetView();
  Reflect::V_EnumerationElement::const_iterator modeItr = cameraModeEnum->m_Elements.begin();
  Reflect::V_EnumerationElement::const_iterator modeEnd = cameraModeEnum->m_Elements.end();
  for ( ; modeItr != modeEnd && ( *modeItr )->m_Value != CameraModes::Count; ++modeItr )
  {
    const Reflect::EnumerationElement* enumElem = *modeItr;
    CameraMode cameraMode = static_cast< CameraMode >( enumElem->m_Value );

    wxMenuItem* item = menu->AppendCheckItem( m_SceneEditor->CameraModeToSceneEditorID( cameraMode ), enumElem->m_Label.c_str() );
    menu->Check( item->GetId(), view->GetCameraMode() == cameraMode );
    Connect( item->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( CameraModeButton::OnChangeMode ), NULL, this );
  }

  SetContextMenu( menu );

  view->AddCameraModeChangedListener( CameraModeChangeSignature::Delegate( this, &CameraModeButton::OnCameraModeChanged ) );
  Connect( GetId(), wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CameraModeButton::OnNextCameraMode ), NULL, this );

  SetCameraMode( view->GetCameraMode() );
}

CameraModeButton::~CameraModeButton()
{
}

void CameraModeButton::SetCameraMode( CameraMode mode )
{
  m_contextMenu->Check( m_SceneEditor->CameraModeToSceneEditorID( m_CurrentMode ), false );
  m_CurrentMode = mode;
  SetBitmapLabel( m_Bitmaps[ m_CurrentMode ] );
  m_contextMenu->Check( m_SceneEditor->CameraModeToSceneEditorID( m_CurrentMode ), true );
}

CameraMode CameraModeButton::GetCameraMode()
{
  return m_CurrentMode;
}

void CameraModeButton::OnChangeMode( wxCommandEvent& args )
{
  m_SceneEditor->GetView()->SetCameraMode( m_SceneEditor->SceneEditorIDToCameraMode( static_cast< SceneEditorID >( args.GetId() ) ) );
}

void CameraModeButton::OnNextCameraMode( wxCommandEvent& args )
{
  m_SceneEditor->GetView()->NextCameraMode();
}

void CameraModeButton::OnCameraModeChanged( const CameraModeChangeArgs& args )
{
  SetCameraMode( args.m_NewMode );
}
