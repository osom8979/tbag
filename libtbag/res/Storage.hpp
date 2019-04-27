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
#include <libtbag/res/GetText.hpp>
#include <libtbag/database/Sqlite.hpp>
#include <libtbag/security/KeyStore.hpp>
#include <libtbag/script/LuaMachine.hpp>

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
    using SharedLibrary = libtbag::lib::SharedLibrary;
    using GetText       = libtbag::res::GetText;
    using Sqlite        = libtbag::database::Sqlite;
    using KeyStore      = libtbag::security::KeyStore;
    using LuaMachine    = libtbag::script::LuaMachine;

public:
    /** Number of retries for temporary name generation. */
    TBAG_CONSTEXPR static std::size_t const RETRY_COUNT_OF_TEMP_NAME = 16;
    TBAG_CONSTEXPR static std::size_t const DEFAULT_RANDOM_STRING_SIZE = 16;

public:
    /**
     * Storage::Impl class implementation.
     *
     * @author zer0
     * @date   2018-11-03
     */
    struct TBAG_API Impl TBAG_FINAL
    {
        DynamicAsset  asset;
        Environments  envs;
        std::string   envs_filename;
        Resource      config;
        std::string   config_gui;
        std::string   module_extension;
        GetText       text;
        Sqlite        sqlite;
        std::string   clear_tempdir;
        KeyStore      keystore;
        LuaMachine    lua;
        std::string   lua_gui;

        Impl();
        ~Impl();
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
    // clang-format off
    TBAG_CONSTEXPR static char const * const LAYOUT_ENV      = "env";
    TBAG_CONSTEXPR static char const * const LAYOUT_CONFIG   = "config";
    TBAG_CONSTEXPR static char const * const LAYOUT_MODULE   = "module";
    TBAG_CONSTEXPR static char const * const LAYOUT_TEXT     = "text";
    TBAG_CONSTEXPR static char const * const LAYOUT_SQLITE   = "sqlite";
    TBAG_CONSTEXPR static char const * const LAYOUT_TEMP     = "temp";
    TBAG_CONSTEXPR static char const * const LAYOUT_KEYSTORE = "keystore";
    TBAG_CONSTEXPR static char const * const LAYOUT_LUA      = "lua";
    TBAG_CONSTEXPR static char const * const LAYOUT_LUAROCKS = "luarocks";
    TBAG_CONSTEXPR static char const * const LAYOUT_USER     = "user";
    // clang-format on

public:
    // clang-format off
    void setLayoutEnv     (std::string const & dir) { asset().set(LAYOUT_ENV     , Path(dir)); }
    void setLayoutConfig  (std::string const & dir) { asset().set(LAYOUT_CONFIG  , Path(dir)); }
    void setLayoutModule  (std::string const & dir) { asset().set(LAYOUT_MODULE  , Path(dir)); }
    void setLayoutText    (std::string const & dir) { asset().set(LAYOUT_TEXT    , Path(dir)); }
    void setLayoutSqlite  (std::string const & dir) { asset().set(LAYOUT_SQLITE  , Path(dir)); }
    void setLayoutTemp    (std::string const & dir) { asset().set(LAYOUT_TEMP    , Path(dir)); }
    void setLayoutKeystore(std::string const & dir) { asset().set(LAYOUT_KEYSTORE, Path(dir)); }
    void setLayoutLua     (std::string const & dir) { asset().set(LAYOUT_LUA     , Path(dir)); }
    void setLayoutLuaRocks(std::string const & dir) { asset().set(LAYOUT_LUAROCKS, Path(dir)); }
    // clang-format on

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

    std::vector<std::string> getEnvFilenames() const;
    std::string convert(std::string const & value) const;

public:
    bool readConfig(std::string const & filename, std::string const & key, std::string & value);
    bool saveConfig(std::string const & filename, std::string const & key, std::string const & value);

    std::vector<std::string> getConfigFilenames() const;
    std::vector<std::string> getConfigKeys(std::string const & filename) const;

    void removeConfig(std::string const & filename);
    void removeAllConfig();

public:
    void setModuleExtension(std::string const & extension);
    void setLayoutModule(std::string const & dir, std::string const & extension);

    std::vector<std::string> getModuleFilenames() const;

    SharedLibrary getModule(std::string const & filename) const;

public:
    void setTextLanguage(std::string const & language);
    void setLayoutText(std::string const & dir, std::string const & language);

    std::vector<std::string> getTextFilenames() const;

    std::string getText(std::string const & language, std::string const & name) const;
    std::string getText(std::string const & name) const;

    void setText(std::string const & language, std::string const & name, std::string const & text);
    void setText(std::string const & name, std::string const & text);

    bool loadText();
    bool saveText();

public:
    Sqlite       & sqlite()       TBAG_NOEXCEPT { return _impl->sqlite; }
    Sqlite const & sqlite() const TBAG_NOEXCEPT { return _impl->sqlite; }

    bool openSqlite(std::string const & filename, bool auto_close = true);
    void closeSqlite();
    bool isOpenSqlite() const;

    std::vector<std::string> getSqliteFilenames() const;

public:
    void setAutoClearTempFiles(bool enable = true);
    bool isAutoClearTempFiles() const;
    void clearTempDir();

    std::string generateTempPath(std::size_t name_size = DEFAULT_RANDOM_STRING_SIZE) const;

public:
    KeyStore       & keystore()       TBAG_NOEXCEPT { return _impl->keystore; }
    KeyStore const & keystore() const TBAG_NOEXCEPT { return _impl->keystore; }

    bool openKeyStore(std::string const & filename);
    void closeKeyStore();
    bool isOpenKeyStore() const;

    bool createKey(std::string const & key);
    bool removeKey(std::string const & key);

    bool getKey(std::string const & key, std::string & result);
    bool setKey(std::string const & key, std::string const & value, bool encrypt = false);
    bool cmpKey(std::string const & key, std::string const & value, bool encrypt = false);

    std::vector<std::string> listKey();
    std::vector<std::string> getKeyStoreFilenames() const;

public:
    LuaMachine       & lua()       TBAG_NOEXCEPT { return _impl->lua; }
    LuaMachine const & lua() const TBAG_NOEXCEPT { return _impl->lua; }

    bool appendLuaPath();
    bool runLuaScriptFile(std::string const & filename);
    bool runLuaScript(std::string const & code);

    std::vector<std::string> getLuaFilenames() const;

public:
    bool appendLuaRocksPath();
    bool appendLuaRocksCPath();

public:
    std::string getInfo() const;
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_STORAGE_HPP__

