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
#include <atomic>

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
    using Task = std::function<void(void)>;
    using Flag = std::atomic_bool;

private:
    Flag _cancel;
    Task _cb;

public:
    ReleaseCallback(Task const & cb, bool cancel = false) : _cancel(cancel), _cb(cb)
    {
        // EMPTY.
    }

    ~ReleaseCallback()
    {
        if (!_cancel && _cb) {
            _cb();
        }
    }

public:
    inline void cancel() TBAG_NOEXCEPT_SP_OP(_cancel.store(true))
    {
        _cancel.store(true);
    }

    inline void restore() TBAG_NOEXCEPT_SP_OP(_cancel.store(false))
    {
        _cancel.store(false);
    }
};

} // namespace functional

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FUNCTIONAL_RELEASECALLBACK_HPP__

