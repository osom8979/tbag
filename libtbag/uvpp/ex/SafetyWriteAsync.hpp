/**
 * @file   SafetyWriteAsync.hpp
 * @brief  SafetyWriteAsync class prototype.
 * @author zer0
 * @date   2017-05-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYWRITEASYNC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYWRITEASYNC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/id/generator/TimeId.hpp>

#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Udp.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>
#include <atomic>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {

// Forward declaration.
class Loop;

namespace ex {

/**
 * SafetyWriteAsync class prototype.
 *
 * @author zer0
 * @date   2017-05-06
 */
class TBAG_API SafetyWriteAsync : public SafetyAsync
{
public:
    friend class uvpp::Loop;

public:
    using Id = id::Id;

    using Buf   = std::vector<char>;
    using Bufs  = std::vector<Buf>;
    using Binfs = std::vector<binf>;

    using WeakStream   = std::weak_ptr<Stream>;
    using WeakUdp      = std::weak_ptr<Udp>;
    using UniqSockAddr = std::unique_ptr<sockaddr>;

public:
    enum class WriteType
    {
        STREAM_WRITE,
        UDP_SEND,
    };

    enum class WriteState : int
    {
        READY,
        CANCEL,
        WRITE,
    };

    using AtomicState = std::atomic<WriteState>;
    using AtomicError = std::atomic<uerr>;

public:
    struct TBAG_API WriterInterface : public SafetyAsync::Job
    {
        Id const id;

        Binfs infos;
        Bufs  bufs;

        AtomicState state;
        AtomicError result;

        WriterInterface();
        virtual ~WriterInterface();

        inline Id getId() const TBAG_NOEXCEPT
        { return id; }
        inline WriteState getState() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(state.load()))
        { return state.load(); }
        inline uerr getWriteError() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(result.load()))
        { return result.load(); }

        bool cancel();

        virtual WriteType getType() const = 0;
    };

    struct TBAG_API StreamWriter : public WriterInterface
    {
        WriteRequest req;
        WeakStream stream;

        virtual WriteType getType() const override
        { return WriteType::STREAM_WRITE; }

        virtual void run(SafetyAsync * handle) override;
    };

    struct TBAG_API UdpWriter : public WriterInterface
    {
        UdpSendRequest req;
        WeakUdp udp;
        UniqSockAddr addr;

        virtual WriteType getType() const override
        { return WriteType::UDP_SEND; }

        virtual void run(SafetyAsync * handle) override;
    };

public:
    using SharedWriter = std::shared_ptr<WriterInterface>;

public:
    SafetyWriteAsync(Loop & loop);
    virtual ~SafetyWriteAsync();

private:
    bool updateWriterInfo(WriterInterface * writer, binf * infos, std::size_t size);

public:
    SharedWriter asyncWrite(WeakStream stream, binf * infos, std::size_t size);
    SharedWriter asyncWrite(WeakStream stream, char const * buffer, std::size_t size);

public:
    SharedWriter asyncWrite(WeakUdp udp, binf * infos, std::size_t size, sockaddr const * addr);
    SharedWriter asyncWrite(WeakUdp udp, char const * buffer, std::size_t size, sockaddr const * addr);
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_SAFETYWRITEASYNC_HPP__

