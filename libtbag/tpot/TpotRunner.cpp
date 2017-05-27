/**
 * @file   TpotRunner.cpp
 * @brief  TpotRunner class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/TpotRunner.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TpotRunner::TpotRunner(TpotParams const & params) : _params(params)
{
    // EMPTY.
}

TpotRunner::~TpotRunner()
{
    // EMPTY.
}

int TpotRunner::run()
{
    return EXIT_SUCCESS;
}

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

