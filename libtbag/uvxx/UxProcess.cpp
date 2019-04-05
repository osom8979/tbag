/**
 * @file   UxProcess.cpp
 * @brief  UxProcess class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/uvxx/UxProcess.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uvxx/UxLoop.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

UxProcess::UxProcess()
{
    // EMPTY.
}

UxProcess::UxProcess(UxLoop & loop, Options const & options)
{
    auto const CODE = spawn(loop, options);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

UxProcess::UxProcess(UxProcess const & obj) TBAG_NOEXCEPT
{
    (*this) = obj;
}

UxProcess::UxProcess(UxProcess && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

UxProcess::~UxProcess()
{
    // EMPTY.
}

UxProcess & UxProcess::operator =(UxProcess const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

UxProcess & UxProcess::operator =(UxProcess && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void UxProcess::copy(UxProcess const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::copy(obj);
    }
}

void UxProcess::swap(UxProcess & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        UxHandle::swap(obj);
    }
}

Err UxProcess::spawn(UxLoop & loop, Options const & options)
{
    UxHandle::release();
    assert(_handle.expired());

    try {
        _handle = loop->newHandle<FuncProcess>(*loop, options);
    } catch (ErrException e) {
        return e.CODE;
    }
    return E_SUCCESS;
}

Err UxProcess::processKill(int signum)
{
    if (auto shared = lock()) {
        return shared->processKill(signum);
    }
    return E_EXPIRED;
}

int UxProcess::getPid() const
{
    if (auto shared = lock()) {
        return shared->getPid();
    }
    return 0;
}

void UxProcess::disableStdioInheritance()
{
    libtbag::uvpp::Process::disableStdioInheritance();
}

Err UxProcess::kill(int pid, int signum)
{
    return libtbag::uvpp::Process::kill(pid, signum);
}

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

