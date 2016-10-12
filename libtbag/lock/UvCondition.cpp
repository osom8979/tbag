/**
 * @file   UvCondition.cpp
 * @brief  UvCondition class implementation.
 * @author zer0
 * @date   2016-10-12
 */

#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/lock/UvLock.hpp>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_cond_t * cast_uv_cond(T * v) noexcept
{
    return static_cast<uv_cond_t*>(v);
}

UvCondition::UvCondition() throw (InitializeException) : _handle(new uv_cond_t)
{
    int error_code = ::uv_cond_init(cast_uv_cond(_handle));
    if (error_code != 0) {
        throw InitializeException(error_code);
    }
}

UvCondition::~UvCondition()
{
    ::uv_cond_destroy(cast_uv_cond(_handle));
    delete cast_uv_cond(_handle);
}

void UvCondition::wait(UvLock & lock)
{
    ::uv_cond_wait(cast_uv_cond(_handle), static_cast<uv_mutex_t*>(lock.getHandle()));
}

void UvCondition::wait(UvLock & lock, int64_t timeout_nano)
{
    ::uv_cond_timedwait(cast_uv_cond(_handle), static_cast<uv_mutex_t*>(lock.getHandle()), timeout_nano);
}

void UvCondition::signal()
{
    ::uv_cond_signal(cast_uv_cond(_handle));
}

void UvCondition::broadcast()
{
    ::uv_cond_broadcast(cast_uv_cond(_handle));
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

