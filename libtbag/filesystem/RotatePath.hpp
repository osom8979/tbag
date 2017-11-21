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

#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/time/TimePoint.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

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
class TBAG_API RotatePath
{
public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

public:
    /**
     * Condition checker interface of RotatePath.
     *
     * @author zer0
     * @date   2017-10-21
     */
    struct CheckerInterface
    {
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
     * Rules of redefine the path.
     *
     * @author zer0
     * @date   2017-11-20
     */
    struct UpdaterInterface
    {
        virtual Path update(Path const & prev) = 0;
    };

    using SharedChecker = std::shared_ptr<CheckerInterface>;
    using   WeakChecker =   std::weak_ptr<CheckerInterface>;
    using SharedUpdater = std::shared_ptr<UpdaterInterface>;
    using   WeakUpdater =   std::weak_ptr<UpdaterInterface>;

public:
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

        SizeChecker(std::size_t size = DEFAULT_MAX_SIZE) : max_size(size) { /* EMPTY. */ }
        virtual ~SizeChecker() { /* EMPTY. */ }

        virtual bool test(Path const & prev, char const * buffer, std::size_t size) override
        {
            FileState state = {0};
            if (details::getState(prev, &state) == false) {
                return false;
            }
            if (state.size + size <= max_size) {
                return false;
            }
            return true;
        }

        static std::size_t getDefaultMaxSize()
        {
            return DEFAULT_MAX_SIZE;
        }
    };

    /**
     * Count based updater.
     *
     * @author zer0
     * @date   2017-11-20
     */
    struct CounterUpdater : public UpdaterInterface
    {
        std::string prefix;
        std::string suffix;
        int count;

        CounterUpdater(std::string const & p, std::string const & s, int c = 0) : prefix(p), suffix(s), count(c)
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
        TBAG_CONSTEXPR static char const * const DEFAULT_TIME_FORMAT_STRING = "-$py$pm$pdT$ph$pi$ps_$pl$pc";

        std::string format;

        TimeFormatUpdater() : format(DEFAULT_TIME_FORMAT_STRING) { /* EMPTY. */ }
        explicit TimeFormatUpdater(std::string const & f) : format(f) { /* EMPTY. */ }
        explicit TimeFormatUpdater(Path const & p) : format(getDefaultTimeFormatString(p)) { /* EMPTY. */ }
        virtual ~TimeFormatUpdater() { /* EMPTY. */ }

        virtual Path update(Path const & prev) override
        {
            return Path(time::TimePoint::now().fformat(format));
        }

        static std::string getDefaultTimeFormatString()
        {
            return DEFAULT_TIME_FORMAT_STRING;
        }

        static std::string getDefaultTimeFormatString(Path const & path)
        {
            std::string const PREFIX = path.getNameWithoutExtension();
            std::string const FORMAT = TimeFormatUpdater::getDefaultTimeFormatString();
            std::string const SUFFIX = path.getExtensionName();
            return (path.getParent() / (PREFIX + FORMAT + SUFFIX)).toString();
        }
    };

private:
    SharedChecker _checker;
    SharedUpdater _updater;
    Path _path;

public:
    RotatePath();
    RotatePath(Path const & path);
    RotatePath(Path const & path, SharedChecker const & checker, SharedUpdater const & updater);
    RotatePath(Path const & path, std::size_t size = SizeChecker::getDefaultMaxSize());
    RotatePath(RotatePath const & obj);
    RotatePath(RotatePath && obj);
    virtual ~RotatePath();

public:
    RotatePath & operator =(RotatePath const & obj);
    RotatePath & operator =(RotatePath && obj);

public:
    void setChecker(CheckerInterface * checker) { setChecker(checker); }
    void setChecker(SharedChecker const & checker) { _checker = checker; }

    void setUpdater(UpdaterInterface * updater) { setUpdater(updater); }
    void setUpdater(SharedUpdater const & updater) { _updater = updater; }

    WeakChecker getChecker() const { return WeakChecker(_checker); }
    WeakUpdater getUpdater() const { return WeakUpdater(_updater); }

    void setPath(Path const & path) { _path = path; }
    Path getPath() const { return _path; }

public:
    inline bool isReady() const { return static_cast<bool>(_checker) && static_cast<bool>(_updater); }

public:
    FileState getState() const;

public:
    bool update();

public:
    bool testIfRead(Path const & prev) const;
    bool testIfRead() const;

    bool testIfWrite(Path const & prev, char const * buffer, std::size_t size) const;
    bool testIfWrite(char const * buffer, std::size_t size) const;

public:
    bool next(char const * buffer, std::size_t size);
    bool next();

public:
    static RotatePath createDefault(Path const & path, std::size_t size = SizeChecker::getDefaultMaxSize());
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

