/**
 * @file   BoxPacket.cpp
 * @brief  BoxPacket class implementation.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
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

/**
 * A FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE macro is required to use the Verifier::GetComputedSize() method.
 *
 * @see <https://github.com/google/flatbuffers/issues/4604>
 */
#define FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE

// FlatBuffers generated files.
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <libtbag/proto/fbs/box_generated.h>
#include <libtbag/proto/fbs/box_d2s.h>

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

#include <libtbag/proto/BoxPacket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

using namespace proto::fbs;

using FlatBufferBuilder = flatbuffers::FlatBufferBuilder;
using FlatBufferParser  = flatbuffers::Parser;

using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
using StringOffsetVector = flatbuffers::Vector<StringOffset>;

using PairOffset             = flatbuffers::Offset<box::Pair>;
using PairOffsetVector       = flatbuffers::Vector<PairOffset>;
using PairOffsetVectorOffset = flatbuffers::Offset<PairOffsetVector>;

static libtbag::proto::fbs::box::AnyArr getAnyArr(libtbag::container::BoxTypeTable type) TBAG_NOEXCEPT
{
    using namespace libtbag::proto::fbs::box;
    using namespace libtbag::container;

    switch (type) {
    // clang-format off
    case BoxTypeTable::BTT_NONE   : return AnyArr_NONE     ;
    case BoxTypeTable::BTT_INT8   : return AnyArr_ByteArr  ;
    case BoxTypeTable::BTT_UINT8  : return AnyArr_UbyteArr ;
    case BoxTypeTable::BTT_INT16  : return AnyArr_ShortArr ;
    case BoxTypeTable::BTT_UINT16 : return AnyArr_UshortArr;
    case BoxTypeTable::BTT_INT32  : return AnyArr_IntArr   ;
    case BoxTypeTable::BTT_UINT32 : return AnyArr_UintArr  ;
    case BoxTypeTable::BTT_INT64  : return AnyArr_LongArr  ;
    case BoxTypeTable::BTT_UINT64 : return AnyArr_UlongArr ;
    case BoxTypeTable::BTT_FLOAT32: return AnyArr_FloatArr ;
    case BoxTypeTable::BTT_FLOAT64: return AnyArr_DoubleArr;
    // clang-format on
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
struct BoxPacketBuilder::Impl
{
public:
    FlatBufferBuilder builder;
    FlatBufferParser  parser;

public:
    Impl(std::size_t capacity) : builder(capacity, nullptr)
    {
        auto const PROTOCOL_TEXT = libtbag::string::mergeArgv((char**)__data_to_string__box__);
        if (!parser.Parse(PROTOCOL_TEXT.c_str())) {
            tDLogA("BoxPacketBuilder::Impl() Parse fail.");
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
        using namespace libtbag::proto::fbs::box;
        clear();
        builder.PushFlatBuffer(buffer, size);
        return E_SUCCESS;
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
        using namespace libtbag::proto::fbs::box;
        clear();
        finish(CreateBoxPacket(builder, id, type, code));
        return E_SUCCESS;
    }

    Err build(uint64_t id, int32_t type, int32_t code, BoxMap const & boxes)
    {
        using namespace libtbag::proto::fbs::box;
        clear();
        finish(CreateBoxPacket(builder, id, type, code, createPairs(boxes)));
        return E_SUCCESS;
    }

    Err build(uint64_t id, int32_t type, int32_t code, std::string const & content)
    {
        using namespace libtbag::proto::fbs::box;
        clear();
        finish(CreateBoxPacket(builder, id, type, code, createPairs(content)));
        return E_SUCCESS;
    }

    Err build(uint64_t id, int32_t type, int32_t code, std::string const & key, std::string const & val)
    {
        using namespace libtbag::proto::fbs::box;
        clear();
        finish(CreateBoxPacket(builder, id, type, code, createPairs(key, val)));
        return E_SUCCESS;
    }

    PairOffsetVectorOffset createPairs(BoxMap const boxes)
    {
        std::vector<PairOffset> offsets;
        for (auto & bag : boxes) {
            offsets.push_back(createPair(bag.first, bag.second));
        }
        return builder.CreateVector(offsets);
    }

    PairOffsetVectorOffset createPairs(std::string const & content)
    {
        std::vector<PairOffset> offsets;
        offsets.push_back(createPair(content, Box()));
        return builder.CreateVector(offsets);
    }

    PairOffsetVectorOffset createPairs(std::string const & key, std::string const & val)
    {
        std::vector<PairOffset> offsets;
        offsets.push_back(createPair(key, Box(val)));
        return builder.CreateVector(offsets);
    }

    PairOffset createPair(std::string const & key, Box const & bag)
    {
        using namespace libtbag::proto::fbs::box;
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

    flatbuffers::Offset<void> createAnyArr(libtbag::proto::fbs::box::AnyArr value_type, Box const & bag)
    {
        using namespace libtbag::proto::fbs::box;
        switch (value_type) {
        // clang-format off
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
        // clang-format on
        }
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return flatbuffers::Offset<void>();
    }
};

// ---------------------------------
// BoxPacketBuilder implementation.
// ---------------------------------

BoxPacketBuilder::BoxPacketBuilder(std::size_t capacity) : _impl(std::make_unique<Impl>(capacity))
{
    assert(static_cast<bool>(_impl));
}

BoxPacketBuilder::~BoxPacketBuilder()
{
    // EMPTY.
}

uint8_t * BoxPacketBuilder::point() const
{
    assert(static_cast<bool>(_impl));
    return _impl->point();
}

std::size_t BoxPacketBuilder::size() const
{
    assert(static_cast<bool>(_impl));
    return _impl->size();
}

Err BoxPacketBuilder::assign(uint8_t const * buffer, std::size_t size)
{
    assert(static_cast<bool>(_impl));
    return _impl->assign(buffer, size);
}

Err BoxPacketBuilder::assign(Buffer const & buffer)
{
    return assign((uint8_t const *)buffer.data(), buffer.size());
}

std::string BoxPacketBuilder::toJsonString() const
{
    assert(static_cast<bool>(_impl));
    return _impl->toJsonString();
}

Err BoxPacketBuilder::build(uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code);
}

Err BoxPacketBuilder::build(BoxMap const & boxes, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, boxes);
}

Err BoxPacketBuilder::build(std::string const & content, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, content);
}

Err BoxPacketBuilder::build(std::string const & key, std::string const & val, uint64_t id, int32_t type, int32_t code)
{
    assert(static_cast<bool>(_impl));
    return _impl->build(id, type, code, key, val);
}

BoxPacketBuilder::Buffer BoxPacketBuilder::toBuffer() const
{
    return Buffer(point(), point() + size());
}

/**
 * FlatBuffer parser implementation.
 *
 * @author zer0
 * @date   2018-10-24
 */
struct BoxPacketParser::Impl
{
private:
    BoxPacketParser * _parent = nullptr;

public:
    Impl(BoxPacketParser * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    std::pair<Err, std::size_t> parse(char const * buffer, std::size_t size, void * arg = nullptr,
                                      bool only_header = false, char const * key = nullptr)
    {
        using namespace flatbuffers;
        using namespace libtbag::proto::fbs::box;

        assert(_parent != nullptr);

        Verifier verifier((uint8_t const *)buffer, size);
        auto const VERIFY_RESULT = VerifyBoxPacketBuffer(verifier);

#if defined(FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE)
        std::size_t const COMPUTED_SIZE = verifier.GetComputedSize();
#else
        std::size_t const COMPUTED_SIZE = 0U;
#endif

        if (!VERIFY_RESULT) {
            return std::make_pair(E_VERIFIER, COMPUTED_SIZE);
        }

        auto const * PACKET = GetBoxPacket(buffer);
        _parent->onHeader(PACKET->id(), PACKET->type(), PACKET->code(), arg);
        if (only_header) {
            return std::make_pair(E_SUCCESS, COMPUTED_SIZE);
        }

        auto const * PAIRS = PACKET->pairs();
        if (PAIRS == nullptr) {
            // Not exists 'pairs'.
            return std::make_pair(E_SUCCESS, COMPUTED_SIZE);
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
                return std::make_pair(E_ENOMSG, COMPUTED_SIZE);
            }

            if (!VerifyAnyArr(verifier, itr->val(), VAL_TYPE)) {
                // Use 'Parsing error' to distinguish it from 'Verifier error' at the top.
                return std::make_pair(E_PARSING, COMPUTED_SIZE);
            }

            _parent->onPair(std::string(itr->key()->str()), createBagEx(itr, VAL_TYPE), arg);
            if (!FIND_KEY.empty() && FIND_KEY == itr->key()->str()) {
                // Found the key !! Exit the loop.
                return std::make_pair(E_SUCCESS, COMPUTED_SIZE);
            }
        }

        if (!FIND_KEY.empty()) {
            // Not found the key.
            return std::make_pair(E_ENFOUND, COMPUTED_SIZE);
        }
        return std::make_pair(E_SUCCESS, COMPUTED_SIZE);
    }

    template <typename PairItr>
    Box createBagEx(PairItr itr, box::AnyArr arr_type) const
    {
        using namespace libtbag::proto::fbs::box;
        switch (arr_type) {
        // clang-format off
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
        case AnyArr_NONE:       return Box();
        default: TBAG_INACCESSIBLE_BLOCK_ASSERT();
        // clang-format on
        }
        return Box();
    }

    template <typename PairItr, typename TbagArrType, typename ValueType>
    Box createBagEx(PairItr itr) const
    {
        Box bag;

        Err code = bag.create<ValueType>();
        if (isFailure(code)) {
            return Box();
        }

        code = bag.resize(itr->i0(), itr->i1(), itr->i2(), itr->i3(), itr->i4(), itr->i5(), itr->i6(), itr->i7());
        if (isFailure(code)) {
            return Box();
        }

        auto val = (TbagArrType const *)itr->val();
        bag.copyFrom(val->data()->data(), val->data()->size());
        return bag;
    }
};

// --------------------------------
// BoxPacketParser implementation.
// --------------------------------

BoxPacketParser::BoxPacketParser() : _impl(std::make_unique<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

BoxPacketParser::~BoxPacketParser()
{
    // EMPTY.
}

Err BoxPacketParser::parse(char const * buffer, std::size_t size, void * arg, std::size_t * computed_size)
{
    assert(static_cast<bool>(_impl));
    auto const RESULT = _impl->parse(buffer, size, arg, false, nullptr);
    if (computed_size != nullptr) {
        *computed_size = RESULT.second;
    }
    return RESULT.first;
}

Err BoxPacketParser::parse(Buffer const & buffer, void * arg, std::size_t * computed_size)
{
    return parse(buffer.data(), buffer.size(), arg, computed_size);
}

Err BoxPacketParser::parseOnlyHeader(char const * buffer, std::size_t size, void * arg)
{
    assert(static_cast<bool>(_impl));
    return _impl->parse(buffer, size, arg, true, nullptr).first;
}

Err BoxPacketParser::parseFindKey(char const * buffer, std::size_t size, std::string const & key, void * arg)
{
    assert(static_cast<bool>(_impl));
    return _impl->parse(buffer, size, arg, false, key.c_str()).first;
}

// -------------------------
// BoxPacket implementation.
// -------------------------

BoxPacket::BoxPacket(std::size_t capacity) : BoxPacketBuilder(capacity), BoxPacketParser()
{
    // EMPTY.
}

BoxPacket::~BoxPacket()
{
    // EMPTY.
}

void BoxPacket::onHeader(uint64_t id, int32_t type, int32_t code, void * arg)
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
        Box * bag = (Box*)user_arg->user;
        assert(bag != nullptr);
    } else {
        // Unknown types.
    }
}

void BoxPacket::onPair(std::string && key, Box && val, void * arg)
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
        Box * bag = (Box*)user_arg->user;
        assert(bag != nullptr);
        *bag = std::move(val);
    } else {
        // Unknown types.
    }
}

void BoxPacket::clear()
{
    _id = 0;
    _type = 0;
    _code = 0;
    _bags.clear();
}

Err BoxPacket::parse(char const * buffer, std::size_t size, std::size_t * computed_size)
{
    return BoxPacketParser::parse(buffer, size, this, computed_size);
}

Err BoxPacket::parse(Buffer const & buffer, std::size_t * computed_size)
{
    return parse(buffer.data(), buffer.size(), computed_size);
}

Err BoxPacket::parseSelf(std::size_t * computed_size)
{
    return parse((char const *)point(), size(), computed_size);
}

BoxPacket::Box BoxPacket::findKey(char const * buffer, std::size_t size, std::string const & key, Err * code)
{
    Box result;
    UserArg arg;
    arg.type = static_cast<int>(UserArgType::UAT_BAG_EX);
    arg.user = &result;

    Err const PARSE_RESULT = parseFindKey(buffer, size, key, &arg);
    if (code != nullptr) {
        *code = PARSE_RESULT;
    }
    return result;
}

BoxPacket::Box BoxPacket::findKey(Buffer const & buffer, std::string const & key, Err * code)
{
    return findKey(buffer.data(), buffer.size(), key, code);
}

BoxPacket::Box BoxPacket::findKey(std::string const & key, Err * code)
{
    return findKey((char const *)point(), size(), key, code);
}

Err BoxPacket::buildSelf()
{
    return build(_bags, _id, _type, _code);
}

Err BoxPacket::saveFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    return writeFile(path, (char const *)point(), size());
}

Err BoxPacket::loadFile(std::string const & path)
{
    using namespace libtbag::filesystem;
    Buffer buffer;
    auto const CODE = readFile(path, buffer);
    if (isSuccess(CODE)) {
        return parse(buffer);
    }
    return CODE;
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

