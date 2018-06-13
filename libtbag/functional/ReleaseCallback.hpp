/**
 * @file   ReleaseCallback.hpp
 * @brief  ReleaseCallback class prototype.
 * @author zer0
 * @date   2018-06-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_RELEASECALLBACK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_RELEASECALLBACK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace functional {

/**
 * ReleaseCallback class prototype.
 *
 * @author zer0
 * @date   2018-06-13
 */
class ReleaseCallback : private Noncopyable
{
public:
    using Callback = std::function<void(void)>;

private:
    bool _cancel;
    Callback _cb;

public:
    ReleaseCallback(Callback const & cb, bool cancel = false) : _cancel(cancel), _cb(cb)
    {
        // EMPTY.
    }

    ~ReleaseCallback()
    {
        if (_cancel == false && _cb) {
            _cb();
        }
    }

public:
    inline void cancel() TBAG_NOEXCEPT
    {
        _cancel = true;
    }

    inline void restore() TBAG_NOEXCEPT
    {
        _cancel = false;
    }
};

} // namespace functional

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_RELEASECALLBACK_HPP__

