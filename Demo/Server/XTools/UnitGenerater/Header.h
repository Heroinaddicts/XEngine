#ifndef __Header_h__
#define __Header_h__

#include "MultiSys.h"
#include "SafeString.h"
#include "SafeMemory.h"
#include "SafeSystem.h"
#include <string>
#include <map>
#include <vector>

#include "TinyXML/tinyxml2.h"

UsingXEngine;

namespace UnitGenerater {
    enum eValueType {
        UInt16 = 0,
        Int16 = 1,
        UInt32 = 2,
        Int32 = 3,
        UInt64 = 4,
        Int64 = 5,
        Float = 6,
        String = 7,
        Bool = 8,
        Vector2 = 9,
        Vector3 = 10,
        Vector4 = 11,

        __Unknown__
    };


    __forceinline const char* GetValueTypeString(const eValueType type) {
        switch (type)
        {
        case eValueType::UInt16: return "UInt16";
        case eValueType::Int16: return "Int16";
        case eValueType::UInt32: return "UInt32";
        case eValueType::Int32: return "Int32";
        case eValueType::UInt64: return "UInt64";
        case eValueType::Int64: return "Int64";
        case eValueType::Float: return "Float";
        case eValueType::String: return "String";
        case eValueType::Bool: return "Bool";
        case eValueType::Vector2: return "Vector2";
        case eValueType::Vector3: return "Vector3";
        case eValueType::Vector4: return "Vector4";
        }

        return nullptr;
    }

    __forceinline eValueType GetValueType(const std::string& type) {
        if (type == "uint16")
            return eValueType::UInt16;
        if (type == "int16")
            return eValueType::Int16;

        if (type == "uint32")
            return eValueType::UInt32;
        if (type == "int32")
            return eValueType::Int32;

        if (type == "uint64")
            return eValueType::UInt64;
        if (type == "int64")
            return eValueType::Int64;

        if (type == "float")
            return eValueType::Float;

        if (type == "string")
            return eValueType::String;

        if (type == "bool")
            return eValueType::Bool;

        if (type == "vector2")
            return eValueType::Vector2;

        if (type == "vector3")
            return eValueType::Vector3;

        if (type == "vector4")
            return eValueType::Vector4;

        return eValueType::__Unknown__;
    }

    struct AttributeTemplate
    {
        const std::string _UnitName;
        const int _Index;
        const std::string _Name;
        const eValueType _Type;
        const std::string _Desc;
        const std::string _TypeString;
        const bool _Sync;
        const bool _Share;
        const bool _Save;
        const bool _Important;

        AttributeTemplate(
            const std::string& unitName,
            const int index,
            const std::string& name,
            const eValueType type,
            const std::string& desc,
            const bool sync,
            const bool share,
            const bool save,
            const bool important)
            : _UnitName(unitName),
            _Index(index),
            _Name(name),
            _Type(type),
            _Desc(desc),
            _TypeString(GetValueTypeString(type)),
            _Sync(sync),
            _Share(share),
            _Save(save),
            _Important(important)
        {
        }
    };


    struct UnitTemplate
    {
        static int s_IndexOffset;

        const UnitTemplate* const _Parent;
        const std::string _Name;
        const int _Index;

        const std::vector<AttributeTemplate*> _AttributeTemplates;

        UnitTemplate(const UnitTemplate* parent, const std::string& name, const int index, const std::vector<AttributeTemplate*>& attrs)
            : _Parent(parent), _Name(name), _Index(index), _AttributeTemplates(attrs)
        {
        }
    };

    extern std::map<std::string, const UnitTemplate*> g_UnitTemplates;
    extern std::map<std::string, const tinyxml2::XMLElement*> g_UnitXmls;
    extern std::map<std::string, const std::string> g_CSharpCodeTemplates;
    extern std::map<std::string, const std::string> g_CppCodeTemplates;
}

#endif //__Header_h__
