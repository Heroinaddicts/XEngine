#include "Array.h"
#include "Enum.h"
#include "Const.h"

namespace ExcelConfiger {
    Array* Array::CreateArray(const TypeInfo& info, const std::vector<std::string>& valueStrings) {
        return txnew Array(info, valueStrings);
    }

    Array::Array(const TypeInfo& info, const std::vector<std::string>& valueStrings) : _Info(info), _Size(valueStrings.size()) {
        _ValueArray = (Value**)txmalloc(_Size * sizeof(Value**));
        switch (_Info._ValueType)
        {
        case eValueType::Int:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, valueStrings[i], "", SafeString::StringToInt64(valueStrings[i]));
            }
            break;
        case eValueType::Float:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, valueStrings[i], "", SafeString::StringToFloat(valueStrings[i]));
            }
            break;
        case eValueType::String:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, valueStrings[i], "", valueStrings[i]);
            }
            break;
        case eValueType::Bool:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, valueStrings[i], "", SafeString::StringToBool(valueStrings[i]));
            }
            break;
        case eValueType::Enum:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, SafeString::Replace(valueStrings[i], " ", ""), "", SafeString::Replace(valueStrings[i], " ", ""));
            }
            break;
        case eValueType::Const:
            for (int i = 0; i < _Size; i++) {
                _ValueArray[i] = txnew Value(info, SafeString::Replace(valueStrings[i], " ", ""), "", SafeString::Replace(valueStrings[i], " ", ""));
            }
            break;
        default:
            XASSERT(false, "wtf");
            break;
        }
    }
}
