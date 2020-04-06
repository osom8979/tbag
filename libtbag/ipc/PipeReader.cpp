/**
 * @file   PipeReader.cpp
 * @brief  PipeReader class implementation.
 * @author zer0
 * @date   2020-03-21
 */

#include <libtbag/ipc/PipeReader.hpp>
#include <libtbag/log/Log.hpp>

#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxSignal.hpp>
#include <libtbag/uvxx/UxAsync.hpp>
#include <libtbag/uvxx/UxPipe.hpp>

#include <cassert>
#include <cstdio>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace ipc {

/**
 * PipeReader::Impl class implementation.
 *
 * @author zer0
 * @date   2019-06-26
 */
struct PipeReader::Impl : private Noncopyable
{
    using ThreadPool = libtbag::thread::ThreadPool;
    using Buffer     = libtbag::util::Buffer;
    using UxLoop     = libtbag::uvxx::UxLoop;
    using UxSignal   = libtbag::uvxx::UxSignal;
    using UxAsync    = libtbag::uvxx::UxAsync;
    using UxPipe     = libtbag::uvxx::UxPipe;

    TBAG_CONSTEXPR static int const THREAD_COUNT = 1;
    TBAG_CONSTEXPR static bool const WAIT_ACTIVE = true;
    TBAG_CONSTEXPR static bool const SIGNAL_HANDING = false;
    TBAG_CONSTEXPR static int const SIGNAL_NUMBER = libtbag::signal::TBAG_SIGNAL_INTERRUPT;

    Callback    READ_CALLBACK;
    ThreadPool  thread;
    UxLoop      loop;
    UxSignal    signal;
    UxAsync     exit_async;
    UxAsync     raise_async;
    UxPipe      pipe;
    Buffer      pipe_buffer;

    Impl(ufile f, Callback const & cb)
            : READ_CALLBACK(cb),
              thread(THREAD_COUNT, WAIT_ACTIVE, SIGNAL_HANDING)
    {
        assert(READ_CALLBACK);
        auto const code = init(f);
        if (isFailure(code)) {
            throw ErrException(code);
        }
    }

    ~Impl()
    {
        exit();
    }

    /**
     * @warning
     *  It must be called inside a loop thread.
     */
    void __close_all_handles_unsafe()
    {
        exit_async.close();
        raise_async.close();
        signal.close();
        pipe.close();
    }

    Err init(ufile f)
    {
        auto code = signal.init(loop);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Signal initialize error.");
            return code;
        }

        code = exit_async.init(loop);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Async(exit) initialize error.");
            return code;
        }

        code = raise_async.init(loop);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Async(raise) initialize error.");
            return code;
        }

        code = pipe.init(loop, true);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Pipe initialize error.");
            return code;
        }

        signal.setOnSignal([this](int signum){
            tDLogD("PipeReader::Impl::init() Signal catch! {}", signum);
            auto const code = exit_async.send();
            if (isSuccess(code)) {
                tDLogN("PipeReader::Impl::init() Send 'exit_signal' to an async handler.");
            } else {
                tDLogN("PipeReader::Impl::init() Send 'exit_signal' error: {}", code);
                __close_all_handles_unsafe();
            }
        });
        exit_async.setOnAsync([this](){
            __close_all_handles_unsafe();
        });
        raise_async.setOnAsync([this](){
            auto const code = libtbag::convertSystemErrorToErr(libtbag::signal::raise(SIGNAL_NUMBER));
            if (isSuccess(code)) {
                tDLogN("PipeReader::Impl::init() Raise signal: {}", int(SIGNAL_NUMBER));
            } else {
                tDLogE("PipeReader::Impl::init() Raise signal({}) error: {}", int(SIGNAL_NUMBER), code);
                __close_all_handles_unsafe();
            }
        });

        pipe.setOnAlloc([&](std::size_t suggested_size) -> UxPipe::binf {
            return libtbag::uvpp::defaultOnAlloc(pipe_buffer, suggested_size);
        });
        pipe.setOnRead([&](Err code, char const * data, std::size_t size){
            if (code == E_EOF) {
                pipe.close();
            } else if (isFailure(code)) {
                pipe.close();
            } else {
                if (!READ_CALLBACK(data, size)) {
                    pipe.close();
                }
            }
        });
        pipe.setOnClose([&](){
            exit_async.send();
        });

        assert(signal.isInit());
        assert(exit_async.isInit());
        assert(raise_async.isInit());
        assert(pipe.isInit());
        assert(!loop.empty());

        code = pipe.open(f);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Pipe open({}) error: {}", f, code);
            return code;
        }

        code = signal.start(SIGNAL_NUMBER);
        if (isFailure(code)) {
            tDLogE("PipeReader::Impl::init() Signal start error: {}", code);
            return code;
        }

        auto const push_result = thread.push([this](){
            tDLogI("PipeReader::Impl::init() Event Loop run.");
            auto const code = this->loop.run();
            if (isSuccess(code)) {
                tDLogI("PipeReader::Impl::init() Event Loop done.");
            } else {
                tDLogE("PipeReader::Impl::init() Event Loop error: {}", code);
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
            tDLogE("PipeReader::Impl::exit() Event loop join error: {}", e.what());
        } catch (...) {
            tDLogE("PipeReader::Impl::exit() Event loop join, unknown error.");
        }
    }

    Err raiseAsync()
    {
        return raise_async.send();
    }
};

// -------------
// PipeReader
// -------------

PipeReader::PipeReader() : _impl()
{
    // EMPTY.
}

PipeReader::PipeReader(ufile f, Callback const & cb)
{
    auto const CODE = start(f, cb);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

PipeReader::~PipeReader()
{
    done();
}

Err PipeReader::start(ufile f, Callback const & cb)
{
    if (_impl) {
        return E_ILLSTATE;
    }
    try {
        _impl = std::make_unique<Impl>(f, cb);
    } catch (ErrException const & e) {
        return e.CODE;
    } catch (...) {
        return E_UNKEXCP;
    }
    assert(static_cast<bool>(_impl));
    return E_SUCCESS;
}

Err PipeReader::asyncRaise()
{
    if (!_impl) {
        return E_ILLSTATE;
    }
    return _impl->raiseAsync();
}

void PipeReader::done()
{
    _impl.reset();
}

} // namespace ipc

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

