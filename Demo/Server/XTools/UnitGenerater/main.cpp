#include "Header.h"
#include "Launcher.h"
#include "XDumper.h"
#include "CSharpGenerater.h"
#include "CppGenerater.h"

using namespace UnitGenerater;

int UnitTemplate::s_IndexOffset = 0;

std::map<std::string, const UnitTemplate*> UnitGenerater::g_UnitTemplates;
std::map<std::string, const tinyxml2::XMLElement*> UnitGenerater::g_UnitXmls;
std::map<std::string, const std::string> UnitGenerater::g_CSharpCodeTemplates;
std::map<std::string, const std::string> UnitGenerater::g_CppCodeTemplates;

#ifdef WIN32
class AttachDumper {
public:
    AttachDumper() {
        XEngine::XDumper::GetInstance().SetDumpFilename((SafeSystem::Time::GetCurrentTimeString("%4d_%02d_%02d_%02d_%02d_%02d").append(".dmp")).c_str());
    }
};
static AttachDumper dumper;
#endif //WIN32

const UnitTemplate* GenerateUnitTemplate(const std::string& name) {
    auto itor = g_UnitTemplates.find(name);
    if (g_UnitTemplates.end() != itor) {
        return itor->second;
    }

    const UnitTemplate* parent = nullptr;

    const tinyxml2::XMLElement* root = g_UnitXmls[name];
    if (nullptr != root->Attribute("parent")) {
        parent = GenerateUnitTemplate(root->Attribute("parent"));
    }

    std::vector<AttributeTemplate*>* pAttributeTemplates =
        nullptr != parent ?
        txnew std::vector<AttributeTemplate*>(parent->_AttributeTemplates)
        : txnew std::vector<AttributeTemplate*>();

    const tinyxml2::XMLElement* attribute = root->FirstChildElement("attribute");
    int index = 0;
    int attrIndexStart = pAttributeTemplates->size();
    while (attribute) {
        AttributeTemplate* at = txnew AttributeTemplate(
            name,
            attrIndexStart + index,
            attribute->Attribute("name"),
            GetValueType(attribute->Attribute("type")),
            attribute->Attribute("desc") ? attribute->Attribute("desc") : "",
            attribute->Attribute("sync") ? std::string(attribute->Attribute("sync")) == "true" : false,
            attribute->Attribute("share") ? std::string(attribute->Attribute("share")) == "true" : false,
            attribute->Attribute("save") ? std::string(attribute->Attribute("save")) == "true" : false,
            attribute->Attribute("important") ? std::string(attribute->Attribute("important")) == "true" : false
        );

        pAttributeTemplates->push_back(at);

        index++;
        attribute = attribute->NextSiblingElement("attribute");
    }

    UnitTemplate* unitTemplate = txnew UnitTemplate(parent, name, UnitTemplate::s_IndexOffset++, *pAttributeTemplates);

    g_UnitTemplates.insert(std::make_pair(name, unitTemplate));

    return unitTemplate;
}

int main(int argc, const char** argv) {
    Launcher::GetInstance()->ParseLauncherArgs(argc, argv);
    if (Launcher::GetInstance()->GetLauncherArg("Pause")) {
        getchar();
    }

    std::string unitXmlPath = Launcher::GetInstance()->GetLauncherArg("UnitXmlPath");

    tinyxml2::XMLDocument* doc = txnew tinyxml2::XMLDocument();
    if (tinyxml2::XMLError::XML_SUCCESS != doc->LoadFile(unitXmlPath.c_str())) {
        XASSERT(false, "wtf");
        return -1;
    }

    const tinyxml2::XMLElement* root = doc->RootElement();
    const tinyxml2::XMLElement* go = root->FirstChildElement("Go");
    while (nullptr != go) {
        g_UnitXmls.insert({ go->Attribute("name"), go });
        go = go->NextSiblingElement("Go");
    }

    for (auto itor = g_UnitXmls.begin(); itor != g_UnitXmls.end(); itor++) {
        GenerateUnitTemplate(itor->first);
    }

    if (Launcher::GetInstance()->GetLauncherArg("CSharpCodeTemplatePath")) {
        std::string csharpCodeTemplatePath = Launcher::GetInstance()->GetLauncherArg("CSharpCodeTemplatePath");
        std::string csharpUnitCodePath = Launcher::GetInstance()->GetLauncherArg("CSharpUnitCodePath");
        std::vector<std::string> paths, names;
        names.clear();
        paths.clear();
        int count = SafeSystem::File::GetFileInDirectory(csharpCodeTemplatePath, ".template", paths, names);
        for (int i = 0; i < count; i++) {
            std::string fileContent = SafeSystem::File::LoadFileAsString(paths[i]);
            g_CSharpCodeTemplates.insert(std::make_pair(names[i], fileContent));
        }

        for (auto itor = g_UnitTemplates.begin(); itor != g_UnitTemplates.end(); itor++) {
            std::string code = CSharpGenerater::GenerateUnitCode(itor->second);
            SafeSystem::File::SaveStringToFile(csharpUnitCodePath + "/" + itor->first + "Unit.cs", code);
        }

        std::string code = g_CSharpCodeTemplates["UnitCommon"];
        std::string tempUnitTypeIndex;
        std::string tempAddComponentUnit;
        for (auto itor = g_UnitTemplates.begin(); itor != g_UnitTemplates.end(); itor++) {
            tempUnitTypeIndex += "        " + itor->second->_Name + " = " + SafeString::Int64ToString(itor->second->_Index) += ",\n";
            tempAddComponentUnit += g_CSharpCodeTemplates["AddComponentUnit"];
            tempAddComponentUnit = SafeString::Replace(tempAddComponentUnit, "{UnitName}", itor->second->_Name);
        }
        code = SafeString::Replace(code, "{UnitTypeIndex}", tempUnitTypeIndex);
        code = SafeString::Replace(code, "{UnitTypeCount}", SafeString::Int32ToString(g_UnitTemplates.size()));
        code = SafeString::Replace(code, "{AddComponentUnit}", tempAddComponentUnit);
        SafeSystem::File::SaveStringToFile(csharpUnitCodePath + "/UnitCommon.cs", code);
    }


    std::string unitsCommonCode;
    if (Launcher::GetInstance()->GetLauncherArg("CppCodeTemplatePath")) {
        std::string cppCodeTemplatePath = Launcher::GetInstance()->GetLauncherArg("CppCodeTemplatePath");
        std::string cppUnitCodePath = Launcher::GetInstance()->GetLauncherArg("CppUnitCodePath");
        std::vector<std::string> paths, names;
        names.clear();
        paths.clear();
        int count = SafeSystem::File::GetFileInDirectory(cppCodeTemplatePath, ".template", paths, names);
        for (int i = 0; i < count; i++) {
            std::string fileContent = SafeSystem::File::LoadFileAsString(paths[i]);
            g_CppCodeTemplates.insert(std::make_pair(names[i], fileContent));
        }

        std::string unitsCommonCode = g_CppCodeTemplates["UnitsCommon"];
        std::string temp;
        std::string gameObjectTypeIndexs;
        std::string createGameObjectMacro = g_CppCodeTemplates["CreateGameObjectMarco"];
        std::string caseMacro;
        for (auto itor = g_UnitTemplates.begin(); itor != g_UnitTemplates.end(); itor++) {
            temp += CppGenerater::GenerateUnitCode(itor->second, caseMacro);

            gameObjectTypeIndexs += g_CppCodeTemplates["GameObjectTypeIndex"];
            gameObjectTypeIndexs = SafeString::Replace(gameObjectTypeIndexs, "{GameObjectName}", itor->second->_Name);
            gameObjectTypeIndexs = SafeString::Replace(gameObjectTypeIndexs, "{TypeIndex}", SafeString::Int64ToString(itor->second->_Index));
        }

        createGameObjectMacro = SafeString::Replace(createGameObjectMacro, "{CaseCreateGameObject}", caseMacro);

        std::string istype = g_CppCodeTemplates["IsType"];
        std::string caseIsType;
        for (auto itor = g_UnitTemplates.begin(); itor != g_UnitTemplates.end(); itor++) {
            caseIsType += g_CppCodeTemplates["CaseIsType"];
            caseIsType = SafeString::Replace(caseIsType, "{Name}", itor->second->_Name);
        }
        istype = SafeString::Replace(istype, "{CaseTypes}", caseIsType);

        unitsCommonCode = SafeString::Replace(unitsCommonCode, "{IsType}", istype);
        unitsCommonCode = SafeString::Replace(unitsCommonCode, "{GameObjectTypeCount}", SafeString::Int64ToString(g_UnitTemplates.size()));
        unitsCommonCode = SafeString::Replace(unitsCommonCode, "{CreateGameObjectMarco}", createGameObjectMacro);
        unitsCommonCode = SafeString::Replace(unitsCommonCode, "{GameObjectDefines}", temp);
        unitsCommonCode = SafeString::Replace(unitsCommonCode, "{GameObjectTypeIndexs}", gameObjectTypeIndexs);
        SafeSystem::File::SaveStringToFile(cppUnitCodePath + "/UnitCommon.h", unitsCommonCode);
    }

    return 0;
}
