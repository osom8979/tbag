/**
 * @file   UxStream.cpp
 * @brief  UxStream class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxStream.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxStream::UxStream()
{
    // EMPTY.
}

UxStream::UxStream(UxStream const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxStream::UxStream(UxStream && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxStream::~UxStream()
{
    // EMPTY.
}

UxStream & UxStream::operator =(UxStream const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxStream & UxStream::operator =(UxStream && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxStream::copy(UxStream const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxStream::swap(UxStream & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

std::size_t UxStream::getWriteQueueSize() const
{
    if (auto shared = lockStream()) {
        return shared->getWriteQueueSize();
    }
    return 0;
}

bool UxStream::isReadable() const
{
    if (auto shared = lockStream()) {
        return shared->isReadable();
    }
    return false;
}

bool UxStream::isWritable() const
{
    if (auto shared = lockStream()) {
        return shared->isWritable();
    }
    return false;
}

Err UxStream::setBlocking(bool enable)
{
    if (auto shared = lockStream()) {
        return shared->setBlocking(enable);
    }
    return Err::E_EXPIRED;
}

Err UxStream::shutdown(ShutdownRequest & request)
{
    if (auto shared = lockStream()) {
        return shared->shutdown(request);
    }
    return Err::E_EXPIRED;
}

Err UxStream::listen(int backlog)
{
    if (auto shared = lockStream()) {
        return shared->listen(backlog);
    }
    return Err::E_EXPIRED;
}

Err UxStream::accept(Stream & client)
{
    if (auto shared = lockStream()) {
        return shared->accept(client);
    }
    return Err::E_EXPIRED;
}

Err UxStream::startRead()
{
    if (auto shared = lockStream()) {
        return shared->startRead();
    }
    return Err::E_EXPIRED;
}

Err UxStream::stopRead()
{
    if (auto shared = lockStream()) {
        return shared->stopRead();
    }
    return Err::E_EXPIRED;
}

Err UxStream::write(WriteRequest & request, binf const * infos, std::size_t infos_size)
{
    if (auto shared = lockStream()) {
        return shared->write(request, infos, infos_size);
    }
    return Err::E_EXPIRED;
}

Err UxStream::write(WriteRequest & request, char const * buffer, std::size_t size)
{
    if (auto shared = lockStream()) {
        return shared->write(request, buffer, size);
    }
    return Err::E_EXPIRED;
}

std::size_t UxStream::tryWrite(binf * infos, std::size_t infos_size, Err * result)
{
    if (auto shared = lockStream()) {
        return shared->tryWrite(infos, infos_size, result);
    }
    if (result != nullptr) {
        *result = Err::E_EXPIRED;
    }
    return 0;
}

std::size_t UxStream::tryWrite(char const * buffer, std::size_t size, Err * result)
{
    if (auto shared = lockStream()) {
        return shared->tryWrite(buffer, size, result);
    }
    if (result != nullptr) {
        *result = Err::E_EXPIRED;
    }
    return 0;
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

