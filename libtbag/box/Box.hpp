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
#include <libtbag/box/BoxBase.hpp>
#include <libtbag/box/BoxCursor.hpp>
#include <libtbag/box/BoxIterator.hpp>
#include <libtbag/box/BoxIteratorGenerator.hpp>
#include <libtbag/box/BoxPacket.hpp>
#include <libtbag/box/BoxTraits.hpp>
#include <libtbag/dom/json/JsonUtils.hpp>

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
#include <random>

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
class TBAG_API Box : public BoxBase
{
public:
    using ErrBox = libtbag::ErrPair<Box>;

public:
    using si8   = libtbag::box::si8;
    using si16  = libtbag::box::si16;
    using si32  = libtbag::box::si32;
    using si64  = libtbag::box::si64;

    using ui8   = libtbag::box::ui8;
    using ui16  = libtbag::box::ui16;
    using ui32  = libtbag::box::ui32;
    using ui64  = libtbag::box::ui64;

    using fp32  = libtbag::box::fp32;
    using fp64  = libtbag::box::fp64;

    using c64  = libtbag::box::c64;
    using c128 = libtbag::box::c128;

public:
    TBAG_CONSTEXPR static int const nop = libtbag::box::nop;

public:
    TBAG_CONSTEXPR static btype const type_none() TBAG_NOEXCEPT { return libtbag::box::type_none(); }
    TBAG_CONSTEXPR static btype const type_bool() TBAG_NOEXCEPT { return libtbag::box::type_bool(); }
    TBAG_CONSTEXPR static btype const type_si8 () TBAG_NOEXCEPT { return libtbag::box::type_si8 (); }
    TBAG_CONSTEXPR static btype const type_si16() TBAG_NOEXCEPT { return libtbag::box::type_si16(); }
    TBAG_CONSTEXPR static btype const type_si32() TBAG_NOEXCEPT { return libtbag::box::type_si32(); }
    TBAG_CONSTEXPR static btype const type_si64() TBAG_NOEXCEPT { return libtbag::box::type_si64(); }
    TBAG_CONSTEXPR static btype const type_ui8 () TBAG_NOEXCEPT { return libtbag::box::type_ui8 (); }
    TBAG_CONSTEXPR static btype const type_ui16() TBAG_NOEXCEPT { return libtbag::box::type_ui16(); }
    TBAG_CONSTEXPR static btype const type_ui32() TBAG_NOEXCEPT { return libtbag::box::type_ui32(); }
    TBAG_CONSTEXPR static btype const type_ui64() TBAG_NOEXCEPT { return libtbag::box::type_ui64(); }
    TBAG_CONSTEXPR static btype const type_fp32() TBAG_NOEXCEPT { return libtbag::box::type_fp32(); }
    TBAG_CONSTEXPR static btype const type_fp64() TBAG_NOEXCEPT { return libtbag::box::type_fp64(); }
    TBAG_CONSTEXPR static btype const type_c64 () TBAG_NOEXCEPT { return libtbag::box::type_c64 (); }
    TBAG_CONSTEXPR static btype const type_c128() TBAG_NOEXCEPT { return libtbag::box::type_c128(); }

    TBAG_CONSTEXPR static bdev const device_cpu () TBAG_NOEXCEPT { return libtbag::box::device_cpu (); }
    TBAG_CONSTEXPR static bdev const device_cuda() TBAG_NOEXCEPT { return libtbag::box::device_cuda(); }
    TBAG_CONSTEXPR static bdev const device_cl  () TBAG_NOEXCEPT { return libtbag::box::device_cl  (); }

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
            _base.swap(obj._base);
        }
    }

    inline friend void swap(Box & lh, Box & rh) TBAG_NOEXCEPT
    {
        lh.swap(rh);
    }

private:
    void __create_if_not_exists();

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

    Err _resize_like(btype type, box_data const * reference_box);
    Err _resize_like(btype type, Box const & reference_box);

private:
    template <typename ... Args>
    Err _resize(shape_error_t, Args && ... UNUSED_PARAM(args))
    {
        return E_INACCESSIBLE_BLOCK;
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

    Err _resize(shape_like1_t, btype type, box_data const * reference_box)
    {
        return _resize_like(type, reference_box);
    }

    Err _resize(shape_like2_t, btype type, Box const & reference_box)
    {
        return _resize_like(type, reference_box);
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
                _base->set_data(begin, type, device, ext, offset);
                ++offset;
            }
        }
        return E_SUCCESS;
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
                    _base->set_data(&i2, type, device, ext, offset);
                    ++offset;
                }
            }
        }
        return E_SUCCESS;
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
                        _base->set_data(&i3, type, device, ext, offset);
                        ++offset;
                    }
                }
            }
        }
        return E_SUCCESS;
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
                            _base->set_data(&i4, type, device, ext, offset);
                            ++offset;
                        }
                    }
                }
            }
        }
        return E_SUCCESS;
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
        return *((T*)_base->get_data_ptr_by_offset(i));
    }

    template <typename T>
    inline T const & offset(ui32 i) const
    {
        assert(exists());
        assert(i < size());
        assert(is_btype_equals<T>(type()));
        return *((T*)_base->get_data_ptr_by_offset(i));
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
    Err _get(shape_error_t, T * UNUSED_PARAM(result), Args && ... UNUSED_PARAM(args))
    {
        return E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _get(shape_args1_t, T * out, bdev out_device, ui64 const * out_ext, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return E_EXPIRED;
        }
        return _base->get_data_args((void*)out, get_btype<T>(), out_device, out_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    Err _get(shape_args2_t, T * out, Args ... args) const
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return E_EXPIRED;
        }
        auto const shape_device = device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _base->get_data_args((void*)out, get_btype<T>(), shape_device, shape_ext,
                                    static_cast<ui32>(sizeof...(Args)),
                                    std::forward<Args>(args) ...);
    }

    template <typename T>
    Err _get(shape_dims1_t, T * out, bdev out_device, ui64 const * out_ext,
             ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return E_EXPIRED;
        }
        return _base->get_data_dims((void*)out, get_btype<T>(), out_device, out_ext, index_rank, index_dims);
    }

    template <typename T>
    Err _get(shape_dims2_t, T * out, ui32 index_rank, ui32 const * index_dims) const
    {
        if (!exists()) {
            return E_EXPIRED;
        }
        auto const shape_device = device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _base->get_data_dims((void*)out, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
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
    Err _set(shape_error_t, T const * UNUSED_PARAM(src), Args && ... UNUSED_PARAM(args))
    {
        return E_INACCESSIBLE_BLOCK;
    }

    template <typename T, typename ... Args>
    Err _set(shape_args1_t, T const * src, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        static_assert(static_cast<ui32>(sizeof...(Args)) >= 1u, "At least one Args is required.");
        if (!exists()) {
            return E_EXPIRED;
        }
        return _base->set_data_args((void const *)src, get_btype<T>(), src_device, src_ext,
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
            return E_EXPIRED;
        }
        auto const shape_device = device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _base->set_data_args((void const *)src, get_btype<T>(), shape_device, shape_ext,
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
            return E_EXPIRED;
        }
        return _base->set_data_dims((void const *)src, get_btype<T>(), src_device, src_ext, index_rank, index_dims);
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
            return E_EXPIRED;
        }
        auto const shape_device = device();
        ui64 const shape_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return _base->set_data_dims((void const *)src, get_btype<T>(), shape_device, shape_ext, index_rank, index_dims);
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
    Json::Value toJsonValue(Err * code = nullptr) const;
    bool fromJsonText(std::string const & json, Err * code = nullptr);
    bool fromJsonValue(Json::Value const & json, Err * code = nullptr);

public:
    ErrPair<BoxCursor> cursor(box_slice const & slice) const;
    ErrPair<BoxCursor> cursor(int begin, int end, int step) const;
    ErrPair<BoxCursor> cursor(int begin, int end) const;
    ErrPair<BoxCursor> cursor(int begin) const;
    ErrPair<BoxCursor> cursor() const;

    template <typename SliceIterator, typename Predicated>
    Err forEach(SliceIterator slice_begin, SliceIterator slice_end, Predicated predicated) const
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

    template <typename ContainerT, typename Predicated>
    Err forEach(ContainerT const & slices, Predicated predicated) const
    {
        return forEach(slices.begin(), slices.end(), predicated);
    }

    template <typename Predicated>
    Err forEach(Predicated predicated) const
    {
        return forEach<box_slice const *, Predicated>(nullptr, nullptr, predicated);
    }

    std::vector<ui32> diffs(box_slice const * slice_begin, box_slice const * slice_end) const;
    std::vector<ui32> diffs(box_slice const * slices, std::size_t size) const;
    std::vector<ui32> diffs(std::vector<box_slice> const & slices) const;
    std::vector<ui32> diffs() const;

    TBAG_CONSTEXPR static char const * const SLICE_ARGUMENT_DELIMITER = ":";
    TBAG_CONSTEXPR static char const * const SLICE_DELIMITER = ",";

    /**
     * Example:
     * @code{.cpp}
     *  auto slices = Box::parseSliceText("::, 1:2:, ::-3, 4, 5:6")
     * @endcode
     */
    static std::vector<box_slice> parseSliceText(
            std::string const & slice_text,
            std::string const & argument_delimiter = SLICE_ARGUMENT_DELIMITER,
            std::string const & slice_delimiter = SLICE_DELIMITER);
    static std::string convertSliceText(
            box_slice const & slice,
            std::string const & argument_delimiter = SLICE_ARGUMENT_DELIMITER);
    static std::string convertSliceText(
            std::vector<box_slice> const & slices,
            std::string const & argument_delimiter = SLICE_ARGUMENT_DELIMITER,
            std::string const & slice_delimiter = SLICE_DELIMITER);

    Err sliceTo(Box & result, box_slice const * slice_begin, box_slice const * slice_end) const;
    Err sliceTo(Box & result, box_slice const * slices, std::size_t size) const;
    Err sliceTo(Box & result, std::vector<box_slice> const & slices) const;
    Err sliceTo(Box & result, std::string const & slice_text,
                std::string const & argument_delimiter = SLICE_ARGUMENT_DELIMITER,
                std::string const & slice_delimiter = SLICE_DELIMITER) const;

    Box slice(box_slice const * slice_begin, box_slice const * slice_end) const;
    Box slice(box_slice const * slices, std::size_t size) const;
    Box slice(std::vector<box_slice> const & slices) const;
    Box slice(std::string const & slice_text,
              std::string const & argument_delimiter = SLICE_ARGUMENT_DELIMITER,
              std::string const & slice_delimiter = SLICE_DELIMITER) const;

public:
    template <typename T>
    Err fill(T const * value, bdev src_device, ui64 const * src_ext)
    {
        if (!exists()) {
            return E_EXPIRED;
        }
        return forEach([&](void * data){
            _base->set_data(value, get_btype<T>(), src_device, src_ext, data);
        });
    }

    template <typename T>
    Err fill(T const & value, bdev src_device, ui64 const * src_ext)
    {
        return fill(&value, src_device, src_ext);
    }

    template <typename T>
    Err fill(T const * value)
    {
        auto const src_device = device();
        ui64 const src_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return fill(value, src_device, src_ext);
    }

    template <typename T>
    Err fill(T const & value)
    {
        return fill(&value);
    }

public:
    Err zeros();
    Err ones();

public:
    template <typename ... Args>
    static Box zeros(btype type, Args ... args)
    {
        auto box = array(type, std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.zeros())) {
            return Box(nullptr);
        }
        return box;
    }

    template <typename T, typename ... Args>
    static Box zeros(Args ... args)
    {
        return zeros(get_btype<T>(), std::forward<Args>(args) ...);
    }

    template <typename ... Args>
    static Box ones(btype type, Args ... args)
    {
        auto box = array(type, std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.ones())) {
            return Box(nullptr);
        }
        return box;
    }

    template <typename T, typename ... Args>
    static Box ones(Args ... args)
    {
        return ones(get_btype<T>(), std::forward<Args>(args) ...);
    }

    template <typename ValT, typename ... Args>
    static Box full(ValT const * value, btype type, Args ... args)
    {
        auto box = array(type, std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.fill(value))) {
            return Box(nullptr);
        }
        return box;
    }

    template <typename ValT, typename ... Args>
    static Box full(ValT const & value, btype type, Args ... args)
    {
        auto box = array(type, std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.fill(value))) {
            return Box(nullptr);
        }
        return box;
    }

    template <typename BaseT, typename ValT, typename ... Args>
    static Box full(ValT const * value, Args ... args)
    {
        return full(value, get_btype<BaseT>(), std::forward<Args>(args) ...);
    }

    template <typename BaseT, typename ValT, typename ... Args>
    static Box full(ValT const & value, Args ... args)
    {
        return full(value, get_btype<BaseT>(), std::forward<Args>(args) ...);
    }

public:
    template <typename RangeT, typename EngineT = std::mt19937>
    Err rand(RangeT range, EngineT engine, bdev src_device, ui64 const * src_ext)
    {
        if (!exists()) {
            return E_EXPIRED;
        }
        return forEach([&](void * data){
            auto src = range(engine);
            _base->set_data(&src, get_btype<decltype(src)>(), src_device, src_ext, data);
        });
    }

    template <typename RangeT, typename EngineT = std::mt19937>
    Err rand(RangeT range, EngineT engine)
    {
        auto const src_device = device();
        ui64 const src_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return rand(range, engine, src_device, src_ext);
    }

    template <typename T,
              typename RangeT = typename std::conditional<
                      std::is_floating_point<T>::value,
                      typename std::uniform_real_distribution<T>,
                      typename std::uniform_int_distribution<T>
              >::type,
              typename DeviceT = std::random_device,
              typename EngineT = std::mt19937>
    Err rand(T start, T end, bdev src_device, ui64 const * src_ext)
    {
        return rand(RangeT(start, end), EngineT(DeviceT()()), src_device, src_ext);
    }

    template <typename T>
    Err rand(T start, T end)
    {
        auto const src_device = device();
        ui64 const src_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return rand(start, end, src_device, src_ext);
    }

public:
    template <typename T, typename ... Args>
    static Box rand(T start, T end, btype type, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        auto box = array(type, std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.rand(start, end))) {
            return Box(nullptr);
        }
        return box;
    }

    template <typename T, typename ... Args>
    static Box rand(T start, T end, bdev src_device, ui64 const * src_ext, Args ... args)
    {
        return rand(start, end, get_btype<T>(), src_device, src_ext, std::forward<Args>(args) ...);
    }

    template <typename T, typename ... Args>
    static Box rand(T start, T end, Args ... args)
    {
        auto box = array<T>(std::forward<Args>(args) ...);
        if (!box) {
            return Box(nullptr);
        }
        if (isFailure(box.rand(start, end))) {
            return Box(nullptr);
        }
        return box;
    }

private:
    using BoxCompareMethod = Err (box_data::*)(box_data const *, box_data *) const;
    ErrBox comp(Box const & box, BoxCompareMethod m) const;

public:
    ErrBox lt(Box const & box) const;
    ErrBox le(Box const & box) const;
    ErrBox gt(Box const & box) const;
    ErrBox ge(Box const & box) const;
    ErrBox eq(Box const & box) const;
    ErrBox ne(Box const & box) const;

    Box operator < (Box const & box) const { return lt(box).value; }
    Box operator <=(Box const & box) const { return le(box).value; }
    Box operator > (Box const & box) const { return gt(box).value; }
    Box operator >=(Box const & box) const { return ge(box).value; }
    Box operator ==(Box const & box) const { return eq(box).value; }
    Box operator !=(Box const & box) const { return ne(box).value; }

private:
    using ValCompareMethod = Err (box_data::*)(btype, bdev, ui64 const *, void const *, box_data *) const;
    ErrBox comp(btype val_type, bdev val_device, ui64 const * val_ext, void const * val, ValCompareMethod m) const;

public:
    ErrBox lt(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;
    ErrBox le(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;
    ErrBox gt(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;
    ErrBox ge(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;
    ErrBox eq(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;
    ErrBox ne(btype val_type, bdev val_device, ui64 const * val_ext, void const * val) const;

    template <typename T> ErrBox lt(bdev d, ui64 const * e, T const * v) const { return lt(get_btype<T>(), d, e, v); }
    template <typename T> ErrBox le(bdev d, ui64 const * e, T const * v) const { return le(get_btype<T>(), d, e, v); }
    template <typename T> ErrBox gt(bdev d, ui64 const * e, T const * v) const { return gt(get_btype<T>(), d, e, v); }
    template <typename T> ErrBox ge(bdev d, ui64 const * e, T const * v) const { return ge(get_btype<T>(), d, e, v); }
    template <typename T> ErrBox eq(bdev d, ui64 const * e, T const * v) const { return eq(get_btype<T>(), d, e, v); }
    template <typename T> ErrBox ne(bdev d, ui64 const * e, T const * v) const { return ne(get_btype<T>(), d, e, v); }

    template <typename T>
    ErrBox lt(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return lt(val_device, val_ext, v);
    }

    template <typename T>
    ErrBox le(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return le(val_device, val_ext, v);
    }

    template <typename T>
    ErrBox gt(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return gt(val_device, val_ext, v);
    }

    template <typename T>
    ErrBox ge(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return ge(val_device, val_ext, v);
    }

    template <typename T>
    ErrBox eq(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return eq(val_device, val_ext, v);
    }

    template <typename T>
    ErrBox ne(T const * v) const
    {
        auto const val_device = device();
        ui64 const val_ext[TBAG_BOX_EXT_SIZE] = { ext0(), ext1(), ext2(), ext3() };
        return ne(val_device, val_ext, v);
    }

    template <typename T> ErrBox lt(T const & v) const { return lt(&v); }
    template <typename T> ErrBox le(T const & v) const { return le(&v); }
    template <typename T> ErrBox gt(T const & v) const { return gt(&v); }
    template <typename T> ErrBox ge(T const & v) const { return ge(&v); }
    template <typename T> ErrBox eq(T const & v) const { return eq(&v); }
    template <typename T> ErrBox ne(T const & v) const { return ne(&v); }

    template <typename T> Box operator < (T const & v) const { return lt(v).value; }
    template <typename T> Box operator <=(T const & v) const { return le(v).value; }
    template <typename T> Box operator > (T const & v) const { return gt(v).value; }
    template <typename T> Box operator >=(T const & v) const { return ge(v).value; }
    template <typename T> Box operator ==(T const & v) const { return eq(v).value; }
    template <typename T> Box operator !=(T const & v) const { return ne(v).value; }

public:
    bool all() const;
    bool any() const;
    std::size_t count() const;

    static bool all(Box const & box)
    { return box.all(); }
    static bool any(Box const & box)
    { return box.any(); }
    static std::size_t count(Box const & box)
    { return box.count(); }
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

