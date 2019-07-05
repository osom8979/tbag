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
 * Count based updater.
 *
 * @author zer0
 * @date   2017-11-20
 */
struct CounterUpdater : public UpdaterInterface
{
    std::string prefix;
    std::string suffix;
    int count = 0;

    CounterUpdater(std::string const & p, std::string const & s, int c = 0)
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
    TBAG_CONSTEXPR static char const * const DEFAULT_TIME_FORMAT_STRING = "-$py$pm$pdT$ph$pi$ps_$pl$pc";

    std::string format;

    TimeFormatUpdater() : format(DEFAULT_TIME_FORMAT_STRING)
    { /* EMPTY. */ }
    explicit TimeFormatUpdater(std::string const & f) : format(f)
    { /* EMPTY. */ }
    explicit TimeFormatUpdater(Path const & p) : format(getDefaultTimeFormatString(p))
    { /* EMPTY. */ }
    virtual ~TimeFormatUpdater()
    { /* EMPTY. */ }

    virtual Path update(Path const & prev) override
    {
        return Path(libtbag::time::TimePoint::now().fformat(format));
    }

    static std::string getDefaultTimeFormatString(Path const & path)
    {
        std::string const PREFIX = path.getNameWithoutExtension();
        std::string const FORMAT = DEFAULT_TIME_FORMAT_STRING;
        std::string const SUFFIX = path.getExtensionName();
        return (path.getParent() / (PREFIX + FORMAT + SUFFIX)).toString();
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
    using SharedChecker = std::shared_ptr<CheckerInterface>;
    using SharedUpdater = std::shared_ptr<UpdaterInterface>;

    Path path;

    SharedChecker checker;
    SharedUpdater updater;

    RotatePath();
    RotatePath(Path const & path);
    RotatePath(Path const & path, SharedChecker const & checker, SharedUpdater const & updater);
    RotatePath(SharedChecker const & checker, SharedUpdater const & updater);

    inline bool isReady() const TBAG_NOEXCEPT_SPECIFIER(
            TBAG_NOEXCEPT_OPERATOR((bool)checker) &&
            TBAG_NOEXCEPT_OPERATOR((bool)updater))
    { return checker && updater; }

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

