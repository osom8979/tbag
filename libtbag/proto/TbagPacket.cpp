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
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cassert>
#include <algorithm>

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

static libtbag::proto::fbs::tbag::AnyArr getAnyArr(libtbag::container::EggTypeTable type) TBAG_NOEXCEPT
{
    using namespace libtbag::proto::fbs::tbag;
    using namespace libtbag::container;

    switch (type) {
    // @formatter:off
    case EggTypeTable::BT_INT8   : return AnyArr_ByteArr  ;
    case EggTypeTable::BT_UINT8  : return AnyArr_UbyteArr ;
    case EggTypeTable::BT_INT16  : return AnyArr_ShortArr ;
    case EggTypeTable::BT_UINT16 : return AnyArr_UshortArr;
    case EggTypeTable::BT_INT32  : return AnyArr_IntArr   ;
    case EggTypeTable::BT_UINT32 : return AnyArr_UintArr  ;
    case EggTypeTable::BT_INT64  : return AnyArr_LongArr  ;
    case EggTypeTable::BT_UINT64 : return AnyArr_UlongArr ;
    case EggTypeTable::BT_FLOAT32: return AnyArr_FloatArr ;
    case EggTypeTable::BT_FLOAT64: return AnyArr_DoubleArr;
    case EggTypeTable::BT_NONE   : return AnyArr_NONE;
    // @formatter:on
    default:
        return AnyArr_NONE;
    }
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

    Err assign(uint8_t const * buffer, std::size_t size)
    {
        using namespace libtbag::proto::fbs::tbag;
        clear();
        builder.PushFlatBuffer(buffer, size);
        return Err::E_SUCCESS;
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

    Err build(uint64_t id, int32_t type, int32_t code, std::string const & key, std::string const & val)
    {
        using namespace libtbag::proto::fbs::tbag;
        clear();
        finish(CreateTbagPacket(builder, id, type, code, createPairs(key, val)));
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
        offsets.push_back(createPair(content, Egg()));
        return builder.CreateVector(offsets);
    }

    PairOffsetVectorOffset createPairs(std::string const & key, std::string const & val)
    {
        std::vector<PairOffset> offsets;
        offsets.push_back(createPair(key, Egg(val)));
        return builder.CreateVector(offsets);
    }

    PairOffset createPair(std::string const & key, Egg const & bag)
    {
        using namespace libtbag::proto::fbs::tbag;
        AnyArr const VALUE_TYPE = getAnyArr(bag.getType());
        assert(AnyArr_MIN <= COMPARE_AND(VALUE_TYPE) <= AnyArr_MAX);
        if (VALUE_TYPE == AnyArr_NONE) {
            return CreatePair(builder, builder.CreateString(key));
        }
        auto const I0 = bag.size(0);
        auto const I1 = bag.size(1);
        auto const I2 = bag.size(2);
        auto const I3 = bag.size(3);
        auto const I4 = bag.size(4);
        auto const I5 = bag.size(5);
        auto const I6 = bag.size(6);
        auto const I7 = bag.size(7);
        return CreatePair(builder, builder.CreateString(key), I0, I1, I2, I3, I4, I5, I6, I7,
                          VALUE_TYPE, createAnyArr(VALUE_TYPE, bag));
    }

    flatbuffers::Offset<void> createAnyArr(libtbag::proto::fbs::tbag::AnyArr value_type, Egg const & bag)
    {
        using namespace libtbag::proto::fbs::tbag;
        switch (value_type) {
        // @formatter:off
        case AnyArr_ByteArr:    return   CreateByteArr(builder, builder.CreateVector(bag.cast<  int8_t>(), bag.size())).Union();
        case AnyArr_UbyteArr:   return  CreateUbyteArr(builder, builder.CreateVector(bag.cast< uint8_t>(), bag.size())).Union();
        case AnyArr_ShortArr:   return  CreateShortArr(builder, builder.CreateVector(bag.cast< int16_t>(), bag.size())).Union();
        case AnyArr_UshortArr:  return CreateUshortArr(builder, builder.CreateVector(bag.cast<uint16_t>(), bag.size())).Union();
        case AnyArr_IntArr:     return    CreateIntArr(builder, builder.CreateVector(bag.cast< int32_t>(), bag.size())).Union();
        case AnyArr_UintArr:    return   CreateUintArr(builder, builder.CreateVector(bag.cast<uint32_t>(), bag.size())).Union();
        case AnyArr_LongArr:    return   CreateLongArr(builder, builder.CreateVector(bag.cast< int64_t>(), bag.size())).Union();
        case AnyArr_UlongArr:   return  CreateUlongArr(builder, builder.CreateVector(bag.cast<uint64_t>(), bag.size())).Union();
        case AnyArr_FloatArr:   return  CreateFloatArr(builder, builder.CreateVector(bag.cast<   float>(), bag.size())).Union();
        case AnyArr_DoubleArr:  return CreateDoubleArr(builder, builder.CreateVector(bag.cast<  double>(), bag.size())).Union();
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
    assert(static_cast<bool>(_impl));
    return _impl->point();
}

std::size_t TbagPacketBuilder::size() const
{
    assert(static_cast<bool>(_impl));
    return _impl->size();
}

Err TbagPacketBuilder::assign(uint8_t const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_impl));
    return _impl->assign(buffer, size);
}

Err TbagPacketBuilder::assign(Buffer const & buffer)
{
    return assign((uint8_t const *)buffer.data(), buffer.size());
}

std::string TbagPacketBuilder::toJsonString() const
{
    assert(static_cast<bool>(_impl));
    return _impl->toJsonString();
}

Err TbagPacketBuilder::build(uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code);
}

Err TbagPacketBuilder::build(BagExMap const & bags, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, bags);
}

Err TbagPacketBuilder::build(std::string const & content, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, content);
}

Err TbagPacketBuilder::build(std::string const & key, std::string const & val, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, key, val);
}

TbagPacketBuilder::Buffer TbagPacketBuilder::toBuffer() const
{
    return Buffer(point(), point() + size());
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
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    Err parse(char const * buffer, std::size_t size,
              void * arg = nullptr, bool only_header = false, char const * key = nullptr)
    {
        using namespace flatbuffers;
        using namespace libtbag::proto::fbs::tbag;

        assert(_parent != nullptr);

        Verifier verifier((uint8_t const *)buffer, size);
        if (!VerifyTbagPacketBuffer(verifier)) {
            return Err::E_PARSING;
        }

        auto const * PACKET = GetTbagPacket(buffer);
        _parent->onHeader(PACKET->id(), PACKET->type(), PACKET->code(), arg);
        if (only_header) {
            return Err::E_SUCCESS;
        }

        auto const * PAIRS = PACKET->pairs();
        if (PAIRS == nullptr) {
            // Not exists 'pairs'.
            return Err::E_SUCCESS;
        }

        _parent->onPairSize(PACKET->pairs()->size(), arg);

        auto const FIND_KEY = (key != nullptr ? std::string(key) : std::string());
        auto const END = PACKET->pairs()->end();
        for (auto itr = PACKET->pairs()->begin(); itr != END; ++itr) {
            if (!FIND_KEY.empty() && FIND_KEY != itr->key()->str()) {
                continue;
            }

            auto const VAL_TYPE = itr->val_type();
            if (!(AnyArr_MIN <= COMPARE_AND(VAL_TYPE) <= AnyArr_MAX)) {
                return Err::E_ENOMSG;
            }

            if (!VerifyAnyArr(verifier, itr->val(), VAL_TYPE)) {
                // Use 'Verifier error' to distinguish it from 'Parsing error' at the top.
                return Err::E_VERIFIER;
            }

            _parent->onPair(std::string(itr->key()->str()), createBagEx(itr, VAL_TYPE), arg);
            if (!FIND_KEY.empty() && FIND_KEY == itr->key()->str()) {
                // Found the key !! Exit the loop.
                return Err::E_SUCCESS;
            }
        }

        if (!FIND_KEY.empty()) {
            // Not found the key.
            return Err::E_ENFOUND;
        }
        return Err::E_SUCCESS;
    }

    template <typename PairItr>
    Egg createBagEx(PairItr itr, tbag::AnyArr arr_type) const
    {
        using namespace libtbag::proto::fbs::tbag;
        switch (arr_type) {
        // @formatter:off
        case AnyArr_ByteArr:    return createBagEx<PairItr,   ByteArr,     char>(itr);
        case AnyArr_UbyteArr:   return createBagEx<PairItr,  UbyteArr,  uint8_t>(itr);
        case AnyArr_ShortArr:   return createBagEx<PairItr,  ShortArr,  int16_t>(itr);
        case AnyArr_UshortArr:  return createBagEx<PairItr, UshortArr, uint16_t>(itr);
        case AnyArr_IntArr:     return createBagEx<PairItr,    IntArr,  int32_t>(itr);
        case AnyArr_UintArr:    return createBagEx<PairItr,   UintArr, uint32_t>(itr);
        case AnyArr_LongArr:    return createBagEx<PairItr,   LongArr,  int64_t>(itr);
        case AnyArr_UlongArr:   return createBagEx<PairItr,  UlongArr, uint64_t>(itr);
        case AnyArr_FloatArr:   return createBagEx<PairItr,  FloatArr,    float>(itr);
        case AnyArr_DoubleArr:  return createBagEx<PairItr, DoubleArr,   double>(itr);
        case AnyArr_NONE:       return Egg();
        default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
        // @formatter:on
        }
        return Egg();
    }

    template <typename PairItr, typename TbagArrType, typename ValueType>
    Egg createBagEx(PairItr itr) const
    {
        Egg bag;

        Err code = bag.create<ValueType>();
        if (isFailure(code)) {
            return Egg();
        }

        code = bag.resize(itr->i0(), itr->i1(), itr->i2(), itr->i3(), itr->i4(), itr->i5(), itr->i6(), itr->i7());
        if (isFailure(code)) {
            return Egg();
        }

        auto val = (TbagArrType const *)itr->val();
        bag.copyFrom(val->data()->data(), val->data()->size());
        return bag;
    }
};

// --------------------------------
// TbagPacketParser implementation.
// --------------------------------

TbagPacketParser::TbagPacketParser() : _impl(std::make_unique<Impl>(this)), _parsing(false)
{
    assert(static_cast<bool>(_impl));
}

TbagPacketParser::~TbagPacketParser()
{
    // EMPTY.
}

Err TbagPacketParser::parse(char const * buffer, std::size_t size, void * arg)
{
    assert(static_cast<bool>(_impl));
    _parsing = true;
    auto const CODE = _impl->parse(buffer, size, arg, false, nullptr);
    _parsing = false;
    return CODE;
}

Err TbagPacketParser::parse(Buffer const & buffer, void * arg)
{
    return parse(buffer.data(), buffer.size(), arg);
}

Err TbagPacketParser::parseOnlyHeader(char const * buffer, std::size_t size, void * arg)
{
    assert(static_cast<bool>(_impl));
    _parsing = true;
    auto const CODE = _impl->parse(buffer, size, arg, true, nullptr);
    _parsing = false;
    return CODE;
}

Err TbagPacketParser::parseFindKey(char const * buffer, std::size_t size, std::string const & key, void * arg)
{
    assert(static_cast<bool>(_impl));
    _parsing = true;
    auto const CODE = _impl->parse(buffer, size, arg, false, key.c_str());
    _parsing = false;
    return CODE;
}

// --------------------------
// TbagPacket implementation.
// --------------------------

TbagPacket::TbagPacket(std::size_t capacity) : TbagPacketBuilder(capacity), TbagPacketParser()
{
    // EMPTY.
}

TbagPacket::~TbagPacket()
{
    // EMPTY.
}

void TbagPacket::onHeader(uint64_t id, int32_t type, int32_t code, void * arg)
{
    if (arg == nullptr) {
        return;
    }

    if (this == arg) {
        _id = id;
        _type = type;
        _code = code;
        return;
    }

    auto * user_arg = (UserArg*)arg;
    assert(user_arg != nullptr);

    if (user_arg->type == static_cast<int>(UserArgType::UAT_BAG_EX)) {
        Egg * bag = (Egg*)user_arg->user;
        assert(bag != nullptr);
    } else {
        // Unknown types.
    }
}

void TbagPacket::onPair(std::string && key, Egg && val, void * arg)
{
    if (arg == nullptr) {
        return;
    }

    if (this == arg) {
        _bags.insert(std::make_pair(key, val));
        return;
    }

    auto * user_arg = (UserArg*)arg;
    assert(user_arg != nullptr);

    if (user_arg->type == static_cast<int>(UserArgType::UAT_BAG_EX)) {
        Egg * bag = (Egg*)user_arg->user;
        assert(bag != nullptr);
        *bag = std::move(val);
    } else {
        // Unknown types.
    }
}

void TbagPacket::clear()
{
    _id = 0;
    _type = 0;
    _code = 0;
    _bags.clear();
}

Err TbagPacket::update(char const * buffer, std::size_t size)
{
    return parse(buffer, size, this);
}

Err TbagPacket::update(Buffer const & buffer)
{
    return update(buffer.data(), buffer.size());
}

Err TbagPacket::update()
{
    return update((char const *)point(), size());
}

TbagPacket::Egg TbagPacket::findKey(char const * buffer, std::size_t size, std::string const & key, Err * code)
{
    Egg result;
    UserArg arg;
    arg.type = static_cast<int>(UserArgType::UAT_BAG_EX);
    arg.user = &result;

    Err const PARSE_RESULT = parseFindKey(buffer, size, key, &arg);
    if (code != nullptr) {
        *code = PARSE_RESULT;
    }
    return result;
}

TbagPacket::Egg TbagPacket::findKey(Buffer const & buffer, std::string const & key, Err * code)
{
    return findKey(buffer.data(), buffer.size(), key, code);
}

TbagPacket::Egg TbagPacket::findKey(std::string const & key, Err * code)
{
    return findKey((char const *)point(), size(), key, code);
}

Err TbagPacket::buildFromSelf()
{
    return build(_bags, _id, _type, _code);
}

Err TbagPacket::saveFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    return writeFile(path, (char const *)point(), size());
}

Err TbagPacket::loadFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    Buffer buffer;
    auto const CODE = readFile(path, buffer);
    if (isSuccess(CODE)) {
        return update(buffer);
    }
    return CODE;
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

