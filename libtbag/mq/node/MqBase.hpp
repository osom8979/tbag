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

#include <libtbag/lock/UvLock.hpp>
#include <libtbag/lock/UvCondition.hpp>
#include <libtbag/uvpp/Loop.hpp>

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
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqInternal     = libtbag::mq::details::MqInternal;
    using MqIsConsume    = libtbag::mq::details::MqIsConsume;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqQueue        = libtbag::mq::details::MqQueue;

public:
    using Loop = libtbag::uvpp::Loop;

public:
    using AtomicState = std::atomic<MqMachineState>;
    using AtomicInt   = std::atomic_int;

public:
    using UvLock      = libtbag::lock::UvLock;
    using UvCondition = libtbag::lock::UvCondition;

public:
    MqInternal const INTERNAL;
    MqParams   const PARAMS;

protected:
    MqQueue _receives;

protected:
    AtomicState _state;
    AtomicInt   _sending;

protected:
    UvLock mutable _wait_lock;
    UvCondition    _wait_cond;
    bool           _wait_enable;

protected:
    MqBase(MqInternal const & internal, MqParams const & params, MqMachineState state);
    MqBase(Loop & loop, MqInternal const & internal, MqParams const & params, MqMachineState state);
    virtual ~MqBase();

protected:
    void enableWait(bool wait_enable = true);
    void disableWait();

public:
    virtual MqMachineState state() const TBAG_NOEXCEPT override;
    virtual MqParams params() const override;

public:
    virtual Err send(MqMsg const & msg) override;
    virtual Err recv(MqMsg & msg) override;

public:
    virtual Err waitEnable(uint64_t timeout_nano) override;
    virtual Err waitRecv(MqMsg & msg, uint64_t timeout_nano) override;
};

} // namespace node
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NODE_MQBASE_HPP__

