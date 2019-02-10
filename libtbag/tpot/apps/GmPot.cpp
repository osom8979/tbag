/**
 * @file   GmPot.cpp
 * @brief  GmPot class implementation.
 * @author zer0
 * @date   2019-01-30
 */

#include <libtbag/tpot/apps/GmPot.hpp>
#include <libtbag/log/Log.hpp>

#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

GmPot::GmPot()
{
    // EMPTY.
}

GmPot::~GmPot()
{
    // EMPTY.
}

char const * const GmPot::getName() const TBAG_NOEXCEPT
{
    return "gm";
}

char const * const GmPot::getRemarks() const TBAG_NOEXCEPT
{
    return "Game Maker";
}

int GmPot::onMain()
{
    return run("");
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

