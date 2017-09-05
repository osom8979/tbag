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

#include <atomic>

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
class TBAG_API ComInitializer : public pattern::Singleton2<ComInitializer>
{
public:
    friend class pattern::Singleton2<ComInitializer>;

private:
    std::atomic_bool _init;

protected:
    ComInitializer();

public:
    virtual ~ComInitializer();

public:
    inline bool isInit() const TBAG_NOEXCEPT_SP_OP(_init.load())
    { return _init.load(); }
};

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LIB_COMINITIALIZER_HPP__

