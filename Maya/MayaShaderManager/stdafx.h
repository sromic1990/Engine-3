#pragma once

// windows
#include "Windows/Windows.h"

// stdlib
#include <algorithm>

// maya
#include <maya/M3dView.h>
#include <maya/MArgDatabase.h>
#include <maya/MArgList.h>
#include <maya/MColor.h>
#include <maya/MDGMessage.h>
#include <maya/MDagPath.h>
#include <maya/MDagPathArray.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MDistance.h>
#include <maya/MFnAnimCurve.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFnDagNode.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnMesh.h>
#include <maya/MFnMessageAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnSet.h>
#include <maya/MFnSkinCluster.h>
#include <maya/MFnTransform.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MGlobal.h>
#include <maya/MImage.h>
#include <maya/MItDag.h>
#include <maya/MItDependencyGraph.h>
#include <maya/MItDependencyNodes.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshEdge.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MItMeshVertex.h>
#include <maya/MItSelectionList.h>
#include <maya/MMatrix.h>
#include <maya/MPlug.h>
#include <maya/MPlugArray.h>
#include <maya/MPxCommand.h>
#include <maya/MPxHwShaderNode.h>
#include <maya/MPxLocatorNode.h>
#include <maya/MPxManipContainer.h>
#include <maya/MSceneMessage.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MString.h>
#include <maya/MSyntax.h>
#include <maya/MTypeId.h>
#include <maya/MVector.h>

// wx
#include <wx/wx.h>
#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/choice.h>
#include <wx/image.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/splitter.h>
#include <wx/statbmp.h>
#include <wx/statline.h>