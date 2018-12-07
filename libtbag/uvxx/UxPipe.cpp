/**
 * @file   UxPipe.cpp
 * @brief  UxPipe class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxPipe.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxPipe::UxPipe()
{
    // EMPTY.
}

UxPipe::UxPipe(UxLoop & loop, bool is_ipc)
{
    auto const CODE = init(loop, is_ipc);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxPipe::UxPipe(UxPipe const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxPipe::UxPipe(UxPipe && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxPipe::~UxPipe()
{
    // EMPTY.
}

UxPipe & UxPipe::operator =(UxPipe const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxPipe & UxPipe::operator =(UxPipe && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxPipe::copy(UxPipe const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::copy(obj);
    }
}

void UxPipe::swap(UxPipe & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::swap(obj);
    }
}

Err UxPipe::init(UxLoop & loop, bool is_ipc)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncPipe>(*loop, is_ipc);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxPipe::open(ufile file)
{
    if (auto shared = lock()) {
        return shared->open(file);
    }
    return Err::E_EXPIRED;
}

Err UxPipe::bind(char const * name)
{
    if (auto shared = lock()) {
        return shared->bind(name);
    }
    return Err::E_EXPIRED;
}

void UxPipe::connect(ConnectRequest & request, char const * name)
{
    if (auto shared = lock()) {
        shared->connect(request, name);
    }
}

std::string UxPipe::getSockName() const
{
    if (auto shared = lock()) {
        return shared->getSockName();
    }
    return std::string();
}

std::string UxPipe::getPeerName() const
{
    if (auto shared = lock()) {
        return shared->getPeerName();
    }
    return std::string();
}

void UxPipe::pendingInstances(int count)
{
    if (auto shared = lock()) {
        shared->pendingInstances(count);
    }
}

int UxPipe::getPendingCount()
{
    if (auto shared = lock()) {
        return shared->getPendingCount();
    }
    return 0;
}

int UxPipe::getPendingType()
{
    if (auto shared = lock()) {
        return shared->getPendingType();
    }
    return 0;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

