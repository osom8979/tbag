/**
 * @file   MqPipeline.hpp
 * @brief  MqPipeline class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQPIPELINE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQPIPELINE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/mq/MqNode.hpp>

#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * MqPipeline class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API MqPipeline : private Noncopyable
{
public:
    using MqMsg = MqNode::MqMsg;

private:
    std::atomic<MqNode*> _recv;
    std::atomic<MqNode*> _send;

private:
    std::atomic<uint64_t> _wait_nano;
    std::atomic<bool> _exit;

public:
    MqPipeline(MqNode * recv = nullptr, MqNode * send = nullptr, uint64_t wait_nano = 0);
    virtual ~MqPipeline();

public:
    inline void setRecv(MqNode * node) TBAG_NOEXCEPT_SP_OP(_recv.store(node))
    { _recv.store(node); }
    inline void setSend(MqNode * node) TBAG_NOEXCEPT_SP_OP(_send.store(node))
    { _send.store(node); }
    inline void setWaitNano(uint64_t nano) TBAG_NOEXCEPT_SP_OP(_wait_nano.store(nano))
    { _wait_nano.store(nano); }
    inline void exit(bool flag = true) TBAG_NOEXCEPT_SP_OP(_exit.store(flag))
    { _exit.store(flag); }

public:
    void run(bool validate = true, bool verbose = false);

protected:
    virtual bool onBegin();
    virtual void onEnd(Err code);

protected:
    virtual void onError(Err code);

protected:
    virtual bool onMsg(MqMsg const & recv_msg, MqMsg & send_msg);
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQPIPELINE_HPP__

