#ifndef __Template_h__
#define __Template_h__

const char* header_h = "#ifndef __Header_h__\n\
#define __Header_h__\n\
\n\
#include \"i{name}.h\"\n\
\n\
using namespace XEngine;\n\
using namespace XEngine::Api;\n\
\n\
extern iEngine* g_engine;\n\
class {name};\n\
extern {name}* g_{name};\n\
\n\
#endif //__Header_h__\n\
";

const char* interface_h = "#ifndef __i{name}_h__\n\
#define __i{name}_h__\n\
\n\
#include \"iModule.h\"\n\
\n\
namespace XEngine {\n\
    class i{name} : public Api::iModule {\n\
    public:\n\
        virtual ~i{name}() {}\n\
    };\n\
}\n\
\n\
#endif //__i{name}_h__\n\
";

const char* template_h = "#ifndef __{name}_h__\n\
#define __{name}_h__ \n\
\n\
#include \"Header.h\"\n\
\n\
class {name} : public i{name} {\n\
public:\n\
    virtual ~{name}() {}\n\
    virtual bool Initialize(iEngine* const engine);\n\
    virtual bool Launch(iEngine* const engine);\n\
    virtual bool Destroy(iEngine* const engine);\n\
};\n\
\n\
#endif //__{name}_h__\n\
";

const char* template_cpp = "#include \"{name}.h\"\n\
\n\
iEngine * g_engine = nullptr;\n\
{name}* g_{name} = nullptr;\n\
\n\
bool {name}::Initialize(iEngine* const engine) {\n\
    g_{name} = this;\n\
    g_engine = engine;\n\
    return true;\n\
}\n\
\n\
bool {name}::Launch(iEngine* const engine) {\n\
    return true;\n\
}\n\
\n\
bool {name}::Destroy(iEngine* const engine) {\n\
    return true;\n\
}\n\
";

const char* dllexport_cpp = "#include \"{name}.h\"\n\
\n\
DLL_INSTANCE;\n\
CREATE_MODULE({name});\n";

const char* cmakelists = "#cmake file for project core\n\
#author : {auther}\n\
#created : {date}\n\
\n\
include_directories(\n\
    \"${PROJECT_SOURCE_DIR}/Api\"\n\
    \"${CMAKE_CURRENT_SOURCE_DIR}\"\n\
)\n\
\n\
file(GLOB {name} \"*.*\")\
\n\
source_group(Api FILES ${Api})\n\
source_group(Interface FILES ${Interface})\n\
source_group(Utils\\\\TinyXML FILES ${TinyXML})\n\
source_group(Utils\\\\Geometry FILES ${Geometry})\n\
source_group(Utils FILES ${Utils})\n\
source_group(\\\\ FILES ${{name}})\n\
\n\
add_library({name} SHARED\n\
    ${Api}\n\
    ${Interface}\n\
    ${TinyXML}\n\
    ${Geometry}\n\
    ${Utils}\n\
    ${{name}}\n\
)\n\
\n\
set_property(TARGET {name} PROPERTY FOLDER \"{group}\")\n\
";

#endif //__Template_h__
