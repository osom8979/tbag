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
class TBAG_API MqNode
{
public:
    struct Impl;
    friend struct Impl;

public:
    using MqEvent        = libtbag::mq::details::MqEvent;
    using MqType         = libtbag::mq::details::MqType;
    using MqRequestState = libtbag::mq::details::MqRequestState;
    using MqMachineState = libtbag::mq::details::MqMachineState;
    using MqMsg          = libtbag::mq::details::MqMsg;
    using MqMode         = libtbag::mq::details::MqMode;
    using MqParams       = libtbag::mq::details::MqParams;

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    MqNode();
    MqNode(MqNode const & obj) TBAG_NOEXCEPT;
    MqNode(MqNode && obj) TBAG_NOEXCEPT;
    ~MqNode();

public:
    MqNode & operator =(MqNode const & obj) TBAG_NOEXCEPT;
    MqNode & operator =(MqNode && obj) TBAG_NOEXCEPT;

public:
    void copy(MqNode const & obj) TBAG_NOEXCEPT;
    void swap(MqNode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(MqNode & lh, MqNode & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(MqNode const & x, MqNode const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(MqNode const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(MqNode const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    static MqParams getParams(std::string const & uri);

public:
    Err bind(MqParams const & params);
    Err bind(std::string const & uri);

public:
    Err connect(MqParams const & params);
    Err connect(std::string const & uri);

public:
    Err close();

public:
    Err send(MqMsg const & msg);
    Err send(char const * buffer, std::size_t size);

public:
    Err recv(MqMsg & msg);

public:
    Err recvWait(MqMsg & msg, uint64_t timeout_nano = 0);
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_MQNODE_HPP__

