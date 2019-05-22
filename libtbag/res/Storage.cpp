/**
 * @file   Storage.cpp
 * @brief  Storage class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/Storage.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cassert>
#include <algorithm>
#include <utility>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

static void _remove_child_files(std::string const & dir)
{
    if (dir.empty()) {
        return;
    }

    auto const TEMP_PATH = libtbag::filesystem::Path(dir);
    if (!TEMP_PATH.isDirectory()) {
        return;
    }

    for (auto & path : TEMP_PATH.scanDir()) {
        if (!path.removeAll()) {
            tDLogW("_remove_child_files() Delete failed: {}", path);
        }
    }
}

// ----------------------------------
// Storage::Impl class implementation
// ----------------------------------

Storage::Impl::Impl()
{
    // EMPTY.
}

Storage::Impl::~Impl()
{
    if (clear_tempdir.empty()) {
        _remove_child_files(clear_tempdir);
    }
}

// ----------------------------
// Storage class implementation
// ----------------------------

Storage::Storage() : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

Storage::Storage(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Storage::Storage(Storage const & obj) TBAG_NOEXCEPT : Storage(nullptr)
{
    (*this) = obj;
}

Storage::Storage(Storage && obj) TBAG_NOEXCEPT : Storage(nullptr)
{
    (*this) = std::move(obj);
}

Storage::~Storage()
{
    // EMPTY.
}

Storage & Storage::operator =(Storage const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Storage & Storage::operator =(Storage && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Storage::copy(Storage const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Storage::swap(Storage & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Storage::reset()
{
    _impl.reset();
}

void Storage::clear()
{
    _impl = std::make_shared<Impl>();
}

std::vector<std::string> Storage::getFilenames(std::string const & key) const
{
    std::vector<std::string> result;
    if (asset().exists(key)) {
        for (auto & cursor : asset().get(key).scanNameOnly()) {
            result.push_back(cursor);
        }
    }
    return result;
}

void Storage::setEnvFilename(std::string const & filename)
{
    _impl->envs_filename = filename;
}

void Storage::setLayoutEnv(std::string const & dir, std::string const & filename, bool auto_load)
{
    setLayoutEnv(dir);
    setEnvFilename(filename);
    if (auto_load && !readEnv()) {
        tDLogE("Storage::setLayoutEnv() auto load failure: {}", libtbag::filesystem::Path(dir) / filename);
    }
}

bool Storage::readEnv()
{
    if (_impl->envs_filename.empty()) {
        return false;
    }
    if (!asset().exists(LAYOUT_ENV)) {
        return false;
    }
    _impl->envs.readResourceXmlFile(asset().get(LAYOUT_ENV) / _impl->envs_filename);
    return true;
}

void Storage::readEnvDefault()
{
    _impl->envs = Environments::createDefaultEnvironments();
}

void Storage::readEnvParams(char ** envs)
{
    if (envs != nullptr) {
        _impl->envs.parse(envs);
    }
}

bool Storage::saveEnv()
{
    if (_impl->envs_filename.empty()) {
        return false;
    }
    if (!asset().exists(LAYOUT_ENV)) {
        return false;
    }
    auto const ENV_DIR = asset().get(LAYOUT_ENV);
    if (!ENV_DIR.exists()) {
        ENV_DIR.createDir();
    }
    if (!ENV_DIR.isDirectory() || !ENV_DIR.isWritable()) {
        return false;
    }
    return _impl->envs.saveResourceXmlFile(ENV_DIR / _impl->envs_filename);
}

void Storage::clearEnv()
{
    _impl->envs.clear();
}

void Storage::setEnv(std::string const & key, std::string const & value)
{
    _impl->envs.set(key, value);
}

bool Storage::getEnv(std::string const & key, std::string & value) const
{
    return _impl->envs.get(key, value);
}

std::vector<std::string> Storage::getEnvFilenames() const
{
    return getFilenames(LAYOUT_ENV);
}

std::string Storage::convert(std::string const & value) const
{
    return _impl->envs.convert(value);
}

bool Storage::readConfig(std::string const & filename, std::string const & key, std::string & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    Resource res;
    if (!res.readFile(asset().get(LAYOUT_CONFIG) / filename)) {
        return false;
    }
    return res.getString(key, &value);
}

bool Storage::saveConfig(std::string const & filename, std::string const & key, std::string const & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    auto const CONFIG_DIR = asset().get(LAYOUT_CONFIG);
    if (!CONFIG_DIR.exists()) {
        CONFIG_DIR.createDir();
    }
    if (!CONFIG_DIR.isDirectory() || !CONFIG_DIR.isWritable()) {
        return false;
    }
    auto const PATH = CONFIG_DIR / filename;
    Resource res;
    res.readFile(PATH);
    res.set(key, value);
    return res.saveFile(PATH);
}

std::vector<std::string> Storage::getConfigFilenames() const
{
    return getFilenames(LAYOUT_CONFIG);
}

std::vector<std::string> Storage::getConfigKeys(std::string const & filename) const
{
    std::vector<std::string> result;
    if (asset().exists(LAYOUT_CONFIG)) {
        Resource res;
        if (res.readFile(asset().get(LAYOUT_CONFIG) / filename)) {
            return res.keys();
        }
    }
    return result;
}

void Storage::removeConfig(std::string const & filename)
{
    if (asset().exists(LAYOUT_CONFIG)) {
        (asset().get(LAYOUT_CONFIG) / filename).remove();
    }
}

void Storage::removeAllConfig()
{
    for (auto const & filename : getConfigFilenames()) {
        removeConfig(filename);
    }
}

void Storage::setModuleExtension(std::string const & extension)
{
    _impl->module_extension = extension;
}

void Storage::setLayoutModule(std::string const & dir, std::string const & extension)
{
    setLayoutModule(dir);
    setModuleExtension(extension);
}

std::vector<std::string> Storage::getModuleFilenames() const
{
    using namespace libtbag::filesystem;
    using namespace libtbag::string;

    auto const LOWER_EXTENSION = lower(_impl->module_extension);
    if (LOWER_EXTENSION.empty()) {
        return getFilenames(LAYOUT_MODULE);
    }

    std::vector<std::string> result;
    for (auto & cursor : getFilenames(LAYOUT_MODULE)) {
        if (LOWER_EXTENSION == lower(Path(cursor).getExtensionName())) {
            result.push_back(cursor);
        }
    }
    return result;
}

Storage::SharedLibrary Storage::getModule(std::string const & filename) const
{
    if (asset().exists(LAYOUT_MODULE)) {
        return SharedLibrary(asset().get(LAYOUT_MODULE) / filename);
    }
    return SharedLibrary(nullptr);
}

void Storage::setTextLanguage(std::string const & language)
{
    _impl->text.setLanguage(language);
}

void Storage::setLayoutText(std::string const & dir, std::string const & language)
{
    setLayoutText(dir);
    setTextLanguage(language);
}

std::vector<std::string> Storage::getTextFilenames() const
{
    return getFilenames(LAYOUT_TEXT);
}

std::string Storage::getText(std::string const & language, std::string const & name) const
{
    return _impl->text.get(language, name);
}

std::string Storage::getText(std::string const & name) const
{
    return _impl->text.get(name);
}

void Storage::setText(std::string const & language, std::string const & name, std::string const & text)
{
    _impl->text.set(language, name, text);
}

void Storage::setText(std::string const & name, std::string const & text)
{
    _impl->text.set(name, text);
}

bool Storage::loadText()
{
    return _impl->text.load(asset().get(LAYOUT_TEXT));
}

bool Storage::saveText()
{
    return _impl->text.save(asset().get(LAYOUT_TEXT));
}

bool Storage::openSqlite(std::string const & filename, bool auto_close)
{
    if (auto_close && _impl->sqlite.isOpen()) {
        _impl->sqlite.close();
    }
    return _impl->sqlite.open(asset().get(LAYOUT_SQLITE) / filename);
}

void Storage::closeSqlite()
{
    _impl->sqlite.close();
}

bool Storage::isOpenSqlite() const
{
    return _impl->sqlite.isOpen();
}

std::vector<std::string> Storage::getSqliteFilenames() const
{
    return getFilenames(LAYOUT_SQLITE);
}

void Storage::setAutoClearTempFiles(bool enable)
{
    if (enable) {
        _impl->clear_tempdir = asset().get(LAYOUT_TEMP);
    } else {
        _impl->clear_tempdir.clear();
    }
}

bool Storage::isAutoClearTempFiles() const
{
    return !_impl->clear_tempdir.empty();
}

void Storage::clearTempDir()
{
    _remove_child_files(asset().get(LAYOUT_TEMP).getString());
}

std::string Storage::generateTempPath(std::size_t name_size) const
{
    std::string name;
    name.resize(name_size);
    auto const TEMP_PREFIX = asset().get(LAYOUT_TEMP);
    for (std::size_t i = 0; i < RETRY_COUNT_OF_TEMP_NAME; ++i) {
        if (libtbag::string::createRandomString(&name[0], name_size)) {
            auto const TEMP_PATH = TEMP_PREFIX / name;
            if (!TEMP_PATH.exists()) {
                return TEMP_PATH.toString();
            }
        }
    }
    return std::string();
}

bool Storage::openKeyStore(std::string const & filename)
{
    return _impl->keystore.open(KeyStore::Type::T_FILE, asset().get(LAYOUT_KEYSTORE) / filename);
}

void Storage::closeKeyStore()
{
    return _impl->keystore.close();
}

bool Storage::isOpenKeyStore() const
{
    return _impl->keystore.isOpen();
}

bool Storage::createKey(std::string const & key)
{
    return _impl->keystore.create(key);
}

bool Storage::removeKey(std::string const & key)
{
    return _impl->keystore.remove(key);
}

bool Storage::getKey(std::string const & key, std::string & result)
{
    return _impl->keystore.get(key, result);
}

bool Storage::setKey(std::string const & key, std::string const & value, bool encrypt)
{
    return _impl->keystore.set(key, value, encrypt);
}

bool Storage::cmpKey(std::string const & key, std::string const & value, bool encrypt)
{
    return _impl->keystore.cmp(key, value, encrypt);
}

std::vector<std::string> Storage::listKey()
{
    return _impl->keystore.list();
}

std::vector<std::string> Storage::getKeyStoreFilenames() const
{
    return getFilenames(LAYOUT_KEYSTORE);
}

void Storage::initLuaDefault()
{
    _impl->lua.initDefault();
}

bool Storage::appendLuaCPath()
{
    auto const CPATH = asset().get(LAYOUT_MODULE);
    auto const CANONICAL_CPATH = (CPATH / "?.so").getCanonicalString();
    return _impl->lua.appendLuaCPath(CANONICAL_CPATH);
}

bool Storage::appendLuaPath()
{
    auto const PATH = asset().get(LAYOUT_LUA);
    auto const CANONICAL_PATH = (PATH / "?.lua").getCanonicalString();
    return _impl->lua.appendLuaPath(CANONICAL_PATH);
}

bool Storage::runLuaScriptFile(std::string const & filename)
{
    return _impl->lua.runScriptFile(asset().get(LAYOUT_LUA) / filename);
}

bool Storage::runLuaScript(std::string const & code)
{
    return _impl->lua.runScript(code);
}

std::vector<std::string> Storage::getLuaFilenames() const
{
    return getFilenames(LAYOUT_LUA);
}

bool Storage::appendLuaRocksPrefix()
{
    return _impl->lua.appendPrefix(asset().get(LAYOUT_LUAROCKS));
}

std::string Storage::getInfo() const
{
    using namespace libtbag::string;
    std::stringstream ss;
    auto const ENV_KEYS = envs().keys();
    if (!ENV_KEYS.empty()) {
        ss << "[ENVIRONMENTS]\n";
        for (auto & env_key : ENV_KEYS) {
            ss << fformat(" {}={}\n", env_key, envs().opt(env_key));
        }
    }

    auto const ASSET_KEYS = asset().getKeys();
    if (!ASSET_KEYS.empty()) {
        ss << "[ASSET]\n";
        for (auto & asset_key : ASSET_KEYS) {
            ss << fformat(" {}={}\n", asset_key, asset().get(asset_key));
        }
    }

    auto const LUA_CPATHS = lua().getLuaCPaths();
    if (!LUA_CPATHS.empty()) {
        ss << "[LUA_CPATH]\n";
        for (auto & cpath : LUA_CPATHS) {
            ss << fformat(" {}\n", cpath);
        }
    }

    auto const LUA_PATHS = lua().getLuaPaths();
    if (!LUA_PATHS.empty()) {
        ss << "[LUA_PATH]\n";
        for (auto & path : LUA_PATHS) {
            ss << fformat(" {}\n", path);
        }
    }

    return ss.str();
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

