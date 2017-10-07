/**
 * @file   StreamServer.hpp
 * @brief  StreamServer class prototype.
 * @author zer0
 * @date   2017-05-10
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <libtbag/network/details/FunctionalNet.hpp>
#include <libtbag/network/details/ServerProperty.hpp>
#include <libtbag/network/stream/StreamNode.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace stream  {

/**
 * StreamServer class prototype.
 *
 * @author zer0
 * @date   2017-05-10
 */
class TBAG_API StreamServer
        : public details::ServerInterface,
          public details::ServerTypes,
          private Noncopyable
{
public:
    using StreamType = details::StreamType;

public:
    using SharedStreamNode = std::shared_ptr<StreamNode>;
    STATIC_ASSERT_CHECK_IS_BASE_OF(typename details::ServerInterface::SharedClient::element_type,
                                   typename SharedStreamNode::element_type);

public:
    /**
     * ClientGuard class prototype.
     *
     * @author zer0
     * @date   2017-08-10
     * @date   2017-09-28 (Rename: ClientIteratorGuard -> ClientGuard)
     *
     * @remarks
     *  Use a mutex to protect the thread.
     */
    class TBAG_API ClientGuard : private Noncopyable
    {
    private:
        StreamServer * _parent;

    public:
        ClientGuard(StreamServer * parent);
        ~ClientGuard();

    public:
        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

        bool empty() const;
        size_type size() const;
        size_type max_size() const;
    };

    friend class ClientGuard;
    using UniqueClientGuard = std::unique_ptr<ClientGuard>;

public:
    class Internal;
    friend class Internal;
    using UniqueInternal = std::unique_ptr<Internal>;

private:
    mutable Mutex _mutex;
    UniqueInternal _internal;

public:
    StreamServer(Loop & loop, StreamType type);
    virtual ~StreamServer();

public:
    bool empty() const;
    std::size_t size() const;

    WeakStream getServer();
    WeakSafetyAsync getAsync();

public:
    Err sendJob(SharedJob job);

public:
    /** Create(new) & push & send job. */
    template <typename JobType, typename ... Args>
    inline std::shared_ptr<typename remove_cr<JobType>::type> newSendJob(Args && ... args)
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(JobInterface, JobType);
        typedef typename remove_cr<JobType>::type ResultJobType;
        typedef std::shared_ptr<ResultJobType> ResultSharedJobType;
        SharedJob shared = SharedJob(new (std::nothrow) JobType(std::forward<Args>(args) ...));
        if (static_cast<bool>(shared)) {
            sendJob(shared);
            return std::static_pointer_cast<ResultJobType, JobInterface>(shared);
        }
        return ResultSharedJobType();
    }

    template <typename ... Args>
    inline std::shared_ptr<FunctionalJob> newSendFunc(Args && ... args)
    {
        return newSendJob<FunctionalJob, Args ...>(std::forward<Args>(args) ...);
    }

public:
    UniqueClientGuard getIterators();

public:
    template <typename Predicated>
    void forEach(Predicated predicated)
    {
        auto itrs = getIterators();
        auto const END = itrs->end();
        for (auto itr = itrs->begin(); itr != END; ++itr) {
            predicated(*itr);
        }
    }

public:
    virtual std::string dest() const override;
    virtual int         port() const override;

public:
    /** Initialize this class. */
    virtual Err init(char const * destination, int port = 0) override;

    /**
     * Safety close() operation.
     *
     * @remarks
     *  Select sync/async operations automatically according to Thread ID.
     */
    virtual Err close() override;

    /**
     * Accept client.
     *
     * @warning
     *  This operation can only used with the onConnection() method.
     */
    virtual WeakClient accept() override;

    /**
     * Obtain client.
     *
     * @param[in] id Node id.
     */
    virtual WeakClient get(Id id) override;

    /**
     * Remove client in buffer(map).
     *
     * @param[in] id Node id.
     */
    virtual Err remove(Id id) override;

public:
    virtual void backConnection(Err code) override;
    virtual void backClose() override;

// StreamServer extension.
public:
    /**
     * Create an instance of node.
     *
     * @warning Don't use the mutex.
     */
    virtual SharedStreamNode createClient(StreamType type, Loop & loop, SharedStream & server);

public:
    template <typename T>
    inline static std::shared_ptr<T> castSharedClient(WeakClient const & client)
            TBAG_NOEXCEPT_SPECIFIER(
                    TBAG_NOEXCEPT_OPERATOR(client.lock()) &&
                    TBAG_NOEXCEPT_OPERATOR(std::static_pointer_cast<T>(SharedClient()))
            )
    {
        return std::static_pointer_cast<T>(client.lock());
    }
};

/**
 * TcpServer class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
struct TcpServer : public stream::StreamServer
{
    TcpServer(Loop & loop) : stream::StreamServer(loop, StreamType::TCP)
    { /* EMPTY. */ }
    ~TcpServer()
    { /* EMPTY. */ }
};

/**
 * PipeServer class prototype.
 *
 * @author zer0
 * @date   2017-05-09
 */
struct PipeServer : public stream::StreamServer
{
    PipeServer(Loop & loop) : stream::StreamServer(loop, StreamType::PIPE)
    { /* EMPTY. */ }
    ~PipeServer()
    { /* EMPTY. */ }
};

using FuncTcpServer  = details::FunctionalServer<TcpServer>;
using FuncPipeServer = details::FunctionalServer<PipeServer>;

} // namespace stream
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_STREAM_STREAMSERVER_HPP__

