#include "ExcelData.h"
#include "Enum.h"
#include "Const.h"
#include "Array.h"

#define EXCEL_ROW_NAME 0
#define EXCEL_ROW_DESC 1
#define EXCEL_ROW_TYPE 2
#define EXCEL_DATA_START_ROW 3

namespace ExcelConfiger {
    static std::map<std::string, ExcelData*> s_ExcelDatas;

    ExcelData* ExcelData::LoadExcel(const std::string& name, Excel::Sheet* sheet) {
        std::vector<ExcelDef::MemberInfo> infos;

        for (int i = 0; i < sheet->columnsCount(); i++) {
            const std::string columnName = FormatExcelString(sheet->cell(EXCEL_ROW_NAME, i).getString());
            const std::string columndesc = FormatExcelString(sheet->cell(EXCEL_ROW_DESC, i).getString());
            const std::string columnTypeString = FormatExcelString(sheet->cell(EXCEL_ROW_TYPE, i).getString());

            TypeInfo typeInfo = GetTypeInfo(columnTypeString);

            infos.push_back(ExcelDef::MemberInfo(i == 0, columnName, typeInfo, columndesc));
        }

        ExcelDef* def = txnew ExcelDef(name, "", infos);

        std::vector<std::vector<Value*>> values;

        for (int row = EXCEL_DATA_START_ROW; row < sheet->rowsCount(); row++) {
            std::vector<Value*> value;
            for (int column = 0; column < def->_MemberInfos.size(); column++) {
                const std::string valueString = FormatExcelString(sheet->cell(row, column).getString());

                if (def->_MemberInfos[column]._TypeInfo._IsArray) {
                    std::vector<std::string> valueStrings;
                    SafeString::Split(valueString, ARRAY_SPLIT_TAG, valueStrings);
                    Array* arr = Array::CreateArray(def->_MemberInfos[column]._TypeInfo, valueStrings);
                    value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", arr));
                }
                else {
                    switch (def->_MemberInfos[column]._TypeInfo._ValueType) {
                    case eValueType::Int: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::StringToInt64(valueString)));
                        break;
                    }
                    case eValueType::String: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", valueString.c_str()));
                        break;
                    }
                    case eValueType::Float: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::StringToFloat(valueString)));
                        break;
                    }
                    case eValueType::Bool: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::StringToBool(valueString)));
                        break;
                    }
                    case eValueType::Enum: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::Replace(valueString, " ", "")));
                        break;
                    }
                    case eValueType::Const: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::Replace(valueString, " ", "")));
                        break;
                    }
                    case eValueType::Table: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", SafeString::Replace(valueString, " ", "")));
                        break;
                    }
                    case eValueType::Script: {
                        value.push_back(txnew Value(def->_MemberInfos[column]._TypeInfo, valueString, "", valueString));
                        break;
                    }
                    }
                }
            }
            values.push_back(value);
        }

        ExcelData* ed = txnew ExcelData(def, values);

        s_ExcelDatas.insert({ name, ed });
        return ed;
    }
    const std::map<std::string, ExcelData*>& ExcelData::GetExcelDataMap() {
        return s_ExcelDatas;
    }
}
