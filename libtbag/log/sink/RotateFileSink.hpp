/**
 * @file   RotateFileSink.hpp
 * @brief  RotateFileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/FileSink.hpp>
#include <libtbag/time/TimePoint.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * RotateFileSink class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 */
template <typename MutexType = lock::FakeLock>
class RotateFileSink : public FileSink<MutexType>
{
public:
    using Parent = FileSink<MutexType>;
    using String = typename Parent::String;
    using Mutex  = typename Parent::Mutex;
    using File   = filesystem::File;

public:
    RotateFileSink(std::string const & path, bool force_flush = false) : Parent(getInsertedDateIntoFilename(path), force_flush)
    { /* EMPTY. */ }
    virtual ~RotateFileSink()
    { /* EMPTY. */ }

public:
    static std::string getNowTimeString()
    {
        return time::TimePoint::now().toShortString();
    }

    static std::string getInsertedDateIntoFilename(std::string const & path)
    {
        return path + "." + getNowTimeString();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

