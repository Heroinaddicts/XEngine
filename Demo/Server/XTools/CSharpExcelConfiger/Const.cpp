#include "Const.h"

#define CONST_TAG L"Const"
#include "Enum.h"
#include "Array.h"

#define CONST_HEAD_COLUMN 0
#define CONST_NAME_COLUMN 1
#define CONST_DESC_COLUMN 2

#define CONST_MEMBER_NAME 0
#define CONST_MEMBER_TYPE 1
#define CONST_MEMBER_VALUE 2
#define CONST_MEMBER_DESC 3

namespace ExcelConfiger {
    static std::map<std::string, Const*> s_Consts;

    bool Const::LoadConst(Excel::Sheet* sheet) {
        for (int i = 0; i < sheet->rowsCount();) {
            if (sheet->cell(i, 0).getString() == CONST_TAG) {
                std::string constName = FormatExcelString(sheet->cell(i, CONST_NAME_COLUMN).getString());
                std::string constDesc = FormatExcelString(sheet->cell(i, CONST_DESC_COLUMN).getString());

                std::map<std::string, Value*>  members;

                i++;
                while (sheet->cell(i, CONST_HEAD_COLUMN).getString() != CONST_TAG
                    && sheet->cell(i, CONST_HEAD_COLUMN).getString() != L"") {

                    std::string memberName = FormatExcelString(sheet->cell(i, CONST_MEMBER_NAME).getString());
                    std::string memberDesc = FormatExcelString(sheet->cell(i, CONST_MEMBER_DESC).getString());
                    std::string memberValueString = FormatExcelString(sheet->cell(i, CONST_MEMBER_VALUE).getString());
                    std::string memberTypeString = FormatExcelString(sheet->cell(i, CONST_MEMBER_TYPE).getString());

                    TypeInfo info = GetTypeInfo(memberTypeString);

                    Value* value = nullptr;

                    if (info._IsArray) {
                        std::vector<std::string> valueStrings;
                        SafeString::Split(memberValueString, ARRAY_SPLIT_TAG, valueStrings);
                        Array* arr = Array::CreateArray(info, valueStrings);
                        value = txnew Value(info, memberValueString, memberDesc, arr);
                    }
                    else {
                        switch (info._ValueType) {
                        case eValueType::Int: {
                            value = txnew Value(info, memberValueString, memberDesc, SafeString::StringToInt64(memberValueString));
                            break;
                        }
                        case eValueType::Float: {
                            value = txnew Value(info, memberValueString, memberDesc, SafeString::StringToFloat(memberValueString));
                            break;
                        }
                        case eValueType::String: {
                            value = txnew Value(info, memberValueString, memberDesc, memberValueString);
                            break;
                        }
                        case eValueType::Bool: {
                            value = txnew Value(info, memberValueString, memberDesc, SafeString::StringToBool(memberValueString));
                            break;
                        }
                        case eValueType::Script: {
                            value = txnew Value(info, memberValueString, memberDesc, memberValueString);
                            break;
                        }
                        case eValueType::Enum: {
                            value = txnew Value(info, memberValueString, memberDesc, SafeString::Replace(memberValueString, " ", "").c_str());
                            break;
                        }
                        case eValueType::Const: {
                            value = txnew Value(info, memberValueString, memberDesc, SafeString::Replace(memberValueString, " ", "").c_str());
                            break;
                        }
                        default:
                            XASSERT(false, "wtf");
                            break;
                        }
                    }

                    members.insert({ memberName, value });
                    i++;
                }

                s_Consts.insert(std::make_pair(constName, txnew Const(constName, constDesc, members)));
            }
            else {
                i++;
            }
        }
        return true;
        return false;
    }

    Const* Const::GetConst(const std::string& name) {
        auto itor = s_Consts.find(name);
        if (s_Consts.end() == itor) {
            return nullptr;
        }

        return itor->second;
    }

    Value* Const::GetConstValue(const std::string& constName, const std::string& memberName) {
        Const* c = Const::GetConst(constName);
        if (nullptr == c) {
            XASSERT(false, "error const name %ls", constName.c_str());
            return nullptr;
        }

        auto itor = c->_Values.find(memberName);
        if (c->_Values.end() == itor) {
            XASSERT(false, "error const value %ls::%ls", constName.c_str(), memberName.c_str());
            return nullptr;
        }

        return itor->second;
    }

    const std::map<std::string, Const*>& Const::GetConsts() {
        return s_Consts;
    }
}
