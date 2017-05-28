/**
 * @file   TpotRequest.cpp
 * @brief  TpotRequest class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/client/TpotRequest.hpp>
#include <libtbag/log/Log.hpp>

#include <cstdlib>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot   {
namespace client {

TpotRequest::TpotRequest()
{
    // EMPTY.
}

TpotRequest::TpotRequest(TpotRequest const & obj)
{
    (*this) = obj;
}

TpotRequest::TpotRequest(TpotRequest && obj)
{
    (*this) = std::move(obj);
}

TpotRequest::~TpotRequest()
{
    // EMPTY.
}

TpotRequest & TpotRequest::operator =(TpotRequest const & obj)
{
    if (this != &obj) {
    }
    return *this;
}

TpotRequest & TpotRequest::operator =(TpotRequest && obj)
{
    if (this != &obj) {
    }
    return *this;
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

