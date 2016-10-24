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
#include <libtbag/filesystem/Common.hpp>

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
    Asset() = default;
    ~Asset() = default;

    Asset(Asset const & obj) = default;
    Asset(Asset && obj) = default;

    Asset & operator =(Asset const & obj) = default;
    Asset & operator =(Asset && obj) = default;

public:
    static std::vector<Path> scanDir(Path const & path)
    {
        std::vector<Path> result;
        for (auto & cursor : filesystem::common::scanDir(path.getNativeString())) {
            result.push_back(Path(Path::append(path, cursor)));
        }
        return result;
    }

public:
    /** Obtain HOME directory path. */
    static Path getHomeDirPath()
    {
        return Path(filesystem::common::getHomeDir());
    }

    /** Obtain executable file directory path. */
    static Path getExeDirPath()
    {
        return Path(filesystem::common::getExeDir());
    }
};

#ifndef CREATE_ASSET_PATH
/** Create a main directory accessor & mutator macro. */
#define CREATE_ASSET_PATH(name, path)                   \
public:                                                 \
    static Path get_##name() {                          \
        return Path(path);                              \
    }                                                   \
    static bool create_##name() {                       \
        using namespace ::libtbag::filesystem::common;  \
        return createDir(get_##name());                 \
    }                                                   \
    static bool remove_##name() {                       \
        using namespace ::libtbag::filesystem::common;  \
        return removeDir(get_##name());                 \
    }                                                   \
    static bool exists_##name() {                       \
        using namespace ::libtbag::filesystem::common;  \
        return isDirectory(get_##name());               \
    }                                                   \
    static std::vector<Path> scan_##name() {            \
        return scanDir(get_##name());                   \
    }                                                   \
private:
#endif

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_ASSET_HPP__

