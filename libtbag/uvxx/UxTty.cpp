/**
 * @file   UxTty.cpp
 * @brief  UxTty class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxTty.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxTty::UxTty()
{
    // EMPTY.
}

UxTty::UxTty(UxLoop & loop, ufile fd, bool readable)
{
    auto const CODE = init(loop, fd, readable);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxTty::UxTty(UxLoop & loop, GeneralFile fd)
{
    auto const CODE = init(loop, fd);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxTty::UxTty(UxTty const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxTty::UxTty(UxTty && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxTty::~UxTty()
{
    // EMPTY.
}

UxTty & UxTty::operator =(UxTty const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxTty & UxTty::operator =(UxTty && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxTty::copy(UxTty const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::copy(obj);
    }
}

void UxTty::swap(UxTty & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxStream::swap(obj);
    }
}

Err UxTty::init(UxLoop & loop, ufile fd, bool readable)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncTty>(*loop, fd, readable);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxTty::init(UxLoop & loop, GeneralFile fd)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncTty>(*loop, fd);
    } catch (ErrException e) {
        return e.CODE;
    }
    return Err::E_SUCCESS;
}

Err UxTty::setMode(TtyMode mode)
{
    if (auto shared = lock()) {
        return shared->setMode(mode);
    }
    return Err::E_EXPIRED;
}

Err UxTty::resetMode()
{
    if (auto shared = lock()) {
        return shared->resetMode();
    }
    return Err::E_EXPIRED;
}

Err UxTty::getWinSize(int * width, int * height)
{
    if (auto shared = lock()) {
        return shared->getWinSize(width, height);
    }
    if (width != nullptr) {
        *width = 0;
    }
    if (height != nullptr) {
        *height = 0;
    }
    return Err::E_EXPIRED;
}

UxTty::ufile UxTty::toFile(GeneralFile fd) TBAG_NOEXCEPT
{
    return libtbag::uvpp::Tty::toFile(fd);
}

bool UxTty::guessHandle(ufile fd)
{
    return libtbag::uvpp::Tty::guessHandle(fd);
}

bool UxTty::guessHandle(GeneralFile fd)
{
    return libtbag::uvpp::Tty::guessHandle(fd);
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

