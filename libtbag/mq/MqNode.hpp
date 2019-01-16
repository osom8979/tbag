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
#include <libtbag/util/BufferInfo.hpp>

#include <string>
#include <memory>
#include <functional>
#include <utility>

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
    using MqBindMode     = libtbag::mq::details::MqBindMode;
    using MqConnectMode  = libtbag::mq::details::MqConnectMode;
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
    struct no_init_t { /* EMPTY. */ };
    TBAG_CONSTEXPR static no_init_t const no_init = no_init_t{};

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
    MqNode(MqParams const & params, MqMode mode, no_init_t);
    MqNode(std::string const & uri, MqMode mode, no_init_t);

    MqNode(MqParams const & params, MqMode mode, Callbacks const & cbs = Callbacks{});
    MqNode(std::string const & uri, MqMode mode, Callbacks const & cbs = Callbacks{});

    MqNode(MqBindMode, MqParams const & params, Callbacks const & cbs = Callbacks{});
    MqNode(MqBindMode, std::string const & uri, Callbacks const & cbs = Callbacks{});

    MqNode(MqConnectMode, MqParams const & params, Callbacks const & cbs = Callbacks{});
    MqNode(MqConnectMode, std::string const & uri, Callbacks const & cbs = Callbacks{});

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

protected:
    bool init(MqInternal const & internal);

private:
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

/**
 * Bind node.
 *
 * @author zer0
 * @date   2019-01-16
 */
struct MqBind : public MqNode
{
    MqBind(MqParams const & params, no_init_t) : MqNode(params, MqMode::MM_BIND, no_init) { /* EMPTY. */ }
    MqBind(std::string const & uri, no_init_t) : MqNode(uri, MqMode::MM_BIND, no_init) { /* EMPTY. */ }

    MqBind(MqParams const & params, Callbacks const & cbs) : MqNode(params, MqMode::MM_BIND, cbs) { /* EMPTY. */ }
    MqBind(std::string const & uri, Callbacks const & cbs) : MqNode(uri, MqMode::MM_BIND, cbs) { /* EMPTY. */ }

    MqBind(MqParams const & params) : MqNode(params, MqMode::MM_BIND) { /* EMPTY. */ }
    MqBind(std::string const & uri) : MqNode(uri, MqMode::MM_BIND) { /* EMPTY. */ }

    ~MqBind() { /* EMPTY. */ }
};

/**
 * Connect node.
 *
 * @author zer0
 * @date   2019-01-16
 */
struct MqConn : public MqNode
{
    MqConn(MqParams const & params, no_init_t) : MqNode(params, MqMode::MM_CONNECT, no_init) { /* EMPTY. */ }
    MqConn(std::string const & uri, no_init_t) : MqNode(uri, MqMode::MM_CONNECT, no_init) { /* EMPTY. */ }

    MqConn(MqParams const & params, Callbacks const & cbs) : MqNode(params, MqMode::MM_CONNECT, cbs) { /* EMPTY. */ }
    MqConn(std::string const & uri, Callbacks const & cbs) : MqNode(uri, MqMode::MM_CONNECT, cbs) { /* EMPTY. */ }

    MqConn(MqParams const & params) : MqNode(params, MqMode::MM_CONNECT) { /* EMPTY. */ }
    MqConn(std::string const & uri) : MqNode(uri, MqMode::MM_CONNECT) { /* EMPTY. */ }

    ~MqConn() { /* EMPTY. */ }
};

/**
 * Syntactic sugar for end users.
 *
 * @author zer0
 * @date   2019-01-16
 */
struct MqTypes
{
    using Err    = libtbag::Err;
    using Buffer = libtbag::util::Buffer;

    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqBindMode     = libtbag::mq::details::MqBindMode;
    using MqConnectMode  = libtbag::mq::details::MqConnectMode;
    using MqParams       = libtbag::mq::details::MqParams;
    using MqInternal     = libtbag::mq::details::MqInternal;
    using MqIsConsume    = libtbag::mq::details::MqIsConsume;
    using MqInterface    = libtbag::mq::details::MqInterface;

    using MqOnActive       = libtbag::mq::details::MqOnActive;
    using MqOnAccept       = libtbag::mq::details::MqOnAccept;
    using MqOnWrite        = libtbag::mq::details::MqOnWrite;
    using MqOnRecv         = libtbag::mq::details::MqOnRecv;
    using MqOnClose        = libtbag::mq::details::MqOnClose;
    using MqOnDefaultWrite = libtbag::mq::details::MqOnDefaultWrite;
    using MqOnDefaultRead  = libtbag::mq::details::MqOnDefaultRead;
    using MqOnCloseNode    = libtbag::mq::details::MqOnCloseNode;
    using MqOnCreateLoop   = libtbag::mq::details::MqOnCreateLoop;

    using MqNode = libtbag::mq::MqNode;
    using MqBind = libtbag::mq::MqBind;
    using MqConn = libtbag::mq::MqConn;

    using MqUniqueNode = std::unique_ptr<MqNode>;
    using MqSharedNode = std::shared_ptr<MqNode>;

    using MqUniqueBind = std::unique_ptr<MqBind>;
    using MqSharedBind = std::shared_ptr<MqBind>;

    using MqUniqueConn = std::unique_ptr<MqConn>;
    using MqSharedConn = std::shared_ptr<MqConn>;

    template <typename ... Args>
    MqUniqueNode bindUniqueNode(Args && ... args)
    { return std::make_unique<MqNode>(libtbag::mq::details::MQ_BIND, std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqSharedNode bindSharedNode(Args && ... args)
    { return std::make_shared<MqNode>(libtbag::mq::details::MQ_BIND, std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqUniqueBind bindUnique(Args && ... args)
    { return std::make_unique<MqBind>(std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqSharedBind bindShared(Args && ... args)
    { return std::make_shared<MqBind>(std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqUniqueNode connectUniqueNode(Args && ... args)
    { return std::make_unique<MqNode>(libtbag::mq::details::MQ_CONNECT, std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqSharedNode connectSharedNode(Args && ... args)
    { return std::make_shared<MqNode>(libtbag::mq::details::MQ_CONNECT, std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqUniqueConn connectUnique(Args && ... args)
    { return std::make_unique<MqConn>(std::forward<Args>(args) ...); }

    template <typename ... Args>
    MqSharedConn bindShared(Args && ... args)
    { return std::make_shared<MqConn>(std::forward<Args>(args) ...); }
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

