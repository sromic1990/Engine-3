#include "utils.h"

#include <maya/MItDag.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MFnDagNode.h>
#include <maya/MFileObject.h>
#include <maya/MPlug.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnStringData.h>

#include "ErrorHelpers.h"

#include <sstream>

namespace Maya
{
  void S_MObjectToObjectArray( const S_MObject &nodes, MObjectArray &array )
  {
    S_MObject::const_iterator itor;
    for( itor = nodes.begin(); itor != nodes.end(); ++itor )
    {
      array.append( *itor );
    }
  }
  static MStatus _findNodesOfType( MItDag &itor, S_MObject &nodes, const MString &typeStr )
  {
    MStatus stat;

    for (;!itor.isDone();itor.next()) 
    {
      MObject obj = itor.item(&stat);
      MCheckErr(stat, "Unable to do: itDag.item");

      if( ! obj.hasFn( MFn::kDagNode ) ) continue;

      MFnDependencyNode node(obj, &stat);
      MCheckErr(stat, "Unable to do: MFnDagNode node");

      MString nodeType = node.typeName( &stat );
      MCheckErr(stat, "Unable to do: node.typeId");

      if( nodeType == typeStr )
      {
        nodes.insert( obj );
      }
    }
    return MS::kSuccess;
  }

  static MStatus _findNodesOfType( MItDag &itor, MObjectArray &array, const MString &typeStr )
  {
    S_MObject set;
    MStatus stat = _findNodesOfType( itor, set, typeStr );

    S_MObjectToObjectArray( set, array );

    return stat;
  }

  static MStatus  _findNodesOfType( MItDependencyNodes &itor, S_MObject &nodes, const MString &typeStr )
  {
    MStatus stat;

    for (;!itor.isDone();itor.next()) 
    {
      MObject obj = itor.item(&stat);
      MCheckErr(stat, "Unable to do: itDag.item");

      if( obj.hasFn( MFn::kDagNode ) ) continue;

      MFnDependencyNode node(obj, &stat);
      MCheckErr(stat, "Unable to do: MFnDagNode node");

      MString nodeType = node.typeName( &stat );
      MCheckErr(stat, "Unable to do: node.typeId");

      if( nodeType == typeStr )
      {
        nodes.insert( obj );
      }
    }
    return MS::kSuccess;
  }

  static MStatus  _findNodesOfType( MItDependencyNodes &itor, MObjectArray &array, const MString &typeStr )
  {
    S_MObject set;
    MStatus stat = _findNodesOfType( itor, set, typeStr );

    S_MObjectToObjectArray( set, array );

    return stat;
  }

  MStatus findNodesOfType(S_MObject& objects, const MTypeId &typeId, const MFn::Type FnType, MObject& pathRoot ) 
  {
    // Find all the ref nodes in the Dag
    MStatus stat;

    if ( pathRoot != MObject::kNullObj )
    {
      // Create the Dag iterator
      MItDag itDag( MItDag::kDepthFirst, FnType, &stat );
      MCheckErr(stat, "Unable to do: MItDag itDag");

      stat = itDag.reset( pathRoot );
      MCheckErr(stat, "Unable to do: itDag.reset()");

      for ( ; !itDag.isDone(); itDag.next() )
      {
        MObject obj = itDag.item(&stat);
        MCheckErr(stat, "Unable to do: itDag.item");

        MFnDagNode node(obj, &stat);
        MCheckErr(stat, "Unable to do: MFnDagNode node");

        MTypeId nodeType = node.typeId( );

        if( nodeType == typeId )
          objects.insert(obj);      
      }
    }
    else
    {
      // Create the Dag iterator
      MItDependencyNodes itDep( FnType, &stat );
      MCheckErr(stat, "Unable to do: MItDependencyNodes itDep");

      for ( ; !itDep.isDone(); itDep.next() )
      {
        MObject obj = itDep.item(&stat);
        MCheckErr(stat, "Unable to do: itDag.item");

        MFnDependencyNode node(obj, &stat);
        MCheckErr(stat, "Unable to do: MFnDagNode node");

        MTypeId nodeType = node.typeId( );

        if( nodeType == typeId )
          objects.insert(obj);      
      }
    }

    return stat;
  }

  MStatus findNodesOfType(MObjectArray& oArray, const MTypeId &typeId, const MFn::Type FnType, MObject& pathRoot ) 
  {
    S_MObject set;

    MStatus stat = findNodesOfType( set, typeId, FnType, pathRoot );

    S_MObjectToObjectArray( set, oArray );

    return stat;
  }

  MStatus findNodesOfType( S_MObject &nodes, const MString &typeStr, bool isDagNode )
  {
    // Find all the ref nodes in the Dag

    MStatus stat;

    if( isDagNode )
    {
      // Create the Dag iterator
      MItDag itor(MItDag::kDepthFirst, MFn::kInvalid, &stat);
      MCheckErr(stat, "Unable to do: MItDag itDag");

      stat = _findNodesOfType( itor, nodes, typeStr );
    }
    else
    {
      MItDependencyNodes itor;
      stat = _findNodesOfType( itor, nodes, typeStr );
    }

    return stat;

  }

  MStatus findNodesOfType( MObjectArray &array, const MString &typeStr, bool isDagNode )
  {
    S_MObject set;
    MStatus stat = findNodesOfType( set, typeStr, isDagNode );

    S_MObjectToObjectArray( set, array );

    return stat;
  }

  MStatus findNodesOfType( S_MObject &nodes, const MFn::Type FnType, MObject& pathRoot )
  {
    // Find all the ref nodes in the Dag
    MStatus stat;

    if ( pathRoot != MObject::kNullObj )
    {
      // Create the dag iterator
      MItDag dagIt( MItDag::kDepthFirst, FnType, &stat );
      MCheckErr(stat, "Unable to do: MItDag itDag");

      stat = dagIt.reset( pathRoot );
      MCheckErr(stat, "Unable to do: itDag.reset()");

      for ( ; !dagIt.isDone(); dagIt.next() ) 
      {
        MObject obj = dagIt.item(&stat);
        MCheckErr(stat, "Unable to do: itDag.item");

        if( FnType != MFn::kInvalid && !obj.hasFn(FnType) )
          continue;

        if( obj != MObject::kNullObj ) nodes.insert( obj );
      }
    }
    else
    {
      // Create the dependency iterator
      MItDependencyNodes depIt( FnType, &stat );
      MCheckErr(stat, "Unable to do: MItDependencyNodes depIt");

      for (;!depIt.isDone();depIt.next()) 
      {
        MObject obj = depIt.item(&stat);
        MCheckErr(stat, "Unable to do: depIt.item");

        if( !obj.hasFn(FnType) )
          continue;

        if( obj != MObject::kNullObj ) nodes.insert( obj );
      }
    }
  
    return stat;
  }

  MStatus findNodesOfType(MObjectArray& oArray, const MFn::Type FnType, MObject& pathRoot ) 
  {
    S_MObject set;

    MStatus stat = findNodesOfType( set, FnType, pathRoot );

    S_MObjectToObjectArray( set, oArray );

    return stat;
  }

  void appendObjectArray( MObjectArray & receiver, const MObjectArray & source )
  {
    const unsigned int sourceLength = source.length();
    for (unsigned int i=0; i<sourceLength; i++)
    {
      receiver.append( source[i] );
    }
  }

  void appendObjectArray( S_MObject &receiver, const MObjectArray & source )
  {
    const unsigned int sourceLength = source.length();
    for (unsigned int i=0; i<sourceLength; i++)
    {
      receiver.insert( source[i] );
    }
  }

  bool Exists( const MString& name )
  {

    MSelectionList selList;

    MGlobal::getSelectionListByName(name, selList);

    if (selList.length() != 1) return false;

    return true;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // 
  MStatus SetTUIDAttribute( MObject &object, const MString& idAttributeName, tuid id, const bool hidden )
  {
    MStatus status( MStatus::kSuccess );

    std::stringstream idStr;
    idStr << TUID::HexFormat << id;

    MString idMStr( idStr.str().c_str() );
    return SetStringAttribute( object, idAttributeName, idStr.str(), hidden );
  }


  ///////////////////////////////////////////////////////////////////////////////
  // 
  tuid GetTUIDAttribute( const MObject &object, const MString& idAttributeName, MStatus* returnStatus )
  {
    MStatus status( MStatus::kSuccess );

    std::string idStr;
    status = GetStringAttribute( object, idAttributeName, idStr );

    tuid id = TUID::Null;

    if ( !idStr.empty() )
    {
      std::istringstream idStream ( idStr );
      idStream >> std::hex >> id;
    }

    if ( returnStatus != NULL )
    {
      *returnStatus = status;
    }

    return id;
  }


  ///////////////////////////////////////////////////////////////////////////////
  // 
  MStatus SetStringAttribute( MObject &object, const MString& attributeName, const std::string& stringAtr, const bool hidden )
  {
    // locate/create an attribute to store our id attribute
    // we use one of maya's compound types to store our 64 bit id
    MStatus status( MStatus::kSuccess );

    MObject attribute;
    MFnTypedAttribute attributeFn;

    MFnDependencyNode nodeFn( object );

    // check to see if we are a locked node
    bool nodeWasLocked = nodeFn.isLocked();
    if ( nodeWasLocked )
    {
      // turn off any node locking so an attribute can be added
      nodeFn.setLocked( false );
    }

    if ( nodeFn.hasAttribute( attributeName, &status ) )
    {
      attribute = nodeFn.attribute( attributeName );
    }
    else
    {
      attribute = attributeFn.create( attributeName, attributeName, MFnData::kString );
      nodeFn.addAttribute( attribute );
    }

    status = attributeFn.setObject( attribute );
    attributeFn.setHidden( hidden );

    MPlug objPlug( object, attribute );

    objPlug.setLocked( false );
    objPlug.setValue( stringAtr.c_str() );
    objPlug.setLocked( true );

    // reset to the prior state of wasLocked
    if ( nodeWasLocked )
    {
      nodeFn.setLocked( nodeWasLocked );
    }

    return status;
  }


  ///////////////////////////////////////////////////////////////////////////////
  // 
  MStatus GetStringAttribute( const MObject &object, const MString& attributeName, std::string& stringAtr )
  {
    MStatus status( MStatus::kSuccess );
    
    stringAtr = "";
    
    MFnDependencyNode nodeFn( object );

    MObject attribute = nodeFn.attribute( attributeName, &status );
    if ( !status )
    {
      return status;
    }

    MPlug objPlug = nodeFn.findPlug( attribute, &status );
    if ( !status )
    {
      return status;
    }

    MObject objValue;
    status = objPlug.getValue( objValue );

    MFnStringData stringDataFn;
    status = stringDataFn.setObject( objValue );

    stringAtr = stringDataFn.string( &status ).asChar();

    return status ;
  }


  ///////////////////////////////////////////////////////////////////////////////
  // Removes an attribute from the given object
  //
  MStatus RemoveAttribute( MObject& object, const MString& attributeName )
  {
    MStatus status( MStatus::kSuccess );

    MFnDependencyNode nodeFn( object, &status );

    if ( nodeFn.hasAttribute( attributeName, &status ) )
    {
      MObject attrObj = nodeFn.attribute( attributeName, &status );
      NOC_ASSERT( status != MS::kFailure );

      MPlug attrObjPlug( object, attrObj );

      // check to see if we are a locked node
      bool nodeWasLocked = nodeFn.isLocked();
      if ( nodeWasLocked )
      {
        // turn off any node locking so an attribute can be added
        nodeFn.setLocked( false );
      }

      // unlock the attribute
      status = attrObjPlug.setLocked( false );
      NOC_ASSERT( status != MS::kFailure );

      // remove the attribute
      status = nodeFn.removeAttribute( attrObj );
      NOC_ASSERT( status != MS::kFailure );

      // reset to the prior state of wasLocked
      if ( nodeWasLocked )
      {
        nodeFn.setLocked( nodeWasLocked );
      }
    }

    return status;
  }


  MStatus LockHierarchy(MObject& obj, bool state) 
  {

    MStatus stat;

    MFnDependencyNode node(obj);

    // Create the Dag iterator - only care about transforms
    MItDag itDag(MItDag::kDepthFirst, MFn::kTransform, &stat);
    MCheckErr(stat, "Unable to do: MItDag itDag");

    // set the start object - only care about transforms
    stat = itDag.reset(obj, MItDag::kDepthFirst, MFn::kTransform);
    MCheckErr(stat, "Unable to do: itDag.reset");

    // Iterate the Dag
    for (;!itDag.isDone();itDag.next()) {

      // Get child object
      MObject childObj = itDag.item(&stat);
      MCheckErr(stat, "Unable to do: itDag.item");

      // Lock child object attributes and node
      stat = LockAttributes(childObj, state);
      MCheckErr(stat, "Unable to do: LockAttributes");

    }

    // Lock start object - node and attributes
    stat = LockAttributes(obj, state);
    MCheckErr(stat, "Unable to do: LockAttributes");

    return MS::kSuccess;
  }

  MStatus LockAttributes(MObject& obj, bool state) 
  {

    MStatus stat;

    // get node
    MFnDependencyNode node(obj, &stat);
    MCheckErr(stat, "Unable to do: MFnDependencyNode node");

    // Get number of attribs
    unsigned attributeCount = node.attributeCount(&stat);
    MCheckErr(stat, "Unable to do: node.attributeCount");

    // Walk attribs
    for (unsigned i=0;i<attributeCount;++i) {

      // Get attrib object
      MObject attribObj = node.attribute(i, &stat);
      MCheckErr(stat, "Unable to do: node.attribute");

      // Get plug to attrib
      MPlug plug(obj, attribObj);	
      if (plug.isNull()) {
        stat = MS::kFailure;
        MCheckErr(stat, "Unable to do: MPlug plug(obj, attribObj)");
      }

      // Lock attrib
      stat = plug.setLocked(state);
      MCheckErr(stat, "Unable to do: plug.setLocked");
    }

    // Lock node
    //stat = setNodeLocked(obj, state);
    stat = node.setLocked(state);
    MCheckErr(stat, "Unable to do: node.setLocked(state)");

    return MS::kSuccess;
  }

  MStatus RemoveHierarchy( MObject& object, MSelectionList& list )
  {
    MStatus status;

    u32 len = list.length();
    for( u32 i = 0; i < len; ++i )
    {
      MObject obj;
      status = list.getDependNode( i, obj );
      if( obj == object )
      {
        status = list.remove( i );
        break;
      }
    }

    MFnDagNode nodeFn( object );
    len = nodeFn.childCount();
    for( u32 i = 0; i < len; ++i )
    {
      status = RemoveHierarchy( nodeFn.child( i ), list );
    }
    return status;
  }
  
}
