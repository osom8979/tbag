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
    bool reopen(std::string const & path)
    {
        if (_file.isOpen()) {
            _file.close();
        }
        assert(_file.isOpen() == false);

        Path parent_dir = Path(path).getParent();
        if (parent_dir.exists() == false) {
            parent_dir.createDir();
        }

        if (parent_dir.isDirectory() == false) {
            std::cerr << "RotateFileSink::reopen() Parent is not a directory: " << parent_dir.toString() << std::endl;
            return false;
        }
        if (parent_dir.isWritable() == false) {
            std::cerr << "RotateFileSink::reopen() Parent is not a writable: " << parent_dir.toString() << std::endl;
            return false;
        }
        if (_file.open(path, File::Flags().clear().creat().rdwr().append()) == false) {
            std::cerr << "RotateFileSink::reopen() File open error: " << path << std::endl;
            return false;
        }
        assert(_file.isOpen());
        return true;
    }

    bool reopen()
    {
        return reopen(_path.getPath().toString());
    }

    bool update()
    {
        return _path.update();
    }

    bool init()
    {
        return update() && reopen();
    }

public:
    bool isOpen() const
    {
        return _file.isOpen();
    }

public:
    virtual void write(String const & message) override
    {
        if (_path.next(message.data(), message.size())) {
            if (reopen() == false) {
                std::cerr << "RotateFileSink::write() Next open error!\n";
            }
        }

        if (isOpen() == false) {
            // Retry the open.
            update();
            reopen();

            if (isOpen() == false) {
                std::cerr << "RotateFileSink::write() Retry open error!\n";
                assert(false && "File open error!");
                return;
            }
        }

        assert(isOpen());
        _file.write(message.c_str(), message.size(), -1);
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

