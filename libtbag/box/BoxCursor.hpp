/**
 * @file   BoxCursor.hpp
 * @brief  BoxCursor class prototype.
 * @author zer0
 * @date   2019-12-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCURSOR_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCURSOR_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/box/BoxTraits.hpp>
#include <libtbag/box/BoxIterator.hpp>
#include <libtbag/box/BoxIteratorGenerator.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

/**
 * BoxCursor class prototype.
 *
 * @author zer0
 * @date   2019-12-16
 * @date   2019-12-30 (Rename: Cursor -> BoxCursor)
 */
class TBAG_API BoxCursor
{
private:
    box_cursor _cursor;

public:
    BoxCursor() TBAG_NOEXCEPT;
    BoxCursor(box_cursor const & c) TBAG_NOEXCEPT;
    BoxCursor(box_cursor && c) TBAG_NOEXCEPT;
    BoxCursor(BoxCursor const & obj) TBAG_NOEXCEPT;
    BoxCursor(BoxCursor && obj) TBAG_NOEXCEPT;
    ~BoxCursor();

public:
    BoxCursor & operator =(BoxCursor const & obj) TBAG_NOEXCEPT;
    BoxCursor & operator =(BoxCursor && obj) TBAG_NOEXCEPT;

public:
    ErrPair<BoxCursor> sub(box_slice const & slice) const;
    ErrPair<BoxCursor> sub(int begin_index, int end_index, int step_index) const;
    ErrPair<BoxCursor> sub(int begin_index, int end_index) const;
    ErrPair<BoxCursor> sub(int begin_index) const;
    ErrPair<BoxCursor> sub() const;

public:
    void * getBegin() TBAG_NOEXCEPT;
    void const * getBegin() const TBAG_NOEXCEPT;

public:
    void * getEnd() TBAG_NOEXCEPT;
    void const * getEnd() const TBAG_NOEXCEPT;

public:
    int getStrideByte() const TBAG_NOEXCEPT;
    int getDiff() const TBAG_NOEXCEPT;

public:
    bool isLastDimension() const TBAG_NOEXCEPT;
    bool isContinue() const TBAG_NOEXCEPT;
    bool isEnd() const TBAG_NOEXCEPT;

public:
    bool next() TBAG_NOEXCEPT;

public:
    template <typename T>
    inline T * data() TBAG_NOEXCEPT
    {
        return static_cast<T*>(_cursor.begin);
    }

    template <typename T>
    inline T const * data() const TBAG_NOEXCEPT
    {
        return static_cast<T const *>(_cursor.begin);
    }

    inline void * data() TBAG_NOEXCEPT
    {
        return _cursor.begin;
    }

    inline void const * data() const TBAG_NOEXCEPT
    {
        return _cursor.begin;
    }

public:
    template <typename T>
    T & at()
    {
        assert(_cursor.box != nullptr);
        assert(is_btype_equals<T>(_cursor.box->type));
        return *((T*)_cursor.begin);
    }

    template <typename T>
    T const & at() const
    {
        assert(_cursor.box != nullptr);
        assert(is_btype_equals<T>(_cursor.box->type));
        return *((T*)_cursor.begin);
    }

public:
    template <typename T>
    Err get(T * out, bdev out_device, ui64 const * out_ext) const
    {
        assert(_cursor.box != nullptr);
        return _cursor.box->get_data(out, get_btype<T>(), out_device, out_ext, _cursor.begin);
    }

    template <typename T>
    Err get(T * out) const
    {
        using namespace libtbag::box::details;
        assert(_cursor.box != nullptr);
        auto const out_device = _cursor.box->device == BD_NONE ? BD_CPU : _cursor.box->device;
        ui64 const out_ext[TBAG_BOX_EXT_SIZE] = { _cursor.box->ext[0], _cursor.box->ext[1],
                                                  _cursor.box->ext[2], _cursor.box->ext[3] };
        return _cursor.box->get_data(out, get_btype<T>(), out_device, out_ext, _cursor.begin);
    }

    template <typename T>
    T get(bdev out_device, ui64 const * out_ext) const
    {
        T out = T();
        get(&out, out_device, out_ext);
        return out;
    }

    template <typename T>
    T get() const
    {
        T out = T();
        get(&out);
        return out;
    }

public:
    template <typename T>
    Err set(T const * src, bdev src_device, ui64 const * src_ext)
    {
        assert(_cursor.box != nullptr);
        return _cursor.box->set_data(src, get_btype<T>(), src_device, src_ext, _cursor.begin);
    }

    template <typename T>
    Err set(T const & src, bdev src_device, ui64 const * src_ext)
    {
        return set((T const *)&src, src_device, src_ext);
    }

    template <typename T>
    Err set(T const * src)
    {
        using namespace libtbag::box::details;
        assert(_cursor.box != nullptr);
        auto const src_device = _cursor.box->device == BD_NONE ? BD_CPU : _cursor.box->device;
        ui64 const src_ext[TBAG_BOX_EXT_SIZE] = { _cursor.box->ext[0], _cursor.box->ext[1],
                                                  _cursor.box->ext[2], _cursor.box->ext[3] };
        return _cursor.box->set_data(src, get_btype<T>(), src_device, src_ext, _cursor.begin);
    }

    template <typename T>
    Err set(T const & src)
    {
        return set((T const *)&src);
    }

public:
    template <typename T>
    BoxIteratorGenerator<T> itr()
    {
        assert(is_btype_equals<T>(_cursor.box->type));
        return BoxIteratorGenerator<T>((T*)getBegin(), (T*)getEnd(), getStrideByte());
    }

    template <typename T>
    BoxIteratorGenerator<T const> itr() const
    {
        assert(is_btype_equals<T>(_cursor.box->type));
        return BoxIteratorGenerator<T const>((T const *)getBegin(), (T const *)getEnd(), getStrideByte());
    }

private:
    template <typename T>
    void * _access(access_unknown_t)
    {
        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return data();
    }

    template <typename T>
    T const * _access(access_const_ptr_t) const
    {
        return data<T>();
    }

    template <typename T>
    T /* const & // Obtain only */ _access(access_const_ref_t) const
    {
        return get<T>();
    }

    template <typename T>
    T * _access(access_ptr_t)
    {
        return data<T>();
    }

    template <typename T>
    T & _access(access_ref_t)
    {
        return at<T>();
    }

public:
    template <typename Predicated>
    Err forEach(box_slice const * slice_begin, box_slice const * slice_end, Predicated predicated)
    {
        if (isLastDimension()) {
            using __arg0 = typename libtbag::function_traits<Predicated>::template arguments<0>::type;
            using __type = typename libtbag::remove_cpr<__arg0>::type;
            do {
                predicated(_access<__type>(access_selector<__arg0>::value));
            } while (next());
        } else {
            do {
                ErrPair<BoxCursor> err_cursor;
                if (slice_begin == slice_end) {
                    err_cursor = sub();
                } else {
                    err_cursor = sub(*slice_begin);
                    ++slice_begin;
                }
                if (!err_cursor) {
                    return err_cursor.code;
                }
                auto const code = err_cursor.value.forEach(slice_begin, slice_end, predicated);
                if (isFailure(code)) {
                    return code;
                }
            } while (next());
        }
        return libtbag::E_SUCCESS;
    }

    template <typename T, typename Predicated>
    Err forEach(box_slice const * slice, std::size_t size, Predicated predicated)
    {
        return forEach(slice, slice + size, predicated);
    }

    template <typename T, typename Predicated>
    Err forEach(Predicated predicated)
    {
        return forEach(static_cast<box_slice const *>(nullptr), static_cast<std::size_t>(0u), predicated);
    }
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXCURSOR_HPP__

