/**
 * @file   NngSocket.hpp
 * @brief  NngSocket class prototype.
 * @author zer0
 * @date   2019-05-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/mq/NngBypass.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

/**
 * NngSocket class prototype.
 *
 * @author zer0
 * @date   2019-05-29
 */
class TBAG_API NngSocket
{
public:
    enum class SocketType
    {
        ST_BUS0 = 0,
        ST_PAIR0,
        ST_PAIR1,
        ST_PULL0,
        ST_PUSH0,
        ST_PUB0,
        ST_SUB0,
        ST_REP0,
        ST_REQ0,
        ST_RESPONDENT0,
        ST_SURVEYOR0,
    };

public:
    static char const * const getSocketTypeName(SocketType type) TBAG_NOEXCEPT;

public:
    /**
     * NngSocket::Impl class prototype.
     *
     * @author zer0
     * @date   2019-05-29
     *
     */
    struct TBAG_API Impl TBAG_FINAL
    {
        nng_socket  socket;
        SocketType  type;
        bool        raw;

        Impl(SocketType t, bool r);
        ~Impl();
    };

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    NngSocket();
    NngSocket(NngSocket const & obj) TBAG_NOEXCEPT;
    NngSocket(NngSocket && obj) TBAG_NOEXCEPT;
    ~NngSocket();

public:
    NngSocket & operator =(NngSocket const & obj) TBAG_NOEXCEPT;
    NngSocket & operator =(NngSocket && obj) TBAG_NOEXCEPT;

public:
    void copy(NngSocket const & obj) TBAG_NOEXCEPT;
    void swap(NngSocket & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(NngSocket & lh, NngSocket & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Impl       & ref()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & ref() const TBAG_NOEXCEPT { return *get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return ref(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return ref(); }

public:
    friend inline bool operator <(NngSocket const & x, NngSocket const & y) TBAG_NOEXCEPT
    { return x.get() < y.get(); }

    friend inline bool operator >(NngSocket const & x, NngSocket const & y) TBAG_NOEXCEPT
    { return x.get() > y.get(); }

    friend inline bool operator <=(NngSocket const & x, NngSocket const & y) TBAG_NOEXCEPT
    { return x.get() <= y.get(); }

    friend inline bool operator >=(NngSocket const & x, NngSocket const & y) TBAG_NOEXCEPT
    { return x.get() >= y.get(); }

    inline bool operator ==(NngSocket const & obj) const TBAG_NOEXCEPT
    { return get() == obj.get(); }

    inline bool operator !=(NngSocket const & obj) const TBAG_NOEXCEPT
    { return get() != obj.get(); }

public:
    void reset();

public:
    Err open(SocketType type, bool raw = false);

public:
    SocketType getType() const;
    char const * const getTypeName() const;

public:
    bool isRaw() const;

public:
    Err listen(char const * url, nng_listener * lp = nullptr, int flags = 0);
    Err dial(char const * url, nng_dialer * lp = nullptr, int flags = 0);

public:
    Err send(void * data, size_t size, int flags = 0);
    Err recv(void * data, size_t * size, int flags = 0);

public:
    Err setRecvTimeout(nng_duration ms);
    Err setSendTimeout(nng_duration ms);

    Err getRecvTimeout(nng_duration * ms) const;
    Err getSendTimeout(nng_duration * ms) const;

public:
    Err setRecvBuffer(int size);
    Err setSendBuffer(int size);

    Err getRecvBuffer(int * size) const;
    Err getSendBuffer(int * size) const;
};

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_MQ_NNGSOCKET_HPP__

