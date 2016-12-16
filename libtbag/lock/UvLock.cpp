/**
 * @file   UvLock.cpp
 * @brief  UvLock class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/util/UvUtils.hpp>

#include <cassert>
#include <new>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_mutex_t * cast_uv_mutex(T * v) TBAG_NOEXCEPT
{
    return static_cast<uv_mutex_t*>(v);
}

// ----------------------
// UvLock implementation.
// ----------------------

UvLock::UvLock() : _handle(new (std::nothrow) uv_mutex_t)
{
    assert(_handle != nullptr);
    int error_code = ::uv_mutex_init(cast_uv_mutex(_handle));
    if (error_code != 0) {
        __tbag_error("UvLock::UvLock() error[{}] {}", error_code, util::getUvErrorName(error_code));
    }
}

UvLock::~UvLock()
{
    assert(_handle != nullptr);
    ::uv_mutex_destroy(cast_uv_mutex(_handle));
    delete cast_uv_mutex(_handle);
}

void UvLock::lock()
{
    ::uv_mutex_lock(cast_uv_mutex(_handle));
}

bool UvLock::tryLock()
{
    return ::uv_mutex_trylock(cast_uv_mutex(_handle)) == 0;
}

void UvLock::unlock()
{
    ::uv_mutex_unlock(cast_uv_mutex(_handle));
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

