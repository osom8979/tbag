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

FunctionalThread::FunctionalThread(bool join_in_destructors) TBAG_NOEXCEPT
        : Thread(join_in_destructors), _cb()
{
    // EMPTY.
}

FunctionalThread::FunctionalThread(Callback const & cb, bool join_in_destructors)
        : Thread(join_in_destructors), _cb(cb)
{
    // EMPTY.
}

FunctionalThread::FunctionalThread(Callback && cb, bool join_in_destructors) TBAG_NOEXCEPT
        : Thread(join_in_destructors), _cb(std::move(cb))
{
    // EMPTY.
}

FunctionalThread::FunctionalThread(start_t, bool join_in_destructors)
        : Thread(join_in_destructors), _cb()
{
    auto const code = run();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

FunctionalThread::FunctionalThread(start_t, Callback const & cb, bool join_in_destructors)
        : Thread(join_in_destructors), _cb(cb)
{
    auto const code = run();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

FunctionalThread::FunctionalThread(start_t, Callback && cb, bool join_in_destructors)
        : Thread(join_in_destructors), _cb(std::move(cb))
{
    auto const code = run();
    if (isFailure(code)) {
        throw ErrException(code);
    }
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

