/**
 * @file   UvSemaphore.cpp
 * @brief  UvSemaphore class implementation.
 * @author zer0
 * @date   2017-05-03
 */

#include <libtbag/lock/UvSemaphore.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cassert>
#include <exception>
#include <new>

#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lock {

template <typename T>
inline uv_sem_t * cast_uv_sem(T * v) TBAG_NOEXCEPT
{
    return static_cast<uv_sem_t*>(v);
}

// ----------------------
// UvLock implementation.
// ----------------------

UvSemaphore::UvSemaphore(unsigned int value) : _handle(new (std::nothrow) uv_sem_t)
{
    assert(_handle != nullptr);
    int const CODE = ::uv_sem_init(cast_uv_sem(_handle), value);
    if (uvpp::getUerr2("UvSemaphore::UvSemaphore()", CODE) != uvpp::uerr::UVPP_SUCCESS) {
        throw std::bad_alloc();
    }
}

UvSemaphore::~UvSemaphore()
{
    assert(_handle != nullptr);
    ::uv_sem_destroy(cast_uv_sem(_handle));
    delete cast_uv_sem(_handle);
}

void UvSemaphore::wait()
{
    assert(_handle != nullptr);
    ::uv_sem_wait(cast_uv_sem(_handle));
}

bool UvSemaphore::tryWait()
{
    assert(_handle != nullptr);
    return ::uv_sem_trywait(cast_uv_sem(_handle)) == 0;
}

void UvSemaphore::post()
{
    assert(_handle != nullptr);
    ::uv_sem_post(cast_uv_sem(_handle));
}

} // namespace lock

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

