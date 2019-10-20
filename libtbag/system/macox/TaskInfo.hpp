/**
 * @file   TaskInfo.hpp
 * @brief  TaskInfo class prototype.
 * @author zer0
 * @date   2019-10-20
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_MACOX_TASKINFO_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_MACOX_TASKINFO_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/ErrPair.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {
namespace macox  {

struct TaskInfo
{
//    mach_vm_size_t  virtual_size;       /* virtual memory size (bytes) */
//    mach_vm_size_t  resident_size;      /* resident memory size (bytes) */
//    mach_vm_size_t  resident_size_max;  /* maximum resident memory size (bytes) */
//    time_value_t    user_time;          /* total user run time for terminated threads */
//    time_value_t    system_time;        /* total system run time for terminated threads */
//    policy_t        policy;             /* default policy for new threads */
//    integer_t       suspend_count;      /* suspend count for task */
};

using ErrTaskInfo = ErrPair<TaskInfo>;

TBAG_API ErrTaskInfo getTaskInfo(int pid);
TBAG_API std::string getPrintableTextOfTaskInfo(TaskInfo const & state);

} // namespace macox
} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_SYSTEM_MACOX_TASKINFO_HPP__

