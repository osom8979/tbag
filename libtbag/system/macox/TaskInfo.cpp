/**
 * @file   TaskInfo.cpp
 * @brief  TaskInfo class implementation.
 * @author zer0
 * @date   2019-10-20
 */

#include <libtbag/system/macox/TaskInfo.hpp>

#if defined(TBAG_PLATFORM_MACOS)
#include <mach/mach.h>
#endif

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace system {
namespace macox  {

ErrTaskInfo getTaskInfo(int pid)
{
    return E_ENOSYS;
}

std::string getPrintableTextOfTaskInfo(TaskInfo const & state)
{
    return {};
}

} // namespace macox
} // namespace system

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

