/**
 * @file   DatagramAdapter.hpp
 * @brief  DatagramAdapter class prototype.
 * @author zer0
 * @date   2017-01-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/debug/ErrorCode.hpp>
#include <libtbag/container/CircularBuffer.hpp>
#include <libtbag/container/ReuseQueue.hpp>
#include <libtbag/uvpp/UvCommon.hpp>

#include <cstdlib>
#include <cstdint>

#include <limits>
#include <vector>
#include <mutex>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * Datagram interface.
 *
 * @author zer0
 * @date   2017-01-24
 */
struct TBAG_API DatagramInterface
{
    using binf = uvpp::binf;
    using Size = uint32_t;

    using Buffer         = std::vector<char>;
    using SharedBuffer   = std::shared_ptr<Buffer>;
    using SharedBuffers  = container::ReuseQueue<SharedBuffer>;
    using CircularBuffer = container::CircularBuffer<char>;

    using Mutex = std::mutex;
    using Guard = std::lock_guard<Mutex>;

#if defined(max)
    TBAG_PUSH_MACRO(max);
#undef max
#define __RESTORE_MAX__
#endif

    TBAG_CONSTEXPR static Size const    NO_NEXT_READ_SIZE = std::numeric_limits<Size>::max();
    TBAG_CONSTEXPR static Size const DATAGRAM_HEADER_SIZE = sizeof(Size);

#if defined(__RESTORE_MAX__)
    TBAG_POP_MACRO(max);
#undef __RESTORE_MAX__
#endif

    inline static  int16_t toHost( int16_t network) { return ntohs (network); }
    inline static uint16_t toHost(uint16_t network) { return ntohs (network); }
    inline static  int32_t toHost( int32_t network) { return ntohl (network); }
    inline static uint32_t toHost(uint32_t network) { return ntohl (network); }
    //inline static  int64_t toHost( int64_t network) { return ntohll(network); }
    //inline static uint64_t toHost(uint64_t network) { return ntohll(network); }

    inline static  int16_t toNetwork( int16_t host) { return htons (host); }
    inline static uint16_t toNetwork(uint16_t host) { return htons (host); }
    inline static  int32_t toNetwork( int32_t host) { return htonl (host); }
    inline static uint32_t toNetwork(uint32_t host) { return htonl (host); }
    //inline static  int64_t toNetwork( int64_t host) { return htonll(host); }
    //inline static uint64_t toNetwork(uint64_t host) { return htonll(host); }
};

/**
 * DatagramWrite class prototype.
 *
 * @author zer0
 * @date   2017-01-24
 */
class TBAG_API DatagramWrite : public DatagramInterface, public Noncopyable
{
private:
    SharedBuffers _writers;
    mutable Mutex _writers_mutex;

public:
    DatagramWrite();
    virtual ~DatagramWrite();

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_writers.empty()))
    { Guard g(_writers_mutex); return _writers.empty(); }
    inline Size size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_writers.size()))
    { Guard g(_writers_mutex); return _writers.size();  }

public:
    bool pushWriteBuffer(char const * buffer, Size size);

public:
    template <typename Predicated>
    bool popWriteBuffer(Predicated predicated)
    {
        Guard guard(_writers_mutex);

        SharedBuffer * shared = _writers.front();
        if (shared == nullptr || static_cast<bool>(shared) == false) {
            return false;
        }

        Buffer & cursor = *(shared->get());
        predicated(&cursor[0], cursor.size());

        _writers.pop();
        return true;
    }

public:
    static Size parseBufferSize(char const * buffer, Size size);
};

/**
 * DatagramRead class prototype.
 *
 * @author zer0
 * @date   2017-01-24
 */
class TBAG_API DatagramRead : public DatagramInterface, public Noncopyable
{
private:
    Buffer _read_buffer;
    Size   _next_read_size;

private:
    CircularBuffer _data_buffer;

private:
    mutable Mutex _read_mutex;

public:
    DatagramRead();
    virtual ~DatagramRead();

public:
    // @formatter:off
    inline CircularBuffer       & atDataBuffer()       TBAG_NOEXCEPT { Guard g(_read_mutex); return _data_buffer; }
    inline CircularBuffer const & atDataBuffer() const TBAG_NOEXCEPT { Guard g(_read_mutex); return _data_buffer; }
    // @formatter:on

private:
    Size readNextDatagramSize();

// Event by-pass methods.
public:
    void alloc(Size suggested_size);
    void push(char const * buffer, Size size);
    bool next(binf * result);
};

/**
 * DatagramAdapter class prototype.
 *
 * @author zer0
 * @date   2017-01-02
 */
class TBAG_API DatagramAdapter : public DatagramWrite, public DatagramRead
{
public:
    DatagramAdapter();
    virtual ~DatagramAdapter();
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_DATAGRAMADAPTER_HPP__

