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
#include <cstring>

#include <functional>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

using namespace libtbag::box::details;

using ErrBox = Box::ErrBox;
using ErrMsgBox = Box::ErrMsgBox;

Box::Box() : BoxBase()
{
    assert(exists());
}

Box::Box(std::nullptr_t) TBAG_NOEXCEPT : BoxBase(nullptr)
{
    assert(!exists());
}

Box::Box(box_data && data) TBAG_NOEXCEPT : BoxBase(std::move(data))
{
    // EMPTY.
}

Box::Box(Box const & obj) TBAG_NOEXCEPT : BoxBase(obj._base)
{
    // EMPTY.
}

Box::Box(Box && obj) TBAG_NOEXCEPT : BoxBase(std::move(obj._base))
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
        _base = obj._base;
    }
    return *this;
}

Box & Box::operator =(Box && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base = std::move(obj._base);
    }
    return *this;
}

Box & Box::operator =(std::nullptr_t) TBAG_NOEXCEPT
{
    _base = nullptr;
    return *this;
}

void Box::__create_if_not_exists()
{
    if (!exists()) {
        _base = std::make_shared<box_data>();
    }
}

void Box::setInfo(ui8 const * info, ui32 size)
{
    __create_if_not_exists();
    _base->checked_assign_info_buffer(info, size);
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
    if (_base && _base->info) {
        return std::string(_base->info, _base->info + _base->info_size);
    }
    return {};
}

Box::Buffer Box::getInfoBuffer() const
{
    if (_base && _base->info) {
        return Buffer(_base->info, _base->info + _base->info_size);
    }
    return {};
}

Err Box::_resize_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const code = _resize_vargs(type, device, ext, rank, ap);
    va_end(ap);
    return code;
}

Err Box::_resize_args(btype type, ui32 rank, ...)
{
    va_list ap;
    va_start(ap, rank);
    auto const code = _resize_vargs(type, rank, ap);
    va_end(ap);
    return code;
}

Err Box::_resize_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap)
{
    __create_if_not_exists();
    return _base->resize_vargs(type, device, ext, rank, ap);
}

Err Box::_resize_vargs(btype type, ui32 rank, va_list ap)
{
    btype reshape_device = device_cpu();
    ui64 reshape_ext[TBAG_BOX_EXT_SIZE] = {0};
    if (exists()) {
        reshape_ext[0] = ext0();
        reshape_ext[1] = ext1();
        reshape_ext[2] = ext2();
        reshape_ext[3] = ext3();
    }
    return _resize_vargs(type, reshape_device, reshape_ext, rank, ap);
}

Err Box::_resize_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
{
    __create_if_not_exists();
    return _base->resize_dims(type, device, ext, rank, dims);
}

Err Box::_resize_dims(btype type, ui32 rank, ui32 const * dims)
{
    btype reshape_device = device_cpu();
    ui64 reshape_ext[TBAG_BOX_EXT_SIZE] = {0};
    if (exists()) {
        reshape_ext[0] = ext0();
        reshape_ext[1] = ext1();
        reshape_ext[2] = ext2();
        reshape_ext[3] = ext3();
    }
    return _resize_dims(type, reshape_device, reshape_ext, rank, dims);
}

Err Box::_resize_like(btype type, box_data const * reference_box)
{
    if (reference_box == nullptr) {
        return E_ILLARGS;
    }
    return _resize_dims(type,
                         reference_box->device,
                         reference_box->ext,
                         reference_box->rank,
                         reference_box->dims);
}

Err Box::_resize_like(btype type, Box const & reference_box)
{
    if (!reference_box) {
        return E_ILLARGS;
    }
    return _resize_like(type, reference_box.base());
}

Box Box::astype(btype change_type) const
{
    if (!exists()) {
        return Box(nullptr);
    }
    Box result;
    auto const code = result._resize_like(change_type, *this);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    auto const assign_data_code = result.base()->assign_data(type(), device(), ext(), size(), data());
    if (isFailure(assign_data_code)) {
        return Box(nullptr);
    }
    assert(result.exists());
    if (info_size() >= 1) {
        result.setInfo(info(), info_size());
    }
    result.setOpaque(getOpaque<box_any>());
    result.setOpaqueDeleter(getOpaqueDeleter());
    return result;
}

Err Box::copyFromData(Box const & box)
{
    if (box.exists()) {
        __create_if_not_exists();
        return _base->checked_assign_data(box.type(), box.device(), box.ext(), box.rank(), box.dims(), box.data());
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
        __create_if_not_exists();
        _base->checked_assign_info_buffer(box.info(), box.info_size());
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
    auto result = _base->clone();
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
    if (!exists()) {
        return E_EXPIRED;
    }
    return builder.build(_base.get());
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
    if (!exists()) {
        return E_EXPIRED;
    }
    return parser.parse(buffer, size, _base.get(), computed_size);
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
    if (!exists()) {
        return E_EXPIRED;
    }
    return parser.parseJson(std::string(json, json + size), _base.get());
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

ErrPair<BoxCursor> Box::cursor(box_slice const & slice) const
{
    return cursor(slice.begin, slice.end, slice.step);
}

ErrPair<BoxCursor> Box::cursor(int begin, int end, int step) const
{
    if (!exists()) {
        return E_EXPIRED;
    }
    if (rank() == 0) {
        return E_EINVAL;
    }
    auto const result = _base->init_cursor(0u, begin, end, step);
    return { result.code, result.value };
}

ErrPair<BoxCursor> Box::cursor(int begin, int end) const
{
    return cursor(begin, end, 1);
}

ErrPair<BoxCursor> Box::cursor(int begin) const
{
    return cursor(begin, nop);
}

ErrPair<BoxCursor> Box::cursor() const
{
    return cursor(nop);
}

std::vector<ui32> Box::diffs(box_slice const * slice_begin, box_slice const * slice_end) const
{
    if (!exists()) {
        return {};
    }

    ErrPair<BoxCursor> err_cursor;
    if (slice_begin == slice_end) {
        err_cursor = cursor();
    } else {
        err_cursor = cursor(*slice_begin);
        ++slice_begin;
    }

    std::vector<ui32> result;
    if (err_cursor) {
        result.emplace_back(err_cursor.value.getDiff());
    } else {
        return {};
    }

    while (true) {
        if (slice_begin == slice_end) {
            err_cursor = err_cursor.value.sub();
        } else {
            err_cursor = err_cursor.value.sub(*slice_begin);
            ++slice_begin;
        }
        if (err_cursor) {
            result.emplace_back(err_cursor.value.getDiff());
        } else {
            break;
        }
    }
    return result;
}

std::vector<ui32> Box::diffs(box_slice const * slices, std::size_t size) const
{
    return diffs(slices, slices + size);
}

std::vector<ui32> Box::diffs(std::vector<box_slice> const & slices) const
{
    return diffs(slices.data(), slices.size());
}

std::vector<ui32> Box::diffs() const
{
    return diffs(static_cast<box_slice const *>(nullptr), static_cast<std::size_t>(0u));
}

Err Box::sliceTo(Box & result, box_slice const * slice_begin, box_slice const * slice_end) const
{
    if (!exists()) {
        return E_EXPIRED;
    }
    auto const sliced_dims = diffs(slice_begin, slice_end);
    auto const resize_code = result._resize_dims(type(), device(), ext(), sliced_dims.size(), sliced_dims.data());
    if (isFailure(resize_code)) {
        return resize_code;
    }
    ui32 offset = 0;
    return forEach(slice_begin, slice_end, [&](void const * elem){
        result.base()->set_data(elem, type(), device(), ext(), offset);
        ++offset;
    });
}

Err Box::sliceTo(Box & result, box_slice const * slices, std::size_t size) const
{
    return sliceTo(result, slices, slices + size);
}

Err Box::sliceTo(Box & result, std::vector<box_slice> const & slices) const
{
    return sliceTo(result, slices.data(), slices.size());
}

Box Box::slice(box_slice const * slice_begin, box_slice const * slice_end) const
{
    if (!exists()) {
        return Box(nullptr);
    }
    Box result;
    auto const code = sliceTo(result, slice_begin, slice_end);
    if (isFailure(code)) {
        return Box(nullptr);
    }
    return result;
}

Box Box::slice(box_slice const * slices, std::size_t size) const
{
    return slice(slices, slices + size);
}

Box Box::slice(std::vector<box_slice> const & slices) const
{
    return slice(slices.data(), slices.size());
}

Err Box::zeros()
{
    if (!exists()) {
        return E_EXPIRED;
    }
    if (type() == device_cpu()) {
        ::memset(data(), 0x00, size());
        return E_SUCCESS;
    } else {
        return fill(0);
    }
}

Err Box::ones()
{
    return fill(1);
}

ErrBox Box::comp(Box const & box, BoxCompareMethod m) const
{
    if (!exists() || !box.exists()) {
        return { E_EXPIRED, Box(nullptr) };
    }
    if (type() != box.type()) {
        return { E_INVALID_TYPE, Box(nullptr) };
    }
    if (device() != box.device()) {
        return { E_EXDEV, Box(nullptr) };
    }
    if (!box_ext_is_equals(ext(), box.ext())) {
        return { E_EXDEV, Box(nullptr) };
    }
    if (!box_dim_is_equals(dims(), rank(), box.dims(), box.rank())) {
        return { E_SHAPE, Box(nullptr) };
    }
    Box result;
    auto const resize_code = result.resize<bool>(*this);
    if (isFailure(resize_code)) {
        return { resize_code, Box(nullptr) };
    }
    auto const * base = _base.get();
    auto const comp_code = (base->*m)(box.base(), result.base());
    if (isFailure(comp_code)) {
        return { comp_code, Box(nullptr) };
    }
    return { E_SUCCESS, result };
}

// clang-format off
ErrBox Box::lt(Box const & box) const { return comp(box, &box_data::lt); }
ErrBox Box::le(Box const & box) const { return comp(box, &box_data::le); }
ErrBox Box::gt(Box const & box) const { return comp(box, &box_data::gt); }
ErrBox Box::ge(Box const & box) const { return comp(box, &box_data::ge); }
ErrBox Box::eq(Box const & box) const { return comp(box, &box_data::eq); }
ErrBox Box::ne(Box const & box) const { return comp(box, &box_data::ne); }
// clang-format on

ErrBox Box::comp(btype val_type, bdev val_device, ui64 const * val_ext, void const * val, ValCompareMethod m) const
{
    if (!exists()) {
        return { E_EXPIRED, Box(nullptr) };
    }
    if (type() != val_type) {
        return { E_INVALID_TYPE, Box(nullptr) };
    }
    if (device() != val_device) {
        return { E_EXDEV, Box(nullptr) };
    }
    if (!box_ext_is_equals(ext(), val_ext)) {
        return { E_EXDEV, Box(nullptr) };
    }
    Box result;
    auto const resize_code = result.resize<bool>(*this);
    if (isFailure(resize_code)) {
        return { resize_code, Box(nullptr) };
    }
    auto const * base = _base.get();
    auto const comp_code = (base->*m)(val_type, val_device, val_ext, val, result.base());
    if (isFailure(comp_code)) {
        return { comp_code, Box(nullptr) };
    }
    return { E_SUCCESS, result };
}

// clang-format off
ErrBox Box::lt(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::lt); }
ErrBox Box::le(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::le); }
ErrBox Box::gt(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::gt); }
ErrBox Box::ge(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::ge); }
ErrBox Box::eq(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::eq); }
ErrBox Box::ne(btype t, bdev d, ui64 const * e, void const * v) const { return comp(t, d, e, v, &box_data::ne); }
// clang-format on

bool Box::all() const
{
    if (!exists()) {
        return false;
    }
    return _base->all();
}

bool Box::any() const
{
    if (!exists()) {
        return false;
    }
    return _base->any();
}

std::size_t Box::count() const
{
    if (!exists()) {
        return 0;
    }
    return _base->count();
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

