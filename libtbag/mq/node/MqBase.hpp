/**
 * @file   MqBase.hpp
 * @brief  MqBase class prototype.
 * @author zer0
 * @date   2018-11-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQBASE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQBASE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/mq/details/MqEventQueue.hpp>
#include <libtbag/mq/details/MqQueue.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/uvpp/Async.hpp>
#include <libtbag/uvpp/Timer.hpp>

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>

#include <cassert>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq   {
namespace node {

/**
 * MqBase class prototype.
 *
 * @author zer0
 * @date   2018-11-25
 */
class TBAG_API MqBase : protected libtbag::mq::details::MqEventQueue,
                        public    libtbag::mq::details::MqInterface
{
public:
    using Loop  = libtbag::uvpp::Loop;
    using Async = libtbag::uvpp::Async;
    using Timer = libtbag::uvpp::Timer;

    using Buffer = libtbag::util::Buffer;
    using binf   = libtbag::util::binf;
    using cbinf  = libtbag::util::cbinf;

    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqEventQueue   = libtbag::mq::details::MqEventQueue;
    using MqQueue        = libtbag::mq::details::MqQueue;
    using MqInternal     = libtbag::mq::details::MqInternal;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqIsConsume    = libtbag::mq::details::MqIsConsume;

    using AsyncMsg        = MqEventQueue::AsyncMsg;
    using AfterAction     = MqEventQueue::AfterAction;
    using AsyncMsgPointer = libtbag::container::Pointer<AsyncMsg>;
    using AsyncMsgQueue   = std::queue<AsyncMsgPointer>;

    using AtomicState = std::atomic<MqMachineState>;
    using AtomicBool  = std::atomic_bool;
    using AtomicInt   = std::atomic_int;

    using UvLock      = libtbag::lock::UvLock;
    using UvCondition = libtbag::lock::UvCondition;

public:
    struct Initializer : public Async
    {
        MqBase * parent = nullptr;

        Initializer(Loop & loop, MqBase * p)
                : Async(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Initializer()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onInitializerAsync(this); }
        virtual void onClose() override
        { parent->onInitializerClose(this); }
    };

    struct Terminator : public Async
    {
        MqBase * parent = nullptr;

        Terminator(Loop & loop, MqBase * p)
                : Async(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~Terminator()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onTerminatorAsync(this); }
        virtual void onClose() override
        { parent->onTerminatorClose(this); }
    };

    struct Writer : public Async
    {
        MqBase * parent = nullptr;

        MqRequestState state;
        AsyncMsgQueue  queue;
        std::size_t    write_count;

        Writer(Loop & loop, MqBase * p)
                : Async(loop), parent(p),
                  state(MqRequestState::MRS_WAITING),
                  queue(), write_count(0)
        { assert(parent != nullptr); }
        virtual ~Writer()
        { /* EMPTY. */ }

        virtual void onAsync() override
        { parent->onWriterAsync(this); }
        virtual void onClose() override
        { parent->onWriterClose(this); }
    };

    struct CloseTimer : public Timer
    {
        MqBase * parent = nullptr;

        CloseTimer(Loop & loop, MqBase * p) : Timer(loop), parent(p)
        { assert(parent != nullptr); }
        virtual ~CloseTimer()
        { /* EMPTY. */ }

        virtual void onTimer() override
        { parent->onCloseTimerTimer(this); }
        virtual void onClose() override
        { parent->onCloseTimerClose(this); }
    };

public:
    using SharedInitializer = std::shared_ptr<Initializer>;
    using SharedTerminator  = std::shared_ptr<Terminator>;

public:
    MqInternal const INTERNAL;
    MqParams   const PARAMS;

protected:
    MqQueue _receives;

protected:
    SharedTerminator _terminator;

protected:
    AtomicState _state;
    AtomicInt   _sending;
    AtomicInt   _exiting;

private:
    UvLock mutable _wait_lock;
    UvCondition    _wait_cond;
    bool           _wait_enable;

protected:
    MqBase(MqInternal const & internal, MqParams const & params);
    MqBase(Loop & loop, MqInternal const & internal, MqParams const & params);
    virtual ~MqBase();

protected:
    /** Obtain an inaccurate active send-queue size. */
    std::size_t getActiveSendSize() const;

    /** Obtain an inaccurate active recv-queue size. */
    std::size_t getActiveRecvSize() const;

    void enableWait(bool enable = true);
    void disableWait();

    void createTerminator(Loop & loop);
    void closeTerminator();

    void changeClosingState();
    void changeClosedState();

    Err enqueueReceiveForSingleProducer(MqMsg const & msg);

protected:
    virtual void onInitializerAsync(Initializer * init) { /* EMPTY. */ }
    virtual void onInitializerClose(Initializer * init) { /* EMPTY. */ }

    virtual void onTerminatorAsync(Terminator * terminator) { /* EMPTY. */ }
    virtual void onTerminatorClose(Terminator * terminator) { /* EMPTY. */ }

    virtual void onWriterAsync(Writer * writer) { /* EMPTY. */ }
    virtual void onWriterClose(Writer * writer) { /* EMPTY. */ }

    virtual void onCloseTimerTimer(CloseTimer * timer) { /* EMPTY. */ }
    virtual void onCloseTimerClose(CloseTimer * timer) { /* EMPTY. */ }

public:
    inline MqMachineState state() const TBAG_NOEXCEPT
    { return _state.load(); }

    inline int getTypeInteger() const TBAG_NOEXCEPT
    { return static_cast<int>(PARAMS.type); }

    inline char const * const getTypeName() const TBAG_NOEXCEPT
    { return libtbag::mq::details::getTypeName(PARAMS.type); }

public:
    virtual Err exit() override;

    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

    virtual Err waitEnable(uint64_t timeout_nano) override;
    virtual Err waitRecv(MqMsg & msg, uint64_t timeout_nano) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQBASE_HPP__

