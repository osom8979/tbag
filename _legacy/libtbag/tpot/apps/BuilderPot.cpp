/**
 * @file   BuilderPot.cpp
 * @brief  BuilderPot class implementation.
 * @author zer0
 * @date   2019-06-07
 */

#include <libtbag/tpot/apps/BuilderPot.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace apps {

BuilderPot::BuilderPot()
{
    // EMPTY.
}

BuilderPot::~BuilderPot()
{
    // EMPTY.
}

char const * const BuilderPot::name() TBAG_NOEXCEPT
{
    return "builder";
}

char const * const BuilderPot::remarks() TBAG_NOEXCEPT
{
    return "Builder program";
}

char const * const BuilderPot::help() TBAG_NOEXCEPT
{
    return "Usage: builder";
}

char const * const BuilderPot::getName() const TBAG_NOEXCEPT
{
    return name();
}

char const * const BuilderPot::getRemarks() const TBAG_NOEXCEPT
{
    return remarks();
}

char const * const BuilderPot::getHelp() const TBAG_NOEXCEPT
{
    return help();
}

int BuilderPot::onMain()
{
    return EXIT_SUCCESS;
}

} // namespace apps
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

