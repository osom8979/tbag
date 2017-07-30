/**
 * @file   FileSink.hpp
 * @brief  FileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/filesystem/File.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

/**
 * FileSink class prototype.
 *
 * @author zer0
 * @date   2017-04-13
 */
template <typename MutexType = lock::FakeLock>
class FileSink : public Sink<MutexType>
{
public:
    using Parent = Sink<MutexType>;
    using String = typename Parent::String;
    using Mutex  = typename Parent::Mutex;
    using File   = filesystem::File;

private:
    File _file;

public:
    FileSink(std::string const & path, bool force_flush = false) : Parent(force_flush)
    {
        if (_file.open(path, File::Flags().clear().creat().rdwr().append()) == false) {
            throw std::bad_exception();
        }
    }

    virtual ~FileSink()
    {
        _file.close();
    }

public:
    virtual void write(String const & message) override
    {
        if (_file.isOpen()) {
            _file.write(message.c_str(), message.size(), -1);
        }
    }

    virtual void flush() override
    {
        // EMPTY.
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_FILESINK_HPP__

