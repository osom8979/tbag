/**
 * @file   TbagPacket.cpp
 * @brief  TbagPacket class implementation.
 * @author zer0
 * @date   2018-10-24
 */

#if defined(TBAG_COMP_MSVC)
# if defined(min)
TBAG_PUSH_MACRO(min);
# undef min
# define __RESTORE_MIN__
# endif // defined(min)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif // defined(TBAG_COMP_MSVC)

// FlatBuffers generated files.
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <libtbag/proto/fbs/tbag_generated.h>
#include <libtbag/proto/fbs/tbag_t2s.h>

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MIN__)
TBAG_POP_MACRO(min);
# undef __RESTORE_MIN__
# endif // defined(__RESTORE_MIN__)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif // defined(TBAG_COMP_MSVC)

#include <libtbag/proto/TbagPacket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

using namespace proto::fbs;

using FlatBufferBuilder = flatbuffers::FlatBufferBuilder;
using FlatBufferParser  = flatbuffers::Parser;

using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
using StringOffsetVector = flatbuffers::Vector<StringOffset>;

using PairOffset             = flatbuffers::Offset<tbag::Pair>;
using PairOffsetVector       = flatbuffers::Vector<PairOffset>;
using PairOffsetVectorOffset = flatbuffers::Offset<PairOffsetVector>;

static libtbag::proto::fbs::tbag::AnyArr getAnyArr(libtbag::type::TypeTable type) TBAG_NOEXCEPT
{
    using namespace libtbag::proto::fbs::tbag;
    using namespace libtbag::type;

    switch (type) {
    // @formatter:off
    case type::TT_INT8:         return AnyArr_ByteArr;
    case type::TT_UINT8:        return AnyArr_UbyteArr;
    case type::TT_INT16:        return AnyArr_ShortArr;
    case type::TT_UINT16:       return AnyArr_UshortArr;
    case type::TT_INT32:        return AnyArr_IntArr;
    case type::TT_UINT32:       return AnyArr_UintArr;
    case type::TT_INT64:        return AnyArr_LongArr;
    case type::TT_UINT64:       return AnyArr_UlongArr;
    case TypeTable::TT_FLOAT:   return AnyArr_FloatArr;
    case TypeTable::TT_DOUBLE:  return AnyArr_DoubleArr;
    case TypeTable::TT_UNKNOWN: return AnyArr_NONE;
    case TypeTable::TT_BOOL:    return AnyArr_NONE;
    case TypeTable::TT_LDOUBLE: return AnyArr_NONE;
    default:                    break;
    // @formatter:on
    }

    auto const TYPE_BYTE = getTypeSize(type);

    switch (type) {
    // @formatter:off
    case TypeTable::TT_CHAR:
        assert(TYPE_BYTE == 1);
        return AnyArr_ByteArr;

    case TypeTable::TT_SCHAR:
        assert(TYPE_BYTE == 1);
        return AnyArr_ByteArr;
    case TypeTable::TT_UCHAR:
        assert(TYPE_BYTE == 1);
        return AnyArr_UbyteArr;

    case TypeTable::TT_WCHAR:
        if (TYPE_BYTE == 1) {
            return AnyArr_ByteArr;
        } else if (TYPE_BYTE == 2) {
            return AnyArr_ShortArr;
        } else if (TYPE_BYTE == 4) {
            return AnyArr_IntArr;
        } else if (TYPE_BYTE == 8) {
            return AnyArr_LongArr;
        }
        break;

    case TypeTable::TT_CHAR16:
        assert(TYPE_BYTE == 2);
        return AnyArr_ShortArr;
    case TypeTable::TT_CHAR32:
        assert(TYPE_BYTE == 4);
        return AnyArr_IntArr;

    case TypeTable::TT_SHORT:
        assert(TYPE_BYTE == 2);
        return AnyArr_ShortArr;
    case TypeTable::TT_USHORT:
        assert(TYPE_BYTE == 2);
        return AnyArr_UshortArr;

    case TypeTable::TT_INT:
    case TypeTable::TT_LONG:
        if (TYPE_BYTE == 2) {
            return AnyArr_ShortArr;
        } else if (TYPE_BYTE == 4) {
            return AnyArr_IntArr;
        }
        break;

    case TypeTable::TT_UINT:
    case TypeTable::TT_ULONG:
        if (TYPE_BYTE == 2) {
            return AnyArr_UshortArr;
        } else if (TYPE_BYTE == 4) {
            return AnyArr_UintArr;
        }
        break;

    case TypeTable::TT_LLONG:
        if (TYPE_BYTE == 4) {
            return AnyArr_IntArr;
        } else if (TYPE_BYTE == 8) {
            return AnyArr_LongArr;
        }
        break;

    case TypeTable::TT_ULLONG:
        if (TYPE_BYTE == 4) {
            return AnyArr_UintArr;
        } else if (TYPE_BYTE == 8) {
            return AnyArr_UlongArr;
        }
        break;

    case TypeTable::TT_UNKNOWN:
    case TypeTable::TT_BOOL:
    case TypeTable::TT_FLOAT:
    case TypeTable::TT_DOUBLE:
    case TypeTable::TT_LDOUBLE:
    default: break;
    // @formatter:on
    }

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return AnyArr_NONE;
}

/**
 * FlatBuffer builder implementation.
 *
 * @author zer0
 * @date   2018-10-24
 */
struct TbagPacketBuilder::Impl
{
public:
    FlatBufferBuilder builder;
    FlatBufferParser  parser;

public:
    Impl(std::size_t capacity) : builder(capacity, nullptr)
    {
        if (parser.Parse(__get_text_to_cpp11_string__tbag__()) == false) {
            tDLogA("TbagPacketBuilder::Impl() Parse fail.");
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    uint8_t * point() const
    {
        return builder.GetBufferPointer();
    }

    std::size_t size() const
    {
        return builder.GetSize();
    }

    void clear()
    {
        builder.Clear();
    }

    std::string toJsonString() const
    {
        std::string result;
        if (flatbuffers::GenerateText(parser, builder.GetBufferPointer(), &result)) {
            return result;
        }
        return std::string();
    }

    template <typename T>
    void finish(flatbuffers::Offset<T> root, char const * file_identifier = nullptr)
    {
        builder.Finish(root, file_identifier);
    }

    Err build(uint64_t id, int32_t type, int32_t code)
    {
        using namespace libtbag::proto::fbs::tbag;
        clear();
        finish(CreateTbagPacket(builder, id, type, code));
        return Err::E_SUCCESS;
    }

    Err build(uint64_t id, int32_t type, int32_t code, BagExMap const & bags)
    {
        using namespace libtbag::proto::fbs::tbag;
        clear();
        finish(CreateTbagPacket(builder, id, type, code, createPairs(bags)));
        return Err::E_SUCCESS;
    }

    Err build(uint64_t id, int32_t type, int32_t code, std::string const & content)
    {
        using namespace libtbag::proto::fbs::tbag;
        clear();
        finish(CreateTbagPacket(builder, id, type, code, createPairs(content)));
        return Err::E_SUCCESS;
    }

    PairOffsetVectorOffset createPairs(BagExMap const bags)
    {
        std::vector<PairOffset> offsets;
        for (auto & bag : bags) {
            offsets.push_back(createPair(bag.first, bag.second));
        }
        return builder.CreateVector(offsets);
    }

    PairOffsetVectorOffset createPairs(std::string const & content)
    {
        std::vector<PairOffset> offsets;
        offsets.push_back(createPair(content, BagEx()));
        return builder.CreateVector(offsets);
    }

    PairOffset createPair(std::string const & key, BagEx const & bag)
    {
        using namespace libtbag::proto::fbs::tbag;
        AnyArr const VALUE_TYPE = getAnyArr(bag.getType());
        assert(AnyArr_MIN <= COMPARE_AND(VALUE_TYPE) <= AnyArr_MAX);
        if (VALUE_TYPE == AnyArr_NONE) {
            return CreatePair(builder, builder.CreateString(key));
        }
        return CreatePair(builder, builder.CreateString(key), VALUE_TYPE, createAnyArr(VALUE_TYPE, bag));
    }

    flatbuffers::Offset<void> createAnyArr(libtbag::proto::fbs::tbag::AnyArr value_type, BagEx const & bag)
    {
        using namespace libtbag::proto::fbs::tbag;
        switch (value_type) {
        // @formatter:off
        case AnyArr_ByteArr:    return   CreateByteArr(builder, builder.CreateVector(bag.castData<  int8_t>(), bag.size())).Union();
        case AnyArr_UbyteArr:   return  CreateUbyteArr(builder, builder.CreateVector(bag.castData< uint8_t>(), bag.size())).Union();
        case AnyArr_ShortArr:   return  CreateShortArr(builder, builder.CreateVector(bag.castData< int16_t>(), bag.size())).Union();
        case AnyArr_UshortArr:  return CreateUshortArr(builder, builder.CreateVector(bag.castData<uint16_t>(), bag.size())).Union();
        case AnyArr_IntArr:     return    CreateIntArr(builder, builder.CreateVector(bag.castData< int32_t>(), bag.size())).Union();
        case AnyArr_UintArr:    return   CreateUintArr(builder, builder.CreateVector(bag.castData<uint32_t>(), bag.size())).Union();
        case AnyArr_LongArr:    return   CreateLongArr(builder, builder.CreateVector(bag.castData< int64_t>(), bag.size())).Union();
        case AnyArr_UlongArr:   return  CreateUlongArr(builder, builder.CreateVector(bag.castData<uint64_t>(), bag.size())).Union();
        case AnyArr_FloatArr:   return  CreateFloatArr(builder, builder.CreateVector(bag.castData<   float>(), bag.size())).Union();
        case AnyArr_DoubleArr:  return CreateDoubleArr(builder, builder.CreateVector(bag.castData<  double>(), bag.size())).Union();
        default:                break;
        // @formatter:on
        }
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return flatbuffers::Offset<void>();
    }
};

// ---------------------------------
// TbagPacketBuilder implementation.
// ---------------------------------

TbagPacketBuilder::TbagPacketBuilder(std::size_t capacity) : _impl(std::make_unique<Impl>(capacity))
{
    assert(static_cast<bool>(_impl));
}

TbagPacketBuilder::~TbagPacketBuilder()
{
    // EMPTY.
}

uint8_t * TbagPacketBuilder::point() const
{
    assert(exists());
    return _impl->point();
}

std::size_t TbagPacketBuilder::size() const
{
    assert(exists());
    return _impl->size();
}

std::string TbagPacketBuilder::toJsonString() const
{
    assert(exists());
    return _impl->toJsonString();
}

Err TbagPacketBuilder::build(uint64_t id, int32_t type, int32_t code)
{
    assert(exists());
    return _impl->build(id, type, code);
}

Err TbagPacketBuilder::build(BagExMap const & bags, uint64_t id, int32_t type, int32_t code)
{
    assert(exists());
    return _impl->build(id, type, code, bags);
}

Err TbagPacketBuilder::build(std::string const & content, uint64_t id, int32_t type, int32_t code)
{
    assert(exists());
    return _impl->build(id, type, code, content);
}

/**
 * FlatBuffer parser implementation.
 *
 * @author zer0
 * @date   2018-10-24
 */
struct TbagPacketParser::Impl
{
private:
    TbagPacketParser * _parent = nullptr;

public:
    Impl(TbagPacketParser * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    Err parse(char const * buffer, std::size_t size, void * arg)
    {
        return Err::E_SUCCESS;
    }
};

// --------------------------------
// TbagPacketParser implementation.
// --------------------------------

TbagPacketParser::TbagPacketParser() : _impl(std::make_unique<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

TbagPacketParser::~TbagPacketParser()
{
    // EMPTY.
}

void TbagPacketParser::set(char const * buffer, std::size_t size)
{
    _buffer.assign(buffer, buffer + size);
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

