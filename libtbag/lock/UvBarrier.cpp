/**
 * @file   UvBarrier.cpp
 * @brief  UvBarrier class implementation.
 * @author zer0
 * @date   2017-05-18
 */

#include <libtbag/lock/UvBarrier.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cassert>
#include <new>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_barrier_t * cast_uv_barrier(T * v) TBAG_NOEXCEPT
{
    return static_cast<uv_barrier_t*>(v);
}

// -------------------------
// UvBarrier implementation.
// -------------------------

UvBarrier::UvBarrier(int count) : COUNT(count), _handle(new (std::nothrow) uv_barrier_t)
{
    assert(_handle != nullptr);
    int error_code = ::uv_barrier_init(cast_uv_barrier(_handle), COUNT);
    if (error_code != 0) {
        tDLogE("UvBarrier::UvBarrier() error[{}] {}", error_code, getUvErrorName(error_code));
    }
}

UvBarrier::~UvBarrier()
{
    assert(_handle != nullptr);
    ::uv_barrier_destroy(cast_uv_barrier(_handle));
    delete cast_uv_barrier(_handle);
}

int UvBarrier::wait()
{
    return ::uv_barrier_wait(cast_uv_barrier(_handle));
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

