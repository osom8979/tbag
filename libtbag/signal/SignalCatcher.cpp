/**
 * @file   SignalCatcher.cpp
 * @brief  SignalCatcher class implementation.
 * @author zer0
 * @date   2019-06-26
 */

#include <libtbag/signal/SignalCatcher.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxSignal.hpp>
#include <libtbag/uvxx/UxAsync.hpp>

#include <cassert>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace signal {

/**
 * SignalCatcher::Impl class implementation.
 *
 * @author zer0
 * @date   2019-06-26
 */
struct SignalCatcher::Impl : private Noncopyable
{
    using ThreadPool = libtbag::thread::ThreadPool;
    using UxLoop     = libtbag::uvxx::UxLoop;
    using UxSignal   = libtbag::uvxx::UxSignal;
    using UxAsync    = libtbag::uvxx::UxAsync;

    TBAG_CONSTEXPR static int const THREAD_COUNT = 1;
    TBAG_CONSTEXPR static bool const WAIT_ACTIVE = true;
    TBAG_CONSTEXPR static bool const SIGNAL_HANDING = false;

    int const   SIGNAL_NUMBER;
    Callback    callback;
    ThreadPool  thread;
    UxLoop      loop;
    UxSignal    signal;
    UxAsync     exit_async;
    UxAsync     raise_async;

    Impl(int signum, Callback const & cb)
            : SIGNAL_NUMBER(signum), callback(cb),
              thread(THREAD_COUNT, WAIT_ACTIVE, SIGNAL_HANDING)
    {
        auto const CODE = init();
        if (isFailure(CODE)) {
            throw ErrException(CODE);
        }
    }

    ~Impl()
    {
        exit();
    }

    Err init()
    {
        auto code = signal.init(loop);
        if (isFailure(code)) {
            tDLogE("SignalCatcher::Impl::init() Signal initialize error.");
            return code;
        }

        code = exit_async.init(loop);
        if (isFailure(code)) {
            tDLogE("SignalCatcher::Impl::init() Async(exit) initialize error.");
            return code;
        }

        code = raise_async.init(loop);
        if (isFailure(code)) {
            tDLogE("SignalCatcher::Impl::init() Async(raise) initialize error.");
            return code;
        }

        signal.setOnSignal([this](int signum){
            tDLogN("SignalCatcher::Impl::init() Signal catch: {}", signum);
            callback();
        });
        exit_async.setOnAsync([this](){
            exit_async.close();
            raise_async.close();
            signal.close();
        });
        raise_async.setOnAsync([this](){
            libtbag::signal::raise(SIGNAL_NUMBER);
        });

        assert(signal.isInit());
        assert(exit_async.isInit());
        assert(raise_async.isInit());
        assert(!loop.empty());

        code = signal.start(SIGNAL_NUMBER);
        if (isFailure(code)) {
            tDLogE("SignalCatcher::Impl::init() Signal start error.");
            return code;
        }

        auto const push_result = thread.push([this](){
            tDLogI("SignalCatcher::Impl::init() Event Loop run.");
            auto const code = this->loop.run();
            if (isSuccess(code)) {
                tDLogI("SignalCatcher::Impl::init() Event Loop done.");
            } else {
                tDLogE("SignalCatcher::Impl::init() Event Loop error: {}", code);
            }
        });

        assert(push_result);
        return E_SUCCESS;
    }

    void exit()
    {
        auto const code = exit_async.send();
        assert(isSuccess(code));

        thread.exit();
        try {
            thread.join();
        } catch (std::exception const & e) {
            tDLogE("SignalCatcher::Impl::exit() Event loop join error: {}", e.what());
        } catch (...) {
            tDLogE("SignalCatcher::Impl::exit() Event loop join, unknown error.");
        }
    }

    Err raiseAsync()
    {
        return raise_async.send();
    }
};

// -------------
// SignalCatcher
// -------------

SignalCatcher::SignalCatcher()
{
    // EMPTY.
}

SignalCatcher::SignalCatcher(int signum, Callback const & cb)
{
    auto const CODE = start(signum, cb);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

SignalCatcher::~SignalCatcher()
{
    done();
}

Err SignalCatcher::start(int signum, Callback const & cb)
{
    if (_impl) {
        return E_ILLSTATE;
    }
    try {
        _impl = std::make_unique<Impl>(signum, cb);
    } catch (ErrException const & e) {
        return e.CODE;
    } catch (...) {
        return E_UNKEXCP;
    }
    assert(static_cast<bool>(_impl));
    return E_SUCCESS;
}

Err SignalCatcher::startInterrupt(Callback const & cb)
{
    return start(libtbag::signal::TBAG_SIGNAL_INTERRUPT, cb);
}

Err SignalCatcher::asyncRaise()
{
    if (!_impl) {
        return E_ILLSTATE;
    }
    return _impl->raiseAsync();
}

void SignalCatcher::done()
{
    _impl.reset();
}

} // namespace signal

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

