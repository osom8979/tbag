/**
 * @file   TpotRequest.cpp
 * @brief  TpotRequest class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

TpotRequest::TpotRequest()
{
    // EMPTY.
}

TpotRequest::~TpotRequest()
{
    // EMPTY.
}

// ------------
// Entry-point.
// ------------

int runTpotRequestWithInteractiveMode()
{
    return EXIT_SUCCESS;
}

} // namespace client
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

