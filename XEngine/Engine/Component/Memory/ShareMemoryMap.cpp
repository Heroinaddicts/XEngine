#include "ShareMemoryMap.h"

#include <algorithm>
#include <limits>

#include <boost/interprocess/shared_memory_object.hpp>

namespace XEngine {
    namespace {
        constexpr UInt64 kShareMemoryMapMagic = 0x58454E47534D4150ULL;
        constexpr UInt64 kShareMemoryMapVersion = 1;
        constexpr UInt64 kInvalidShareMemoryNode = (std::numeric_limits<UInt64>::max)();

        using Byte = unsigned char;

        class BufferLess {
        public:
            template<typename T>
            bool operator()(const T& left, const T& right) const {
                return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
            }
        };
    }

    class ShareMemoryMap::Core {
    public:
        using Segment = boost::interprocess::managed_shared_memory;
        using SegmentManager = Segment::segment_manager;
        using ByteAllocator = boost::interprocess::allocator<Byte, SegmentManager>;
        using ByteBuffer = boost::container::vector<Byte, ByteAllocator>;
        using MapValue = std::pair<const ByteBuffer, ByteBuffer>;
        using MapAllocator = boost::interprocess::allocator<MapValue, SegmentManager>;
        using RawMap = boost::container::map<ByteBuffer, ByteBuffer, BufferLess, MapAllocator>;

        struct Object {
            UInt64 _Magic;
            UInt64 _Version;
            UInt64 _KeySize;
            UInt64 _ValueSize;
            RawMap _Map;

            Object(const UInt64 keySize, const UInt64 valueSize, SegmentManager* const manager)
                : _Magic(kShareMemoryMapMagic),
                _Version(kShareMemoryMapVersion),
                _KeySize(keySize),
                _ValueSize(valueSize),
                _Map(BufferLess(), MapAllocator(manager)) {
            }
        };

        Core(Segment* const segment, Object* const object) : _Segment(segment), _Object(object) {}

        Segment* _Segment;
        Object* _Object;
    };

    namespace {
        bool IsValidSize(const UInt64 size) {
            return size > 0 && size <= static_cast<UInt64>((std::numeric_limits<std::size_t>::max)());
        }

        bool IsValidObject(const ShareMemoryMap::Core::Object* const object, const UInt64 keySize, const UInt64 valueSize) {
            if (nullptr == object) {
                return false;
            }

            if (object->_Magic != kShareMemoryMapMagic || object->_Version != kShareMemoryMapVersion) {
                return false;
            }

            return object->_KeySize == keySize && object->_ValueSize == valueSize;
        }

        ShareMemoryMap::Core::ByteBuffer MakeBuffer(const void* const data, const UInt64 size, ShareMemoryMap::Core::SegmentManager* const manager) {
            ShareMemoryMap::Core::ByteBuffer buffer((ShareMemoryMap::Core::ByteAllocator(manager)));
            if (size > 0) {
                const Byte* const begin = static_cast<const Byte*>(data);
                buffer.insert(buffer.end(), begin, begin + static_cast<std::size_t>(size));
            }

            return buffer;
        }
    }

    ShareMemoryMap::ShareMemoryMap(const std::string& shareMemoryName, const std::string& name, Core* const core)
        : _ShareMemoryName(shareMemoryName), _Name(name), _Core(core) {
    }

    ShareMemoryMap* ShareMemoryMap::OpenOrCreate(const std::string& shareMemoryName, const UInt64 shareMemorySize, const std::string& name, const UInt64 keySize, const UInt64 valueSize) {
        if (shareMemoryName.empty() || name.empty() || !IsValidSize(shareMemorySize) || !IsValidSize(keySize) || !IsValidSize(valueSize)) {
            return nullptr;
        }

        try {
            Core::Segment* const segment = txnew Core::Segment(
                boost::interprocess::open_or_create,
                shareMemoryName.c_str(),
                static_cast<std::size_t>(shareMemorySize)
            );

            auto found = segment->find<Core::Object>(name.c_str());
            Core::Object* object = found.first;
            if (nullptr == object) {
                object = segment->construct<Core::Object>(name.c_str())(keySize, valueSize, segment->get_segment_manager());
            }
            else if (!IsValidObject(object, keySize, valueSize)) {
                txdel segment;
                return nullptr;
            }

            return txnew ShareMemoryMap(shareMemoryName, name, txnew Core(segment, object));
        }
        catch (...) {
            return nullptr;
        }
    }

    ShareMemoryMap* ShareMemoryMap::Load(const std::string& shareMemoryName, const std::string& name, const UInt64 keySize, const UInt64 valueSize) {
        if (shareMemoryName.empty() || name.empty() || !IsValidSize(keySize) || !IsValidSize(valueSize)) {
            return nullptr;
        }

        try {
            Core::Segment* const segment = txnew Core::Segment(
                boost::interprocess::open_only,
                shareMemoryName.c_str()
            );

            auto found = segment->find<Core::Object>(name.c_str());
            if (!IsValidObject(found.first, keySize, valueSize)) {
                txdel segment;
                return nullptr;
            }

            return txnew ShareMemoryMap(shareMemoryName, name, txnew Core(segment, found.first));
        }
        catch (...) {
            return nullptr;
        }
    }

    void ShareMemoryMap::Release(ShareMemoryMap* const map) {
        if (nullptr == map) {
            return;
        }

        if (nullptr != map->_Core) {
            txdel map->_Core->_Segment;
            txdel map->_Core;
        }

        txdel map;
    }

    void ShareMemoryMap::Destory(const std::string& shareMemoryName, const std::string& name) {
        if (shareMemoryName.empty() || name.empty()) {
            return;
        }

        try {
            Core::Segment segment(
                boost::interprocess::open_only,
                shareMemoryName.c_str()
            );

            segment.destroy<Core::Object>(name.c_str());
        }
        catch (...) {
        }
    }

    const std::string& ShareMemoryMap::ShareMemoryName() const {
        return _ShareMemoryName;
    }

    const std::string& ShareMemoryMap::Name() const {
        return _Name;
    }

    UInt64 ShareMemoryMap::KeySize() const {
        return nullptr != _Core ? _Core->_Object->_KeySize : 0;
    }

    UInt64 ShareMemoryMap::ValueSize() const {
        return nullptr != _Core ? _Core->_Object->_ValueSize : 0;
    }

    UInt64 ShareMemoryMap::Size() const {
        return nullptr != _Core ? static_cast<UInt64>(_Core->_Object->_Map.size()) : 0;
    }

    bool ShareMemoryMap::Empty() const {
        return nullptr == _Core || _Core->_Object->_Map.empty();
    }

    void ShareMemoryMap::Clear() {
        if (nullptr != _Core) {
            _Core->_Object->_Map.clear();
        }
    }

    UInt64 ShareMemoryMap::InvalidNode() const {
        return kInvalidShareMemoryNode;
    }

    UInt64 ShareMemoryMap::FirstNode() const {
        if (Empty()) {
            return InvalidNode();
        }

        const auto it = _Core->_Object->_Map.begin();
        return ToNode(&(*it));
    }

    UInt64 ShareMemoryMap::LastNode() const {
        if (Empty()) {
            return InvalidNode();
        }

        auto it = _Core->_Object->_Map.end();
        --it;
        return ToNode(&(*it));
    }

    UInt64 ShareMemoryMap::NextNode(const UInt64 node) const {
        if (nullptr == _Core || InvalidNode() == node) {
            return InvalidNode();
        }

        const auto* const pair = static_cast<const Core::MapValue*>(FromNode(node));
        if (nullptr == pair) {
            return InvalidNode();
        }

        const auto it = _Core->_Object->_Map.upper_bound(pair->first);
        if (it == _Core->_Object->_Map.end()) {
            return InvalidNode();
        }

        return ToNode(&(*it));
    }

    UInt64 ShareMemoryMap::PrevNode(const UInt64 node) const {
        if (Empty()) {
            return InvalidNode();
        }

        if (InvalidNode() == node) {
            return LastNode();
        }

        const auto* const pair = static_cast<const Core::MapValue*>(FromNode(node));
        if (nullptr == pair) {
            return InvalidNode();
        }

        auto it = _Core->_Object->_Map.lower_bound(pair->first);
        if (it == _Core->_Object->_Map.begin()) {
            return InvalidNode();
        }

        --it;
        return ToNode(&(*it));
    }

    const void* ShareMemoryMap::Key(const UInt64 node) const {
        const auto* const pair = static_cast<const Core::MapValue*>(FromNode(node));
        if (nullptr == pair || pair->first.empty()) {
            return nullptr;
        }

        return pair->first.data();
    }

    void* ShareMemoryMap::Value(const UInt64 node) const {
        auto* const pair = const_cast<Core::MapValue*>(static_cast<const Core::MapValue*>(FromNode(node)));
        if (nullptr == pair || pair->second.empty()) {
            return nullptr;
        }

        return pair->second.data();
    }

    UInt64 ShareMemoryMap::FindNode(const void* const key, const UInt64 keySize) const {
        if (nullptr == _Core || nullptr == key || keySize != KeySize()) {
            return InvalidNode();
        }

        const Core::ByteBuffer buffer = MakeBuffer(key, keySize, _Core->_Segment->get_segment_manager());
        const auto it = _Core->_Object->_Map.find(buffer);
        if (it == _Core->_Object->_Map.end()) {
            return InvalidNode();
        }

        return ToNode(&(*it));
    }

    bool ShareMemoryMap::Contains(const void* const key, const UInt64 keySize) const {
        return InvalidNode() != FindNode(key, keySize);
    }

    bool ShareMemoryMap::Insert(const void* const key, const UInt64 keySize, const void* const value, const UInt64 valueSize, const bool overwrite) {
        if (nullptr == _Core || nullptr == key || nullptr == value || keySize != KeySize() || valueSize != ValueSize()) {
            return false;
        }

        Core::ByteBuffer keyBuffer = MakeBuffer(key, keySize, _Core->_Segment->get_segment_manager());
        auto it = _Core->_Object->_Map.find(keyBuffer);
        if (it != _Core->_Object->_Map.end()) {
            if (!overwrite) {
                return false;
            }

            it->second = MakeBuffer(value, valueSize, _Core->_Segment->get_segment_manager());
            return true;
        }

        Core::ByteBuffer valueBuffer = MakeBuffer(value, valueSize, _Core->_Segment->get_segment_manager());
        _Core->_Object->_Map.insert(Core::MapValue(std::move(keyBuffer), std::move(valueBuffer)));
        return true;
    }

    bool ShareMemoryMap::Erase(const void* const key, const UInt64 keySize) {
        if (nullptr == _Core || nullptr == key || keySize != KeySize()) {
            return false;
        }

        const Core::ByteBuffer keyBuffer = MakeBuffer(key, keySize, _Core->_Segment->get_segment_manager());
        return _Core->_Object->_Map.erase(keyBuffer) > 0;
    }

    UInt64 ShareMemoryMap::ToNode(const void* const address) const {
        if (nullptr == _Core || nullptr == address) {
            return InvalidNode();
        }

        return static_cast<UInt64>(_Core->_Segment->get_handle_from_address(address));
    }

    const void* ShareMemoryMap::FromNode(const UInt64 node) const {
        if (nullptr == _Core || InvalidNode() == node) {
            return nullptr;
        }

        return _Core->_Segment->get_address_from_handle(static_cast<Core::Segment::handle_t>(node));
    }
}
