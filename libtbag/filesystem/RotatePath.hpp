/**
 * @file   RotatePath.hpp
 * @brief  RotatePath class prototype.
 * @author zer0
 * @date   2017-07-31
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>

#include <libtbag/debug/Assert.hpp>
#include <libtbag/archive/Archive.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <cassert>
#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * Condition check and data writer interface of RotatePath.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct WriterInterface
{
    using Err = libtbag::Err;

    WriterInterface() { /* EMPTY. */ }
    virtual ~WriterInterface() { /* EMPTY. */ }

    virtual bool ready() const = 0;
    virtual bool open(char const * path) = 0;
    virtual void flush() = 0;
    virtual void close() = 0;

    /**
     * Write method.
     *
     * @param[in] buffer
     *      If you want a wirte in file, this parameter is the source buffer.
     * @param[in] size
     *      If you want a wirte in file, this parameter is the size of source buffer.
     * @param[out] written
     *      If the function returns successfully, it returns the size written.
     *
     * @retval E_SUCCESS
     * @retval E_CONTINUE
     *
     * @retval E_EOPEN
     * @retval E_WRERR
     * @retval E_SMALLBUF
     * @retval E_UNKNOWN
     */
    virtual Err write(char const * buffer, int size, int * written) = 0;
};

/**
 * Maximum file size check and writer.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct MaxSizeWriter : public WriterInterface
{
    using File = libtbag::filesystem::File;

    TBAG_CONSTEXPR static std::size_t const DEFAULT_MAX_SIZE = 1 * MEGA_BYTE_TO_BYTE;

    std::size_t max_size;
    File file;

    MaxSizeWriter(std::size_t size = DEFAULT_MAX_SIZE) : max_size(size)
    { /* EMPTY. */ }

    virtual ~MaxSizeWriter()
    {
        if (ready()) {
            flush();
            close();
        }
    }

    virtual bool ready() const override
    {
        return file.isOpen();
    }

    virtual bool open(char const * path) override
    {
        using namespace libtbag::filesystem::details;
        if (file.open(path, File::Flags(FILE_OPEN_FLAG_WRITE_ONLY | FILE_OPEN_CREATE))) {
            file.seek(0);
            return true;
        }
        return false;
    }

    virtual void flush() override
    {
        // EMPTY.
    }

    virtual void close() override
    {
        file.close();
    }

    virtual Err write(char const * buffer, int size, int * written) override
    {
        assert(buffer != nullptr);
        assert(written != nullptr);

        if (!file.isOpen()) {
            return E_OPEN;
        }

        if (file.offset() >= max_size) {
            *written = 0;
            return E_SMALLBUF;
        }

        assert(file.offset() < max_size);
        auto const MAXIMUM_WRITE_SIZE = file.offset() + size;

        if (MAXIMUM_WRITE_SIZE == max_size) {
            *written = file.write(buffer, size);
            if (*written > 0 && *written == size) {
                return E_CONTINUE;
            } else {
                return E_WRERR;
            }

        } else if (MAXIMUM_WRITE_SIZE < max_size) {
            *written = file.write(buffer, size);
            if (*written > 0 && *written == size) {
                return E_SUCCESS;
            } else {
                return E_WRERR;
            }

        } else {
            assert(MAXIMUM_WRITE_SIZE > max_size);
            assert(max_size > file.offset());
            *written = file.write(buffer, max_size - file.offset());
            if (*written > 0) {
                return E_SMALLBUF;
            } else {
                return E_WRERR;
            }
        }

        TBAG_INACCESSIBLE_BLOCK_ASSERT();
        return E_UNKNOWN;
    }
};

/**
 * Rules of redefine the path.
 *
 * @author zer0
 * @date   2017-11-20
 */
struct UpdaterInterface
{
    UpdaterInterface() { /* EMPTY. */ }
    virtual ~UpdaterInterface() { /* EMPTY. */ }

    virtual Path update(Path const & prev) = 0;
};

/**
 * Count based updater.
 *
 * @author zer0
 * @date   2017-11-20
 */
struct CounterUpdater : public UpdaterInterface
{
    TBAG_CONSTEXPR static char const * const DEFAULT_PREFIX = "";
    TBAG_CONSTEXPR static char const * const DEFAULT_SUFFIX = ".log";

    std::string prefix;
    std::string suffix;
    std::size_t count;

    CounterUpdater(std::string const & p = DEFAULT_PREFIX, std::string const & s = DEFAULT_SUFFIX, std::size_t c = 0)
            : prefix(p), suffix(s), count(c)
    { /* EMPTY. */ }
    virtual ~CounterUpdater()
    { /* EMPTY. */ }

    virtual Path update(Path const & prev) override
    {
        Path next;
        do {
            next = (prefix + std::to_string(count) + suffix);
            count++;
        } while (next.exists());
        return next;
    }
};

/**
 * Time based updater.
 *
 * @author zer0
 * @date   2017-11-20
 */
struct TimeFormatUpdater : public UpdaterInterface
{
    TBAG_CONSTEXPR static char const * const DEFAULT_TIME_FORMAT_STRING = "$py$pm$pd_$ph$pi$ps.log";

    std::string format;

    TimeFormatUpdater() : format(DEFAULT_TIME_FORMAT_STRING)
    { /* EMPTY. */ }
    TimeFormatUpdater(std::string const & f) : format(f)
    { /* EMPTY. */ }
    virtual ~TimeFormatUpdater()
    { /* EMPTY. */ }

    virtual Path update(Path const & prev) override
    {
        return Path(libtbag::time::TimePoint::now().fformat(format));
    }

    /**
     * @remarks
     *  <pre>
     *   /prefix/file.log -> /prefix/file-$py$pm$pdT$ph$pi$ps_$pl$pc.log
     *  </pre>
     */
    static std::string getDefaultTimeFormatString(Path const & path)
    {
        std::string const PREFIX = path.getNameWithoutExtension();
        std::string const FORMAT = "-$py$pm$pdT$ph$pi$ps_$pl$pc";
        std::string const SUFFIX = path.getExtensionName();
        return (path.getParent() / (PREFIX + FORMAT + SUFFIX)).toString();
    }
};

/**
 * Cleaning an expired path.
 *
 * @author zer0
 * @date   2019-07-08
 */
struct CleanerInterface
{
    CleanerInterface() { /* EMPTY. */ }
    virtual ~CleanerInterface() { /* EMPTY. */ }

    virtual void clean(Path const & path) = 0;
};

/**
 * Archiving an expired path.
 *
 * @author zer0
 * @date   2019-07-08
 */
struct ArchiveCleaner : public CleanerInterface
{
    TBAG_CONSTEXPR static char const * const DEFAULT_ARCHIVE_SUFFIX = ".zip";

    std::string archive_suffix;
    bool remove_source_file;

    ArchiveCleaner() : archive_suffix(DEFAULT_ARCHIVE_SUFFIX), remove_source_file(true)
    { /* EMPTY. */ }
    ArchiveCleaner(std::string const & suffix) : archive_suffix(suffix), remove_source_file(true)
    { /* EMPTY. */ }
    ArchiveCleaner(std::string const & suffix, bool remove) : archive_suffix(suffix), remove_source_file(remove)
    { /* EMPTY. */ }
    virtual ~ArchiveCleaner()
    { /* EMPTY. */ }

    virtual void clean(Path const & path) override
    {
        using namespace libtbag::archive;
        auto const success_count = compressArchive(path.toString() + archive_suffix,
                                                   { path.toString() },
                                                   getCompressFormatFromOutputFileName(archive_suffix));
        assert(success_count == 1);
        if (remove_source_file) {
            path.remove();
        }
    }
};

/**
 * RotatePath class prototype.
 *
 * @author zer0
 * @date   2017-07-31
 *
 * @remarks
 *  - absolute time.
 *  - file size.
 */
struct TBAG_API RotatePath
{
    using Environments = libtbag::string::Environments;

    using SharedWriter = std::shared_ptr<WriterInterface>;
    using SharedUpdater = std::shared_ptr<UpdaterInterface>;
    using SharedCleaner = std::shared_ptr<CleanerInterface>;

    struct InitParams
    {
        SharedWriter writer;
        SharedUpdater updater;
        SharedCleaner cleaner;
    };

    TBAG_CONSTEXPR static char const * const WRITER_KEY_SIZE = "size";
    TBAG_CONSTEXPR static char const * const UPDATER_KEY_COUNTER = "counter";
    TBAG_CONSTEXPR static char const * const UPDATER_KEY_TIME = "time";
    TBAG_CONSTEXPR static char const * const CLEANER_KEY_ARCHIVE = "archive";

    /**
     * @remarks
     *  Examples:
     *  - size and counter: <code>size=1024m counter=/prefix/path/log,.log,0</code>
     *  - size and time and archive: <code>size=1024m time=/prefix/path/file-$py$pm$pdT$ph$pi$ps.log archive=.zip</code>
     */
    static InitParams createParams(std::string const & arguments, Environments const & envs);
    static InitParams createParams(std::string const & arguments);

    Path path;

    SharedWriter writer;
    SharedUpdater updater;
    SharedCleaner cleaner;

    RotatePath();
    RotatePath(std::string const & arguments);

    explicit RotatePath(InitParams const & params);
    explicit RotatePath(Path const & p, InitParams const & params);
    explicit RotatePath(Path const & p);
    explicit RotatePath(Path const & p, SharedWriter const & w, SharedUpdater const & u);
    explicit RotatePath(SharedWriter const & w, SharedUpdater const & u);
    explicit RotatePath(Path const & p, SharedWriter const & w, SharedUpdater const & u, SharedCleaner const & c);
    explicit RotatePath(SharedWriter const & w, SharedUpdater const & u, SharedCleaner const & c);

    ~RotatePath();

    inline bool isReady() const TBAG_NOEXCEPT_SPECIFIER(
            TBAG_NOEXCEPT_OPERATOR((bool)writer) &&
            TBAG_NOEXCEPT_OPERATOR((bool)updater))
    { return writer && updater; }

    void init(std::string const & args, Environments const & envs);
    void init(std::string const & args);

    void update();
    void flush();

    Err write(char const * buffer, std::size_t size);
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

