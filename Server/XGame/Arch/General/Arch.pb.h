// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: Arch.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_Arch_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_Arch_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3014000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3014000 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_Arch_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_Arch_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxiliaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[2]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_Arch_2eproto;
class oNodeReport;
class oNodeReportDefaultTypeInternal;
extern oNodeReportDefaultTypeInternal _oNodeReport_default_instance_;
class oRouteReport;
class oRouteReportDefaultTypeInternal;
extern oRouteReportDefaultTypeInternal _oRouteReport_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::oNodeReport* Arena::CreateMaybeMessage<::oNodeReport>(Arena*);
template<> ::oRouteReport* Arena::CreateMaybeMessage<::oRouteReport>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum eArchProtoID : int {
  Unknown = 0,
  NodeReport = 1,
  HeartBeat = 2,
  RouteReport = 3,
  DeveloperDefined = 101,
  eArchProtoID_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  eArchProtoID_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool eArchProtoID_IsValid(int value);
constexpr eArchProtoID eArchProtoID_MIN = Unknown;
constexpr eArchProtoID eArchProtoID_MAX = DeveloperDefined;
constexpr int eArchProtoID_ARRAYSIZE = eArchProtoID_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* eArchProtoID_descriptor();
template<typename T>
inline const std::string& eArchProtoID_Name(T enum_t_value) {
  static_assert(::std::is_same<T, eArchProtoID>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function eArchProtoID_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    eArchProtoID_descriptor(), enum_t_value);
}
inline bool eArchProtoID_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, eArchProtoID* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<eArchProtoID>(
    eArchProtoID_descriptor(), name, value);
}
enum eRouteEvent : int {
  Appear = 0,
  Disappear = 1,
  eRouteEvent_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  eRouteEvent_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool eRouteEvent_IsValid(int value);
constexpr eRouteEvent eRouteEvent_MIN = Appear;
constexpr eRouteEvent eRouteEvent_MAX = Disappear;
constexpr int eRouteEvent_ARRAYSIZE = eRouteEvent_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* eRouteEvent_descriptor();
template<typename T>
inline const std::string& eRouteEvent_Name(T enum_t_value) {
  static_assert(::std::is_same<T, eRouteEvent>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function eRouteEvent_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    eRouteEvent_descriptor(), enum_t_value);
}
inline bool eRouteEvent_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, eRouteEvent* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<eRouteEvent>(
    eRouteEvent_descriptor(), name, value);
}
// ===================================================================

class oNodeReport PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:oNodeReport) */ {
 public:
  inline oNodeReport() : oNodeReport(nullptr) {}
  virtual ~oNodeReport();

  oNodeReport(const oNodeReport& from);
  oNodeReport(oNodeReport&& from) noexcept
    : oNodeReport() {
    *this = ::std::move(from);
  }

  inline oNodeReport& operator=(const oNodeReport& from) {
    CopyFrom(from);
    return *this;
  }
  inline oNodeReport& operator=(oNodeReport&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const oNodeReport& default_instance();

  static inline const oNodeReport* internal_default_instance() {
    return reinterpret_cast<const oNodeReport*>(
               &_oNodeReport_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(oNodeReport& a, oNodeReport& b) {
    a.Swap(&b);
  }
  inline void Swap(oNodeReport* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(oNodeReport* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline oNodeReport* New() const final {
    return CreateMaybeMessage<oNodeReport>(nullptr);
  }

  oNodeReport* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<oNodeReport>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const oNodeReport& from);
  void MergeFrom(const oNodeReport& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(oNodeReport* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "oNodeReport";
  }
  protected:
  explicit oNodeReport(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Arch_2eproto);
    return ::descriptor_table_Arch_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kNameFieldNumber = 2,
    kIdFieldNumber = 1,
  };
  // string Name = 2;
  void clear_name();
  const std::string& name() const;
  void set_name(const std::string& value);
  void set_name(std::string&& value);
  void set_name(const char* value);
  void set_name(const char* value, size_t size);
  std::string* mutable_name();
  std::string* release_name();
  void set_allocated_name(std::string* name);
  private:
  const std::string& _internal_name() const;
  void _internal_set_name(const std::string& value);
  std::string* _internal_mutable_name();
  public:

  // uint32 Id = 1;
  void clear_id();
  ::PROTOBUF_NAMESPACE_ID::uint32 id() const;
  void set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint32 _internal_id() const;
  void _internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value);
  public:

  // @@protoc_insertion_point(class_scope:oNodeReport)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr name_;
  ::PROTOBUF_NAMESPACE_ID::uint32 id_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Arch_2eproto;
};
// -------------------------------------------------------------------

class oRouteReport PROTOBUF_FINAL :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:oRouteReport) */ {
 public:
  inline oRouteReport() : oRouteReport(nullptr) {}
  virtual ~oRouteReport();

  oRouteReport(const oRouteReport& from);
  oRouteReport(oRouteReport&& from) noexcept
    : oRouteReport() {
    *this = ::std::move(from);
  }

  inline oRouteReport& operator=(const oRouteReport& from) {
    CopyFrom(from);
    return *this;
  }
  inline oRouteReport& operator=(oRouteReport&& from) noexcept {
    if (GetArena() == from.GetArena()) {
      if (this != &from) InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return GetMetadataStatic().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return GetMetadataStatic().reflection;
  }
  static const oRouteReport& default_instance();

  static inline const oRouteReport* internal_default_instance() {
    return reinterpret_cast<const oRouteReport*>(
               &_oRouteReport_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(oRouteReport& a, oRouteReport& b) {
    a.Swap(&b);
  }
  inline void Swap(oRouteReport* other) {
    if (other == this) return;
    if (GetArena() == other->GetArena()) {
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(oRouteReport* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetArena() == other->GetArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  inline oRouteReport* New() const final {
    return CreateMaybeMessage<oRouteReport>(nullptr);
  }

  oRouteReport* New(::PROTOBUF_NAMESPACE_ID::Arena* arena) const final {
    return CreateMaybeMessage<oRouteReport>(arena);
  }
  void CopyFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void MergeFrom(const ::PROTOBUF_NAMESPACE_ID::Message& from) final;
  void CopyFrom(const oRouteReport& from);
  void MergeFrom(const oRouteReport& from);
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  ::PROTOBUF_NAMESPACE_ID::uint8* _InternalSerialize(
      ::PROTOBUF_NAMESPACE_ID::uint8* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _cached_size_.Get(); }

  private:
  inline void SharedCtor();
  inline void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(oRouteReport* other);
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "oRouteReport";
  }
  protected:
  explicit oRouteReport(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  private:
  static void ArenaDtor(void* object);
  inline void RegisterArenaDtor(::PROTOBUF_NAMESPACE_ID::Arena* arena);
  public:

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;
  private:
  static ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadataStatic() {
    ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&::descriptor_table_Arch_2eproto);
    return ::descriptor_table_Arch_2eproto.file_level_metadata[kIndexInFileMessages];
  }

  public:

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kGuidFieldNumber = 2,
    kEventFieldNumber = 1,
  };
  // uint64 Guid = 2;
  void clear_guid();
  ::PROTOBUF_NAMESPACE_ID::uint64 guid() const;
  void set_guid(::PROTOBUF_NAMESPACE_ID::uint64 value);
  private:
  ::PROTOBUF_NAMESPACE_ID::uint64 _internal_guid() const;
  void _internal_set_guid(::PROTOBUF_NAMESPACE_ID::uint64 value);
  public:

  // .eRouteEvent Event = 1;
  void clear_event();
  ::eRouteEvent event() const;
  void set_event(::eRouteEvent value);
  private:
  ::eRouteEvent _internal_event() const;
  void _internal_set_event(::eRouteEvent value);
  public:

  // @@protoc_insertion_point(class_scope:oRouteReport)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  ::PROTOBUF_NAMESPACE_ID::uint64 guid_;
  int event_;
  mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  friend struct ::TableStruct_Arch_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// oNodeReport

// uint32 Id = 1;
inline void oNodeReport::clear_id() {
  id_ = 0u;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 oNodeReport::_internal_id() const {
  return id_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint32 oNodeReport::id() const {
  // @@protoc_insertion_point(field_get:oNodeReport.Id)
  return _internal_id();
}
inline void oNodeReport::_internal_set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  
  id_ = value;
}
inline void oNodeReport::set_id(::PROTOBUF_NAMESPACE_ID::uint32 value) {
  _internal_set_id(value);
  // @@protoc_insertion_point(field_set:oNodeReport.Id)
}

// string Name = 2;
inline void oNodeReport::clear_name() {
  name_.ClearToEmpty();
}
inline const std::string& oNodeReport::name() const {
  // @@protoc_insertion_point(field_get:oNodeReport.Name)
  return _internal_name();
}
inline void oNodeReport::set_name(const std::string& value) {
  _internal_set_name(value);
  // @@protoc_insertion_point(field_set:oNodeReport.Name)
}
inline std::string* oNodeReport::mutable_name() {
  // @@protoc_insertion_point(field_mutable:oNodeReport.Name)
  return _internal_mutable_name();
}
inline const std::string& oNodeReport::_internal_name() const {
  return name_.Get();
}
inline void oNodeReport::_internal_set_name(const std::string& value) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, value, GetArena());
}
inline void oNodeReport::set_name(std::string&& value) {
  
  name_.Set(
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::move(value), GetArena());
  // @@protoc_insertion_point(field_set_rvalue:oNodeReport.Name)
}
inline void oNodeReport::set_name(const char* value) {
  GOOGLE_DCHECK(value != nullptr);
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(value), GetArena());
  // @@protoc_insertion_point(field_set_char:oNodeReport.Name)
}
inline void oNodeReport::set_name(const char* value,
    size_t size) {
  
  name_.Set(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, ::std::string(
      reinterpret_cast<const char*>(value), size), GetArena());
  // @@protoc_insertion_point(field_set_pointer:oNodeReport.Name)
}
inline std::string* oNodeReport::_internal_mutable_name() {
  
  return name_.Mutable(::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::EmptyDefault{}, GetArena());
}
inline std::string* oNodeReport::release_name() {
  // @@protoc_insertion_point(field_release:oNodeReport.Name)
  return name_.Release(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), GetArena());
}
inline void oNodeReport::set_allocated_name(std::string* name) {
  if (name != nullptr) {
    
  } else {
    
  }
  name_.SetAllocated(&::PROTOBUF_NAMESPACE_ID::internal::GetEmptyStringAlreadyInited(), name,
      GetArena());
  // @@protoc_insertion_point(field_set_allocated:oNodeReport.Name)
}

// -------------------------------------------------------------------

// oRouteReport

// .eRouteEvent Event = 1;
inline void oRouteReport::clear_event() {
  event_ = 0;
}
inline ::eRouteEvent oRouteReport::_internal_event() const {
  return static_cast< ::eRouteEvent >(event_);
}
inline ::eRouteEvent oRouteReport::event() const {
  // @@protoc_insertion_point(field_get:oRouteReport.Event)
  return _internal_event();
}
inline void oRouteReport::_internal_set_event(::eRouteEvent value) {
  
  event_ = value;
}
inline void oRouteReport::set_event(::eRouteEvent value) {
  _internal_set_event(value);
  // @@protoc_insertion_point(field_set:oRouteReport.Event)
}

// uint64 Guid = 2;
inline void oRouteReport::clear_guid() {
  guid_ = PROTOBUF_ULONGLONG(0);
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 oRouteReport::_internal_guid() const {
  return guid_;
}
inline ::PROTOBUF_NAMESPACE_ID::uint64 oRouteReport::guid() const {
  // @@protoc_insertion_point(field_get:oRouteReport.Guid)
  return _internal_guid();
}
inline void oRouteReport::_internal_set_guid(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  
  guid_ = value;
}
inline void oRouteReport::set_guid(::PROTOBUF_NAMESPACE_ID::uint64 value) {
  _internal_set_guid(value);
  // @@protoc_insertion_point(field_set:oRouteReport.Guid)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::eArchProtoID> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::eArchProtoID>() {
  return ::eArchProtoID_descriptor();
}
template <> struct is_proto_enum< ::eRouteEvent> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::eRouteEvent>() {
  return ::eRouteEvent_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_Arch_2eproto
