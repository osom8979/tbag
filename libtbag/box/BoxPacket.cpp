/**
 * @file   BoxPacket.cpp
 * @brief  BoxPacket class implementation.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

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
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

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

inline static AnyArr convertBtypeToAnyArr(btype type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case BT_NONE      : return AnyArr_NONE;
    case BT_BOOL      : return AnyArr_BoolArr;
    case BT_INT8      : return AnyArr_ByteArr;
    case BT_INT16     : return AnyArr_ShortArr;
    case BT_INT32     : return AnyArr_IntArr;
    case BT_INT64     : return AnyArr_LongArr;
    case BT_UINT8     : return AnyArr_UbyteArr;
    case BT_UINT16    : return AnyArr_UshortArr;
    case BT_UINT32    : return AnyArr_UintArr;
    case BT_UINT64    : return AnyArr_UlongArr;
    case BT_FLOAT32   : return AnyArr_FloatArr;
    case BT_FLOAT64   : return AnyArr_DoubleArr;
    case BT_COMPLEX64 : return AnyArr_Complex64Arr;
    case BT_COMPLEX128: return AnyArr_Complex128Arr;
    default           : return AnyArr_NONE;
    }
    // clang-format on
}

inline static btype convertAnyArrToBtype(AnyArr type) TBAG_NOEXCEPT
{
    // clang-format off
    switch (type) {
    case AnyArr_NONE         : return BT_NONE;
    case AnyArr_BoolArr      : return BT_BOOL;
    case AnyArr_ByteArr      : return BT_INT8;
    case AnyArr_ShortArr     : return BT_INT16;
    case AnyArr_IntArr       : return BT_INT32;
    case AnyArr_LongArr      : return BT_INT64;
    case AnyArr_UbyteArr     : return BT_UINT8;
    case AnyArr_UshortArr    : return BT_UINT16;
    case AnyArr_UintArr      : return BT_UINT32;
    case AnyArr_UlongArr     : return BT_UINT64;
    case AnyArr_FloatArr     : return BT_FLOAT32;
    case AnyArr_DoubleArr    : return BT_FLOAT64;
    case AnyArr_Complex64Arr : return BT_COMPLEX64;
    case AnyArr_Complex128Arr: return BT_COMPLEX128;
    default                  : return BT_NONE;
    }
    // clang-format on
}

static bool initParser(FlatBufferParser & parser)
{
    return parser.Parse(libtbag::string::mergeArgv((char**)__data_to_string__box__).c_str());
}

/**
 * FlatBuffer builder implementation for BoxPacketBuilder.
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
    std::vector<std::uint8_t> boolean_buffer;
    std::vector<Complex64> complex64_buffer;
    std::vector<Complex128> complex128_buffer;

public:
    Impl(std::size_t capacity) : Impl(Options(), capacity)
    {
        // EMPTY.
    }

    Impl(Options const & options, std::size_t capacity) : builder(capacity, nullptr)
    {
        if (!initParser(parser)) {
            tDLogA("BoxPacketBuilder::Impl() Parse fail.");
            throw std::bad_alloc();
        }
        parser.opts.strict_json = options.strict_json;
        parser.opts.indent_step = options.indent_step;
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
        if (box == nullptr) {
            return E_EXPIRED;
        }
        if (box->device != BD_CPU) {
            return E_DEVICE;
        }

        auto const ANY_TYPE = convertBtypeToAnyArr(box->type);
        clear();

        using namespace flatbuffers;
        Offset<void> box_data = 0;
        if (box->data != nullptr && box->size >= 1) {
            switch (ANY_TYPE) {
            case AnyArr_NONE:
                break;
            case AnyArr_BoolArr:
                box_data = createBoolArr(box->data, box->size);
                break;
            case AnyArr_Complex64Arr:
                box_data = createComplex64Arr((c64 const *)box->data, box->size);
                break;
            case AnyArr_Complex128Arr:
                box_data = createComplex128Arr((c128 const *)box->data, box->size);
                break;
            default:
                box_data = createAnyArr(ANY_TYPE, box->data, box->size);
                break;
            }
        }

        Offset< Vector<uint32_t> > box_dims = 0;
        if (box->dims != nullptr && box->rank >= 1) {
            box_dims = builder.CreateVector<uint32_t>(box->dims, box->rank);
        }

        Offset< Vector<uint8_t> > box_info = 0;
        if (box->info != nullptr && box->info_size >= 1) {
            box_info = builder.CreateVector<uint8_t>(box->info, box->info_size);
        }

        finish(CreateBoxFbs(builder, box->ext[0], box->ext[1], box->ext[2], box->ext[3],
                            ANY_TYPE, box_data, box_dims, box_info));
        return E_SUCCESS;
    }

    struct use_boolean_buffer_t { /* EMPTY. */ };
    struct direct_static_cast_t { /* EMPTY. */ };

    flatbuffers::Offset<void> createBoolArr(use_boolean_buffer_t, bool const * data, ui32 size)
    {
        boolean_buffer.resize(size);
        for (auto i = 0; i < size; ++i) {
            boolean_buffer[i] = static_cast<std::uint8_t>(data[i]);
        }
        return createBoolArr(direct_static_cast_t{}, boolean_buffer.data(), boolean_buffer.size());
    }

    flatbuffers::Offset<void> createBoolArr(direct_static_cast_t, void const * data, ui32 size)
    {
        return CreateBoolArr(builder, builder.CreateVector((uint8_t const *)data, size)).Union();
    }

    flatbuffers::Offset<void> createBoolArr(void const * data, ui32 size)
    {
        using select_t = typename std::conditional<
                sizeof(bool) == 1,
                direct_static_cast_t,
                use_boolean_buffer_t
        >::type;
        return createBoolArr(select_t{}, data, size);
    }

    flatbuffers::Offset<void> createComplex64Arr(c64 const * data, ui32 size)
    {
        complex64_buffer.resize(size);
        for (auto i = 0; i < size; ++i) {
            complex64_buffer[i] = Complex64(data[i].real(), data[i].imag());
        }
        return CreateComplex64ArrDirect(builder, &complex64_buffer).Union();
    }

    flatbuffers::Offset<void> createComplex128Arr(c128 const * data, ui32 size)
    {
        complex128_buffer.resize(size);
        for (auto i = 0; i < size; ++i) {
            complex128_buffer[i] = Complex128(data[i].real(), data[i].imag());
        }
        return CreateComplex128ArrDirect(builder, &complex128_buffer).Union();
    }

    flatbuffers::Offset<void> createAnyArr(AnyArr any_type, void const * data, ui32 size)
    {
        assert(any_type != AnyArr_NONE);
        assert(any_type != AnyArr_BoolArr);
        assert(any_type != AnyArr_Complex64Arr);
        assert(any_type != AnyArr_Complex128Arr);

        // clang-format off
        switch (any_type) {
        case AnyArr_ByteArr  : return CreateByteArr  (builder, builder.CreateVector(( si8 const *)data, size)).Union();
        case AnyArr_ShortArr : return CreateShortArr (builder, builder.CreateVector((si16 const *)data, size)).Union();
        case AnyArr_IntArr   : return CreateIntArr   (builder, builder.CreateVector((si32 const *)data, size)).Union();
        case AnyArr_LongArr  : return CreateLongArr  (builder, builder.CreateVector((si64 const *)data, size)).Union();
        case AnyArr_UbyteArr : return CreateUbyteArr (builder, builder.CreateVector(( ui8 const *)data, size)).Union();
        case AnyArr_UshortArr: return CreateUshortArr(builder, builder.CreateVector((ui16 const *)data, size)).Union();
        case AnyArr_UintArr  : return CreateUintArr  (builder, builder.CreateVector((ui32 const *)data, size)).Union();
        case AnyArr_UlongArr : return CreateUlongArr (builder, builder.CreateVector((ui64 const *)data, size)).Union();
        case AnyArr_FloatArr : return CreateFloatArr (builder, builder.CreateVector((fp32 const *)data, size)).Union();
        case AnyArr_DoubleArr: return CreateDoubleArr(builder, builder.CreateVector((fp64 const *)data, size)).Union();
        case AnyArr_NONE:
            TBAG_FALLTHROUGH
        default:
            break;
        }
        // clang-format on

        return {};
    }
};

// ----------------
// BoxPacketBuilder
// ----------------

BoxPacketBuilder::BoxPacketBuilder(std::size_t capacity)
        : _impl(std::make_unique<Impl>(capacity))
{
    assert(static_cast<bool>(_impl));
}

BoxPacketBuilder::BoxPacketBuilder(Options const & options, std::size_t capacity)
        : _impl(std::make_unique<Impl>(options, capacity))
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
 * FlatBuffer parser implementation for BoxPacketParser.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */
struct BoxPacketParser::Impl
{
public:
    FlatBufferParser parser;

public:
    Impl()
    {
        if (!initParser(parser)) {
            tDLogA("BoxPacketParser::Impl() Parse fail.");
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    std::pair<Err, std::size_t> parseJson(std::string const & json_text, box_data * box)
    {
        if (!parser.Parse(json_text.c_str())) {
            return std::make_pair(E_PARSING, 0);
        }
        return parse(parser.builder_.GetBufferPointer(), parser.builder_.GetSize(), box);
    }

    std::pair<Err, std::size_t> parse(void const * buffer, std::size_t size, box_data * box) const
    {
        using namespace flatbuffers;
        assert(buffer != nullptr);
        assert(size >= 1);
        assert(box != nullptr);

        Verifier verifier((uint8_t const *)buffer, size);
        auto const VERIFY_RESULT = VerifyBoxFbsBuffer(verifier);

#if defined(FLATBUFFERS_TRACK_VERIFIER_BUFFER_SIZE)
        std::size_t const computed_size = verifier.GetComputedSize();
#else
        std::size_t const computed_size = 0U;
#endif

        if (!VERIFY_RESULT) {
            return std::make_pair(E_VERIFIER, computed_size);
        }

        auto const * packet = GetBoxFbs(buffer);
        auto const data_type = packet->data_type();
        if (!(AnyArr_MIN <= COMPARE_AND(data_type) <= AnyArr_MAX)) {
            return std::make_pair(E_ENOMSG, computed_size);
        }

        if (!VerifyAnyArr(verifier, packet->data(), data_type)) {
            // Use 'Parsing error' to distinguish it from 'Verifier error' at the top.
            return std::make_pair(E_PARSING, computed_size);
        }

        auto const * dims = packet->dims();

        ui64 ext[TBAG_BOX_EXT_SIZE];
        ext[0] = packet->ext0();
        ext[1] = packet->ext1();
        ext[2] = packet->ext2();
        ext[3] = packet->ext3();

        if (dims != nullptr) {
            auto const * dims_data = dims->data();
            auto const dims_size = dims->size();

            ui32 total_dims = 0;
            if (dims_data != nullptr && dims_size >= 1) {
                for (auto dims_index = 0; dims_index < dims_size; ++dims_index) {
                    total_dims += dims_data[dims_index];
                }
            }

            if (total_dims >= 1) {
                auto code = box->resize_dims(convertAnyArrToBtype(data_type), BD_CPU, ext, dims_size, dims_data);
                if (isFailure(code)) {
                    return std::make_pair(code, computed_size);
                }

                auto const * fbs_table = packet->data();
                if (fbs_table != nullptr) {
                    // clang-format off
                    switch (data_type) {
                    case AnyArr_BoolArr      : update_data((BoolArr       const *)fbs_table, (bool*)box->data); break;
                    case AnyArr_ByteArr      : update_data((ByteArr       const *)fbs_table, (si8 *)box->data); break;
                    case AnyArr_ShortArr     : update_data((ShortArr      const *)fbs_table, (si16*)box->data); break;
                    case AnyArr_IntArr       : update_data((IntArr        const *)fbs_table, (si32*)box->data); break;
                    case AnyArr_LongArr      : update_data((LongArr       const *)fbs_table, (si64*)box->data); break;
                    case AnyArr_UbyteArr     : update_data((UbyteArr      const *)fbs_table, (ui8 *)box->data); break;
                    case AnyArr_UshortArr    : update_data((UshortArr     const *)fbs_table, (ui16*)box->data); break;
                    case AnyArr_UintArr      : update_data((UintArr       const *)fbs_table, (ui32*)box->data); break;
                    case AnyArr_UlongArr     : update_data((UlongArr      const *)fbs_table, (ui64*)box->data); break;
                    case AnyArr_FloatArr     : update_data((FloatArr      const *)fbs_table, (fp32*)box->data); break;
                    case AnyArr_DoubleArr    : update_data((DoubleArr     const *)fbs_table, (fp64*)box->data); break;
                    case AnyArr_Complex64Arr : update_data((Complex64Arr  const *)fbs_table, (c64 *)box->data); break;
                    case AnyArr_Complex128Arr: update_data((Complex128Arr const *)fbs_table, (c128*)box->data); break;
                    case AnyArr_NONE:
                        break;
                    default:
                        TBAG_INACCESSIBLE_BLOCK_ASSERT();
                    }
                    // clang-format on
                } else {
                    box->size = 0;
                }
            } else {
                box->type = BT_NONE;
                box->device = BD_CPU;
                box->ext[0] = ext[0];
                box->ext[1] = ext[1];
                box->ext[2] = ext[2];
                box->ext[3] = ext[3];
                box->rank = 0;
                box->size = 0;
            }
        } else {
            box->type = BT_NONE;
            box->device = BD_CPU;
            box->ext[0] = ext[0];
            box->ext[1] = ext[1];
            box->ext[2] = ext[2];
            box->ext[3] = ext[3];
            box->rank = 0;
            box->size = 0;
        }

        auto const * info = packet->info();
        if (info != nullptr) {
            auto const * info_data = info->data();
            auto const info_size = info->size();
            if (info_size >= 1) {
                assert(info_data != nullptr);
                box->checked_assign_info_buffer(info_data, info_size);
            } else {
                box->info_size = 0;
            }
        } else {
            box->info_size = 0;
        }

        return std::make_pair(E_SUCCESS, computed_size);
    }

    template <typename FlatT, typename DataT>
    void update_data(FlatT table, DataT * data) const
    {
        assert(table != nullptr);
        auto * arr = table->arr();
        assert(arr != nullptr);
        auto const size = arr->size();
        for (std::size_t i = 0; i < size; ++i) {
            data[i] = arr->Get(i);
        }
    }

    void update_data(Complex64Arr const * table, c64 * data) const
    {
        assert(table != nullptr);
        auto * arr = table->arr();
        assert(arr != nullptr);
        auto const size = arr->size();
        for (std::size_t i = 0; i < size; ++i) {
            data[i].real(arr->Get(i)->real());
            data[i].imag(arr->Get(i)->imag());
        }
    }

    void update_data(Complex128Arr const * table, c128 * data) const
    {
        assert(table != nullptr);
        auto * arr = table->arr();
        assert(arr != nullptr);
        auto const size = arr->size();
        for (std::size_t i = 0; i < size; ++i) {
            data[i].real(arr->Get(i)->real());
            data[i].imag(arr->Get(i)->imag());
        }
    }
};

// ---------------
// BoxPacketParser
// ---------------

BoxPacketParser::BoxPacketParser() : _impl(std::make_unique<Impl>())
{
    assert(_impl);
}

BoxPacketParser::~BoxPacketParser()
{
    // EMPTY.
}

Err BoxPacketParser::parse(void const * buffer, std::size_t size,
                           box_data * box, std::size_t * computed_size) const
{
    assert(_impl);
    auto result = _impl->parse(buffer, size, box);
    if (computed_size != nullptr) {
        *computed_size = result.second;
    }
    return result.first;
}

Err BoxPacketParser::parseJson(std::string const & json_text, box_data * box) const
{
    assert(_impl);
    return _impl->parseJson(json_text, box).first;
}

// ---------
// BoxPacket
// ---------

BoxPacket::BoxPacket(std::size_t capacity)
        : BoxPacketBuilder(capacity), BoxPacketParser()
{
    // EMPTY.
}

BoxPacket::BoxPacket(Options const & options, std::size_t capacity)
        : BoxPacketBuilder(options, capacity), BoxPacketParser()
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

