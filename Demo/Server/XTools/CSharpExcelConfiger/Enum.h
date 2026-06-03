#ifndef __Enum_h__
#define __Enum_h__

#include "Header.h"

namespace ExcelConfiger {
    class Basic;

    class Enum {
    public:
        virtual ~Enum() {}

        static bool LoadEnum(Excel::Sheet* sheet);

        static Enum* GetEnum(const std::string& enumName);
        static Value* GetEnumValue(const std::string& enumName, const std::string& valueName);

        static const std::map<std::string, Enum*>& GetEnums();

        const std::string _Name;
        const std::string _Desc;

        __forceinline const std::map<std::string, Value*>& GetValueMap() const {
            return _Values;
        }

    private:
        Enum(const std::string& name, const std::string& desc, const std::map<std::string, Value*>& values) : _Name(name), _Desc(desc), _Values(values) {}

        const std::map<std::string, Value*> _Values;
    };
}

#endif //__Enum_h__
