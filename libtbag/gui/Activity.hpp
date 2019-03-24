/**
 * @file   Activity.hpp
 * @brief  Activity class prototype.
 * @author zer0
 * @date   2019-03-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GUI_ACTIVITY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GUI_ACTIVITY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace gui {

/**
 * Activity class prototype.
 *
 * @author zer0
 * @date   2019-03-24
 */
class TBAG_API Activity : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    Activity();
    virtual ~Activity();
};

} // namespace gui

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GUI_ACTIVITY_HPP__

