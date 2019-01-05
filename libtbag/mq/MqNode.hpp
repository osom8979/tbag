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
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/mq/details/MqCommon.hpp>

#include <string>
#include <vector>
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
 */
class TBAG_API MqNode : private Noncopyable
{
public:
    struct Impl;
    friend struct Impl;

public:
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqParams       = libtbag::mq::details::MqParams;

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

private:
    UniqueImpl _impl;

public:
    MqNode(MqParams const & params, MqMode mode, Callbacks const & cbs = Callbacks{});
    MqNode(std::string const & uri, MqMode mode, Callbacks const & cbs = Callbacks{});
    MqNode(MqNode && obj) TBAG_NOEXCEPT;
    virtual ~MqNode();

public:
    MqNode & operator =(MqNode && obj) TBAG_NOEXCEPT;

public:
    void swap(MqNode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(MqNode & lh, MqNode & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

protected:
    virtual bool onAccept(std::string const & peer);
    virtual bool onWrite(MqMsg & msg);
    virtual bool onRecv(MqMsg const & msg);

public:
    MqParams params() const;

public:
    Err waitEnable(uint64_t timeout_nano = 0);

public:
    Err join();

public:
    Err send(MqMsg const & msg);

public:
    Err send(char const * buffer, std::size_t size);
    Err send(MqEvent event, char const * buffer, std::size_t size);

public:
    Err send(std::string const & text);
    Err send(MqEvent event, std::string const & text);

public:
    Err send(MqMsg::Buffer const & buffer);
    Err send(MqEvent event, MqMsg::Buffer const & buffer);

public:
    Err sendCloseMsg();

public:
    Err recv(MqMsg & msg);
    Err recvWait(MqMsg & msg, uint64_t timeout_nano = 0);

public:
    static MqParams getParams(std::string const & uri);

public:
    static MqNode bind(MqParams const & params);
    static MqNode bind(std::string const & uri);

    static MqNode connect(MqParams const & params);
    static MqNode connect(std::string const & uri);
};

// ----------
// Utilities.
// ----------

using MqUniqueNode = std::unique_ptr<MqNode>;
using MqSharedNode = std::shared_ptr<MqNode>;

using MqParams  = libtbag::mq::details::MqParams;
using MqNodeCbs = MqNode::Callbacks;

TBAG_API MqUniqueNode bindUniqueNode(MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API MqUniqueNode bindUniqueNode(std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

TBAG_API MqUniqueNode connectUniqueNode(MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API MqUniqueNode connectUniqueNode(std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

TBAG_API MqSharedNode bindSharedNode(MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API MqSharedNode bindSharedNode(std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

TBAG_API MqSharedNode connectSharedNode(MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API MqSharedNode connectSharedNode(std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

TBAG_API Err bind(MqUniqueNode & node, MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err bind(MqUniqueNode & node, std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err bind(MqSharedNode & node, MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err bind(MqSharedNode & node, std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

TBAG_API Err connect(MqUniqueNode & node, MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err connect(MqUniqueNode & node, std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err connect(MqSharedNode & node, MqParams const & params, MqNodeCbs const & cbs = MqNodeCbs{});
TBAG_API Err connect(MqSharedNode & node, std::string const & uri, MqNodeCbs const & cbs = MqNodeCbs{});

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

