#ifndef __AutoEnum_h__
#define __AutoEnum_h__

#include "Header.h"

#define ENUM_TAG L"Enum"

#define ENUM_HEAD_COLUMN 0
#define ENUM_NAME_COLUMN 1
#define ENUM_DESC_COLUMN 2

#define ENUM_MEMBER_NAME 0
#define ENUM_MEMBER_VALUE 1
#define ENUM_MEMBER_DESC 2


__forceinline void ReadAutoEnum(Excel::Sheet* sheet) {
    for (int i = 0; i < sheet->rowsCount();) {
        if (sheet->cell(i, 0).getString() == ENUM_TAG) {
            std::string enumName = FormatExcelString(sheet->cell(i, ENUM_NAME_COLUMN).getString());
            std::string enumDesc = FormatExcelString(sheet->cell(i, ENUM_DESC_COLUMN).getString());

            std::vector<AutoConfigEnum::Member*> members;

            i++;
            while (sheet->cell(i, ENUM_HEAD_COLUMN).getString() != ENUM_TAG
                && sheet->cell(i, ENUM_HEAD_COLUMN).getString() != L"") {

                std::string memberName = FormatExcelString(sheet->cell(i, ENUM_MEMBER_NAME).getString());
                std::string memberValue = FormatExcelString(sheet->cell(i, ENUM_MEMBER_VALUE).getString());
                std::string memberDesc = FormatExcelString(sheet->cell(i, ENUM_MEMBER_DESC).getString());

                members.push_back(txnew AutoConfigEnum::Member(memberName, SafeString::StringToInt(memberValue), memberDesc));
                i++;
            }

            g_AutoConfigEnumMap.insert({ enumName, txnew AutoConfigEnum(enumName, enumDesc, members) });
        }
        else {
            i++;
        }
    }
}

__forceinline std::string GeneratAutoEnumCode() {
    std::string code;
    for (auto itor = g_AutoConfigEnumMap.begin(); itor != g_AutoConfigEnumMap.end(); itor++) {
        const AutoConfigEnum* pEnum = itor->second;
        code += g_CodeTemplates["AutoEnum"];
        SafeString::Replace2(code, "{EnumName}", pEnum->_Name);
        SafeString::Replace2(code, "{EnumDesc}", pEnum->_Desc);
        {
            std::string enumMembersString;
            for (int i = 0; i < pEnum->_Members.size(); i++) {
                const AutoConfigEnum::Member* pMember = pEnum->_Members[i];
                enumMembersString += g_CodeTemplates["EnumMember"];
                SafeString::Replace2(enumMembersString, "{EnumMemberName}", pMember->_Name);
                SafeString::Replace2(enumMembersString, "{EnumMemberValue}", SafeString::Int32ToString(pMember->_Value));
                SafeString::Replace2(enumMembersString, "{EnumMemberDesc}", pMember->_Desc);
            }
            SafeString::Replace2(code, "{EnumMembers}", enumMembersString);
        }
        SafeString::Replace2(code, "{EnumMemberCount}", SafeString::Int32ToString(pEnum->_Members.size()));
    }
    printf("%s\n", code.c_str());
    return code;
}

#endif //__AutoEnum_h__
