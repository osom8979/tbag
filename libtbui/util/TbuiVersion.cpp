/**
 * @file   TbuiVersion.cpp
 * @brief  TbuiVersion class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbui/util/TbuiVersion.hpp>

namespace libtbui {
namespace util    {

int getMajorVersion()
{
#if defined(TBUI_MAJOR_VERSION)
    return TBUI_MAJOR_VERSION;
#else
    return 0;
#endif
}

int getMinorVersion()
{
#if defined(TBUI_MINOR_VERSION)
    return TBUI_MINOR_VERSION;
#else
    return 0;
#endif
}

Version getVersion()
{
    return Version(getMajorVersion(), getMinorVersion());
}

} // namespace util
} // namespace libtbui

