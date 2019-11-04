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
    Property prop;

    // clang-format off
    prop.env     .text = TAG_ENV     ;
    prop.config  .text = TAG_CONFIG  ;
    prop.module  .text = TAG_MODULE  ;
    prop.text    .text = TAG_TEXT    ;
    prop.sqlite  .text = TAG_SQLITE  ;
    prop.temp    .text = TAG_TEMP    ;
    prop.keystore.text = TAG_KEYSTORE;
    prop.lua     .text = TAG_LUA     ;
    prop.luarocks.text = TAG_LUAROCKS;
    // clang-format on

    // clang-format off
    prop.env     .exists = true;
    prop.config  .exists = true;
    prop.module  .exists = true;
    prop.text    .exists = true;
    prop.sqlite  .exists = true;
    prop.temp    .exists = true;
    prop.keystore.exists = true;
    prop.lua     .exists = true;
    prop.luarocks.exists = true;
    // clang-format on

    // clang-format off
    prop.env.name      = VAL_DEFAULT_ENV_NAME;
    prop.module.ext    = VAL_DEFAULT_MODULE_EXT;
    prop.text.name     = VAL_DEFAULT_TEXT_NAME;
    prop.sqlite.name   = VAL_DEFAULT_SQLITE_NAME;
    prop.keystore.name = VAL_DEFAULT_KEYSTORE_NAME;
    prop.lua.name      = VAL_DEFAULT_LUA_NAME;
    prop.lua.init      = VAL_DEFAULT_LUA_INIT;
    // clang-format on

    update(DEFAULT_STORAGE_ROOT, prop);

    return true;
}

void StoragePref::load(Element const & element)
{
    std::string root;
    optAttr(element, ATT_ROOT, root);

    Property prop;
    // clang-format off
    readElement(element, TAG_ENV      , prop.env);
    readElement(element, TAG_CONFIG   , prop.config);
    readElement(element, TAG_MODULE   , prop.module);
    readElement(element, TAG_TEXT     , prop.text);
    readElement(element, TAG_SQLITE   , prop.sqlite);
    readElement(element, TAG_TEMP     , prop.temp);
    readElement(element, TAG_KEYSTORE , prop.keystore);
    readElement(element, TAG_LUA      , prop.lua);
    readElement(element, TAG_LUAROCKS , prop.luarocks);
    // clang-format on

    foreachElement(element, TAG_USER, [&](Element const & node){
        Property::usr_layout user;
        readElement(node, user);
        prop.users.push_back(user);
    });

    update(root, prop);
}

void StoragePref::save(Element & element) const
{
    setAttr(element, ATT_ROOT, _root);

    // clang-format off
    addNewElement(element, TAG_ENV      , _prop.env);
    addNewElement(element, TAG_CONFIG   , _prop.config);
    addNewElement(element, TAG_MODULE   , _prop.module);
    addNewElement(element, TAG_TEXT     , _prop.text);
    addNewElement(element, TAG_SQLITE   , _prop.sqlite);
    addNewElement(element, TAG_TEMP     , _prop.temp);
    addNewElement(element, TAG_KEYSTORE , _prop.keystore);
    addNewElement(element, TAG_LUA      , _prop.lua);
    addNewElement(element, TAG_LUAROCKS , _prop.luarocks);
    // clang-format on

    for (auto & user : _prop.users) {
        addNewElement(element, TAG_USER, user);
    }
}

void StoragePref::update()
{
    _storage = loadStorage(_root, _prop, _envs);
}

void StoragePref::update(std::string const & root)
{
    _root = root;
    _storage = loadStorage(root, _prop, _envs);
}

void StoragePref::update(Property const & prop)
{
    _prop = prop;
    _storage = loadStorage(_root, prop, _envs);
}

void StoragePref::update(std::string const & root, Property const & prop)
{
    _root = root;
    _prop = prop;
    _storage = loadStorage(root, prop, _envs);
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

StoragePref::Storage StoragePref::loadStorage(std::string const & root, Property const & prop, char ** envs)
{
    auto const DEFAULT_ENVS = Environments::createDefaultEnvironments();
    Storage storage;

    std::string updated_root;
    if (root.empty()) {
        updated_root = libtbag::filesystem::Path::getWorkDir();
    } else {
        updated_root = DEFAULT_ENVS.convert(root);
    }

    if (prop.env.exists) {
        storage.setLayoutEnv(getPath(updated_root, TAG_ENV, prop.env.text, DEFAULT_ENVS, prop.env.abs));

        if (!prop.env.name.empty()) {
            storage.setEnvFilename(prop.env.name);
            storage.readEnv();
        }

        if (prop.env.def) {
            storage.readEnvDefault();
        }

        if (prop.env.sys && envs != nullptr) {
            storage.readEnvParams(envs);
        }
    } else {
        storage.readEnvDefault();
    }

    // ----------------------------------------
    auto const & ENVIRONMENTS = storage.envs();
    // ----------------------------------------

    // clang-format off
    if (prop.config  .exists) { storage.setLayoutConfig  (getPath(updated_root, TAG_CONFIG  , prop.config  , ENVIRONMENTS)); }
    if (prop.module  .exists) { storage.setLayoutModule  (getPath(updated_root, TAG_MODULE  , prop.module  , ENVIRONMENTS)); }
    if (prop.text    .exists) { storage.setLayoutText    (getPath(updated_root, TAG_TEXT    , prop.text    , ENVIRONMENTS)); }
    if (prop.sqlite  .exists) { storage.setLayoutSqlite  (getPath(updated_root, TAG_SQLITE  , prop.sqlite  , ENVIRONMENTS)); }
    if (prop.temp    .exists) { storage.setLayoutTemp    (getPath(updated_root, TAG_TEMP    , prop.temp    , ENVIRONMENTS)); }
    if (prop.keystore.exists) { storage.setLayoutKeystore(getPath(updated_root, TAG_KEYSTORE, prop.keystore, ENVIRONMENTS)); }
    if (prop.lua     .exists) { storage.setLayoutLua     (getPath(updated_root, TAG_LUA     , prop.lua     , ENVIRONMENTS)); }
    if (prop.luarocks.exists) { storage.setLayoutLuaRocks(getPath(updated_root, TAG_LUAROCKS, prop.luarocks, ENVIRONMENTS)); }
    // clang-format on

    if (prop.config.exists) {
        // EMPTY.
    }
    if (prop.module.exists) {
        storage.setModuleExtension(prop.module.ext);
    }
    if (prop.text.exists) {
        storage.setTextLanguage(prop.text.name);
        storage.loadText();
    }
    if (prop.sqlite.exists) {
        storage.openSqlite(prop.sqlite.name);
    }
    if (prop.temp.exists && prop.temp.auto_clear) {
        storage.setAutoClearTempFiles();
    }
    if (prop.keystore.exists) {
        storage.openKeyStore(prop.keystore.name);
    }
    if (prop.lua.exists) {
        if (prop.lua.init) {
            storage.initLuaDefault();
            if (prop.module.exists) {
                storage.appendLuaCPath();
            }
            storage.appendLuaPath();
            if (prop.luarocks.exists) {
                storage.appendLuaRocksPrefix();
            }
        }
        if (!prop.lua.name.empty()) {
            storage.runLuaScriptFile(prop.lua.name);
        }
    }

    auto & asset = storage.asset();
    for (auto & user : prop.users) {
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

