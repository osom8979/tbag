/**
 * @file   DemoPot.cpp
 * @brief  DemoPot class implementation.
 * @author zer0
 * @date   2019-06-05
 */

#include <libtbag/tpot/apps/DemoPot.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

DemoPot::DemoPot()
{
    // EMPTY.
}

DemoPot::~DemoPot()
{
    // EMPTY.
}

char const * const DemoPot::name() TBAG_NOEXCEPT
{
    return "demo";
}

char const * const DemoPot::remarks() TBAG_NOEXCEPT
{
    return "Demo Program";
}

char const * const DemoPot::help() TBAG_NOEXCEPT
{
    return "Usage: demo";
}

char const * const DemoPot::getName() const TBAG_NOEXCEPT
{
    return name();
}

char const * const DemoPot::getRemarks() const TBAG_NOEXCEPT
{
    return remarks();
}

char const * const DemoPot::getHelp() const TBAG_NOEXCEPT
{
    return help();
}

int DemoPot::onMain()
{
    return EXIT_SUCCESS;
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

