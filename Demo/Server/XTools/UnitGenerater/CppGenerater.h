#ifndef __CppGenerater_h__
#define __CppGenerater_h__

#include "Header.h"

namespace UnitGenerater {
    namespace CppGenerater {
        __forceinline std::string GenerateUnitCode(const UnitTemplate* const unitTemplate, std::string& createmacro) {
            std::string types;

            const UnitTemplate* temp = unitTemplate;
            int typesCount = 0;
            while (temp) {
                types += "eGameObjectType::" + temp->_Name + ", ";
                temp = temp->_Parent;
                typesCount++;
            }

            std::string code = g_CppCodeTemplates["GameObjectDefine"];
            code = SafeString::Replace(code, "{GameObjectName}", unitTemplate->_Name);
            code = SafeString::Replace(code, "{Types}", types);
            code = SafeString::Replace(code, "{TypesCount}", SafeString::Int64ToString(typesCount));

            std::string newAttributesString = "";
            std::string attributeEnumsString = "";
            for (int i = 0; i < unitTemplate->_AttributeTemplates.size(); i++) {
                newAttributesString += g_CppCodeTemplates["NewAttribute"];
                newAttributesString = SafeString::Replace(newAttributesString, "{Name}", unitTemplate->_Name);
                newAttributesString = SafeString::Replace(newAttributesString, "{AttributeName}", unitTemplate->_AttributeTemplates[i]->_Name);
                newAttributesString = SafeString::Replace(newAttributesString, "{AttributeType}", unitTemplate->_AttributeTemplates[i]->_TypeString);
                newAttributesString = SafeString::Replace(newAttributesString, "{Sync}", unitTemplate->_AttributeTemplates[i]->_Sync ? "true" : "false");
                newAttributesString = SafeString::Replace(newAttributesString, "{Share}", unitTemplate->_AttributeTemplates[i]->_Share ? "true" : "false");

                attributeEnumsString += g_CppCodeTemplates["AttributeEnum"];
                attributeEnumsString = SafeString::Replace(attributeEnumsString, "{AttributeName}", unitTemplate->_AttributeTemplates[i]->_Name);
                attributeEnumsString = SafeString::Replace(attributeEnumsString, "{Type}", GetValueTypeString(unitTemplate->_AttributeTemplates[i]->_Type));
                attributeEnumsString = SafeString::Replace(attributeEnumsString, "{Index}", SafeString::Int64ToString(unitTemplate->_AttributeTemplates[i]->_Index));
                attributeEnumsString = SafeString::Replace(attributeEnumsString, "{AttributeDesc}", unitTemplate->_AttributeTemplates[i]->_Desc);
                attributeEnumsString += "\n";
            }

            code = SafeString::Replace(code, "{AttributeEnums}", attributeEnumsString);

            createmacro += g_CppCodeTemplates["CaseCreateGameObject"];
            createmacro = SafeString::Replace(createmacro, "{Name}", unitTemplate->_Name);
            createmacro = SafeString::Replace(createmacro, "{NewAttributes}", newAttributesString);

            code = SafeString::Replace(code, "{AttributeCount}", SafeString::Int64ToString(unitTemplate->_AttributeTemplates.size()));
            return code;
        }
    }
}

#endif //__CppGenerater_h__