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
#include <libtbag/filesystem/Path.hpp>

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
struct RotatePathCheckerInterface
{
    /**
     * Rotate check method.
     *
     * @param[in] prev
     *      Previous path information.
     * @param[out] next
     *      Update path information.
     * @param[in] buffer
     *      If you want a wirte in file, this parameter is the source buffer.
     * @param[in] size
     *      If you want a wirte in file, this parameter is the size of source buffer.
     */
    virtual bool test(Path const & prev, Path & next, char const * buffer, std::size_t size) const = 0;
};

/**
 * Path updater.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct TBAG_API RotatePathUpdater : public RotatePathCheckerInterface
{
    std::string update_rule;

    RotatePathUpdater(std::string const & rule);
    virtual ~RotatePathUpdater();

    Path obtain() const;
};

/**
 * Maximum file size checkr.
 *
 * @author zer0
 * @date   2017-10-21
 */
struct TBAG_API RotatePathSizeChecker : public RotatePathUpdater
{
    std::size_t max_size;

    RotatePathSizeChecker(std::string const & rule, std::size_t size = 0);
    virtual ~RotatePathSizeChecker();

    virtual bool test(Path const & prev, Path & next, char const * buffer, std::size_t size) const override;
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
class TBAG_API RotatePath
{
public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

    using SharedChecker = std::shared_ptr<RotatePathCheckerInterface>;

private:
    SharedChecker _checker;
    Path _path;

public:
    RotatePath();
    RotatePath(Path const & path);
    RotatePath(Path const & path, SharedChecker const & checker);
    RotatePath(RotatePath const & obj);
    RotatePath(RotatePath && obj);
    virtual ~RotatePath();

public:
    RotatePath & operator =(RotatePath const & obj);
    RotatePath & operator =(RotatePath && obj);

public:
    void setChecker(RotatePathCheckerInterface * checker) { setChecker(checker); }
    void setChecker(SharedChecker const & checker) { _checker = checker; }

public:
    void setPath(Path const & path) { _path = path; }
    Path getPath() const { return _path; }

public:
    bool testIfRead(Path const & prev, Path & next) const;
    bool testIfRead(Path & next) const;

    bool testIfWrite(Path const & prev, Path & next, char const * buffer, std::size_t size) const;
    bool testIfWrite(Path & next, char const * buffer, std::size_t size) const;

public:
    bool next(char const * buffer, std::size_t size);
    bool next();

public:
    FileState getState() const;
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_ROTATEPATH_HPP__

