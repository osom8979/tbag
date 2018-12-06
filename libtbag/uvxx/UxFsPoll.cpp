/**
 * @file   UxFsPoll.cpp
 * @brief  UxFsPoll class implementation.
 * @author zer0
 * @date   2018-12-06
 */

#include <libtbag/uvxx/UxFsPoll.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxFsPoll::UxFsPoll()
{
    // EMPTY.
}

UxFsPoll::UxFsPoll(UxLoop & loop)
{
    auto const CODE = init(loop);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxFsPoll::UxFsPoll(UxFsPoll const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxFsPoll::UxFsPoll(UxFsPoll && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxFsPoll::~UxFsPoll()
{
    // EMPTY.
}

UxFsPoll & UxFsPoll::operator =(UxFsPoll const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxFsPoll & UxFsPoll::operator =(UxFsPoll && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxFsPoll::copy(UxFsPoll const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxFsPoll::swap(UxFsPoll & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxFsPoll::init(UxLoop & loop)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncFsPoll>(*loop);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxFsPoll::start(char const * path, unsigned int interval)
{
    if (auto shared = lock()) {
        return shared->start(path, interval);
    }
    return Err::E_EXPIRED;
}

Err UxFsPoll::stop()
{
    if (auto shared = lock()) {
        return shared->stop();
    }
    return Err::E_EXPIRED;
}

std::string UxFsPoll::getPath()
{
    if (auto shared = lock()) {
        return shared->getPath();
    }
    return std::string();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

