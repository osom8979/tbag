/**
 * @file   DatagramCodec.hpp
 * @brief  DatagramCodec class prototype.
 * @author zer0
 * @date   2017-01-02
 * @date   2017-05-13 (Rename: DatagramAdapter -> DatagramCodec)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CODEC_DATAGRAMCODEC_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CODEC_DATAGRAMCODEC_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>

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

namespace codec {

/**
 * Datagram interface.
 *
 * @author zer0
 * @date   2017-01-24
 */
struct DatagramTypes
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
};

/**
 * DatagramEncoder class prototype.
 *
 * @author zer0
 * @date   2017-01-24
 * @date   2017-05-13 (Rename: DatagramWrite -> DatagramEncoder)
 */
class TBAG_API DatagramEncoder : public DatagramTypes, private Noncopyable
{
private:
    SharedBuffers _writers;
    mutable Mutex _writers_mutex;

public:
    DatagramEncoder();
    virtual ~DatagramEncoder();

public:
    inline bool empty() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_writers.empty()))
    { Guard g(_writers_mutex); return _writers.empty(); }
    inline Size size() const TBAG_NOEXCEPT_SPECIFIER(TBAG_NOEXCEPT_OPERATOR(_writers.size()))
    { Guard g(_writers_mutex); return _writers.size();  }

public:
    bool pushWriteBuffer(char const * buffer, Size size);

public:
    template <typename Predicated>
    bool popWriteBuffer(Predicated predicated)
    {
        Guard guard(_writers_mutex);

        SharedBuffer shared;
        if (_writers.front(shared) != Err::E_SUCCESS) {
            return false;
        }

        Buffer & cursor = *shared;
        predicated(&cursor[0], cursor.size());

        _writers.pop();
        return true;
    }

public:
    static Size parseBufferSize(char const * buffer, Size size);
};

/**
 * DatagramDecoder class prototype.
 *
 * @author zer0
 * @date   2017-01-24
 * @date   2017-05-13 (Rename: DatagramRead -> DatagramDecoder)
 */
class TBAG_API DatagramDecoder : public DatagramTypes, private Noncopyable
{
private:
    Buffer _read_buffer;
    Size   _next_read_size;

private:
    CircularBuffer _data_buffer;

private:
    mutable Mutex _read_mutex;

public:
    DatagramDecoder();
    virtual ~DatagramDecoder();

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
 * DatagramCodec class prototype.
 *
 * @author zer0
 * @date   2017-01-02
 * @date   2017-05-13 (Rename: DatagramAdapter -> DatagramCodec)
 */
class TBAG_API DatagramCodec : public DatagramEncoder, public DatagramDecoder
{
public:
    DatagramCodec();
    virtual ~DatagramCodec();
};

} // namespace codec

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CODEC_DATAGRAMCODEC_HPP__

