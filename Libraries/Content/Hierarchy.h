#pragma once

#include "API.h"
#include "Reflect/Serializers.h"

namespace Content
{
  // This contiains the entire hierarchy information for the file.  Each file can contain at most one hierarchy.  The hierarchy element must be
  // the last element of the file, so indiviual nodes can exist before hierarchical constuction begins. An ID of UniqueID::TUID::Null indicates the node
  // is in the root of the hierarchy.
  class CONTENT_API Hierarchy : public Reflect::Element
  {
  public:
    // UIDs of the objects
    UniqueID::V_TUID m_Objects;

    // UIDs of thier parents
    UniqueID::V_TUID m_Parents;

    REFLECT_DECLARE_CLASS(Hierarchy, Reflect::Element);

    static void EnumerateClass( Reflect::Compositor<Hierarchy>& comp );

    // Allocates the object and parent vectors
    void Reserve (unsigned p_NodeCount);

    // Appends a pair of IDs to thier respective arrays
    void AddParentID (UniqueID::TUID p_Node, UniqueID::TUID p_Parent = UniqueID::TUID::Null);

    // Searches the child ID array for the passed ID, and returns the corresponding ID in the parent ID array
    UniqueID::TUID GetParentID (UniqueID::TUID p_Node);
  
  };

  typedef Nocturnal::SmartPtr<Hierarchy> HierarchyPtr;
  typedef std::vector<HierarchyPtr> V_Hierarchy;
}