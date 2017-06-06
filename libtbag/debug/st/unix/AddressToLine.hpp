/**
 * @file   AddressToLine.hpp
 * @brief  AddressToLine class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdio>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace debug {
namespace st    {
namespace unix  {

/**
 * AddressToLine class prototype.
 *
 * @author zer0
 * @date   2017-06-06
 */
class TBAG_API AddressToLine : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    AddressToLine(const char * flag, const char * exec_path, const char * addr);
    virtual ~AddressToLine();

public:
    FILE * getFilePointer() const TBAG_NOEXCEPT;
};

} // namespace unix
} // namespace st
} // namespace debug

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DEBUG_ST_UNIX_ADDRESSTOLINE_HPP__

