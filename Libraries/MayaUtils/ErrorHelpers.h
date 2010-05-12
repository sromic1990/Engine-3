#pragma once

#include <sstream>
#include <iostream>
#include <maya/MStatus.h>
#include <maya/MString.h>
#include <maya/M3DView.h>


//
// Error code helpers
//

inline void ErrorHelper(const MStatus& stat, const char* msg, const char* file, const char* func, long line)
{
  std::cerr << msg
            << "("
            << stat.errorString().asChar()
            << ") in file: "
            << file
            << "::"
            << func
            << ":"
            << line
            << std::endl
            << std::flush;
}

#ifdef MContinueErr
#undef MContinueErr
#endif
#define MContinueErr(stat,msg)                                    \
if ( MS::kSuccess != stat)                                        \
{                                                                 \
  ErrorHelper(stat, msg, __FILE__, __FUNCTION__, __LINE__);       \
  continue;                                                       \
}                                           

#ifdef MErr
#undef MErr
#endif
#define MErr(stat,msg)                                            \
if ( MS::kSuccess != stat)                                        \
{                                                                 \
  ErrorHelper(stat, msg, __FILE__, __FUNCTION__, __LINE__);       \
}  

#ifdef MCheckErr
#undef MCheckErr
#endif
#define MCheckErr(stat,msg)                                                         \
if( MS::kSuccess !=  stat )                                                         \
{                                                                                   \
  ErrorHelper(stat, msg, __FILE__, __FUNCTION__, __LINE__);                         \
  return MS::kFailure;                                                              \
}

#ifdef MCheckNoErr
#undef MCheckNoErr
#endif
#define MCheckNoErr(stat,msg)                                                       \
if( MS::kSuccess != stat )                                                          \
{                                                                                   \
  ErrorHelper(stat, msg, __FILE__, __FUNCTION__, __LINE__);                         \
  return;                                                                           \
}

#ifdef MMsgErr
#undef MMsgErr
#endif
#define MMsgErr(stat,msg)                                                                         \
if( MS::kSuccess != stat )                                                                        \
{                                                                                                 \
  std::string str = std::string(msg) + "(" + std::string(stat.errorString().asChar()) + ")\n";    \
  MGlobal::displayError(str.c_str());                                                             \
  return MS::kFailure;                                                                            \
}


//
// SEH exception helpers
//

#include "Debug\Utils.h"

inline void ProcessMayaPluginException( Debug::StructuredException& ex, bool exit = true )
{
  Debug::ProcessException( ex, true );

  std::ostringstream str;
  str << "A fatal plugin error occurred so Maya must be shut down." << std::endl;
  str << ex.what();

  ::MessageBox( M3dView::applicationShell(),
                str.str().c_str(),
                "Error",
                MB_OK );

  if (exit)
  {
    ::ExitProcess( -1 );
  }
}

#define MAYA_START_EXCEPTION_HANDLING() Debug::EnableTranslator<Debug::TranslateException> translator; try {

#define MAYA_FINISH_EXCEPTION_HANDLING(...) } catch ( Debug::StructuredException& ex ) { ProcessMayaPluginException( ex, __VA_ARGS__ ); }