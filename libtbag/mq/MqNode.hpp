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

#include <memory>

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

private:
    UniqueImpl _impl;

public:
    MqNode(MqParams const & params, MqMode mode);
    MqNode(std::string const & uri, MqMode mode);
    MqNode(MqNode && obj) TBAG_NOEXCEPT;
    ~MqNode();

public:
    MqNode & operator =(MqNode && obj) TBAG_NOEXCEPT;

public:
    void swap(MqNode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(MqNode & lh, MqNode & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    Err join();

public:
    Err send(MqMsg const & msg);
    Err send(char const * buffer, std::size_t size);

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

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

