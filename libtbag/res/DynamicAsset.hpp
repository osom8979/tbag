/**
 * @file   DynamicAsset.hpp
 * @brief  DynamicAsset class prototype.
 * @author zer0
 * @date   2016-10-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <string>
#include <vector>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * DynamicAsset class prototype.
 *
 * @author zer0
 * @date   2016-10-13
 */
class TBAG_API DynamicAsset
{
public:
    using Value  = char;
    using String = std::basic_string<Value>;
    using Path   = libtbag::filesystem::Path;

    using StringVector = std::vector<String>;
    using PathVector   = std::vector<Path>;
    using PathMap      = std::map<String, Path>;
    using PathMapPair  = PathMap::value_type;

    static_assert(std::is_same<Value, char>::value
            , "Value must be the same type as char");
    static_assert(std::is_same<Value, typename String::value_type>::value
            , "Value must be the same type as String::value_type");

private:
    PathMap _paths; ///< Parent directories.

public:
    DynamicAsset() = default;
    ~DynamicAsset() = default;

    DynamicAsset(DynamicAsset const & obj) = default;
    DynamicAsset & operator =(DynamicAsset const & obj) = default;

#if defined(TBAG_HAS_DEFAULTED_FUNCTIONS) && !defined(TBAG_HAS_DEFAULTED_FUNCTIONS_BUT_NOT_MOVE_FUNCTION)
    DynamicAsset(DynamicAsset && obj) = default;
    DynamicAsset & operator =(DynamicAsset && obj) = default;
#endif

public:
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_paths.empty()))
    { return _paths.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_paths.size()))
    { return _paths.size(); }
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_paths.clear()))
    { _paths.clear(); }

public:
    bool init();

public:
    bool addPath(String const & key, Path const & path);
    Path getPath(String const & key) const;
    StringVector getKeys() const;
    PathVector getPaths() const;

public:
    bool exists(String const & key) const;
    bool create(String const & key) const;
    bool remove(String const & key) const;
    PathVector scan(String const & key) const;

public:
    Path findWriteableDir() const;
    Path findFile(String const & filename) const;
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_DYNAMICASSET_HPP__

