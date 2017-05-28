/**
 * @file   FakeStackTrace.hpp
 * @brief  FakeStackTrace class prototype.
 * @author zer0
 * @date   2017-05-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_FAKESTACKTRACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_FAKESTACKTRACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/debug/st/StackTraceInterface.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

/**
 * FakeStackTrace class prototype.
 *
 * @author zer0
 * @date   2017-05-28
 */
class TBAG_API FakeStackTrace : public StackTraceInterface
{
public:
    FakeStackTrace();
    virtual ~FakeStackTrace();

public:
    virtual std::string name() const override;
    virtual std::string source() const override;
    virtual std::size_t line() const override;
};

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_FAKESTACKTRACE_HPP__

