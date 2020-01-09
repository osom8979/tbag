/**
 * @file   TempDirGuard.hpp
 * @brief  TempDirGuard class prototype.
 * @author zer0
 * @date   2018-03-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_TEMPDIRGUARD_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_TEMPDIRGUARD_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 *  Create & Remove temp directory.
 *
 * @author zer0
 * @date   2017-03-20
 * @date   2018-03-24 (Rename: tester/TempDirGenerator -> libtbag/filesystem/TempDirGuard)
 *
 * @remarks
 *  Can be used in Google-test.
 */
class TBAG_API TempDirGuard : private Noncopyable
{
public:
    TBAG_CONSTEXPR static char const * const DEFAULT_TEMP_DIR_NAME = "temp.dir";

public:
    TBAG_CONSTEXPR static char const * const getDefaultTempDirName() TBAG_NOEXCEPT
    { return DEFAULT_TEMP_DIR_NAME; }

private:
    bool _auto_remove;
    Path _temp_dir;

public:
    TempDirGuard();
    TempDirGuard(std::string const & case_name,
                 std::string const & name,
                 std::string const & prefix,
                 bool auto_create = true,
                 bool auto_remove = true);
    TempDirGuard(std::string const & case_name,
                 std::string const & name,
                 bool auto_create = true,
                 bool auto_remove = true);
    ~TempDirGuard();

public:
    void init(std::string const & case_name,
              std::string const & name,
              std::string const & prefix,
              bool auto_create = true,
              bool auto_remove = true);
    void init(std::string const & case_name,
              std::string const & name,
              bool auto_create = true,
              bool auto_remove = true);
    void release();

public:
    inline Path getDir() const { return _temp_dir; }

public:
    static Path getDefaultPrefix();
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_TEMPDIRGUARD_HPP__

