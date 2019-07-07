/**
 * @file   RotateFileSink.hpp
 * @brief  RotateFileSink class prototype.
 * @author zer0
 * @date   2017-04-13
 * @date   2019-07-02 (Create a cpp file)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/sink/Sink.hpp>

#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/RotatePath.hpp>

#include <string>

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
 * @date   2019-07-02 (Create a cpp file)
 */
class TBAG_API RotateFileSink : public Sink
{
public:
    using Path = libtbag::filesystem::Path;
    using File = libtbag::filesystem::File;

public:
    using RotatePath = libtbag::filesystem::RotatePath;

public:
    using CheckerInterface  = libtbag::filesystem::CheckerInterface;
    using SizeChecker       = libtbag::filesystem::SizeChecker;

public:
    using UpdaterInterface  = libtbag::filesystem::UpdaterInterface;
    using CounterUpdater    = libtbag::filesystem::CounterUpdater;
    using TimeFormatUpdater = libtbag::filesystem::TimeFormatUpdater;

private:
    RotatePath  _rotate;
    File        _file;

private:
    bool _verbose;

public:
    RotateFileSink(std::string const & arguments, bool verbose = true);
    virtual ~RotateFileSink();

private:
    void close();
    bool reopen(std::string const & path);
    bool reopen();
    bool update();
    bool isOpen() const;

public:
    virtual bool write(char const * message, int size) override;
    virtual void flush() override;
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

