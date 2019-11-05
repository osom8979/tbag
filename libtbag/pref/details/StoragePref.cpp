/**
 * @file   StoragePref.cpp
 * @brief  StoragePref class implementation.
 * @author zer0
 * @date   2018-11-03
 * @date   2019-11-03 (Rename: libtbag/res/node/StorageNode -> libtbag/pref/details/StoragePref)
 */

#include <libtbag/pref/details/StoragePref.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

StoragePref::StoragePref()
{
    // EMPTY.
}

StoragePref::~StoragePref()
{
    // EMPTY.
}

std::string StoragePref::name() const
{
    return TAG_STORAGE;
}

bool StoragePref::init()
{
    // clang-format off
    _prop.env      .text = TAG_ENV     ;
    _prop.config   .text = TAG_CONFIG  ;
    _prop.module   .text = TAG_MODULE  ;
    _prop.text     .text = TAG_TEXT    ;
    _prop.sqlite   .text = TAG_SQLITE  ;
    _prop.temp     .text = TAG_TEMP    ;
    _prop.keystore .text = TAG_KEYSTORE;
    _prop.lua      .text = TAG_LUA     ;
    _prop.lua_rocks.text = TAG_LUAROCKS;
    // clang-format on

    // clang-format off
    _prop.env      .exists = true;
    _prop.config   .exists = true;
    _prop.module   .exists = true;
    _prop.text     .exists = true;
    _prop.sqlite   .exists = true;
    _prop.temp     .exists = true;
    _prop.keystore .exists = true;
    _prop.lua      .exists = true;
    _prop.lua_rocks.exists = true;
    // clang-format on

    // clang-format off
    _prop.env.name      = VAL_DEFAULT_ENV_NAME;
    _prop.module.ext    = VAL_DEFAULT_MODULE_EXT;
    _prop.text.name     = VAL_DEFAULT_TEXT_NAME;
    _prop.sqlite.name   = VAL_DEFAULT_SQLITE_NAME;
    _prop.keystore.name = VAL_DEFAULT_KEYSTORE_NAME;
    _prop.lua.name      = VAL_DEFAULT_LUA_NAME;
    _prop.lua.init      = VAL_DEFAULT_LUA_INIT;
    // clang-format on

    return true;
}

void StoragePref::clear()
{
    _prop = {};
}

void StoragePref::load(Element const & element)
{
    optAttr(element, ATT_ROOT, _prop.root);

    // clang-format off
    readElement(element, TAG_ENV     , _prop.env);
    readElement(element, TAG_CONFIG  , _prop.config);
    readElement(element, TAG_MODULE  , _prop.module);
    readElement(element, TAG_TEXT    , _prop.text);
    readElement(element, TAG_SQLITE  , _prop.sqlite);
    readElement(element, TAG_TEMP    , _prop.temp);
    readElement(element, TAG_KEYSTORE, _prop.keystore);
    readElement(element, TAG_LUA     , _prop.lua);
    readElement(element, TAG_LUAROCKS, _prop.lua_rocks);
    // clang-format on

    foreachElement(element, TAG_USER, [&](Element const & node){
        Property::usr_layout user;
        readElement(node, user);
        _prop.users.push_back(user);
    });
}

void StoragePref::save(Element & element) const
{
    setAttr(element, ATT_ROOT, _prop.root);

    // clang-format off
    addNewElement(element, TAG_ENV      , _prop.env);
    addNewElement(element, TAG_CONFIG   , _prop.config);
    addNewElement(element, TAG_MODULE   , _prop.module);
    addNewElement(element, TAG_TEXT     , _prop.text);
    addNewElement(element, TAG_SQLITE   , _prop.sqlite);
    addNewElement(element, TAG_TEMP     , _prop.temp);
    addNewElement(element, TAG_KEYSTORE , _prop.keystore);
    addNewElement(element, TAG_LUA      , _prop.lua);
    addNewElement(element, TAG_LUAROCKS , _prop.lua_rocks);
    // clang-format on

    for (auto const & user : _prop.users) {
        addNewElement(element, TAG_USER, user);
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::env_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_NAME    , layout.name);
        optAttr(*child, ATT_DEFAULT , layout.def);
        optAttr(*child, ATT_SYSTEM  , layout.sys);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::def_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
        optAttr(*child, ATT_RAW     , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::cfg_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
        optAttr(*child, ATT_RAW     , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::mod_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_EXTENSION, layout.ext);
        optAttr(*child, ATT_ABSOLUTE , layout.abs);
        optAttr(*child, ATT_RAW      , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::txt_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_NAME    , layout.name);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
        optAttr(*child, ATT_RAW     , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::sql_layout & layout)
{
    readElement(element, tag, (Property::txt_layout &)layout);
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::tmp_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_AUTO_CLEAR, layout.auto_clear);
        optAttr(*child, ATT_ABSOLUTE  , layout.abs);
        optAttr(*child, ATT_RAW       , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::key_layout & layout)
{
    readElement(element, tag, (Property::txt_layout &)layout);
}

void StoragePref::readElement(Element const & element, std::string const & tag, Property::lua_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_NAME    , layout.name);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
        optAttr(*child, ATT_RAW     , layout.raw);
        optAttr(*child, ATT_INIT    , layout.init);
    } else {
        layout.exists = false;
    }
}

void StoragePref::readElement(Element const & element, Property::usr_layout & layout)
{
    layout.exists = true;
    layout.text = text(element);
    optAttr(element, ATT_NAME    , layout.name, layout.text);
    optAttr(element, ATT_ABSOLUTE, layout.abs);
    optAttr(element, ATT_RAW     , layout.raw);
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::env_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_NAME    , layout.name);
            setAttr(child, ATT_DEFAULT , layout.def);
            setAttr(child, ATT_SYSTEM  , layout.sys);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::def_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::cfg_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::mod_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_EXTENSION, layout.ext);
            setAttr(child, ATT_ABSOLUTE , layout.abs);
            setAttr(child, ATT_RAW      , layout.raw);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::txt_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_NAME    , layout.name);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::sql_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::tmp_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_AUTO_CLEAR, layout.auto_clear);
            setAttr(child, ATT_ABSOLUTE  , layout.abs);
            setAttr(child, ATT_RAW       , layout.raw);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::key_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::lua_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_NAME    , layout.name);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
            setAttr(child, ATT_INIT    , layout.init);
        });
    }
}

void StoragePref::addNewElement(Element & element, std::string const & tag, Property::usr_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

std::string StoragePref::getPath(std::string const & root, std::string const & tag, std::string const & text,
                                 Environments const & env, bool abs, bool raw)
{
    std::string path;
    if (raw) {
        path = text;
    } else {
        path = env.convert(text);
    }

    if (path.empty()) {
        path = tag;
    }

    if (abs) {
        return path;
    }

    assert(!abs);
    if (root.empty()) {
        return libtbag::filesystem::Path::getWorkDir() / path;
    }
    return libtbag::filesystem::Path(root) / path;
}

std::string StoragePref::getPath(std::string const & root, std::string const & tag,
                                 Property::def_layout const & layout, Environments const & env)
{
    return getPath(root, tag, layout.text, env, layout.abs, layout.raw);
}

StoragePref::Storage StoragePref::loadStorage() const
{
    auto const DEFAULT_ENVS = Environments::createDefaultEnvironments(true);
    Storage storage;

    std::string updated_root;
    if (_prop.root.empty()) {
        updated_root = libtbag::filesystem::Path::getWorkDir();
    } else {
        updated_root = DEFAULT_ENVS.convert(_prop.root);
    }

    if (_prop.env.exists) {
        storage.setLayoutEnv(getPath(updated_root, TAG_ENV, _prop.env.text, DEFAULT_ENVS, _prop.env.abs));

        if (!_prop.env.name.empty()) {
            storage.setEnvFilename(_prop.env.name);
            storage.readEnv();
        }

        if (_prop.env.def) {
            storage.readEnvDefault(_prop.env.sys);
        }
    } else {
        storage.readEnvDefault();
    }

    // ----------------------------------------
    auto const & ENVIRONMENTS = storage.envs();
    // ----------------------------------------

    // clang-format off
    if (_prop.config   .exists) { storage.setLayoutConfig  (getPath(updated_root, TAG_CONFIG  , _prop.config   , ENVIRONMENTS)); }
    if (_prop.module   .exists) { storage.setLayoutModule  (getPath(updated_root, TAG_MODULE  , _prop.module   , ENVIRONMENTS)); }
    if (_prop.text     .exists) { storage.setLayoutText    (getPath(updated_root, TAG_TEXT    , _prop.text     , ENVIRONMENTS)); }
    if (_prop.sqlite   .exists) { storage.setLayoutSqlite  (getPath(updated_root, TAG_SQLITE  , _prop.sqlite   , ENVIRONMENTS)); }
    if (_prop.temp     .exists) { storage.setLayoutTemp    (getPath(updated_root, TAG_TEMP    , _prop.temp     , ENVIRONMENTS)); }
    if (_prop.keystore .exists) { storage.setLayoutKeystore(getPath(updated_root, TAG_KEYSTORE, _prop.keystore , ENVIRONMENTS)); }
    if (_prop.lua      .exists) { storage.setLayoutLua     (getPath(updated_root, TAG_LUA     , _prop.lua      , ENVIRONMENTS)); }
    if (_prop.lua_rocks.exists) { storage.setLayoutLuaRocks(getPath(updated_root, TAG_LUAROCKS, _prop.lua_rocks, ENVIRONMENTS)); }
    // clang-format on

    if (_prop.config.exists) {
        // EMPTY.
    }
    if (_prop.module.exists) {
        storage.setModuleExtension(_prop.module.ext);
    }
    if (_prop.text.exists) {
        storage.setTextLanguage(_prop.text.name);
        storage.loadText();
    }
    if (_prop.sqlite.exists) {
        storage.openSqlite(_prop.sqlite.name);
    }
    if (_prop.temp.exists && _prop.temp.auto_clear) {
        storage.setAutoClearTempFiles();
    }
    if (_prop.keystore.exists) {
        storage.openKeyStore(_prop.keystore.name);
    }
    if (_prop.lua.exists) {
        if (_prop.lua.init) {
            storage.initLuaDefault();
            if (_prop.module.exists) {
                storage.appendLuaCPath();
            }
            storage.appendLuaPath();
            if (_prop.lua_rocks.exists) {
                storage.appendLuaRocksPrefix();
            }
        }
        if (!_prop.lua.name.empty()) {
            storage.runLuaScriptFile(_prop.lua.name);
        }
    }

    auto & asset = storage.asset();
    for (auto & user : _prop.users) {
        if (user.name.empty()) {
            continue;
        }
        if (asset.exists(user.name)) {
            continue;
        }
        using namespace libtbag::filesystem;
        storage.asset().set(user.name, Path(getPath(updated_root, user.name, user, ENVIRONMENTS)));
    }

    storage.addAssetsToEnv();
    storage.setEnv(ENV_KEY_STORAGE_ROOT, updated_root);
    return storage;
}

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

