/**
 * @file   MqNode.hpp
 * @brief  MqNode class prototype.
 * @author zer0
 * @date   2018-11-11
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/uvpp/Loop.hpp>
#include <libtbag/thread/ThreadPool.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <string>
#include <memory>
#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * MqNode class prototype.
 *
 * @author zer0
 * @date   2018-11-11
 * @date   2019-01-15 (Merge with MqNode::Impl class)
 */
class TBAG_API MqNode : private Noncopyable
{
public:
    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqInternal     = libtbag::mq::details::MqInternal;
    using MqIsConsume    = libtbag::mq::details::MqIsConsume;
    using MqInterface    = libtbag::mq::details::MqInterface;

    using Loop     = libtbag::uvpp::Loop;
    using Pool     = libtbag::thread::ThreadPool;
    using UniqueMq = std::unique_ptr<MqInterface>;

    using OnAccept = std::function<bool(std::string const &)>;
    using OnWrite  = std::function<bool(MqMsg &)>;
    using OnRecv   = std::function<bool(MqMsg const &)>;

public:
    struct Callbacks
    {
        OnAccept  accept_cb;
        OnWrite   write_cb;
        OnRecv    recv_cb;
    };

public:
    /**
     * @warning
     *  Don't change this variable.
     */
    TBAG_CONSTEXPR static std::size_t const THREAD_SIZE = 1U;

private:
    MqParams const PARAMS;
    MqMode   const MODE;

private:
    Pool     _pool;
    Loop     _loop;
    UniqueMq _mq;
    Err      _last;

private:
    Callbacks _callbacks;

public:
    MqNode(MqParams const & params, MqMode mode, MqInternal const & internal);
    MqNode(std::string const & uri, MqMode mode, MqInternal const & internal);

    MqNode(MqParams const & params, MqMode mode, Callbacks const & cbs);
    MqNode(std::string const & uri, MqMode mode, Callbacks const & cbs);

    MqNode(MqParams const & params, MqMode mode);
    MqNode(std::string const & uri, MqMode mode);

    ~MqNode();

public:
    // @formatter:off
    MqParams   params() const TBAG_NOEXCEPT { return PARAMS; }
    MqType       type() const TBAG_NOEXCEPT { return PARAMS.type; }
    MqMode       mode() const TBAG_NOEXCEPT { return   MODE; }
    Loop       & loop()       TBAG_NOEXCEPT { return  _loop; }
    Loop const & loop() const TBAG_NOEXCEPT { return  _loop; }
    // @formatter:on

public:
    inline char const * const getTypeName() const TBAG_NOEXCEPT
    { return libtbag::mq::details::getTypeName(PARAMS.type); }

    inline char const * const getModeName() const TBAG_NOEXCEPT
    { return libtbag::mq::details::getModeName(MODE); }

private:
    friend bool onAcceptCb(void * node, std::string const & peer, void * parent);
    friend MqIsConsume onWriteCb(MqMsg & msg, void * parent);
    friend MqIsConsume onRecvCb(MqMsg const & msg, void * parent);

private:
    bool init(MqInternal const & internal);
    void runner();

public:
    void join();

public:
    Err exit();

public:
    Err send(MqMsg const & msg);

    Err send(char const * buffer, std::size_t size, std::intptr_t id = 0);
    Err send(MqEvent event, char const * buffer, std::size_t size, std::intptr_t id = 0);

    Err send(std::string const & text, std::intptr_t id = 0);
    Err send(MqEvent event, std::string const & text, std::intptr_t id = 0);

    Err send(MqMsg::Buffer const & buffer, std::intptr_t id = 0);
    Err send(MqEvent event, MqMsg::Buffer const & buffer, std::intptr_t id = 0);

    Err sendClose(std::intptr_t id = 0);

public:
    Err recv(MqMsg & msg);
    Err recvWait(MqMsg & msg, uint64_t timeout_nano = 0);

public:
    Err waitEnable(uint64_t timeout_nano = 0);

public:
    static MqParams getParams(std::string const & uri);
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

