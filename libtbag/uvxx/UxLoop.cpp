/**
 * @file   UxLoop.cpp
 * @brief  UxLoop class implementation.
 * @author zer0
 * @date   2018-12-05
 */

#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

/**
 * @warning
 *  Don't change this variables.
 */
TBAG_CONSTEXPR static bool const AUTO_ERASE = true;

UxLoop::UxLoop(bool print_internal, bool verbose)
        : _loop(std::make_shared<Loop>(AUTO_ERASE, print_internal, verbose))
{
    assert(static_cast<bool>(_loop));
}

UxLoop::UxLoop(std::nullptr_t) TBAG_NOEXCEPT : _loop(nullptr)
{
    // EMPTY.
}

UxLoop::UxLoop(UxLoop const & obj) TBAG_NOEXCEPT : UxLoop(nullptr)
{
    (*this) = obj;
}

UxLoop::UxLoop(UxLoop && obj) TBAG_NOEXCEPT : UxLoop(nullptr)
{
    (*this) = std::move(obj);
}

UxLoop::~UxLoop()
{
    // EMPTY.
}

UxLoop & UxLoop::operator =(UxLoop const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxLoop & UxLoop::operator =(UxLoop && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxLoop::copy(UxLoop const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _loop = obj._loop;
    }
}

void UxLoop::swap(UxLoop & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _loop.swap(obj._loop);
    }
}

void UxLoop::reset()
{
    _loop.reset();
}

UxLoop::ThreadId UxLoop::getOwnerThreadId() const
{
    assert(exists());
    return _loop->getOwnerThreadId();
}

bool UxLoop::isRunning() const
{
    assert(exists());
    return _loop->isRunning();
}

bool UxLoop::isAlive() const
{
    assert(exists());
    return _loop->isAlive();
}

std::size_t UxLoop::size() const
{
    assert(exists());
    return _loop->size();
}

bool UxLoop::empty() const
{
    assert(exists());
    return _loop->empty();
}

Err UxLoop::run(RunMode mode)
{
    assert(exists());
    return _loop->run(mode);
}

Err UxLoop::close()
{
    assert(exists());
    return _loop->close();
}

void UxLoop::walk(void * arg)
{
    assert(exists());
    return _loop->walk(arg);
}

void UxLoop::stop()
{
    assert(exists());
    _loop->stop();
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

