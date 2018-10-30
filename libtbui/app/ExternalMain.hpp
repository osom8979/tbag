/**
 * @file   ExternalMain.hpp
 * @brief  ExternalMain class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_APP_EXTERNALMAIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_APP_EXTERNALMAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbui/export.h>

namespace libtbui {
namespace app     {

/**
 * ExternalMain class prototype.
 *
 * @author zer0
 * @date   2018-10-30
 */
class TBUI_API ExternalMain : private libtbag::Noncopyable
{
private:
    // Insert member variables.

public:
    ExternalMain();
    virtual ~ExternalMain();
};

} // namespace app
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_APP_EXTERNALMAIN_HPP__

