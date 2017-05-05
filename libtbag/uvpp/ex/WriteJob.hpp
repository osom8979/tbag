/**
 * @file   WriteJob.hpp
 * @brief  WriteJob class prototype.
 * @author zer0
 * @date   2017-05-05
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_WRITEJOB_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_WRITEJOB_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/uvpp/ex/SafetyAsync.hpp>
#include <libtbag/uvpp/Stream.hpp>
#include <libtbag/uvpp/Request.hpp>

#include <vector>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

/**
 * WriteJob class prototype.
 *
 * @author zer0
 * @date   2017-05-05
 */
class TBAG_API WriteJob : public uvpp::ex::SafetyAsync::Job
{
public:
    using SafetyAsync = uvpp::ex::SafetyAsync;
    using WriteBuffer = std::vector<char>;
    using AtomicBool  = std::atomic_bool;

private:
    Stream * _stream;
    WriteRequest * _request;

    WriteBuffer _buffer;

    AtomicBool _cancel;
    AtomicBool _complete;

    uerr _result;

public:
    WriteJob(Stream * stream, WriteRequest * request, WriteBuffer buffer);
    virtual ~WriteJob();

public:
    inline void cancel() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.store(true)))
    { _cancel.store(true); }
    inline bool isCancel() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_cancel.load()))
    { return _cancel.load(); }
    inline bool isComplete() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_complete.load()))
    { return _complete.load(); }

    inline uerr getWriteResult() const TBAG_NOEXCEPT
    { return _result; }

public:
    virtual void run(SafetyAsync * handle) override;
};

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_UVPP_EX_WRITEJOB_HPP__

