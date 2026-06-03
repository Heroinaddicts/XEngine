#ifndef __AutoStruct_h__
#define __AutoStruct_h__

#include "Header.h"

#define EXCEL_ROW_NAME 0
#define EXCEL_ROW_DESC 1
#define EXCEL_ROW_TYPE 2
#define EXCEL_DATA_START_ROW 3

__forceinline void GeneratAutoStruct() {
    for (auto itor = g_DataExcel.begin(); itor != g_DataExcel.end(); itor++) {
        const Excel::Sheet* sheet = itor->second;
        std::vector<AutoConfigStruct::ColumnInfo*> columnInfos;

        for (int i = 0; i < sheet->columnsCount(); i++) {
            const std::string columnName = FormatExcelString(sheet->cell(EXCEL_ROW_NAME, i).getString());
            const std::string columndesc = FormatExcelString(sheet->cell(EXCEL_ROW_DESC, i).getString());
            const std::string columnTypeString = FormatExcelString(sheet->cell(EXCEL_ROW_TYPE, i).getString());

            columnInfos.push_back(txnew AutoConfigStruct::ColumnInfo(columnName, GetType(columnTypeString), columnTypeString, columndesc));
        }


        std::vector<std::vector<const AutoConfigStruct::ColumnValue*>> rows;
        for (int i = EXCEL_DATA_START_ROW; i < sheet->rowsCount(); i++) {
            std::vector<const AutoConfigStruct::ColumnValue*> row;
            for (int column = 0; column < columnInfos.size(); column++) {
                row.push_back(txnew AutoConfigStruct::ColumnValue(columnInfos[column], FormatExcelString(sheet->cell(i, column).getString())));
            }
            rows.push_back(row);
        }

        g_AutoConfigData.insert({ itor->first, txnew AutoConfigStruct(itor->first, columnInfos, rows) });
    }
}

__forceinline std::string GeneratAutoStructCode() {
    std::string code;
    for (auto itor = g_AutoConfigData.begin(); itor != g_AutoConfigData.end(); itor++) {
        const AutoConfigStruct* pStruct = itor->second;

        code += g_CodeTemplates["Struct"];
        SafeString::Replace2(code, "{StructName}", pStruct->_Name);
        {
            std::string membersCode;
            for (int i = 0; i < pStruct->_ColumnInfos.size(); i++) {
                const AutoConfigStruct::ColumnInfo* pColumnInfo = pStruct->_ColumnInfos[i];
                membersCode += g_CodeTemplates["StructMember"];
                SafeString::Replace2(membersCode, "{StructMemberType}", GetTypeCode(pColumnInfo->_Type, pColumnInfo->_TypeString));
                SafeString::Replace2(membersCode, "{StructMemberName}", pColumnInfo->_Name);
                SafeString::Replace2(membersCode, "{StructMemberDesc}", pColumnInfo->_Desc);
            }
            SafeString::Replace2(code, "{StructMembers}", membersCode);
        }

        {
            std::string constructorArgsCode;
            for (int i = 0; i < pStruct->_ColumnInfos.size(); i++) {
                const AutoConfigStruct::ColumnInfo* pColumnInfo = pStruct->_ColumnInfos[i];
                constructorArgsCode += g_CodeTemplates["ConstructorArg"] + (i != pStruct->_ColumnInfos.size() - 1 ? ", " : "");
                SafeString::Replace2(constructorArgsCode, "{ConstructorArgType}", GetTypeCode(pColumnInfo->_Type, pColumnInfo->_TypeString));
                SafeString::Replace2(constructorArgsCode, "{ConstructorArgName}", SafeString::ToLower(pColumnInfo->_Name));;
            }
            SafeString::Replace2(code, "{ConstructorArgs}", constructorArgsCode);
        }

        {
            std::string constructivesCode;
            for (int i = 0; i < pStruct->_ColumnInfos.size(); i++) {
                const AutoConfigStruct::ColumnInfo* pColumnInfo = pStruct->_ColumnInfos[i];
                constructivesCode += pColumnInfo->_Name + "(" + SafeString::ToLower(pColumnInfo->_Name) + ")" + (i != pStruct->_ColumnInfos.size() - 1 ? ", " : "");
            }
            SafeString::Replace2(code, "{Constructives}", constructivesCode);
        }
    }

    return code;
}


#endif //__AutoStruct_h__
