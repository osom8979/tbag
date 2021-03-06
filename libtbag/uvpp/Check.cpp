/**
 * @file   Check.cpp
 * @brief  Check class implementation.
 * @author zer0
 * @date   2017-05-02
 */

#include <libtbag/uvpp/Check.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// --------------------
// Global libuv events.
// --------------------

static void __global_uv_check_cb__(uv_check_t * handle)
{
    Check * h = static_cast<Check*>(handle->data);
    if (h == nullptr) {
        tDLogE("__global_uv_check_cb__() handle.data is nullptr.");
    } else if (isDeletedAddress(h)) {
        tDLogE("__global_uv_check_cb__() handle.data is deleted.");
    } else {
        h->onCheck();
    }
}

// ---------------------
// Check implementation.
// ---------------------

Check::Check() : Handle(uhandle::CHECK)
{
    // EMPTY.
}

Check::Check(Loop & loop) : Check()
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

Check::~Check()
{
    // EMPTY.
}

Err Check::init(Loop & loop)
{
    int const CODE = ::uv_check_init(loop.cast<uv_loop_t>(), Parent::cast<uv_check_t>());
    return convertUvErrorToErrWithLogging("Check::init()", CODE);
}

Err Check::start()
{
    int const CODE = ::uv_check_start(Parent::cast<uv_check_t>(), __global_uv_check_cb__);
    return convertUvErrorToErrWithLogging("Check::start()", CODE);
}

Err Check::stop()
{
    int const CODE = ::uv_check_stop(Parent::cast<uv_check_t>());
    return convertUvErrorToErrWithLogging("Check::stop()", CODE);
}

// --------------
// Event methods.
// --------------

void Check::onCheck()
{
    tDLogD("Check::onCheck() called.");
}

} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

