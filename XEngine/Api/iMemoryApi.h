#ifndef __iMemoryApi_h__
#define __iMemoryApi_h__

#include "MultiSys.h"

#include <limits>
#include <new>
#include <type_traits>
#include <utility>

namespace XEngine {
    namespace Api {
        enum class eAccess {
            ReadOnly,
            ReadWrite
        };

        class iMMap {
        public:
            virtual ~iMMap() {}

            virtual const std::string& FilePath() const = 0;
            virtual const eAccess Access() const = 0;
            virtual void* Address() const = 0;
            virtual UInt64 Length() const = 0;

            virtual void Flush() const = 0;
        };

        class iShareMemoryMap {
        public:
            virtual ~iShareMemoryMap() {}

            virtual const std::string& ShareMemoryName() const = 0;
            virtual const std::string& Name() const = 0;
            virtual UInt64 KeySize() const = 0;
            virtual UInt64 ValueSize() const = 0;
            virtual UInt64 Size() const = 0;
            virtual bool Empty() const = 0;
            virtual void Clear() = 0;

            virtual UInt64 InvalidNode() const = 0;
            virtual UInt64 FirstNode() const = 0;
            virtual UInt64 LastNode() const = 0;
            virtual UInt64 NextNode(const UInt64 node) const = 0;
            virtual UInt64 PrevNode(const UInt64 node) const = 0;
            virtual const void* Key(const UInt64 node) const = 0;
            virtual void* Value(const UInt64 node) const = 0;

            virtual UInt64 FindNode(const void* key, const UInt64 keySize) const = 0;
            virtual bool Contains(const void* key, const UInt64 keySize) const = 0;
            virtual bool Insert(const void* key, const UInt64 keySize, const void* value, const UInt64 valueSize, const bool overwrite = false) = 0;
            virtual bool Erase(const void* key, const UInt64 keySize) = 0;
        };

        template<typename K, typename V>
        class TShareMemoryMap {
            static_assert(std::is_trivially_copyable_v<K>, "TShareMemoryMap key must be trivially copyable.");
            static_assert(std::is_trivially_copyable_v<V>, "TShareMemoryMap value must be trivially copyable.");

        public:
            struct PairRef {
                const K& first;
                V& second;
            };

            class iterator {
            public:
                iterator() : _SMM(nullptr), _Node((std::numeric_limits<UInt64>::max)()), _PairRefNode((std::numeric_limits<UInt64>::max)()), _HasPairRef(false) {}

                iterator(const iterator& other) : _SMM(other._SMM), _Node(other._Node), _PairRefNode((std::numeric_limits<UInt64>::max)()), _HasPairRef(false) {}

                iterator& operator=(const iterator& other) {
                    if (this != &other) {
                        InvalidatePairRef();
                        _SMM = other._SMM;
                        _Node = other._Node;
                    }

                    return *this;
                }

                ~iterator() {
                    InvalidatePairRef();
                }

                bool operator==(const iterator& other) const {
                    return _SMM == other._SMM && _Node == other._Node;
                }

                bool operator!=(const iterator& other) const {
                    return !(*this == other);
                }

                iterator& operator++() {
                    if (nullptr != _SMM) {
                        _Node = _SMM->NextNode(_Node);
                        InvalidatePairRef();
                    }

                    return *this;
                }

                iterator operator++(int) {
                    iterator copy = *this;
                    ++(*this);
                    return copy;
                }

                iterator& operator--() {
                    if (nullptr != _SMM) {
                        _Node = _SMM->PrevNode(_Node);
                        InvalidatePairRef();
                    }

                    return *this;
                }

                iterator operator--(int) {
                    iterator copy = *this;
                    --(*this);
                    return copy;
                }

                PairRef& operator*() const {
                    return *EnsurePairRef();
                }

                PairRef* operator->() const {
                    return EnsurePairRef();
                }

                const K& Key() const {
                    return EnsurePairRef()->first;
                }

                V& Value() const {
                    return EnsurePairRef()->second;
                }

                UInt64 Node() const {
                    return _Node;
                }

            private:
                friend TShareMemoryMap<K, V>;

                iterator(iShareMemoryMap* const smm, const UInt64 node) : _SMM(smm), _Node(node), _PairRefNode((std::numeric_limits<UInt64>::max)()), _HasPairRef(false) {}

                PairRef* EnsurePairRef() const {
                    if (nullptr == _SMM || InvalidNodeValue() == _Node || _SMM->InvalidNode() == _Node) {
                        return nullptr;
                    }

                    if (_HasPairRef && _PairRefNode == _Node) {
                        return reinterpret_cast<PairRef*>(&_PairRefStorage);
                    }

                    InvalidatePairRef();
                    new (&_PairRefStorage) PairRef{
                        *reinterpret_cast<const K*>(_SMM->Key(_Node)),
                        *reinterpret_cast<V*>(_SMM->Value(_Node))
                    };

                    _PairRefNode = _Node;
                    _HasPairRef = true;
                    return reinterpret_cast<PairRef*>(&_PairRefStorage);
                }

                void InvalidatePairRef() const {
                    if (_HasPairRef) {
                        reinterpret_cast<PairRef*>(&_PairRefStorage)->~PairRef();
                        _PairRefNode = InvalidNodeValue();
                        _HasPairRef = false;
                    }
                }

            private:
                mutable iShareMemoryMap* _SMM;
                mutable UInt64 _Node;
                mutable typename std::aligned_storage<sizeof(PairRef), alignof(PairRef)>::type _PairRefStorage;
                mutable UInt64 _PairRefNode;
                mutable bool _HasPairRef;
            };

        public:
            TShareMemoryMap() : _SMM(nullptr) {}

            ~TShareMemoryMap() {}

            TShareMemoryMap(const TShareMemoryMap& other) = delete;
            TShareMemoryMap& operator=(const TShareMemoryMap& other) = delete;

            TShareMemoryMap(TShareMemoryMap&& other) noexcept : _SMM(other._SMM) {
                other._SMM = nullptr;
            }

            TShareMemoryMap& operator=(TShareMemoryMap&& other) noexcept {
                if (this != &other) {
                    _SMM = other._SMM;
                    other._SMM = nullptr;
                }

                return *this;
            }

            explicit operator bool() const {
                return nullptr != _SMM;
            }

            const std::string& ShareMemoryName() const {
                return nullptr != _SMM ? _SMM->ShareMemoryName() : EmptyString();
            }

            const std::string& Name() const {
                return nullptr != _SMM ? _SMM->Name() : EmptyString();
            }

            UInt64 Size() const {
                return nullptr != _SMM ? _SMM->Size() : 0;
            }

            UInt64 size() const {
                return Size();
            }

            bool Empty() const {
                return nullptr == _SMM || _SMM->Empty();
            }

            bool empty() const {
                return Empty();
            }

            void Clear() {
                if (nullptr != _SMM) {
                    _SMM->Clear();
                }
            }

            void clear() {
                Clear();
            }

            iterator Begin() const {
                if (nullptr == _SMM) {
                    return iterator(nullptr, InvalidNodeValue());
                }

                return iterator(_SMM, _SMM->FirstNode());
            }

            iterator begin() const {
                return Begin();
            }

            iterator End() const {
                return iterator(_SMM, nullptr != _SMM ? _SMM->InvalidNode() : InvalidNodeValue());
            }

            iterator end() const {
                return End();
            }

            iterator Find(const K& key) const {
                if (nullptr == _SMM) {
                    return End();
                }

                return iterator(_SMM, _SMM->FindNode(&key, sizeof(K)));
            }

            iterator find(const K& key) const {
                return Find(key);
            }

            bool Contains(const K& key) const {
                return nullptr != _SMM && _SMM->Contains(&key, sizeof(K));
            }

            bool contains(const K& key) const {
                return Contains(key);
            }

            bool TryGetValue(const K& key, V& value) const {
                if (nullptr == _SMM) {
                    return false;
                }

                const UInt64 node = _SMM->FindNode(&key, sizeof(K));
                if (node == _SMM->InvalidNode()) {
                    return false;
                }

                const void* rawValue = _SMM->Value(node);
                if (nullptr == rawValue) {
                    return false;
                }

                value = *reinterpret_cast<const V*>(rawValue);
                return true;
            }

            bool Insert(const K& key, const V& value, const bool overwrite = false) {
                return nullptr != _SMM && _SMM->Insert(&key, sizeof(K), &value, sizeof(V), overwrite);
            }

            bool insert(const K& key, const V& value, const bool overwrite = false) {
                return Insert(key, value, overwrite);
            }

            bool Erase(const K& key) {
                return nullptr != _SMM && _SMM->Erase(&key, sizeof(K));
            }

            bool erase(const K& key) {
                return Erase(key);
            }

        private:
            friend class iMemoryApi;

            explicit TShareMemoryMap(iShareMemoryMap* const smm) : _SMM(smm) {}

            static const std::string& EmptyString() {
                static const std::string s_Empty;
                return s_Empty;
            }

            static constexpr UInt64 InvalidNodeValue() {
                return (std::numeric_limits<UInt64>::max)();
            }

        private:
            iShareMemoryMap* _SMM;
        };

        class iMemoryApi {
        public:
            virtual ~iMemoryApi() {}

            virtual iMMap* CreateMMap(const std::string& file, const eAccess a = eAccess::ReadOnly) = 0;
            virtual void CloseMMap(iMMap* mmap) = 0;

            template<typename K, typename V>
            TShareMemoryMap<K, V> OpenOrCreateShareMemoryMap(const std::string& shareMemoryName, const UInt64 shareMemorySize, const std::string& name) {
                return TShareMemoryMap<K, V>(OpenOrCreateShareMemoryMapInternal(shareMemoryName, shareMemorySize, name, sizeof(K), sizeof(V)));
            }

            template<typename K, typename V>
            TShareMemoryMap<K, V> LoadShareMemoryMap(const std::string& shareMemoryName, const std::string& name) {
                return TShareMemoryMap<K, V>(LoadShareMemoryMapInternal(shareMemoryName, name, sizeof(K), sizeof(V)));
            }

            template<typename K, typename V>
            void ReleaseShareMemoryMap(TShareMemoryMap<K, V>& map) {
                if (nullptr == map._SMM) {
                    return;
                }

                ReleaseShareMemoryMapInternal(map._SMM);
                map._SMM = nullptr;
            }

            virtual void DestoryShareMemoryMap(const std::string& shareMemoryName, const std::string& mapName) = 0;

        protected:
            virtual iShareMemoryMap* OpenOrCreateShareMemoryMapInternal(const std::string& shareMemoryName, const UInt64 shareMemorySize, const std::string& name, const UInt64 keySize, const UInt64 valueSize) = 0;
            virtual iShareMemoryMap* LoadShareMemoryMapInternal(const std::string& shareMemoryName, const std::string& name, const UInt64 keySize, const UInt64 valueSize) = 0;
            virtual void ReleaseShareMemoryMapInternal(iShareMemoryMap* map) = 0;
        };
    }
}

#endif //__iMemoryApi_h__
