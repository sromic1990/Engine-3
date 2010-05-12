#include "stdafx.h"
#include "GameplayCylinder.h"

#include "MayaUtils/NodeTypes.h"

const MTypeId GameplayCylinder::s_TypeID( IGL_GP_CYLINDER );
const char* GameplayCylinder::s_TypeName = "gpCylinder";

MStatus GameplayCylinder::Initialize()
{ 
  return inheritAttributesFrom( Cylinder::s_TypeName );
}
