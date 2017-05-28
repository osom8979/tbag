/**
 * @file   FakeStackTrace.cpp
 * @brief  FakeStackTrace class implementation.
 * @author zer0
 * @date   2017-05-28
 */

#include <libtbag/debug/st/FakeStackTrace.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

FakeStackTrace::FakeStackTrace()
{
    // EMPTY.
}

FakeStackTrace::~FakeStackTrace()
{
    // EMPTY.
}

std::string FakeStackTrace::name() const
{
    return std::string();
}

std::string FakeStackTrace::source() const
{
    return std::string();
}

std::size_t FakeStackTrace::line() const
{
    return 0U;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

