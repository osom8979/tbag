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
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/lib/SharedLibrary.hpp>

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
    using DynamicAsset  = libtbag::res::DynamicAsset;
    using Environments  = libtbag::string::Environments;
    using Resource      = libtbag::dom::xml::Resource;
    using Path          = libtbag::filesystem::Path;

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
        std::string   envs_filename;
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
    // @formatter:off
    TBAG_CONSTEXPR static char const * const LAYOUT_ENV       = "env";
    TBAG_CONSTEXPR static char const * const LAYOUT_CONFIG    = "config";
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
    // @formatter:on

public:
    // @formatter:off
    void setLayoutEnv      (std::string const & dir) { asset().set(LAYOUT_ENV      , Path(dir)); }
    void setLayoutConfig   (std::string const & dir) { asset().set(LAYOUT_CONFIG   , Path(dir)); }
    void setLayoutModule   (std::string const & dir) { asset().set(LAYOUT_MODULE   , Path(dir)); }
    void setLayoutText     (std::string const & dir) { asset().set(LAYOUT_TEXT     , Path(dir)); }
    void setLayoutImage    (std::string const & dir) { asset().set(LAYOUT_IMAGE    , Path(dir)); }
    void setLayoutDrawable (std::string const & dir) { asset().set(LAYOUT_DRAWABLE , Path(dir)); }
    void setLayoutAnimation(std::string const & dir) { asset().set(LAYOUT_ANIMATION, Path(dir)); }
    void setLayoutSprite   (std::string const & dir) { asset().set(LAYOUT_SPRITE   , Path(dir)); }
    void setLayoutLmdb     (std::string const & dir) { asset().set(LAYOUT_LMDB     , Path(dir)); }
    void setLayoutSqlite   (std::string const & dir) { asset().set(LAYOUT_SQLITE   , Path(dir)); }
    void setLayoutTemp     (std::string const & dir) { asset().set(LAYOUT_TEMP     , Path(dir)); }
    void setLayoutKeystore (std::string const & dir) { asset().set(LAYOUT_KEYSTORE , Path(dir)); }
    void setLayoutLua      (std::string const & dir) { asset().set(LAYOUT_LUA      , Path(dir)); }
    void setLayoutRaw      (std::string const & dir) { asset().set(LAYOUT_RAW      , Path(dir)); }
    void setLayoutBagex    (std::string const & dir) { asset().set(LAYOUT_BAGEX    , Path(dir)); }
    void setLayoutExe      (std::string const & dir) { asset().set(LAYOUT_EXE      , Path(dir)); }
    void setLayoutFont     (std::string const & dir) { asset().set(LAYOUT_FONT     , Path(dir)); }
    void setLayoutMusic    (std::string const & dir) { asset().set(LAYOUT_MUSIC    , Path(dir)); }
    void setLayoutSound    (std::string const & dir) { asset().set(LAYOUT_SOUND    , Path(dir)); }
    void setLayoutShader   (std::string const & dir) { asset().set(LAYOUT_SHADER   , Path(dir)); }
    void setLayoutLayout   (std::string const & dir) { asset().set(LAYOUT_LAYOUT   , Path(dir)); }
    void setLayoutStyle    (std::string const & dir) { asset().set(LAYOUT_STYLE    , Path(dir)); }
    void setLayoutColor    (std::string const & dir) { asset().set(LAYOUT_COLOR    , Path(dir)); }
    void setLayoutRlog     (std::string const & dir) { asset().set(LAYOUT_RLOG     , Path(dir)); }
    void setLayoutUser     (std::string const & dir) { asset().set(LAYOUT_USER     , Path(dir)); }
    // @formatter:on

public:
    std::vector<std::string> getFilenames(std::string const & key) const;

public:
    Environments       & envs()       TBAG_NOEXCEPT { return _impl->envs; }
    Environments const & envs() const TBAG_NOEXCEPT { return _impl->envs; }

    void setEnvFilename(std::string const & filename);
    void setLayoutEnv(std::string const & dir, std::string const & filename, bool auto_load = false);

    bool readEnv();
    void readEnvDefault();
    void readEnvParams(char ** envs);
    bool saveEnv();

    void clearEnv();

    void setEnv(std::string const & key, std::string const & value);
    bool getEnv(std::string const & key, std::string & value) const;

    std::string convert(std::string const & value) const;

public:
    bool readConfig(std::string const & filename, std::string const & key, std::string & value);
    bool saveConfig(std::string const & filename, std::string const & key, std::string const & value);

    std::vector<std::string> getConfigFilenames() const;
    std::vector<std::string> getConfigKeys(std::string const & filename) const;

    void removeConfig(std::string const & filename);
    void removeAllConfig();

public:
    std::vector<std::string> getModuleFilenames() const;
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

