#ifndef __ExcelData_h__
#define __ExcelData_h__

#include "Header.h"

namespace ExcelConfiger {
    class ExcelDef {
    public:
        virtual ~ExcelDef() {}

        struct MemberInfo {
            const bool _IsKey;
            const std::string _Name;
            const TypeInfo _TypeInfo;
            const std::string& _Desc;

            MemberInfo(const bool isKey, const std::string& name, const TypeInfo & info, const std::string& desc) : _IsKey(isKey), _Name(name), _TypeInfo(info), _Desc(desc) {}
        };

        const std::string _Name;
        const std::string _Desc;

        const std::vector<MemberInfo> _MemberInfos;

        ExcelDef(const std::string& name, const std::string& desc, const std::vector<MemberInfo>& infos) : _Name(name), _Desc(desc), _MemberInfos(infos) {}
    };

    class ExcelData {
    public:
        virtual ~ExcelData() {}

        const ExcelDef* _Def;

        const std::vector<std::vector<Value*>> _Values;

        ExcelData(const ExcelDef* def, const std::vector<std::vector<Value*>>& values) : _Def(def), _Values(values) {}

        static ExcelData* LoadExcel(const std::string & name, Excel::Sheet* sheet);
        static const std::map<std::string, ExcelData*>& GetExcelDataMap();
    };
}

#endif //__ExcelData_h__
