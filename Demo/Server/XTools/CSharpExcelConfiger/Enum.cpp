#include "Enum.h"

#define ENUM_TAG L"Enum"

#define ENUM_HEAD_COLUMN 0
#define ENUM_NAME_COLUMN 1
#define ENUM_DESC_COLUMN 2

#define ENUM_MEMBER_NAME 0
#define ENUM_MEMBER_VALUE 1
#define ENUM_MEMBER_DESC 2


namespace ExcelConfiger {
    static std::map<std::string, Enum*> s_Enums;

    bool Enum::LoadEnum(Excel::Sheet* sheet) {
        for (int i = 0; i < sheet->rowsCount();) {
            if (sheet->cell(i, 0).getString() == ENUM_TAG) {
                std::string enumName = FormatExcelString(sheet->cell(i, ENUM_NAME_COLUMN).getString());
                std::string enumDesc = FormatExcelString(sheet->cell(i, ENUM_DESC_COLUMN).getString());

                std::map<std::string, Value*> members;

                i++;
                while (sheet->cell(i, ENUM_HEAD_COLUMN).getString() != ENUM_TAG
                    && sheet->cell(i, ENUM_HEAD_COLUMN).getString() != L"") {

                    std::string valueName = FormatExcelString(sheet->cell(i, ENUM_MEMBER_NAME).getString());
                    std::string valueString = FormatExcelString(sheet->cell(i, ENUM_MEMBER_VALUE).getString());
                    std::string valueDesc = FormatExcelString(sheet->cell(i, ENUM_MEMBER_DESC).getString());

                    TypeInfo info = GetTypeInfo("int");

                    members.insert({ valueName, txnew Value(info, valueString, valueDesc, SafeString::StringToInt64(valueString)) });

                    i++;
                }
                s_Enums.insert(std::make_pair(enumName, txnew Enum(enumName, enumDesc, members)));
            }
            else {
                i++;
            }
        }

        return true;
    }

    Enum* Enum::GetEnum(const std::string& enumName) {
        auto itor = s_Enums.find(enumName);
        if (s_Enums.end() == itor) {
            return nullptr;
        }

        return itor->second;
    }

    Value* Enum::GetEnumValue(const std::string& enumName, const std::string& valueName) {
        auto itorEnum = s_Enums.find(enumName);
        if (s_Enums.end() == itorEnum) {
            XASSERT(false, "error enum value %ls::%ls", enumName.c_str(), valueName.c_str());
            return nullptr;
        }

        auto itorValue = itorEnum->second->_Values.find(valueName);
        if (itorEnum->second->_Values.end() == itorValue) {
            XASSERT(false, "error enum value %ls::%ls", enumName.c_str(), valueName.c_str());
            return nullptr;
        }

        return itorValue->second;
    }

    const std::map<std::string, Enum*>& Enum::GetEnums() {
        return s_Enums;
    }
}
