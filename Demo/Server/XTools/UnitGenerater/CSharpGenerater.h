#ifndef __CSharpGenerater_h__
#define __CSharpGenerater_h__

#include "Header.h"

namespace UnitGenerater {
    namespace CSharpGenerater {
        __forceinline std::string GenerateUnitCode(const UnitTemplate* const unitTemplate) {
            std::string code = g_CSharpCodeTemplates["Unit"];
            code = SafeString::Replace(code, "{UnitName}", unitTemplate->_Name);
            code = SafeString::Replace(code, "{ParentName}", unitTemplate->_Parent ? unitTemplate->_Parent->_Name : "");

            std::string attributeIndexsString = "";
            std::string attributesString = "";
            for (int i = 0; i < unitTemplate->_AttributeTemplates.size(); i++) {
                attributeIndexsString += g_CSharpCodeTemplates["AttributeIndex"];
                attributeIndexsString = SafeString::Replace(attributeIndexsString, "{AttributeName}", unitTemplate->_AttributeTemplates[i]->_Name);
                attributeIndexsString = SafeString::Replace(attributeIndexsString, "{Type}", GetValueTypeString(unitTemplate->_AttributeTemplates[i]->_Type));
                attributeIndexsString = SafeString::Replace(attributeIndexsString, "{Index}", SafeString::Int64ToString(unitTemplate->_AttributeTemplates[i]->_Index));
                attributeIndexsString = SafeString::Replace(attributeIndexsString, "{AttributeDesc}", unitTemplate->_AttributeTemplates[i]->_Desc);

                attributesString += g_CSharpCodeTemplates["Attribute"];
                attributesString = SafeString::Replace(attributesString, "{Type}", GetValueTypeString(unitTemplate->_AttributeTemplates[i]->_Type));
                attributesString = SafeString::Replace(attributesString, "{UnitName}", unitTemplate->_Name);
                attributesString = SafeString::Replace(attributesString, "{AttributeName}", unitTemplate->_AttributeTemplates[i]->_Name);
                attributesString = SafeString::Replace(attributesString, "{Sync}", unitTemplate->_AttributeTemplates[i]->_Sync ? "true" : "false");
                attributesString = SafeString::Replace(attributesString, "{Visual}", unitTemplate->_AttributeTemplates[i]->_Share ? "true" : "false");
                attributesString = SafeString::Replace(attributesString, "{Save}", unitTemplate->_AttributeTemplates[i]->_Save ? "true" : "false");
                attributesString = SafeString::Replace(attributesString, "{Important}", unitTemplate->_AttributeTemplates[i]->_Important ? "true" : "false");

                if (i != unitTemplate->_AttributeTemplates.size() - 1)
                {
                    attributeIndexsString += "\n";
                    attributesString += "\n";
                }
            }

            code = SafeString::Replace(code, "{AttributeIndexs}", attributeIndexsString);
            code = SafeString::Replace(code, "{Attributes}", attributesString);


            std::string createEventsString = "";
            std::string releaseEventString = "";
            std::string isUnitsString = "";
            const UnitTemplate* temp = unitTemplate;
            bool isParent = false;
            while (nullptr != temp)
            {
                createEventsString = g_CSharpCodeTemplates["TriggerUnitEvent"] + createEventsString;
                createEventsString = SafeString::Replace(createEventsString, "{Event}", "Create");
                createEventsString = SafeString::Replace(createEventsString, "{UnitName}", temp->_Name);
                createEventsString = SafeString::Replace(createEventsString, "{ParentType}", isParent ? "true" : "false");

                releaseEventString += g_CSharpCodeTemplates["TriggerUnitEvent"];
                releaseEventString = SafeString::Replace(releaseEventString, "{Event}", "Released");
                releaseEventString = SafeString::Replace(releaseEventString, "{UnitName}", temp->_Name);
                releaseEventString = SafeString::Replace(releaseEventString, "{ParentType}", isParent ? "true" : "false");

                isUnitsString += g_CSharpCodeTemplates["IsUnitType"];
                isUnitsString = SafeString::Replace(isUnitsString, "{UnitName}", temp->_Name);

                isParent = true;
                temp = temp->_Parent;

                if (temp != nullptr) {
                    isUnitsString += " || ";
                }
            }

            code = SafeString::Replace(code, "{TriggerUnitCreateEvents}", createEventsString);
            code = SafeString::Replace(code, "{TriggerUnitReleaseEvents}", releaseEventString);
            code = SafeString::Replace(code, "{IsUnitTypes}", isUnitsString);

            return code;
        }
    }
}

#endif //__CSharpGenerater_h__