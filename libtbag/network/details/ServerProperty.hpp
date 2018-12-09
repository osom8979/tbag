/**
 * @file   ServerProperty.hpp
 * @brief  ServerProperty class prototype.
 * @author zer0
 * @date   2017-09-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_SERVERPROPERTY_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_SERVERPROPERTY_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <libtbag/id/Id.hpp>
#include <libtbag/net/Ip.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Request.hpp>
#include <libtbag/uvpp/Loop.hpp>

#include <string>
#include <unordered_map>
#include <memory>
#include <atomic>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace details {

struct ServerTypes
{
    using Loop   = uvpp::Loop;
    using Stream = uvpp::Stream;

    using SharedStream = std::shared_ptr<Stream>;
    using WeakStream   = std::weak_ptr<Stream>;

    using SafetyAsync       = uvpp::ex::SafetyAsync;
    using JobInterface      = SafetyAsync::JobInterface;
    using FunctionalJob     = SafetyAsync::FunctionalJob;
    using SharedJob         = SafetyAsync::SharedJob;
    using SharedSafetyAsync = std::shared_ptr<SafetyAsync>;
    using WeakSafetyAsync   = std::weak_ptr<SafetyAsync>;

    using ClientMap = std::unordered_map<ServerInterface::Id, ServerInterface::SharedClient>;

    using key_type        = typename ClientMap::key_type;
    using mapped_type     = typename ClientMap::mapped_type;
    using hasher          = typename ClientMap::hasher;
    using key_equal       = typename ClientMap::key_equal;
    using allocator_type  = typename ClientMap::allocator_type;
    using value_type      = typename ClientMap::value_type;
    using reference       = typename ClientMap::reference;
    using const_reference = typename ClientMap::const_reference;
    using pointer         = typename ClientMap::pointer;
    using const_pointer   = typename ClientMap::const_pointer;
    using size_type       = typename ClientMap::size_type;
    using difference_type = typename ClientMap::difference_type;
    using iterator        = typename ClientMap::iterator;
    using const_iterator  = typename ClientMap::const_iterator;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

    using AtomicBool = std::atomic_bool;
};

/**
 * ServerProperty structure.
 *
 * @author zer0
 * @date   2017-09-28
 */
struct ServerProperty : public ServerTypes
{
    SharedStream       server_stream;
    SharedSafetyAsync  safety_async;
    ClientMap          clients;
    AtomicBool         on_connection;

    int  max_connection_count; ///< Maximum connection count.
    int  cur_connection_count; ///< Current connection count.

    ServerProperty() : on_connection(false),
                       max_connection_count(details::MAXIMUM_CONNECTION_SIZE),
                       cur_connection_count(0)
    { /* EMPTY. */ }
    ~ServerProperty()
    { /* EMPTY. */ }
};

} // namespace details
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DETAILS_SERVERPROPERTY_HPP__

