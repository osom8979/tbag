/**
 * @file   Asset.hpp
 * @brief  Asset class prototype.
 * @author zer0
 * @date   2016-04-03
 * @date   2016-10-13 (Change namespace: libtbag -> libtbag::res)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * Asset class prototype.
 *
 * @author zer0
 * @date   2016-04-03
 *
 * @remarks
 *  Resource manager class.
 *
 * @warning
 *  Not supported wchar_t string.
 */
class Asset
{
public:
    using Value  = char;
    using String = std::basic_string<Value>;
    using Path   = libtbag::filesystem::Path;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

public:
    // @formatter:off
    TBAG_CONSTEXPR Asset() { /* EMPTY. */ }
    Asset(Asset const & obj) { /* EMPTY. */ }
    Asset(Asset && obj) { /* EMPTY. */ }
    virtual ~Asset() { /* EMPTY. */ }
    // @formatter:on

public:
    // @formatter:off
    inline Asset & operator =(Asset const & obj) { return *this; }
    inline Asset & operator =(Asset && obj) { return *this; }
    // @formatter:on

public:
    static std::vector<Path> scanDir(Path const & path)
    {
        std::vector<Path> result;
        for (auto & cursor : path.scanDir()) {
            result.push_back(Path(Path::append(path, cursor)));
        }
        return result;
    }

public:
    /** Obtain HOME directory path. */
    inline static Path getHomeDirPath()
    {
        return Path::getHomeDir();
    }

    /** Obtain executable file directory path. */
    inline static Path getExeDirPath()
    {
        return Path::getExeDir();
    }

public:
    virtual String getGlobalName() const
    {
        return String(LIBTBAG_TITLE_STRING);
    }

    Path getGlobalDirPath() const
    {
#if defined(TBAG_PLATFORM_WINDOWS)
        Path const PARENT("C:\\Program Files");
#elif defined(TBAG_PLATFORM_MACOS)
        Path const PARENT("/var");
#else
        Path const PARENT("/var");
#endif
        return PARENT / getGlobalName();
    }
};

#ifndef CREATE_ASSET_PATH
/** Create a main directory accessor & mutator macro. */
#define CREATE_ASSET_PATH(name, path)               \
public:                                             \
    static Path get_##name() {                      \
        return Path(path);                          \
    }                                               \
    static bool create_##name(int mode = 0755) {    \
        using namespace ::libtbag::filesystem;      \
        return Path(get_##name()).createDir(mode);  \
    }                                               \
    static bool remove_##name() {                   \
        using namespace ::libtbag::filesystem;      \
        return Path(get_##name()).removeAll();      \
    }                                               \
    static bool exists_##name() {                   \
        using namespace ::libtbag::filesystem;      \
        return Path(get_##name()).isDirectory();    \
    }                                               \
    static std::vector<Path> scan_##name() {        \
        return Path(get_##name()).scanDir();        \
    }                                               \
private:
#endif

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__

