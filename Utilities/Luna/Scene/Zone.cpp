#include "Precompile.h"
#include "Zone.h"
#include "ZonePanel.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneNodeType.h"

#include "Content/Zone.h"
#include "Core/Enumerator.h"
#include "File/Exceptions.h"
#include "UIToolKit/ImageManager.h"

// Using
using namespace Luna;

LUNA_DEFINE_TYPE( Zone );

void Zone::InitializeType()
{
  Reflect::RegisterClass< Zone >( "Luna::Zone" );
  Enumerator::InitializePanel( "Zone", CreatePanelSignature::Delegate( &Zone::CreatePanel ) );
}

void Zone::CleanupType()
{
  Reflect::UnregisterClass< Zone >();
}

Zone::Zone( Luna::Scene* scene, Content::Zone* zone )
: Luna::SceneNode( scene, zone )
{
}

Zone::~Zone()
{

}

i32 Zone::GetImageIndex() const
{
  return UIToolKit::GlobalImageManager().GetImageIndex( "zone_16.png" );
}

std::string Zone::GetApplicationTypeName() const
{
  return "Zone";
}

std::string Zone::GetPath() const
{
  Content::ZonePtr zone = GetPackage< Content::Zone >();
  return zone->GetFilePath();
}

///////////////////////////////////////////////////////////////////////////////
// Returns the TUID of the file that backs this zone.
// 
tuid Zone::GetFileID() const
{
  return GetPackage< Content::Zone >()->m_FileID;
}

const Math::Color3& Zone::GetColor() const
{
  return GetPackage< Content::Zone >()->m_Color;
}

bool Zone::ValidatePanel(const std::string& name)
{
  if ( name == "Zone" )
    return true;

  return __super::ValidatePanel( name );
}

void Zone::CreatePanel( CreatePanelArgs& args )
{
  ZonePanel* panel = new ZonePanel ( args.m_Enumerator, args.m_Selection );

  args.m_Enumerator->Push( panel );
  {
    panel->SetCanvas( args.m_Enumerator->GetContainer()->GetCanvas() );
    panel->Create();
  }
  args.m_Enumerator->Pop();
}

void Zone::PackageChanged( const Reflect::ElementChangeArgs& args )
{
  __super::PackageChanged( args );

  Content::Zone* zone = GetPackage< Content::Zone >();
  if ( args.m_Field == zone->GetClass()->FindField( &Content::Zone::m_Color ) )
  {
    Luna::Scene* loadedScene = m_Scene->GetManager()->GetScene( GetFileID() );
    if ( loadedScene )
    {
      loadedScene->SetColor( zone->m_Color );
    }
  }
}
