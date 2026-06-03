#ifndef __Array_h__
#define __Array_h__

#include "Header.h"

namespace ExcelConfiger {
    class Array {
    public:
        virtual ~Array() {}

        const TypeInfo _Info;
        const int _Size;

        __forceinline const Value* GetValue(const int index) const {
            XASSERT(index < _Size, "wtf");
            return _ValueArray[index];
        }

        static Array* CreateArray(const TypeInfo & info, const std::vector<std::string>& valueStrings);

    private:
        Array(const TypeInfo& sinfo, const std::vector<std::string>& valueStrings);

        Value** _ValueArray;
    };
}

#endif //__Array_h__
