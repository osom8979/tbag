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

Box::Box() : _data(std::make_shared<box_data>())
{
    assert(exists());
}

Box::Box(std::nullptr_t) TBAG_NOEXCEPT : _data(nullptr)
{
    assert(!exists());
}

Box::Box(box_data && data) TBAG_NOEXCEPT : _data(std::make_shared<box_data>(std::move(data)))
{
    // EMPTY.
}

Box::Box(Box const & obj) TBAG_NOEXCEPT : _data(obj._data)
{
    // EMPTY.
}

Box::Box(Box && obj) TBAG_NOEXCEPT : _data(std::move(obj._data))
{
    // EMPTY.
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
    if (this != &obj) {
        _data = std::move(obj._data);
    }
    return *this;
}

Box & Box::operator =(std::nullptr_t) TBAG_NOEXCEPT
{
    _data = nullptr;
    return *this;
}

void Box::createIfNotExists()
{
    if (!exists()) {
        _data = std::make_shared<box_data>();
    }
}

void Box::setInfo(ui8 const * info, ui32 size)
{
    createIfNotExists();
    _data->checked_assign_info_buffer(info, size);
}

void Box::setInfo(std::string const & info)
{
    return setInfo((ui8 const *)info.c_str(), static_cast<ui32>(info.size()));
}

void Box::setInfo(Buffer const & info)
{
    return setInfo((ui8 const *)info.data(), static_cast<ui32>(info.size()));
}

std::string Box::getInfoString() const
{
    if (_data && _data->info) {
        return std::string(_data->info, _data->info + _data->info_size);
    }
    return {};
}

Box::Buffer Box::getInfoBuffer() const
{
    if (_data && _data->info) {
        return Buffer(_data->info, _data->info + _data->info_size);
    }
    return {};
}

Err Box::_reshape_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const code = _reshape_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return code;
}

Err Box::_reshape_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const code = _reshape_vargs(type, rank, ap);
    va_end(ap);
    return code;
}

Err Box::_reshape_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    createIfNotExists();
    return _data->resize_vargs(type, device, ext, rank, ap);
}

Err Box::_reshape_vargs(btype type, ui32 rank, va_list ap)
{
    btype reshape_device = device_cpu();
    ui64 const * reshape_ext = nullptr;
    if (exists()) {
        if (!is_device_none()) {
            reshape_device = getDevice();
        }
        reshape_ext = getExtensions();
    }
    return _reshape_vargs(type, reshape_device, reshape_ext, rank, ap);
}

Err Box::_reshape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
{
    createIfNotExists();
    return _data->resize_dims(type, device, ext, rank, dims);
}

Err Box::_reshape_dims(btype type, ui32 rank, ui32 const * dims)
{
    btype reshape_device = device_cpu();
    ui64 const * reshape_ext = nullptr;
    if (exists()) {
        if (!is_device_none()) {
            reshape_device = getDevice();
        }
        reshape_ext = getExtensions();
    }
    return _reshape_dims(type, reshape_device, reshape_ext, rank, dims);
}

Err Box::_reshape_ref_box(btype type, box_data const * reference_box)
{
    if (reference_box == nullptr) {
        return E_ILLARGS;
    }
    return _reshape_dims(type,
                         reference_box->device,
                         reference_box->ext,
                         reference_box->rank,
                         reference_box->dims);
}

Err Box::_reshape_ref_box(btype type, Box const & reference_box)
{
    if (!reference_box) {
        return E_ILLARGS;
    }
    return _reshape_ref_box(type, reference_box.base());
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
    assert(result.exists());
    auto const code = result._reshape_vargs(type, device, ext, rank, ap);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::shape_vargs(btype type, ui32 rank, va_list ap)
{
    Box result;
    assert(result.exists());
    auto const code = result._reshape_vargs(type, rank, ap);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::shape_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
{
    Box result;
    assert(result.exists());
    auto const code = result._reshape_dims(type, device, ext, rank, dims);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::shape_dims(btype type, ui32 rank, ui32 const * dims)
{
    Box result;
    assert(result.exists());
    auto const code = result._reshape_dims(type, rank, dims);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Err Box::copyFromData(Box const & box)
{
    if (box.exists()) {
        createIfNotExists();
        return _data->checked_assign_data(box.type(), box.device(), box.ext(), box.rank(), box.dims(), box.data());
    } else {
        if (exists()) {
            clearData();
        }
        return E_SUCCESS;
    }
}

Err Box::copyToData(Box & box) const
{
    return box.copyFromData(*this);
}

Err Box::copyFromInfo(Box const & box)
{
    if (box.exists()) {
        createIfNotExists();
        _data->checked_assign_info_buffer(box.info(), box.info_size());
    } else {
        if (exists()) {
            clearInfo();
        }
    }
    return E_SUCCESS;
}

Err Box::copyToInfo(Box & box) const
{
    return box.copyFromInfo(*this);
}

Err Box::copyFrom(Box const & box)
{
    auto const code = copyFromData(box);
    if (isFailure(code)) {
        return code;
    }
    return copyFromInfo(box);
}

Err Box::copyTo(Box & box) const
{
    return box.copyFrom(*this);
}

Box Box::clone() const
{
    if (!exists()) {
        return Box(nullptr);
    }
    auto result = _data->clone();
    if (result) {
        return Box(std::move(result.value));
    }
    return Box(nullptr);
}

Err Box::setData(ui8 const * info, ui32 size)
{
    return assign(info, info + size);
}

Err Box::setData(std::string const & info)
{
    return assign(info.c_str(), info.c_str() + info.size());
}

Err Box::setData(Buffer const & info)
{
    return assign(info.data(), info.data() + info.size());
}

Err Box::getDataString(std::string & result) const
{
    if (isEmpty()) {
        return E_NREADY;
    }
    auto const * begin = data<char>();
    assert(begin != nullptr);
    auto const byte_size = getSize() * getTypeByte();
    assert(byte_size >= 1);
    result.assign(begin, begin + byte_size);
    return E_SUCCESS;
}

std::string Box::getDataString() const
{
    std::string result;
    if (isSuccess(getDataString(result))) {
        return result;
    }
    return {};
}

Err Box::encode(Builder & builder) const
{
    return builder.build(_data.get());
}

Err Box::encode(Builder & builder, Buffer & buffer) const
{
    auto const code = encode(builder);
    if (isFailure(code)) {
        return code;
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
    auto const code = encode(builder);
    if (isFailure(code)) {
        return code;
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

ErrPair<Box::Cursor> Box::cursor(int begin, int end, int step) const
{
    if (!exists()) {
        return E_EXPIRED;
    }
    if (rank() == 0) {
        return E_EINVAL;
    }
    auto const result = _data->init_cursor(0u, begin, end, step);
    return { result.code, result.value };
}

ErrPair<Box::Cursor> Box::cursor(int begin, int end) const
{
    return cursor(begin, end, 1);
}

ErrPair<Box::Cursor> Box::cursor(int begin) const
{
    return cursor(begin, nop);
}

ErrPair<Box::Cursor> Box::cursor() const
{
    return cursor(nop);
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

