/**
 * @file   Logger.hpp
 * @brief  Logger class prototype.
 * @author zer0
 * @date   2016-07-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log     {
namespace details {

/**
 * Logger class prototype.
 *
 * @author zer0
 * @date   2016-07-15
 */
class Logger : public Noncopyable
{
public:
    using CharType = char;
    using SinkType = sink::BaseSinkInterface<CharType>;
    using SinkPtr  = std::unique_ptr<SinkType>;

private:
    SinkPtr _sink;

public:
    Logger();
    virtual ~Logger();
};

} // namespace details
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_DETAILS_LOGGER_HPP__

