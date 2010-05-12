#pragma once

#include "API.h"

#include "Common/Types.h"

#include "ExportNode.h"
#include "MayaUtils/Utils.h"
#include "Content/ContentTypes.h"

#include <maya/MPxObjectSet.h>

class MAYANODES_API ExportNodeSet : public MPxObjectSet 
{
public:
  static MTypeId s_TypeID;  
  static MString s_TypeName;

  static Maya::S_MObject ExportNodeSets;
  static MObject SceneNodeSet;

  ExportNodeSet();
  ~ExportNodeSet();

  static  void *      Creator();
  static  MStatus			Initialize();	

 
  void GetExportNodes( MObjectArray &exportNodes, 
                       Content::ContentType nodeType = Content::ContentTypes::Null, 
                       int num = -1 );
  
  //
  // ExportNodeSet - related callbacks
  //

  // scene-wide callbacks
  static void PreImportCB( void* clientData );
  static void PostImportCB( void* clientData );


  // static query API
  static MObject GetSceneExportNodeSet( bool createIfNoExist = false );
  
  static MStatus GetSceneExportNodes( MObjectArray &exportNodes, 
                                      Content::ContentType nodeType = Content::ContentTypes::Null , 
                                      int num = -1 );

  static MStatus GetExportNodes( const MObject &set,
                                 MObjectArray &exportNodes,
                                 Content::ContentType nodeType = Content::ContentTypes::Null , 
                                 int num = -1 );

};
