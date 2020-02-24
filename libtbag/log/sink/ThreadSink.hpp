/**
 * @file   ThreadSink.hpp
 * @brief  ThreadSink class prototype.
 * @author zer0
 * @date   2019-10-09
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_THREADSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_THREADSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/thread/Thread.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/lock/UvLock.hpp>

#include <string>
#include <deque>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * ThreadSink class prototype.
 *
 * @author zer0
 * @date   2019-10-09
 */
class TBAG_API ThreadSink : public libtbag::thread::Thread, public Sink
{
public:
    using SharedSink = std::shared_ptr<Sink>;

public:
    struct Message TBAG_FINAL
    {
        int level;
        std::string message;
        bool flush;

        explicit Message(int l, std::string && m) : level(l), message(std::move(m)), flush(false)
        { /* EMPTY. */ }
        explicit Message(bool f) : level(), message(), flush(f)
        { /* EMPTY. */ }
        explicit Message() : level(), message(), flush()
        { /* EMPTY. */ }
    };

private:
    using MessageQueue = std::deque<Message>;
    using UvCondition = libtbag::lock::UvCondition;
    using UvLock = libtbag::lock::UvLock;
    using UvGuard = libtbag::lock::UvLockGuard<UvLock>;

private:
    SharedSink _sink;

private:
    UvCondition _condition;
    UvLock _lock;

private:
    MessageQueue _queue;
    bool _exit;

public:
    ThreadSink(SharedSink const & sink);
    virtual ~ThreadSink();

public:
    bool write(int level, char const * message, int size) override;
    bool isRequiredFlush() override;
    void flush() override;

protected:
    void onRunner() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_THREADSINK_HPP__

