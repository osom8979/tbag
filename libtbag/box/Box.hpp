/**
 * @file   Box.hpp
 * @brief  Box class prototype.
 * @author zer0
 * @date   2019-05-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/box/details/box_api.hpp>
#include <libtbag/box/BoxCursor.hpp>
#include <libtbag/box/BoxIterator.hpp>
#include <libtbag/box/BoxIteratorGenerator.hpp>
#include <libtbag/box/BoxPacket.hpp>
#include <libtbag/box/BoxTraits.hpp>

#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cassert>

#include <type_traits>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

/**
 * Box class prototype.
 *
 * Containers:
 * <table>
 *   <tr>
 *     <th>NumPy</th>
 *     <th>Box</th>
 *   </tr>
 *   <tr>
 *     <td><code>a = np.array([[1, 2], [3, 4], [5, 6]])</code></td>
 *     <td><code>Box b = {{1, 2}, {3, 4}, {5, 6}};</code></td>
 *   </tr>
 *   <tr>
 *     <td><code>a.resize([2, 3])</code></td>
 *     <td><code>a.resize(2, 3)</code></td>
 *   </tr>
 *   <tr>
 *     <td><code>a.astype(np.double)</code></td>
 *     <td><code>b.astype<double>()</code></td>
 *   </tr>
 * </table>
 *
 * @author zer0
 * @date   2019-05-16
 */
class TBAG_API Box
{
public:
    using Buffer = std::vector<ui8>;

    using size_type = ui32;
    using difference_type = std::ptrdiff_t;

    using SharedBoxData = std::shared_ptr<box_data>;

private:
    SharedBoxData _data;

public:
    Box();
    explicit Box(std::nullptr_t) TBAG_NOEXCEPT;
    explicit Box(box_data && data) TBAG_NOEXCEPT;
    Box(Box const & obj) TBAG_NOEXCEPT;
    Box(Box && obj) TBAG_NOEXCEPT;
    ~Box();

public:
    template <typename T>
    Box(std::initializer_list<T> const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list<T> > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list<T> > > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    template <typename T>
    Box(std::initializer_list< std::initializer_list< std::initializer_list< std::initializer_list<T> > > > const & items) : Box()
    {
        auto const code = assign<T>(items);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

public:
    Box & operator =(Box const & obj) TBAG_NOEXCEPT;
    Box & operator =(Box && obj) TBAG_NOEXCEPT;
    Box & operator =(std::nullptr_t) TBAG_NOEXCEPT;

    inline void swap(Box & obj) TBAG_NOEXCEPT
    {
        if (this != &obj) {
            _data.swap(obj._data);
        }
    }

    inline friend void swap(Box & lh, Box & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

public:
    inline bool exists() const TBAG_NOEXCEPT
    {
        return static_cast<bool>(_data);
    }

    inline operator bool() const TBAG_NOEXCEPT
    {
        return exists();
    }

    void reset() TBAG_NOEXCEPT_SP_OP(_data.reset())
    {
        _data.reset();
    }

public:
    inline box_data       * base()       TBAG_NOEXCEPT { return _data.get(); }
    inline box_data const * base() const TBAG_NOEXCEPT { return _data.get(); }

public:
    inline btype getType() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->type;
        }
        return type_none();
    }

    inline btype type() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->type;
    }

    inline bool isSupportType() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_type(getType());
    }

    inline char const * getTypeName() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_name(getType());
    }

    inline ui32 getTypeByte() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_type_byte(getType());
    }

    // clang-format off
    inline bool is_none() const TBAG_NOEXCEPT { return getType() == type_none(); }
    inline bool is_si8 () const TBAG_NOEXCEPT { return getType() == type_si8 (); }
    inline bool is_si16() const TBAG_NOEXCEPT { return getType() == type_si16(); }
    inline bool is_si32() const TBAG_NOEXCEPT { return getType() == type_si32(); }
    inline bool is_si64() const TBAG_NOEXCEPT { return getType() == type_si64(); }
    inline bool is_ui8 () const TBAG_NOEXCEPT { return getType() == type_ui8 (); }
    inline bool is_ui16() const TBAG_NOEXCEPT { return getType() == type_ui16(); }
    inline bool is_ui32() const TBAG_NOEXCEPT { return getType() == type_ui32(); }
    inline bool is_ui64() const TBAG_NOEXCEPT { return getType() == type_ui64(); }
    inline bool is_fp32() const TBAG_NOEXCEPT { return getType() == type_fp32(); }
    inline bool is_fp64() const TBAG_NOEXCEPT { return getType() == type_fp64(); }
    // clang-format on

    inline bool is_signed() const TBAG_NOEXCEPT
    {
        return is_si8() || is_si16() || is_si32() || is_si64();
    }

    inline bool is_unsigned() const TBAG_NOEXCEPT
    {
        return is_ui8() || is_ui16() || is_ui32() || is_ui64();
    }

    inline bool is_floating() const TBAG_NOEXCEPT
    {
        return is_fp32() || is_fp64();
    }

public:
    inline btype getDevice() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->device;
        }
        return device_none();
    }

    inline btype device() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->device;
    }

    inline bool isSupportDevice() const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_support_device(getDevice());
    }

    inline char const * getDeviceName(bdev dev) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_get_device_name(getType());
    }

    // clang-format off
    inline bool is_device_none() const TBAG_NOEXCEPT { return getDevice() == device_none(); }
    inline bool is_device_cpu () const TBAG_NOEXCEPT { return getDevice() == device_cpu (); }
    inline bool is_device_cuda() const TBAG_NOEXCEPT { return getDevice() == device_cuda(); }
    inline bool is_device_cl  () const TBAG_NOEXCEPT { return getDevice() == device_cl  (); }
    // clang-format on

public:
    inline ui64 const * getExtensions() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->ext;
        }
        return nullptr;
    }

    inline ui64 const * ext() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->ext;
    }

    // clang-format off
    inline ui64 getExtension0() const TBAG_NOEXCEPT { if (_data) { return _data->ext[0]; } return 0; }
    inline ui64 getExtension1() const TBAG_NOEXCEPT { if (_data) { return _data->ext[1]; } return 0; }
    inline ui64 getExtension2() const TBAG_NOEXCEPT { if (_data) { return _data->ext[2]; } return 0; }
    inline ui64 getExtension3() const TBAG_NOEXCEPT { if (_data) { return _data->ext[3]; } return 0; }
    inline ui64 ext0() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[0]; }
    inline ui64 ext1() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[1]; }
    inline ui64 ext2() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[2]; }
    inline ui64 ext3() const TBAG_NOEXCEPT { assert(exists()); return _data->ext[3]; }
    // clang-format on

    inline bool testExtensions(ui64 e0, ui64 e1, ui64 e2, ui64 e3) const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->ext[0] == e0 &&
                   _data->ext[1] == e1 &&
                   _data->ext[2] == e2 &&
                   _data->ext[3] == e3;
        }
        return false;
    }

public:
    template <typename T>
    inline void * getData() TBAG_NOEXCEPT
    {
        if (_data) {
            return static_cast<T*>(_data->data);
        }
        return nullptr;
    }

    template <typename T>
    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_data) {
            return static_cast<T*>(_data->data);
        }
        return nullptr;
    }

    inline void * getData() TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->data;
        }
        return nullptr;
    }

    inline void const * getData() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->data;
        }
        return nullptr;
    }

    template <typename T>
    inline T * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T*>(_data->data);
    }

    template <typename T>
    inline T const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return static_cast<T const *>(_data->data);
    }

    inline void * data() TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->data;
    }

    inline void const * data() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->data;
    }

    inline ui32 getTotalDataByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_data_byte;
        }
        return 0;
    }

    inline ui32 total_data_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_data_byte;
    }

    inline ui32 getCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDataByte();
    }

    inline ui32 capacity() const TBAG_NOEXCEPT
    {
        return total_data_byte();
    }

    inline ui32 getSize() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->size;
        }
        return 0;
    }

    inline ui32 size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->size;
    }

    inline bool isEmpty() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->size == 0;
        }
        return true;
    }

    inline bool empty() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->size == 0;
    }

public:
    inline ui32 const * getDimensions() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->dims;
        }
        return nullptr;
    }

    inline ui32 const * dims() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->dims;
    }

    inline ui32 getDimension(ui32 i) const TBAG_NOEXCEPT
    {
        if (_data && i < _data->rank) {
            return _data->dims[i];
        }
        return 0;
    }

    inline ui32 dim(ui32 i) const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->dims[i];
    }

    inline ui32 getTotalDimsByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_dims_byte;
        }
        return 0;
    }

    inline ui32 total_dims_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_dims_byte;
    }

    inline ui32 getDimensionsCapacity() const TBAG_NOEXCEPT
    {
        return getTotalDimsByte();
    }

    inline ui32 dims_capacity() const TBAG_NOEXCEPT
    {
        return total_dims_byte();
    }

    inline ui32 getRank() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->rank;
        }
        return 0;
    }

    inline ui32 rank() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->rank;
    }

    inline ui32 getStride(ui32 i) const TBAG_NOEXCEPT
    {
        return libtbag::box::details::box_dim_get_stride(getDimensions(), getRank(), i);
    }

    inline ui32 getStrideByte(ui32 i) const TBAG_NOEXCEPT
    {
        return getStride(i) * getTypeByte();
    }

public:
    inline ui8 * getInfo() TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info;
        }
        return nullptr;
    }

    inline ui8 const * getInfo() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info;
        }
        return nullptr;
    }

    inline ui8 * info() TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info;
    }

    inline ui8 const * info() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info;
    }

    inline ui32 getTotalInfoByte() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->total_info_byte;
        }
        return 0;
    }

    inline ui32 total_info_byte() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->total_info_byte;
    }

    inline ui32 getInfoCapacity() const TBAG_NOEXCEPT
    {
        return getTotalInfoByte();
    }

    inline ui32 info_capacity() const TBAG_NOEXCEPT
    {
        return total_info_byte();
    }

    inline ui32 getInfoSize() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->info_size;
        }
        return 0;
    }

    inline ui32 info_size() const TBAG_NOEXCEPT
    {
        assert(exists());
        return _data->info_size;
    }

public:
    inline void setOpaque(void * v) const TBAG_NOEXCEPT
    {
        if (_data) {
            _data->set_opaque(v);
        }
    }

    template <typename T>
    inline void setOpaque(T v) TBAG_NOEXCEPT
    {
        if (_data) {
            _data->set_opaque(v);
        }
    }

    template <typename T>
    inline void getOpaque(T * v) const TBAG_NOEXCEPT
    {
        if (_data && v != nullptr) {
            _data->get_opaque(v);
        }
    }

    template <typename T>
    inline T getOpaque() const TBAG_NOEXCEPT
    {
        T result = T();
        getOpaque(&result);
        return result;
    }

    inline void * getOpaquePointer() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->get_opaque_pointer();
        }
        return nullptr;
    }

    inline box_opaque_delete_cb getOpaqueDeleter() const TBAG_NOEXCEPT
    {
        if (_data) {
            return _data->opaque_deleter;
        }
        return nullptr;
    }

    inline void setOpaqueDeleter(box_opaque_delete_cb cb) TBAG_NOEXCEPT
    {
        if (_data) {
            _data->opaque_deleter = cb;
        }
    }

public:
    inline void clearData() TBAG_NOEXCEPT
    {
        if (_data) {
            _data->size = 0;
            _data->rank = 0;
        }
    }

    inline void clearInfo() TBAG_NOEXCEPT
    {
        if (_data) {
            _data->info_size = 0;
        }
    }

    inline void clear() TBAG_NOEXCEPT
    {
        clearData();
        clearInfo();
    }

public:
    void createIfNotExists();

public:
    void setInfo(ui8 const * info, ui32 size);
    void setInfo(std::string const & info);
    void setInfo(Buffer const & info);

    std::string getInfoString() const;
    Buffer getInfoBuffer() const;

private:
    Err _resize_args(btype type, bdev device, ui64 const * ext, ui32 rank, ...);
    Err _resize_args(btype type, ui32 rank, ...);

    Err _resize_vargs(btype type, bdev device, ui64 const * ext, ui32 rank, va_list ap);
    Err _resize_vargs(btype type, ui32 rank, va_list ap);

    Err _resize_dims(btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims);
    Err _resize_dims(btype type, ui32 rank, ui32 const * dims);

    Err _resize_ref_box(btype type, box_data const * reference_box);
    Err _resize_ref_box(btype type, Box const & reference_box);

private:
    template <typename ... Args>
    Err _resize(shape_unknown_t, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename ... Args>
    Err _resize(shape_args1_t, btype type, bdev device, ui64 const * ext, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize_args(type, device, ext, static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    Err _resize(shape_args2_t, btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize_args(type, static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...);
    }

    Err _resize(shape_dims1_t, btype type, bdev device, ui64 const * ext, ui32 rank, ui32 const * dims)
    {
        return _resize_dims(type, device, ext, rank, dims);
    }

    Err _resize(shape_dims2_t, btype type, ui32 rank, ui32 const * dims)
    {
        return _resize_dims(type, rank, dims);
    }

    Err _resize(shape_ref_box1_t, btype type, box_data const * reference_box)
    {
        return _resize_ref_box(type, reference_box);
    }

    Err _resize(shape_ref_box2_t, btype type, Box const & reference_box)
    {
        return _resize_ref_box(type, reference_box);
    }

public:
    /**
     * Coordinate the {Header}, {Dims}, and {DataSize} sections.
     *
     * @see libtbag::box::details::box_data::resize_dims
     */
    template <typename ... Args>
    Err resize(btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _resize(shape_selector<Args...>::value, type, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err resize(Args ... args)
    {
        return resize(get_btype<T>(), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    static Box array(btype type, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        Box result;
        if (isFailure(result.resize(type, std::forward<Args>(args) ...))) {
            return Box(nullptr);
        }
        return result;
    }

    template <typename T, typename ... Args>
    static Box array(Args ... args)
    {
        return array(get_btype<T>(), std::forward<Args>(args) ...);
    }

public:
    Box astype(btype type) const;

    template <typename T>
    Box astype() const
    {
        return astype(get_btype<T>());
    }

public:
    Err copyFromData(Box const & box);
    Err copyToData(Box & box) const;

public:
    Err copyFromInfo(Box const & box);
    Err copyToInfo(Box & box) const;

public:
    Err copyFrom(Box const & box);
    Err copyTo(Box & box) const;

public:
    Box clone() const;

public:
    template <typename T>
    using init1d_t = std::initializer_list<T>;
    template <typename T>
    using init2d_t = std::initializer_list< init1d_t<T> >;
    template <typename T>
    using init3d_t = std::initializer_list< init2d_t<T> >;
    template <typename T>
    using init4d_t = std::initializer_list< init3d_t<T> >;

    template <typename T>
    Err assign(bdev device, ui64 const * ext, T const * begin, T const * end)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(std::distance(begin, end));
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 1, dim_1d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (; begin != end; ++begin) {
                *d = static_cast<DataType>(*begin);
                ++d;
            }
        } else {
            ui32 offset = 0;
            for (; begin != end; ++begin) {
                _data->set_data(begin, type, device, ext, offset);
                ++offset;
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(T const * begin, T const * end)
    {
        return assign(device_cpu(), nullptr, begin, end);
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init1d_t<T> const & items)
    {
        return assign(device, ext, items.begin(), items.end());
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init2d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 2, dim_1d, dim_2d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    *d = static_cast<DataType>(i2);
                    ++d;
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    _data->set_data(&i2, type, device, ext, offset);
                    ++offset;
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init3d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const dim_3d = static_cast<ui32>(items.begin()->begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 3, dim_1d, dim_2d, dim_3d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        *d = static_cast<DataType>(i3);
                        ++d;
                    }
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        _data->set_data(&i3, type, device, ext, offset);
                        ++offset;
                    }
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T>
    Err assign(bdev device, ui64 const * ext, init4d_t<T> const & items)
    {
        using DataType = typename libtbag::remove_cr<T>::type;
        auto const dim_1d = static_cast<ui32>(items.size());
        auto const dim_2d = static_cast<ui32>(items.begin()->size());
        auto const dim_3d = static_cast<ui32>(items.begin()->begin()->size());
        auto const dim_4d = static_cast<ui32>(items.begin()->begin()->begin()->size());
        auto const type = get_btype<DataType>();
        auto const code = _resize_args(type, device, ext, 4, dim_1d, dim_2d, dim_3d, dim_4d);
        if (isFailure(code)) {
            return code;
        }

        if (is_device_cpu() && type == device_cpu()) {
            auto * d = data<DataType>();
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        assert(i3.size() == dim_4d);
                        for (auto & i4 : i3) {
                            *d = static_cast<DataType>(i4);
                            ++d;
                        }
                    }
                }
            }
        } else {
            ui32 offset = 0;
            for (auto & i1 : items) {
                assert(i1.size() == dim_2d);
                for (auto & i2 : i1) {
                    assert(i2.size() == dim_3d);
                    for (auto & i3 : i2) {
                        assert(i3.size() == dim_4d);
                        for (auto & i4 : i3) {
                            _data->set_data(&i4, type, device, ext, offset);
                            ++offset;
                        }
                    }
                }
            }
        }
        return libtbag::E_SUCCESS;
    }

    // clang-format off
    template <typename T> Err assign(init1d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init2d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init3d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    template <typename T> Err assign(init4d_t<T> const & items) { return assign(device_cpu(), nullptr, items); }
    // clang-format on

public:
    template <typename T>
    inline T & offset(ui32 i)
    {
        assert(exists());
        assert(i < size());
        assert(is_btype_equals<T>(type()));
        return *((T*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T>
    inline T const & offset(ui32 i) const
    {
        assert(exists());
        assert(i < size());
        assert(is_btype_equals<T>(type()));
        return *((T*)_data->get_data_ptr_by_offset(i));
    }

    template <typename T, typename ... Args>
    inline T & at(Args && ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return offset<T>(libtbag::box::details::box_dim_get_offset_args(
                dims(), static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...));
    }

    template <typename T, typename ... Args>
    inline T const & at(Args && ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return offset<T>(libtbag::box::details::box_dim_get_offset_args(
                dims(), static_cast<ui32>(sizeof...(Args)), std::forward<Args>(args) ...));
    }

private:
    template <typename T, typename ... Args>
    Err _get(shape_unknown_t, T * result, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _get(shape_args1_t, T * out, bdev out_device, ui64 const * out_ext, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->get_data_args((void*)out, get_btype<T>(), out_device, out_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _get(shape_args2_t, T * out, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->get_data_args((void*)out, get_btype<T>(), shape_device, shape_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T>
    Err _get(shape_dims1_t, T * out, bdev out_device, ui64 const * out_ext,
             ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->get_data_dims((void*)out, get_btype<T>(), out_device, out_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _get(shape_dims2_t, T * out, ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->get_data_dims((void*)out, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
    }

public:
    template <typename T, typename ... Args>
    Err get(T * out, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _get<T>(index_selector<Args...>::value, out, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    T get(Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        T out = T();
        _get<T>(index_selector<Args...>::value, &out, std::forward<Args>(args) ...);
        return out;
    }

private:
    template <typename T, typename ... Args>
    Err _set(shape_unknown_t, T const * src, Args && ... args)
    {
        return libtbag::E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _set(shape_args1_t, T const * src, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->set_data_args((void const *)src, get_btype<T>(), src_device, src_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args1_t, T const & src, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        return _set(shape_args1_t{}, (T const *)&src, src_device, src_ext, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args2_t, T const * src, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->set_data_args((void const *)src, get_btype<T>(), shape_device, shape_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _set(shape_args2_t, T const & src, Args ... args)
    {
        return _set(shape_args2_t{}, (T const *)&src, std::forward<Args>(args) ...);
    }

    template <typename T>
    Err _set(shape_dims1_t, T const * src, bdev src_device, ui64 const * src_ext,
             ui32 index_rank, ui32 const * index_dims)
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        return _data->set_data_dims((void const *)src, get_btype<T>(), src_device, src_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims1_t, T const & src, bdev src_device, ui64 const * src_ext,
             ui32 index_rank, ui32 const * index_dims)
    {
        return _set(shape_dims1_t{}, (T const *)&src, src_device, src_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims2_t, T const * src, ui32 index_rank, ui32 const * index_dims)
    {
        if (!exists()) {
            return libtbag::E_EXPIRED;
        }
        auto const shape_device = is_device_none() ? device_cpu() : device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _data->set_data_dims((void const *)src, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _set(shape_dims2_t, T const & src, ui32 index_rank, ui32 const * index_dims)
    {
        return _set(shape_dims2_t{}, (T const *)&src, index_rank, index_dims);
    }

public:
    template <typename T, typename ... Args>
    Err set(T const * src, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        return _set<T>(index_selector<Args...>::value, src, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err set(T const & src, Args ... args)
    {
        return set((T const *)&src, std::forward<Args>(args) ...);
    }

public:
    Err setData(ui8 const * info, ui32 size);
    Err setData(std::string const & info);
    Err setData(Buffer const & info);

public:
    Err getDataString(std::string & result) const;
    std::string getDataString() const;

public:
    using Builder = libtbag::box::BoxPacketBuilder;
    using Parser  = libtbag::box::BoxPacketParser;
    using Packet  = libtbag::box::BoxPacket;

public:
    Err encode(Builder & builder) const;
    Err encode(Builder & builder, Buffer & buffer) const;
    Err encode(Buffer & buffer) const;

public:
    Err decode(void const * buffer, std::size_t size, Parser const & parser, std::size_t * computed_size = nullptr);
    Err decode(void const * buffer, std::size_t size, std::size_t * computed_size = nullptr);
    Err decode(Buffer const & buffer, std::size_t * computed_size = nullptr);

public:
    Err encodeToJson(Builder & builder, std::string & json) const;
    Err encodeToJson(std::string & json) const;

public:
    Err decodeFromJson(char const * json, std::size_t size, Parser const & parser);
    Err decodeFromJson(char const * json, std::size_t size);
    Err decodeFromJson(std::string const & json);

public:
    std::string toJsonText(Err * code = nullptr) const;
    bool fromJsonText(std::string const & json, Err * code = nullptr);

public:
    ErrPair<BoxCursor> cursor(box_slice const & slice) const;
    ErrPair<BoxCursor> cursor(int begin, int end, int step) const;
    ErrPair<BoxCursor> cursor(int begin, int end) const;
    ErrPair<BoxCursor> cursor(int begin) const;
    ErrPair<BoxCursor> cursor() const;

    template <typename Predicated>
    Err forEach(box_slice const * slice_begin, box_slice const * slice_end, Predicated predicated) const
    {
        ErrPair<BoxCursor> err_cursor;
        if (slice_begin == slice_end) {
            err_cursor = cursor();
        } else {
            err_cursor = cursor(*slice_begin);
            ++slice_begin;
        }
        if (!err_cursor) {
            return err_cursor.code;
        }
        return err_cursor.value.forEach(slice_begin, slice_end, predicated);
    }

    template <typename Predicated>
    Err forEach(box_slice const * slices, std::size_t size, Predicated predicated) const
    {
        return forEach(slices, slices + size, predicated);
    }

    template <typename Predicated>
    Err forEach(Predicated predicated) const
    {
        return forEach(static_cast<box_slice const *>(nullptr), static_cast<std::size_t>(0u), predicated);
    }

    std::vector<ui32> diffs(box_slice const * slice_begin, box_slice const * slice_end) const;
    std::vector<ui32> diffs(box_slice const * slices, std::size_t size) const;
    std::vector<ui32> diffs(std::vector<box_slice> const & slices) const;
    std::vector<ui32> diffs() const;

    Box slice(box_slice const * slice_begin, box_slice const * slice_end) const;
    Box slice(box_slice const * slices, std::size_t size) const;
    Box slice(std::vector<box_slice> const & slices) const;
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#include <utility>
#include <functional>

// ------------
namespace std {
// ------------

template <>
struct hash<libtbag::box::Box>
{
    inline std::size_t operator()(libtbag::box::Box const & v) const
    {
        return reinterpret_cast<std::size_t>(v.base());
    }
};

template <>
struct equal_to<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.base() == y.base();
    }
};

template <>
struct less<libtbag::box::Box>
{
    inline bool operator()(libtbag::box::Box const & x, libtbag::box::Box const & y) const
    {
        return x.base() < y.base();
    }
};

// ---------------
} // namespace std
// ---------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOX_HPP__

