/**
 * @file   Storage.hpp
 * @brief  Storage class prototype.
 * @author zer0
 * @date   2018-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/dom/xml/Resource.hpp>

#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * Storage class prototype.
 *
 * @author zer0
 * @date   2018-11-03
 */
class TBAG_API Storage
{
public:
    using DynamicAsset = libtbag::res::DynamicAsset;
    using Environments = libtbag::string::Environments;
    using Resource     = libtbag::dom::xml::Resource;

public:
    /**
     * Storage::Impl class implementation.
     *
     * @author zer0
     * @date   2018-11-03
     */
    struct Impl : private Noncopyable
    {
        DynamicAsset  asset;
        Environments  envs;
        Resource      config;

        Impl() { /* EMPTY. */ }
        ~Impl() { /* EMPTY. */ }
    };

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    Storage();
    Storage(std::nullptr_t) TBAG_NOEXCEPT;
    Storage(Storage const & obj) TBAG_NOEXCEPT;
    Storage(Storage && obj) TBAG_NOEXCEPT;
    ~Storage();

public:
    Storage & operator =(Storage const & obj) TBAG_NOEXCEPT;
    Storage & operator =(Storage && obj) TBAG_NOEXCEPT;

public:
    void copy(Storage const & obj) TBAG_NOEXCEPT;
    void swap(Storage & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(Storage & lh, Storage & rh) TBAG_NOEXCEPT
    { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return *get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(Storage const & x, Storage const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(Storage const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(Storage const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();

public:
    void clear();

public:
    DynamicAsset       & asset()       TBAG_NOEXCEPT { return _impl->asset; }
    DynamicAsset const & asset() const TBAG_NOEXCEPT { return _impl->asset; }

public:
    TBAG_CONSTEXPR static char const * const LAYOUT_ENV = "env";
    TBAG_CONSTEXPR static char const * const ENV_UPDATE_ALL = "*";

    Environments       & envs()       TBAG_NOEXCEPT { return _impl->envs; }
    Environments const & envs() const TBAG_NOEXCEPT { return _impl->envs; }

    /**
     * Set env layout.
     *
     * @param[in] dir
     *      layout directory.
     * @param[in] set
     *      assign default env filename. '*' is update all env files.
     * @param[in] envs
     *      Update extension envs.
     * @param[in] default_set
     *      Update default envs?
     */
    void setEnv(std::string const & dir, std::string const & set, char ** envs = nullptr, bool default_set = false);

public:
    TBAG_CONSTEXPR static char const * const LAYOUT_CONFIG = "config";

    void setConfig(std::string const & dir);
    bool readConfig(std::string const & group, std::string const & key, std::string & value);
    bool saveConfig(std::string const & group, std::string const & key, std::string const & value);

    std::vector<std::string> getConfigGroups() const;
    std::vector<std::string> getConfigKeys(std::string const & group) const;

    void removeConfig(std::string const & group);
    void removeAllConfig();

public:
    TBAG_CONSTEXPR static char const * const LAYOUT_MODULE    = "module";
    TBAG_CONSTEXPR static char const * const LAYOUT_TEXT      = "text";
    TBAG_CONSTEXPR static char const * const LAYOUT_IMAGE     = "image";
    TBAG_CONSTEXPR static char const * const LAYOUT_DRAWABLE  = "drawable";
    TBAG_CONSTEXPR static char const * const LAYOUT_ANIMATION = "animation";
    TBAG_CONSTEXPR static char const * const LAYOUT_SPRITE    = "sprite";
    TBAG_CONSTEXPR static char const * const LAYOUT_LMDB      = "lmdb";
    TBAG_CONSTEXPR static char const * const LAYOUT_SQLITE    = "sqlite";
    TBAG_CONSTEXPR static char const * const LAYOUT_TEMP      = "temp";
    TBAG_CONSTEXPR static char const * const LAYOUT_KEYSTORE  = "keystore";
    TBAG_CONSTEXPR static char const * const LAYOUT_LUA       = "lua";
    TBAG_CONSTEXPR static char const * const LAYOUT_RAW       = "raw";
    TBAG_CONSTEXPR static char const * const LAYOUT_BAGEX     = "bagex";
    TBAG_CONSTEXPR static char const * const LAYOUT_EXE       = "exe";
    TBAG_CONSTEXPR static char const * const LAYOUT_FONT      = "font";
    TBAG_CONSTEXPR static char const * const LAYOUT_MUSIC     = "music";
    TBAG_CONSTEXPR static char const * const LAYOUT_SOUND     = "sound";
    TBAG_CONSTEXPR static char const * const LAYOUT_SHADER    = "shader";
    TBAG_CONSTEXPR static char const * const LAYOUT_LAYOUT    = "layout";
    TBAG_CONSTEXPR static char const * const LAYOUT_STYLE     = "style";
    TBAG_CONSTEXPR static char const * const LAYOUT_COLOR     = "color";
    TBAG_CONSTEXPR static char const * const LAYOUT_RLOG      = "rlog";
    TBAG_CONSTEXPR static char const * const LAYOUT_USER      = "user";
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

