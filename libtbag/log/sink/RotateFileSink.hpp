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
#include <libtbag/log/sink/Sink.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <iostream>

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
class RotateFileSink : public Sink<MutexType>
{
public:
    using Parent = Sink<MutexType>;
    using String = typename Parent::String;
    using Mutex  = typename Parent::Mutex;
    using Path   = filesystem::Path;
    using File   = filesystem::File;

    using RotatePath    = filesystem::RotatePath;
    using SharedChecker = RotatePath::SharedChecker;
    using WeakChecker   = RotatePath::WeakChecker;
    using SharedUpdater = RotatePath::SharedUpdater;
    using WeakUpdater   = RotatePath::WeakUpdater;

    using SizeChecker       = RotatePath::SizeChecker;
    using CounterUpdater    = RotatePath::CounterUpdater;
    using TimeFormatUpdater = RotatePath::TimeFormatUpdater;

private:
    RotatePath _path;
    File _file;

public:
    RotateFileSink(std::string const & path, SharedChecker const & checker, SharedUpdater const & updater, bool force_flush = false)
            : Parent(force_flush), _path(Path(path), checker, updater)
    {
        if (init() == false) {
            throw std::bad_exception();
        }
    }

    RotateFileSink(std::string const & path, std::size_t size = SizeChecker::getDefaultMaxSize(), bool force_flush = false)
            : Parent(force_flush), _path(Path(path), size)
    {
        if (init() == false) {
            throw std::bad_exception();
        }
    }

    virtual ~RotateFileSink()
    {
        if (_file.isOpen()) {
            _file.close();
        }
    }

public:
    inline RotatePath const & atRotatePath() const TBAG_NOEXCEPT { return _path; }
    inline File const & atFile() const TBAG_NOEXCEPT { return _file; }

private:
    bool init()
    {
        if (_path.update() == false) {
            return false;
        }
        if (_file.open(_path.getPath().toString(), File::Flags().clear().creat().rdwr().append()) == false) {
            return false;
        }
        assert(_path.getPath().exists());
        assert(_file.isOpen());
        return true;
    }

    bool reopen(std::string const & path)
    {
        if (_file.isOpen()) {
            _file.close();
        }
        assert(_file.isOpen() == false);
        if (_file.open(path, File::Flags().clear().creat().rdwr().append()) == false) {
            return false;
        }
        assert(_file.isOpen());
        return true;
    }

public:
    virtual void write(String const & message) override
    {
        if (_path.next(message.data(), message.size())) {
            if (reopen(_path.getPath().toString()) == false) {
                std::cerr << "RotateFileSink::write() next open error!\n";
            }
        }

        if (_file.isOpen()) {
            _file.write(message.c_str(), message.size(), -1);
        } else {
            if (_path.update() && reopen(_path.getPath().toString())) {
                // Re-try.
                _file.write(message.c_str(), message.size(), -1);
            } else {
                std::cerr << "RotateFileSink::write() File open error.\n";
            }
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

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

