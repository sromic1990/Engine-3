#pragma once

#include "API.h"
#include "ShaderAsset.h"
#include "TUID/TUID.h"

namespace Asset
{
  class ASSET_API StandardShaderAsset : public ShaderAsset
  {
  public:
    bool m_CinematicShader;


    StandardShaderAsset()
      : m_CinematicShader( false )
    {
    }

    void MakeDefault();

    virtual void PostDeserialize() NOC_OVERRIDE;

    virtual bool ValidateCompatible( const Attribute::AttributePtr& attr, std::string& error ) const NOC_OVERRIDE;

    virtual bool IsCinematicShader() const NOC_OVERRIDE;

    REFLECT_DECLARE_CLASS( StandardShaderAsset, ShaderAsset );

    static void EnumerateClass( Reflect::Compositor<StandardShaderAsset>& comp );
  };

  typedef Nocturnal::SmartPtr< StandardShaderAsset > StandardShaderAssetPtr;
}