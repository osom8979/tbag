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
#include <sstream>
#include <queue>

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
    using Path   = libtbag::filesystem::Path;
    using File   = libtbag::filesystem::File;

    using RotatePath    = libtbag::filesystem::RotatePath;
    using SharedChecker = RotatePath::SharedChecker;
    using WeakChecker   = RotatePath::WeakChecker;
    using SharedUpdater = RotatePath::SharedUpdater;
    using WeakUpdater   = RotatePath::WeakUpdater;

    using SizeChecker       = RotatePath::SizeChecker;
    using CounterUpdater    = RotatePath::CounterUpdater;
    using TimeFormatUpdater = RotatePath::TimeFormatUpdater;

    using PathQueue = std::queue<Path>;

public:
    TBAG_CONSTEXPR static char const * const TYPE_NAME = "ROTATE_FILE_SINK";
    TBAG_CONSTEXPR static char const * const HISTORY_SUFFIX = ".history";

    TBAG_CONSTEXPR static std::size_t const DONT_REMOVE_HISTORY =  0;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_MAX_HISTORY = 10;

    TBAG_CONSTEXPR static std::size_t getDontRemoveHistory() TBAG_NOEXCEPT { return DONT_REMOVE_HISTORY; }
    TBAG_CONSTEXPR static std::size_t getDefaultMaxHistory() TBAG_NOEXCEPT { return DEFAULT_MAX_HISTORY; }

public:
    Path const HISTORY_PATH;

private:
    PathQueue   _history;
    std::size_t _max_history;

    RotatePath  _rotate_path;
    File        _file;

public:
    RotateFileSink(std::string const & path,
                   SharedChecker const & checker,
                   SharedUpdater const & updater,
                   std::size_t max_history = getDefaultMaxHistory(),
                   bool force_flush = false)
            : Parent(force_flush),
              HISTORY_PATH(path + HISTORY_SUFFIX),
              _rotate_path(Path(path), checker, updater),
              _max_history(max_history)
    {
        if (init() == false) {
            throw std::bad_exception();
        }
    }

    RotateFileSink(std::string const & path,
                   std::size_t size = SizeChecker::getDefaultMaxSize(),
                   std::size_t max_history = getDefaultMaxHistory(),
                   bool force_flush = false)
            : Parent(force_flush),
              HISTORY_PATH(path + HISTORY_SUFFIX),
              _rotate_path(Path(path), size),
              _max_history(max_history)
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
    inline PathQueue       & atHistory()       TBAG_NOEXCEPT { return _history; }
    inline PathQueue const & atHistory() const TBAG_NOEXCEPT { return _history; }

    inline bool isEnableHistory() const TBAG_NOEXCEPT { return _max_history != getDontRemoveHistory(); }

    inline std::size_t getMaxHistory() const TBAG_NOEXCEPT { return _max_history; }
    inline RotatePath const & atRotatePath() const TBAG_NOEXCEPT { return _rotate_path; }
    inline File const & atFile() const TBAG_NOEXCEPT { return _file; }

private:
    void readHistoryFile()
    {
        using namespace libtbag::filesystem;
        auto const PATH = Path(HISTORY_PATH);
        if (PATH.isRegularFile() == false) {
            return;
        }

        std::string content;
        if (isSuccess(readFile(HISTORY_PATH, content))) {
            using namespace libtbag::string;
            auto list = splitTokens(content, NEW_LINE);
            for (auto & path : list) {
                updateHistory(path);
            }
        }
    }

    bool saveHistoryFile() const
    {
        auto history_clone = _history;
        std::stringstream ss;
        while (history_clone.empty() == false) {
            using namespace libtbag::string;
            ss << history_clone.front().toString() << NEW_LINE;
            history_clone.pop();
        }
        return isSuccess(libtbag::filesystem::writeFile(HISTORY_PATH, ss.str()));
    }

public:
    void updateHistory(std::string const & path)
    {
        if (isEnableHistory() == false) {
            return;
        }

        assert(_max_history >= 1);
        while (_history.size() >= _max_history) {
            auto & LEGACY_PATH = _history.front();
            if (LEGACY_PATH.remove() == false) {
                std::cerr << "RotateFileSink::updateHistory() Remove legacy file error!\n";
            }
            _history.pop();
        }
        _history.push(Path(path));
    }

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
        updateHistory(path);

        if (isEnableHistory()) {
            saveHistoryFile();
        }
        return true;
    }

    bool reopen()
    {
        return reopen(_rotate_path.getPath().toString());
    }

    bool update()
    {
        return _rotate_path.update();
    }

    bool init()
    {
        if (isEnableHistory()) {
            readHistoryFile();
        }
        return update() && reopen();
    }

    bool isOpen() const
    {
        return _file.isOpen();
    }

public:
    virtual char const * const name() const override
    {
        return TYPE_NAME;
    }

    virtual void write(String const & message) override
    {
        if (_rotate_path.next(message.data(), message.size())) {
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

public:
    TBAG_CONSTEXPR static char const * const MAX_HISTORY_KEY = "MAX_HISTORY";

public:
    virtual void set(std::string const & key, std::string const & val) override
    {
        if (key == MAX_HISTORY_KEY) {
            int change_value = 0;
            if (libtbag::string::toVal(val, change_value)) {
                _max_history = change_value;
            }
        }
    }

    virtual std::string get(std::string const & key) const override
    {
        if (key == MAX_HISTORY_KEY) {
            return libtbag::string::toString(_max_history);
        }
        return std::string();
    }
};

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_SINK_ROTATEFILESINK_HPP__

