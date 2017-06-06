/**
 * @file   MessageQueue.hpp
 * @brief  MessageQueue class prototype.
 * @author zer0
 * @date   2017-06-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_IPC_MESSAGEQUEUE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_IPC_MESSAGEQUEUE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ipc {

/**
 * MessageQueue class prototype.
 *
 * @author zer0
 * @date   2017-06-06
 */
class TBAG_API MessageQueue : private Noncopyable
{
private:
    // Insert member variables.

public:
    MessageQueue();
    virtual ~MessageQueue();
};

} // namespace ipc

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_IPC_MESSAGEQUEUE_HPP__

