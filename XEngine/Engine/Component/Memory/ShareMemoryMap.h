#ifndef __ShareMemoryMap_h__
#define __ShareMemoryMap_h__

#include "Header.h"

namespace XEngine {
    class ShareMemoryMap : public Api::iShareMemoryMap {
    public:
        virtual ~ShareMemoryMap() {}

        class Core;

        static ShareMemoryMap* OpenOrCreate(const std::string& shareMemoryName, const UInt64 shareMemorySize, const std::string& name, const UInt64 keySize, const UInt64 valueSize);
        static ShareMemoryMap* Load(const std::string& shareMemoryName, const std::string& name, const UInt64 keySize, const UInt64 valueSize);
        static void Release(ShareMemoryMap* map);
        static void Destory(const std::string& shareMemoryName, const std::string& name);

        const std::string& ShareMemoryName() const override;
        const std::string& Name() const override;
        UInt64 KeySize() const override;
        UInt64 ValueSize() const override;
        UInt64 Size() const override;
        bool Empty() const override;
        void Clear() override;

        UInt64 InvalidNode() const override;
        UInt64 FirstNode() const override;
        UInt64 LastNode() const override;
        UInt64 NextNode(const UInt64 node) const override;
        UInt64 PrevNode(const UInt64 node) const override;
        const void* Key(const UInt64 node) const override;
        void* Value(const UInt64 node) const override;

        UInt64 FindNode(const void* key, const UInt64 keySize) const override;
        bool Contains(const void* key, const UInt64 keySize) const override;
        bool Insert(const void* key, const UInt64 keySize, const void* value, const UInt64 valueSize, const bool overwrite = false) override;
        bool Erase(const void* key, const UInt64 keySize) override;

    private:
        ShareMemoryMap(const std::string& shareMemoryName, const std::string& name, Core* core);

        UInt64 ToNode(const void* address) const;
        const void* FromNode(const UInt64 node) const;

    private:
        const std::string _ShareMemoryName;
        const std::string _Name;
        Core* const _Core;
    };
}

#endif //__ShareMemoryMap_h__
