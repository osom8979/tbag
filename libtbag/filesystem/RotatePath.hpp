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

#include <libtbag/archive/Archive.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
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
 * Condition checker interface of RotatePath.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct CheckerInterface
{
    CheckerInterface() { /* EMPTY. */ }
    virtual ~CheckerInterface() { /* EMPTY. */ }

    /**
     * Rotate check method.
     *
     * @param[in] prev
     *      Previous path information.
     * @param[in] buffer
     *      If you want a wirte in file, this parameter is the source buffer.
     * @param[in] size
     *      If you want a wirte in file, this parameter is the size of source buffer.
     */
    virtual bool test(Path const & prev, char const * buffer, std::size_t size) = 0;
};

/**
 * Maximum file size checkr.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct SizeChecker : public CheckerInterface
{
    TBAG_CONSTEXPR static std::size_t const DEFAULT_MAX_SIZE = 1 * MEGA_BYTE_TO_BYTE;

    std::size_t max_size;

    SizeChecker(std::size_t size = DEFAULT_MAX_SIZE) : max_size(size)
    { /* EMPTY. */ }
    virtual ~SizeChecker()
    { /* EMPTY. */ }

    virtual bool test(Path const & prev, char const * buffer, std::size_t size) override
    {
        details::FileState state = { 0, };
        if (!details::getState(prev, &state)) {
            return false;
        }
        if (state.size + size <= max_size) {
            return false;
        }
        return true;
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

    virtual bool clean(Path const & path) = 0;
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

    virtual bool clean(Path const & path) override
    {
        using namespace libtbag::archive;
        auto const success_count = compressArchive(path.toString() + archive_suffix, {path.toString()});
        if (success_count == 0) {
            return false;
        }
        assert(success_count == 1);
        if (remove_source_file) {
            path.remove();
        }
        return true;
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

    using SharedChecker = std::shared_ptr<CheckerInterface>;
    using SharedUpdater = std::shared_ptr<UpdaterInterface>;
    using SharedCleaner = std::shared_ptr<CleanerInterface>;

    struct InitParams
    {
        SharedChecker checker;
        SharedUpdater updater;
        SharedCleaner cleaner;
    };

    TBAG_CONSTEXPR static char const * const CHECKER_KEY_SIZE = "size";
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

    SharedChecker checker;
    SharedUpdater updater;
    SharedCleaner cleaner;

    RotatePath();
    RotatePath(std::string const & arguments);

    explicit RotatePath(InitParams const & params);
    explicit RotatePath(Path const & p, InitParams const & params);
    explicit RotatePath(Path const & p);
    explicit RotatePath(Path const & p, SharedChecker const & k, SharedUpdater const & u);
    explicit RotatePath(SharedChecker const & k, SharedUpdater const & u);
    explicit RotatePath(Path const & p, SharedChecker const & k, SharedUpdater const & u, SharedCleaner const & c);
    explicit RotatePath(SharedChecker const & k, SharedUpdater const & u, SharedCleaner const & c);

    inline bool isReady() const TBAG_NOEXCEPT_SPECIFIER(
            TBAG_NOEXCEPT_OPERATOR((bool)checker) &&
            TBAG_NOEXCEPT_OPERATOR((bool)updater))
    { return checker && updater; }

    void init(std::string const & args, Environments const & envs);
    void init(std::string const & args);

    bool update();

    bool testIfRead(Path const & prev) const;
    bool testIfRead() const;

    bool testIfWrite(Path const & prev, char const * buffer, std::size_t size) const;
    bool testIfWrite(char const * buffer, std::size_t size) const;

    bool next(char const * buffer, std::size_t size);
    bool next();
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

