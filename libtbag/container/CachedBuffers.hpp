/**
 * @file   CachedBuffers.hpp
 * @brief  CachedBuffers class prototype.
 * @author zer0
 * @date   2017-07-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CACHEDBUFFERS_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CACHEDBUFFERS_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstring>
#include <vector>
#include <algorithm>
#include <utility>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * CachedBuffers class prototype.
 *
 * @author zer0
 * @date   2017-07-05
 */
template <typename ValueType, typename SizeType = std::size_t>
class CachedBuffers
{
public:
    using Value = ValueType;
    using Size  = SizeType;

    using DataV  = std::vector<Value>;
    using DataVV = std::vector<DataV>;
    using SizeV  = std::vector<Size>;

    using  binf  = util::BufferInfo<ValueType      , SizeType>;
    using cbinf  = util::BufferInfo<ValueType const, SizeType>;

    using  binfs = std::vector<binf>;
    using cbinfs = std::vector<cbinf>;

public:
    TBAG_CONSTEXPR static bool const __size_is_nothrow_assignable__ = std::is_nothrow_assignable<Size, Size>::value;

private:
    DataVV _datas;
    SizeV  _sizes;
    Size   _datas_size;

public:
    CachedBuffers() : _datas(), _sizes(), _datas_size()
    {
        // EMPTY.
    }

    CachedBuffers(CachedBuffers const & obj) : CachedBuffers()
    {
        (*this) = obj;
    }

    CachedBuffers(CachedBuffers && obj) : CachedBuffers()
    {
        (*this) = std::move(obj);
    }

    ~CachedBuffers()
    {
        // EMPTY.
    }

public:
    CachedBuffers & operator =(CachedBuffers const & obj)
    {
        if (this != &obj) {
            _datas = obj._datas;
            _sizes = obj._sizes;
            _datas_size = obj._datas_size;
        }
        return *this;
    }

    CachedBuffers & operator =(CachedBuffers && obj)
    {
        if (this != &obj) {
            _datas.swap(obj._datas);
            _sizes.swap(obj._sizes);
            std::swap(_datas_size, obj._datas_size);
        }
        return *this;
    }

public:
    void clear()
    {
        _datas.clear();
        _sizes.clear();
        _datas_size = 0;
    }

    inline Size size() const TBAG_NOEXCEPT_EXPR(__size_is_nothrow_assignable__)
    {
        return _datas_size;
    }

    inline bool empty() const
    {
        return _datas_size == 0;
    }

public:
    inline DataVV const & datas() const TBAG_NOEXCEPT { return _datas; }
    inline SizeV  const & sizes() const TBAG_NOEXCEPT { return _sizes; }

public:
    void assign(binf const * buffer, std::size_t size)
    {
        if (_datas.size() < size) {
            _datas.resize(size);
        }
        if (_sizes.size() < size) {
            _sizes.resize(size);
        }
        _datas_size = size;

        Value const * cursor_buffer = nullptr;
        Size cursor_size = 0;

        for (std::size_t i = 0; i < size; ++i) {
            cursor_buffer = (buffer + i)->buffer;
            cursor_size   = (buffer + i)->size;

            if (_datas[i].size() < cursor_size) {
                _datas[i].resize(cursor_size);
            }
            _sizes[i] = cursor_size;

            std::copy(cursor_buffer, cursor_buffer + cursor_size, _datas[i].begin());
        }
    }

    binfs info()
    {
        binfs result(_datas_size);
        for (std::size_t i = 0; i < _datas_size; ++i) {
            result[i].buffer = &_datas[i][0];
            result[i].size   =  _sizes[i];
        }
        return result;
    }

    cbinfs info() const
    {
        cbinfs result(_datas_size);
        for (std::size_t i = 0; i < _datas_size; ++i) {
            result[i].buffer = &_datas[i][0];
            result[i].size   =  _sizes[i];
        }
        return result;
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CACHEDBUFFERS_HPP__

