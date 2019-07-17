/**
 * @file   FunctionalThread.cpp
 * @brief  FunctionalThread class implementation.
 * @author zer0
 * @date   2019-07-17
 */

#include <libtbag/thread/FunctionalThread.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

FunctionalThread::FunctionalThread() : _cb()
{
    // EMPTY.
}

FunctionalThread::FunctionalThread(Callback const & cb) : _cb(cb)
{
    // EMPTY.
}

FunctionalThread::FunctionalThread(Callback && cb) TBAG_NOEXCEPT : _cb(std::move(cb))
{
    // EMPTY.
}

FunctionalThread::~FunctionalThread()
{
    // EMPTY.
}

void FunctionalThread::setCallback(Callback const & cb)
{
    _cb = cb;
}

void FunctionalThread::setCallback(Callback && cb) TBAG_NOEXCEPT
{
    _cb = std::move(cb);
}

void FunctionalThread::onRunner()
{
    if (_cb) {
        _cb();
    }
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

