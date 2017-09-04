/**
 * @file   StFrame.cpp
 * @brief  StFrame class implementation.
 * @author zer0
 * @date   2017-05-28
 */

#include <libtbag/debug/st/StFrame.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstring>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {

StFrame::StFrame() TBAG_NOEXCEPT : _addr(nullptr), line(0), index(0)
{
    clearName();
    clearSource();
}

StFrame::StFrame(void const * addr) TBAG_NOEXCEPT : _addr(addr), line(0), index(0)
{
    clearName();
    clearSource();
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
        // @formatter:off
        _addr = obj._addr;
        std::memcpy(name  , obj.name  ,   NAME_MEM_SIZE);
        std::memcpy(source, obj.source, SOURCE_MEM_SIZE);
        line  = obj.line;
        index = obj.index;
        // @formatter:on
    }
    return *this;
}

StFrame & StFrame::operator =(StFrame && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // @formatter:off
        std::swap(_addr , obj._addr);
        std::swap(name  , obj.name);
        std::swap(source, obj.source);
        std::swap(line  , obj.line);
        std::swap(index , obj.index);
        // @formatter:on
    }
    return *this;
}

void StFrame::clearName()
{
    std::memset(name, 0x00, NAME_MEM_SIZE);
}

void StFrame::clearSource()
{
    std::memset(source, 0x00, SOURCE_MEM_SIZE);
}

std::string StFrame::toAddressString() const
{
    return string::convertAddressHexStringToString(string::convertAddressToHexString(_addr));
}

std::string StFrame::toString() const
{
    if (*source == '\0' && *name == '\0') {
        return toAddressString();
    }
    return toAddressString() + " " + source + ":" + std::to_string(line) + " [" + name + "]";
}

} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

