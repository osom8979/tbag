/**
 * @file   TaskManager.hpp
 * @brief  TaskManager class prototype.
 * @author zer0
 * @date   2019-12-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
// Include preprocessor.

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace task {

// Forward declaration.

/**
 * TaskManager class prototype.
 *
 * @author zer0
 * @date   2019-12-19
 */
class TBAG_API TaskManager : private Noncopyable
{
private:
    // Insert member variables.

public:
    TaskManager();
    virtual ~TaskManager();
};

} // namespace task

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TASK_TASKMANAGER_HPP__

