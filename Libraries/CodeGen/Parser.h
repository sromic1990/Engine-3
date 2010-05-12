#pragma once

#include "API.h"
#include "Namespace.h"
#include <string>

// Forwards
class TiXmlElement;

namespace CodeGen
{
  /////////////////////////////////////////////////////////////////////////////
  // Class for parsing an xml configuration file into header information
  // (namespaces, enumerations).
  // 
  class CODEGEN_API Parser
  {
  protected:
    std::string m_File;
    std::string m_LastError;
    NamespacePtr m_Root;

  public:
    Parser();
    virtual ~Parser();
    bool Parse( const std::string& file );
    const std::string& GetLastError() const;
    NamespacePtr GetRootNamespace() const;

  protected:
    NamespacePtr ParseNamespace( TiXmlElement* element );
    EnumPtr ParseEnum( TiXmlElement* element );
  };
}
