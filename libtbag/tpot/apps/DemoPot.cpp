/**
 * @file   DemoPot.cpp
 * @brief  DemoPot class implementation.
 * @author zer0
 * @date   2019-04-06
 */

#include <libtbag/tpot/apps/DemoPot.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>

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

char const * const DemoPot::getName() const TBAG_NOEXCEPT
{
    return "demo";
}

char const * const DemoPot::getRemarks() const TBAG_NOEXCEPT
{
    return "Play Demo Game";
}

char const * const DemoPot::getHelp() const TBAG_NOEXCEPT
{
    return "";
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

