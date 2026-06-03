#pragma once

#include "Header.h"
#define CONST_TAG L"Const"

#define CONST_HEAD_COLUMN 0
#define CONST_NAME_COLUMN 1
#define CONST_DESC_COLUMN 2

#define CONST_MEMBER_NAME 0
#define CONST_MEMBER_TYPE 1
#define CONST_MEMBER_VALUE 2
#define CONST_MEMBER_DESC 3

__forceinline void ReadAutoConst(const Excel::Sheet* const sheet) {
    for (int i = 0; i < sheet->rowsCount(); i++) {
        if (sheet->cell(i, 0).getString() == CONST_TAG) {
            std::string constName = FormatExcelString(sheet->cell(i, CONST_NAME_COLUMN).getString());
            std::string constDesc = FormatExcelString(sheet->cell(i, CONST_DESC_COLUMN).getString());

            std::vector<AutoConfigConst::Member*> members;

            i++;
            while (sheet->cell(i, CONST_HEAD_COLUMN).getString() != CONST_TAG
                && sheet->cell(i, CONST_HEAD_COLUMN).getString() != L"") {

                std::string memberName = FormatExcelString(sheet->cell(i, CONST_MEMBER_NAME).getString());
                std::string memberTypeString = FormatExcelString(sheet->cell(i, CONST_MEMBER_TYPE).getString());
                std::string memberValueString = FormatExcelString(sheet->cell(i, CONST_MEMBER_VALUE).getString());
                std::string memberDesc = FormatExcelString(sheet->cell(i, CONST_MEMBER_DESC).getString());

                members.push_back(txnew AutoConfigConst::Member(memberName, memberTypeString, memberValueString, memberDesc));
                i++;
            }

            g_AutoConfigConstMap.insert({ constName, txnew AutoConfigConst(constName, constDesc, members) });
        }
    }
}

__forceinline std::string GeneratAutoConstCode() {
    std::string code;
    for (auto itor = g_AutoConfigConstMap.begin(); itor != g_AutoConfigConstMap.end(); itor++) {
        const AutoConfigConst* pConst = itor->second;
        code += g_CodeTemplates["AutoConst"];
        SafeString::Replace2(code, "{ConstName}", pConst->_Name);
        {
            std::string constMembersString;
            for (int i = 0; i < pConst->_Members.size(); i++) {
                const AutoConfigConst::Member* pMember = pConst->_Members[i];
                constMembersString += g_CodeTemplates["ConstMember"];
                SafeString::Replace2(constMembersString, "{MemberType}", GetTypeCode(pMember->_Type, pMember->_TypeString));
                SafeString::Replace2(constMembersString, "{MemberName}", pMember->_Name);
                SafeString::Replace2(constMembersString, "{MemberValue}",
                    GetValueCode(pMember->_Type, pMember->_Value, g_AutoConfigEnumMap[SafeString::Replace(pMember->_TypeString, "[]", "")] ? g_AutoConfigEnumMap[SafeString::Replace(pMember->_TypeString, "[]", "")]->_Name : "")
                );
            }
            SafeString::Replace2(code, "{ConstMembers}", constMembersString);
        }
    }

    return code;
}

