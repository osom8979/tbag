/**
 * @file   SafetyPrepareQueue.hpp
 * @brief  SafetyPrepareQueue class prototype.
 * @author zer0
 * @date   2016-08-04
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * SafetyPrepareQueue class prototype.
 *
 * @author zer0
 * @date   2016-08-04
 */
template <typename ValueType>
class SafetyPrepareQueue : public Noncopyable
{
public:
    using Value = ValueType;

public:
    SafetyPrepareQueue()
    {
    }

    ~SafetyPrepareQueue()
    {
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_SAFETYPREPAREQUEUE_HPP__

