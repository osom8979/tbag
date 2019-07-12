/**
 * @file   Box.cpp
 * @brief  Box class implementation.
 * @author zer0
 * @date   2019-05-16
 */

#include <libtbag/box/Box.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <cstdlib>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using namespace libtbag::box::details;

Box::Box() : _data(nullptr)
{
    auto * box = (box_data*)::malloc(sizeof(box_data));
    assert(box != nullptr);
    box_clear(box);
    box->opaque = nullptr;

    _data.reset(box, [](box_data * ptr){
        box_free(ptr);
        ::free(ptr);
    });
}

Box::Box(std::nullptr_t) TBAG_NOEXCEPT : _data(nullptr)
{
    // EMPTY.
}

Box::Box(Box const & obj) TBAG_NOEXCEPT : Box(nullptr)
{
    (*this) = obj;
}

Box::Box(Box && obj) TBAG_NOEXCEPT : Box(nullptr)
{
    (*this) = std::move(obj);
}

Box::~Box()
{
    // EMPTY.
}

Box & Box::operator =(Box const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _data = obj._data;
    }
    return *this;
}

Box & Box::operator =(Box && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Box::swap(Box & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _data.swap(obj._data);
    }
}

void Box::reset()
{
    _data.reset();
}

Err Box::setInfo(ui8 const * info, ui32 size)
{
    return box_info_checked_assign(_data.get(), info, size) ? E_SUCCESS : E_ECOPY;
}

Err Box::setInfo(std::string const & info)
{
    return setInfo((ui8 const *)info.c_str(), static_cast<ui32>(info.size()));
}

Err Box::setInfo(Buffer const & info)
{
    return setInfo((ui8 const *)info.data(), static_cast<ui32>(info.size()));
}

std::string Box::getInfoString() const
{
    if (_data->info == nullptr) {
        return std::string();
    }
    return std::string(_data->info, _data->info + _data->info_size);
}

Box::Buffer Box::getInfoBuffer() const
{
    if (_data->info == nullptr) {
        return Buffer();
    }
    return Buffer(_data->info, _data->info + _data->info_size);
}

Err Box::reshape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = reshape_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return CODE;
}

Err Box::reshape_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const CODE = reshape_vargs(type, rank, ap);
    va_end(ap);
    return CODE;
}

Err Box::reshape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    return box_resize_vargs(_data.get(), type, device, ext, rank, ap);
}

Err Box::reshape_vargs(btype type, ui32 rank, va_list ap)
{
    btype reshape_device = device_cpu();
    ui64 const * reshape_ext = nullptr;
    if (exists()) {
        if (!is_device_none()) {
            reshape_device = device();
        }
        reshape_ext = ext();
    }
    return reshape_vargs(type, reshape_device, reshape_ext, rank, ap);
}

Box Box::shape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto result = shape_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return result;
}

Box Box::shape_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto result = shape_vargs(type, rank, ap);
    va_end(ap);
    return result;
}

Box Box::shape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    Box result;
    assert(result);
    auto const CODE = result.reshape_vargs(type, device, ext, rank, ap);
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::shape_vargs(btype type, ui32 rank, va_list ap)
{
    Box result;
    assert(result);
    auto const CODE = result.reshape_vargs(type, rank, ap);
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Err Box::copyFromData(Box const & box)
{
    return box_data_copy(get(), box.get());
}

Err Box::copyToData(Box & box) const
{
    return box.copyFromData(*this);
}

Err Box::copyFromInfo(Box const & box)
{
    return box_info_checked_assign(get(), box.get()) ? E_SUCCESS : E_ECOPY;
}

Err Box::copyToInfo(Box & box) const
{
    return box.copyFromInfo(*this);
}

Err Box::clone(Box & box) const
{
    return box_clone(box.get(), get());
}

Box Box::clone() const
{
    Box result;
    auto const CODE = clone(result);
    if (isFailure(CODE)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::asType(btype type) const
{
    Box result;
    auto code = box_malloc_copy_dims(result.get(), type, device(), ext(), dims(), dims_capacity(), rank());
    if (isFailure(code)) {
        return Box(nullptr);
    }
    code = result.copyFromData(*this);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    code = result.copyFromInfo(*this);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Err Box::encode(Builder & builder) const
{
    return builder.build(_data.get());
}

Err Box::encode(Builder & builder, Buffer & buffer) const
{
    auto const CODE = encode(builder);
    if (isFailure(CODE)) {
        return CODE;
    }
    buffer.assign(builder.point(), builder.point() + builder.size());
    return E_SUCCESS;
}

Err Box::encode(Buffer & buffer) const
{
    Builder builder;
    return encode(builder, buffer);
}

Err Box::decode(void const * buffer, std::size_t size, Parser const & parser, std::size_t * computed_size)
{
    return parser.parse(buffer, size, _data.get(), computed_size);
}

Err Box::decode(void const * buffer, std::size_t size, std::size_t * computed_size)
{
    Parser parser;
    return decode(buffer, size, parser, computed_size);
}

Err Box::decode(Buffer const & buffer, std::size_t * computed_size)
{
    return decode(buffer.data(), buffer.size(), computed_size);
}

Err Box::encodeToJson(Builder & builder, std::string & json) const
{
    auto const CODE = encode(builder);
    if (isFailure(CODE)) {
        return CODE;
    }
    json = builder.toJsonString();
    return E_SUCCESS;
}

Err Box::encodeToJson(std::string & json) const
{
    Builder builder;
    return encodeToJson(builder, json);
}

Err Box::decodeFromJson(char const * json, std::size_t size, Parser const & parser)
{
    return parser.parseJson(std::string(json, json + size), _data.get());
}

Err Box::decodeFromJson(char const * json, std::size_t size)
{
    Parser parser;
    return decodeFromJson(json, size, parser);
}

Err Box::decodeFromJson(std::string const & json)
{
    return decodeFromJson(json.c_str(), json.size());
}

std::string Box::toJsonText(Err * code) const
{
    std::string json;
    auto const result = encodeToJson(json);
    if (code != nullptr) {
        *code = result;
    }
    return json;
}

bool Box::fromJsonText(std::string const & json, Err * code)
{
    auto const result = decodeFromJson(json);
    if (code != nullptr) {
        *code = result;
    }
    return isSuccess(result);
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

