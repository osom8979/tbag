/**
 * @file   BoxPacket.cpp
 * @brief  BoxPacket class implementation.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
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
#include <libtbag/box/fbs/box_generated.h>
#include <libtbag/box/fbs/box_d2s.h>

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

#include <libtbag/box/BoxPacket.hpp>

#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
//#include <libtbag/filesystem/File.hpp>
//#include <libtbag/filesystem/Path.hpp>
//#include <libtbag/string/StringUtils.hpp>

//#include <cassert>
//#include <algorithm>
//#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using namespace libtbag::box::fbs;
using namespace libtbag::box::details;

using FlatBufferBuilder = flatbuffers::FlatBufferBuilder;
using FlatBufferParser  = flatbuffers::Parser;

using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
using StringOffsetVector = flatbuffers::Vector<StringOffset>;

inline static anys get_anys(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BT_NONE   : return anys_NONE;
    case BT_INT8   : return anys_si8s;
    case BT_UINT8  : return anys_si16s;
    case BT_INT16  : return anys_si32s;
    case BT_UINT16 : return anys_si64s;
    case BT_INT32  : return anys_ui8s;
    case BT_UINT32 : return anys_ui16s;
    case BT_INT64  : return anys_ui32s;
    case BT_UINT64 : return anys_ui64s;
    case BT_FLOAT32: return anys_fp32s;
    case BT_FLOAT64: return anys_fp64s;
    default:
        return anys_NONE;
    }
    // clang-format on
}

/**
 * FlatBuffer builder implementation.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */
struct BoxPacketBuilder::Impl
{
public:
    FlatBufferBuilder builder;
    FlatBufferParser parser;

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

    Err build(box_data const * box)
    {
        auto type = get_anys(box->type);
        clear();
        finish(Createbox_fbs(builder, box->type, box->device,
                             box->ext[0], box->ext[1], box->ext[2], box->ext[3],
                             type, create_anys(type, box->data, box->size),
                             builder.CreateVector(box->dims, (size_t)box->rank),
                             builder.CreateString(box->info, box->info_size)));
        return E_SUCCESS;
    }

    flatbuffers::Offset<void> create_anys(anys value_type, void const * data, ui32 size)
    {
        // clang-format off
        switch (value_type) {
        case anys_si8s : return Createsi8s (builder, builder.CreateVector(( si8 const *)data, size)).Union();
        case anys_si16s: return Createsi16s(builder, builder.CreateVector((si16 const *)data, size)).Union();
        case anys_si32s: return Createsi32s(builder, builder.CreateVector((si32 const *)data, size)).Union();
        case anys_si64s: return Createsi64s(builder, builder.CreateVector((si64 const *)data, size)).Union();
        case anys_ui8s : return Createui8s (builder, builder.CreateVector(( ui8 const *)data, size)).Union();
        case anys_ui16s: return Createui16s(builder, builder.CreateVector((ui16 const *)data, size)).Union();
        case anys_ui32s: return Createui32s(builder, builder.CreateVector((ui32 const *)data, size)).Union();
        case anys_ui64s: return Createui64s(builder, builder.CreateVector((ui64 const *)data, size)).Union();
        case anys_fp32s: return Createfp32s(builder, builder.CreateVector((fp32 const *)data, size)).Union();
        case anys_fp64s: return Createfp64s(builder, builder.CreateVector((fp64 const *)data, size)).Union();
        default: break;
        }
        // clang-format on
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return flatbuffers::Offset<void>();
    }
};

// -------------------------------
// BoxPacketBuilder implementation
// -------------------------------

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

Err BoxPacketBuilder::build(box_data const * box)
{
    assert(static_cast<bool>(_impl));
    assert(box != nullptr);
    return _impl->build(box);
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
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
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
    std::pair<Err, std::size_t> parse(char const * buffer, std::size_t size, box_data * box)
    {
        using namespace flatbuffers;
        assert(_parent != nullptr);
        assert(buffer != nullptr);
        assert(size >= 1);
        assert(box != nullptr);

        Verifier verifier((uint8_t const *)buffer, size);
        auto const VERIFY_RESULT = Verifybox_fbsBuffer(verifier);

#if defined(FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE)
        std::size_t const COMPUTED_SIZE = verifier.GetComputedSize();
#else
        std::size_t const COMPUTED_SIZE = 0U;
#endif

        if (!VERIFY_RESULT) {
            return std::make_pair(E_VERIFIER, COMPUTED_SIZE);
        }

        auto const * packet = Getbox_fbs(buffer);
        auto const data_type = packet->data_type();
        if (!(anys_MIN <= COMPARE_AND(data_type) <= anys_MAX)) {
            return std::make_pair(E_ENOMSG, COMPUTED_SIZE);
        }

        if (!Verifyanys(verifier, packet->data(), data_type)) {
            // Use 'Parsing error' to distinguish it from 'Verifier error' at the top.
            return std::make_pair(E_PARSING, COMPUTED_SIZE);
        }

        auto const type = packet->type();
        auto const device = packet->device();
        ui64 ext[BOX_EXT_SIZE];
        ext[0] = packet->ext0();
        ext[1] = packet->ext1();
        ext[2] = packet->ext2();
        ext[3] = packet->ext3();

        auto const * table = packet->data();
        void const * data_ptr = nullptr;
        ui32 data_size = 0;

        // clang-format off
        switch (data_type) {
        case anys_si8s :  data_ptr = get_table_data((libtbag::box::fbs::si8s  const *)table, &data_size);
        case anys_si16s:  data_ptr = get_table_data((libtbag::box::fbs::si16s const *)table, &data_size);
        case anys_si32s:  data_ptr = get_table_data((libtbag::box::fbs::si32s const *)table, &data_size);
        case anys_si64s:  data_ptr = get_table_data((libtbag::box::fbs::si64s const *)table, &data_size);
        case anys_ui8s :  data_ptr = get_table_data((libtbag::box::fbs::ui8s  const *)table, &data_size);
        case anys_ui16s:  data_ptr = get_table_data((libtbag::box::fbs::ui16s const *)table, &data_size);
        case anys_ui32s:  data_ptr = get_table_data((libtbag::box::fbs::ui32s const *)table, &data_size);
        case anys_ui64s:  data_ptr = get_table_data((libtbag::box::fbs::ui64s const *)table, &data_size);
        case anys_fp32s:  data_ptr = get_table_data((libtbag::box::fbs::fp32s const *)table, &data_size);
        case anys_fp64s:  data_ptr = get_table_data((libtbag::box::fbs::fp64s const *)table, &data_size);
        case anys_NONE:
            TBAG_FALLTHROUGH
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
        }
        // clang-format on

        auto const * dims = packet->dims()->data();
        auto const dims_size = packet->dims()->size();

        auto const info = packet->info()->data();
        auto const info_size = packet->info()->size();

        auto code = box_malloc_copy_dims(box, type, device, ext, dims,
                                         GET_RANK_TO_TOTAL_DIMS_BYTE(dims_size),
                                         dims_size);
        if (isFailure(code)) {
            return std::make_pair(code, COMPUTED_SIZE);
        }

        code = box_data_copy(box, data_ptr, type, device, ext, data_size);
        if (isFailure(code)) {
            return std::make_pair(code, COMPUTED_SIZE);
        }

        box->info = box_info_malloc(info_size);
        box->total_info_byte = GET_SIZE_TO_TOTAL_INFO_BYTE(info_size);
        box->info_size = info_size;
        if (!box_info_assign(box->info, box->total_info_byte, info, info_size)) {
            code = E_ESET;
        }
        return std::make_pair(code, COMPUTED_SIZE);
    }

    template <typename T>
    void const * get_table_data(T table, ui32 * data_size) const
    {
        assert(table != nullptr);
        *data_size = table->arr()->size();
        return table->arr()->data();
    }
};

// ------------------------------
// BoxPacketParser implementation
// ------------------------------

BoxPacketParser::BoxPacketParser() : _impl(std::make_unique<Impl>(this))
{
    assert(_impl);
}

BoxPacketParser::~BoxPacketParser()
{
    // EMPTY.
}

Err BoxPacketParser::parse(char const * buffer, std::size_t size, box_data * box, std::size_t * computed_size)
{
    assert(_impl);
    auto result = _impl->parse(buffer, size, box);
    if (computed_size != nullptr) {
        *computed_size = result.second;
    }
    return result.first;
}

// ------------------------
// BoxPacket implementation
// ------------------------

BoxPacket::BoxPacket(std::size_t capacity) : BoxPacketBuilder(capacity), BoxPacketParser()
{
    // EMPTY.
}

BoxPacket::~BoxPacket()
{
    // EMPTY.
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

