/**
 * @file   ComInitializer.hpp
 * @brief  ComInitializer class prototype.
 * @author zer0
 * @date   2017-09-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMINITIALIZER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMINITIALIZER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pattern/Singleton2.hpp>

#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

/**
 * ComInitializer class prototype.
 *
 * @author zer0
 * @date   2017-09-05
 */
class TBAG_API ComInitializer : public libtbag::pattern::Singleton2<ComInitializer>
{
public:
    friend class libtbag::pattern::Singleton2<ComInitializer>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

private:
    mutable Mutex _mutex;
    bool _init;

protected:
    ComInitializer();

public:
    virtual ~ComInitializer();

public:
    bool init();
    void uninit();

public:
    inline bool isInit() const
    { Guard g(_mutex); return _init; }
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMINITIALIZER_HPP__

