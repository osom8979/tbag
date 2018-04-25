/**
 * @file   ComHolder.hpp
 * @brief  ComHolder class prototype.
 * @author zer0
 * @date   2018-04-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMHOLDER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMHOLDER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

/**
 * ComHolder class prototype.
 *
 * @author zer0
 * @date   2018-04-25
 */
template <typename T>
class ComHolder : public Noncopyable
{
public:
    using ComObject = T;

private:
    ComObject * _holder;

public:
    ComHolder() TBAG_NOEXCEPT : _holder(nullptr)
    { /* EMPTY. */ }

    ~ComHolder()
    {
        if (_holder) {
            _holder->Release();
        }
    }

public:
    inline T * operator->() const TBAG_NOEXCEPT
    {
        return _holder;
    }

    inline bool isInited() const TBAG_NOEXCEPT
    {
        return !!_holder;
    }

public:
    inline void ** toVoidPtrPtr() TBAG_NOEXCEPT
    {
        return reinterpret_cast<void**>(&_holder);
    }
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMHOLDER_HPP__

