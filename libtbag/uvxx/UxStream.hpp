/**
 * @file   UxStream.hpp
 * @brief  UxStream class prototype.
 * @author zer0
 * @date   2018-12-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSTREAM_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSTREAM_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvxx/UxHandle.hpp>
#include <libtbag/uvpp/func/FunctionalStream.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvxx {

// Forward declaration.
class UxLoop;

/**
 * UxStream class prototype.
 *
 * @author zer0
 * @date   2018-12-07
 */
class TBAG_API UxStream : public libtbag::uvxx::UxHandle
{
public:
    using Stream       = libtbag::uvpp::Stream;
    using WeakStream   = std::weak_ptr<Stream>;
    using SharedStream = std::shared_ptr<Stream>;

public:
    using ShutdownRequest = libtbag::uvpp::ShutdownRequest;
    using WriteRequest    = libtbag::uvpp::WriteRequest;

public:
    using binf = libtbag::uvpp::binf;

public:
    TBAG_CONSTEXPR static int const BACKLOG_LIMIT = libtbag::uvpp::BACKLOG_LIMIT;

public:
    UxStream();
    UxStream(UxStream const & obj) TBAG_NOEXCEPT;
    UxStream(UxStream && obj) TBAG_NOEXCEPT;
    ~UxStream();

public:
    UxStream & operator =(UxStream const & obj) TBAG_NOEXCEPT;
    UxStream & operator =(UxStream && obj) TBAG_NOEXCEPT;

public:
    void copy(UxStream const & obj) TBAG_NOEXCEPT;
    void swap(UxStream & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(UxStream & lh, UxStream & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    SharedStream lockStream() const TBAG_NOEXCEPT_SP_OP(_handle.lock())
    { return std::static_pointer_cast<Stream>(_handle.lock()); }

public:
    std::size_t getWriteQueueSize() const;

public:
    bool isReadable() const;
    bool isWritable() const;

public:
    Err setBlocking(bool enable = true);

public:
    Err shutdown(ShutdownRequest & request);
    Err listen(int backlog = BACKLOG_LIMIT);
    Err accept(UxStream & client);

public:
    Err startRead();
    Err stopRead();

public:
    Err write(WriteRequest & request, binf const * infos, std::size_t infos_size);
    Err write(WriteRequest & request, char const * buffer, std::size_t size);

public:
    std::size_t tryWrite(binf * infos, std::size_t infos_size, Err * result = nullptr);
    std::size_t tryWrite(char const * buffer, std::size_t size, Err * result = nullptr);
};

} // namespace uvxx

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVXX_UXSTREAM_HPP__

