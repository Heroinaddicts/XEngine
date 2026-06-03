#include "Header.h"
#include "Enum.h"
#include "Const.h"

namespace ExcelConfiger {
    TypeInfo GetTypeInfo(const std::string& str) {
        const bool isArray = SafeString::StringContain(str, ARRAY_TAG);
        const std::string typeString = SafeString::Replace(str, ARRAY_TAG, "");

        eValueType type;

        if (typeString == INT_TAG) {
            type = eValueType::Int;
        }
        else if (typeString == FLOAT_TAG) {
            type = eValueType::Float;
        }
        else if (typeString == STRING_TAG) {
            type = eValueType::String;
        }
        else if (typeString == BOOL_TAG) {
            type = eValueType::Bool;
        }
        else if (typeString == TABLE_TAG) {
            type = eValueType::Table;
        }
        else if (typeString == SCRIPT_TAG) {
            type = eValueType::Script;
        }
        else if (Enum::GetEnum(typeString)) {
            type = eValueType::Enum;
        }
        else if (Const::GetConst(typeString)) {
            type = eValueType::Const;
        }
        else {
            XASSERT(false, "wtf");
        }

        return TypeInfo(isArray, type, typeString);
    }
}
