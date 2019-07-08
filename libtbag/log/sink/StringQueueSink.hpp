/**
 * @file   StringQueueSink.hpp
 * @brief  StringQueueSink class prototype.
 * @author zer0
 * @date   2019-07-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STRINGQUEUESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STRINGQUEUESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/type/TypeTable.hpp>

#include <string>
#include <deque>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * StringQueueSink class prototype.
 *
 * @author zer0
 * @date   2019-07-03
 */
class TBAG_API StringQueueSink : public Sink
{
public:
    using Deque = std::deque<std::string>;

public:
    TBAG_CONSTEXPR static std::size_t const INFINITY_SIZE =
            libtbag::type::TypeInfo<std::size_t>::maximum();

public:
    std::size_t const MAX_SIZE;

private:
    Deque _deque;

public:
    StringQueueSink(std::size_t max_size = INFINITY_SIZE);
    virtual ~StringQueueSink();

public:
    inline Deque       & deque()       TBAG_NOEXCEPT { return _deque; }
    inline Deque const & deque() const TBAG_NOEXCEPT { return _deque; }

public:
    virtual bool write(int level, char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_STRINGQUEUESINK_HPP__

