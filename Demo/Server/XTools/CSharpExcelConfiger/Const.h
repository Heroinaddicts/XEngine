#ifndef __Const_h__
#define __Const_h__

#include "Header.h"

namespace ExcelConfiger {
    class Const {
    public:
        virtual ~Const() {}
        
        static bool LoadConst(Excel::Sheet* sheet);
        static Const* GetConst(const std::string& name);
        static Value* GetConstValue(const std::string& constName, const std::string& memberName);

        static const std::map<std::string, Const*>& GetConsts();

        const std::string _Name;
        const std::string _Desc;

        const std::map<std::string, Value*> _Values;

    private:
        Const(const std::string& name, const std::string& desc, const std::map<std::string, Value*>& values) : _Name(name), _Desc(desc), _Values(values) {}
    };
}

#endif //__Const_h__
