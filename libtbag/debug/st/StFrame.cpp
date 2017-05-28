/**
 * @file   StFrame.cpp
 * @brief  StFrame class implementation.
 * @author zer0
 * @date   2017-05-28
 */

#include <libtbag/debug/st/StFrame.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

StFrame::StFrame() TBAG_NOEXCEPT : _addr(nullptr)
{
    // EMPTY.
}

StFrame::StFrame(void const * addr) TBAG_NOEXCEPT : _addr(addr)
{
    // EMPTY.
}

StFrame::StFrame(StFrame const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

StFrame::StFrame(StFrame && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

StFrame::~StFrame()
{
    // EMPTY.
}

StFrame & StFrame::operator =(StFrame const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _addr = obj._addr;
    }
    return *this;
}

StFrame & StFrame::operator =(StFrame && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_addr, obj._addr);
    }
    return *this;
}

std::string StFrame::name() const
{
    return std::string();
}

std::string StFrame::source() const
{
    return std::string();
}

std::size_t StFrame::line() const
{
    return 0;
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

