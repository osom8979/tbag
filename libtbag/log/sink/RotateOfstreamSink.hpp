/**
 * @file   RotateOfstreamSink.hpp
 * @brief  RotateOfstreamSink class prototype.
 * @author zer0
 * @date   2016-07-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEOFSTREAMSINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEOFSTREAMSINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/OfstreamSink.hpp>
#include <libtbag/time/Time.hpp>
#include <libtbag/string/Strings.hpp>

#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * StreamSink class prototype.
 *
 * @author zer0
 * @date   2016-07-10
 */
template <typename Mutex = lock::FakeLock, typename CharType = char>
class RotateOfstreamSink : public OfstreamSink<Mutex, CharType>
{
public:
    using Parent = OfstreamSink<Mutex, CharType>;
    using String = typename Parent::String;

public:
    RotateOfstreamSink(String const & path, bool force_flush = false)
            : Parent(getInsertedDateIntoFilename(path), force_flush)
    {
        // EMPTY.
    }

    virtual ~RotateOfstreamSink() = default;

public:
    static String getNowTimeString()
    {
        std::basic_stringstream<CharType> ss;

        // Timestamp.
        auto tp = time::getNowSystemClock();
        tm time = {0,};

        if (time::getLocalTime(time::getTime(tp), &time)) {
            ss << time::getFormatString(time::getDefaultTimestampShortFormat<CharType>(), &time);
        } else {
            ss << CHAR_OR_WIDECHAR(CharType, "SINCE") << tp.time_since_epoch().count();
        }

        return ss.str();
    }

    static String getInsertedDateIntoFilename(String const & path)
    {
        return path + CHAR_OR_WIDECHAR(CharType, ".") + getNowTimeString();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEOFSTREAMSINK_HPP__

