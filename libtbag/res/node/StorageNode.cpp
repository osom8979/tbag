/**
 * @file   StorageNode.cpp
 * @brief  StorageNode class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/node/StorageNode.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res  {
namespace node {

StorageNode::StorageNode(char ** envs)
        : _root(DEFAULT_STORAGE_ROOT), _envs(envs)
{
    // EMPTY.
}

StorageNode::StorageNode(std::string const & root, char ** envs)
        : _root(root), _envs(envs)
{
    // EMPTY.
}

StorageNode::StorageNode(StorageNode const & obj) : StorageNode()
{
    (*this) = obj;
}

StorageNode::StorageNode(StorageNode && obj) TBAG_NOEXCEPT : StorageNode()
{
    (*this) = std::move(obj);
}

StorageNode::~StorageNode()
{
    // EMPTY.
}

StorageNode & StorageNode::operator =(StorageNode const & obj)
{
    copy(obj);
    return *this;
}

StorageNode & StorageNode::operator =(StorageNode && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void StorageNode::copy(StorageNode const & obj)
{
    if (this != &obj) {
        _envs    = obj._envs;
        _storage = obj.storage();
        _root    = obj._root;
        _prop    = obj._prop;
    }
}

void StorageNode::swap(StorageNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_envs, obj._envs);
        _storage.swap(obj._storage);
        _root.swap(obj._root);
        std::swap(_prop, obj._prop);
    }
}

void StorageNode::update()
{
    _storage = loadStorage(_root, _prop, _envs);
}

void StorageNode::update(std::string const & root)
{
    _root = root;
    _storage = loadStorage(root, _prop, _envs);
}

void StorageNode::update(Property const & prop)
{
    _prop = prop;
    _storage = loadStorage(_root, prop, _envs);
}

void StorageNode::update(std::string const & root, Property const & prop)
{
    _root = root;
    _prop = prop;
    _storage = loadStorage(root, prop, _envs);
}

std::string StorageNode::name() const
{
    return std::string(TAG_STORAGE);
}

void StorageNode::setup()
{
    Property prop;

    // @formatter:off
    prop.env     .text = TAG_ENV     ;
    prop.config  .text = TAG_CONFIG  ;
    prop.module  .text = TAG_MODULE  ;
    prop.text    .text = TAG_TEXT    ;
    prop.sqlite  .text = TAG_SQLITE  ;
    prop.temp    .text = TAG_TEMP    ;
    prop.keystore.text = TAG_KEYSTORE;
    prop.lua     .text = TAG_LUA     ;
    prop.luarocks.text = TAG_LUAROCKS;
    // @formatter:on

    // @formatter:off
    prop.env     .exists = true;
    prop.config  .exists = true;
    prop.module  .exists = true;
    prop.text    .exists = true;
    prop.sqlite  .exists = true;
    prop.temp    .exists = true;
    prop.keystore.exists = true;
    prop.lua     .exists = true;
    // @formatter:on

    update(DEFAULT_STORAGE_ROOT, prop);
}

void StorageNode::teardown()
{
    _storage.clear();
    _root.clear();
    _prop = Property{};
}

void StorageNode::load(Element const & element)
{
    std::string root;
    optAttr(element, ATT_ROOT, root);

    Property prop;
    // @formatter:off
    readElement(element, TAG_ENV      , prop.env);
    readElement(element, TAG_CONFIG   , prop.config);
    readElement(element, TAG_MODULE   , prop.module);
    readElement(element, TAG_TEXT     , prop.text);
    readElement(element, TAG_SQLITE   , prop.sqlite);
    readElement(element, TAG_TEMP     , prop.temp);
    readElement(element, TAG_KEYSTORE , prop.keystore);
    readElement(element, TAG_LUA      , prop.lua);
    readElement(element, TAG_LUA      , prop.luarocks);
    // @formatter:on

    foreachElement(element, TAG_USER, [&](Element const & node){
        Property::usr_layout user;
        readElement(node, user);
        prop.users.push_back(user);
    });

    update(root, prop);
}

void StorageNode::save(Element & element) const
{
    setAttr(element, ATT_ROOT, _root);

    // @formatter:off
    addNewElement(element, TAG_ENV      , _prop.env);
    addNewElement(element, TAG_CONFIG   , _prop.config);
    addNewElement(element, TAG_MODULE   , _prop.module);
    addNewElement(element, TAG_TEXT     , _prop.text);
    addNewElement(element, TAG_SQLITE   , _prop.sqlite);
    addNewElement(element, TAG_TEMP     , _prop.temp);
    addNewElement(element, TAG_KEYSTORE , _prop.keystore);
    addNewElement(element, TAG_LUA      , _prop.lua);
    addNewElement(element, TAG_LUA      , _prop.luarocks);
    // @formatter:on

    for (auto & user : _prop.users) {
        addNewElement(element, TAG_USER, user);
    }
}

void StorageNode::readElement(Element const & element, std::string const & tag, Property::env_layout & layout)
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

void StorageNode::readElement(Element const & element, std::string const & tag, Property::def_layout & layout)
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

void StorageNode::readElement(Element const & element, std::string const & tag, Property::mod_layout & layout)
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

void StorageNode::readElement(Element const & element, std::string const & tag, Property::txt_layout & layout)
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

void StorageNode::readElement(Element const & element, std::string const & tag, Property::sql_layout & layout)
{
    readElement(element, tag, (Property::txt_layout &)layout);
}

void StorageNode::readElement(Element const & element, std::string const & tag, Property::tmp_layout & layout)
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

void StorageNode::readElement(Element const & element, std::string const & tag, Property::key_layout & layout)
{
    readElement(element, tag, (Property::txt_layout &)layout);
}

void StorageNode::readElement(Element const & element, std::string const & tag, Property::lua_layout & layout)
{
    if (auto * child = element.FirstChildElement(tag.c_str())) {
        layout.exists = true;
        layout.text = text(*child);
        optAttr(*child, ATT_NAME    , layout.name);
        optAttr(*child, ATT_LIBS    , layout.libs);
        optAttr(*child, ATT_ABSOLUTE, layout.abs);
        optAttr(*child, ATT_RAW     , layout.raw);
    } else {
        layout.exists = false;
    }
}

void StorageNode::readElement(Element const & element, Property::usr_layout & layout)
{
    layout.exists = true;
    layout.text = text(element);
    optAttr(element, ATT_NAME    , layout.name);
    optAttr(element, ATT_ABSOLUTE, layout.abs);
    optAttr(element, ATT_RAW     , layout.raw);
}

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::env_layout const & layout)
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

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::def_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
        });
    }
}

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::mod_layout const & layout)
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

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::txt_layout const & layout)
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

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::sql_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::tmp_layout const & layout)
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

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::key_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::lua_layout const & layout)
{
    if (layout.exists) {
        newElement(element, tag.c_str(), [&](Element & child){
            text(child, layout.text);
            setAttr(child, ATT_NAME    , layout.name);
            setAttr(child, ATT_LIBS    , layout.libs);
            setAttr(child, ATT_ABSOLUTE, layout.abs);
            setAttr(child, ATT_RAW     , layout.raw);
        });
    }
}

void StorageNode::addNewElement(Element & element, std::string const & tag, Property::usr_layout const & layout)
{
    addNewElement(element, tag, (Property::txt_layout const &)(layout));
}

std::string StorageNode::getPath(std::string const & root, std::string const & tag, std::string const & text,
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

std::string StorageNode::getPath(std::string const & root, std::string const & tag,
                                 Property::def_layout const & layout, Environments const & env)
{
    return getPath(root, tag, layout.text, env, layout.abs, layout.raw);
}

StorageNode::Storage StorageNode::loadStorage(std::string const & root, Property const & prop, char ** envs)
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

    // @formatter:off
    if (prop.config  .exists) { storage.setLayoutConfig  (getPath(updated_root, TAG_CONFIG  , prop.config  , ENVIRONMENTS)); }
    if (prop.module  .exists) { storage.setLayoutModule  (getPath(updated_root, TAG_MODULE  , prop.module  , ENVIRONMENTS)); }
    if (prop.text    .exists) { storage.setLayoutText    (getPath(updated_root, TAG_TEXT    , prop.text    , ENVIRONMENTS)); }
    if (prop.sqlite  .exists) { storage.setLayoutSqlite  (getPath(updated_root, TAG_SQLITE  , prop.sqlite  , ENVIRONMENTS)); }
    if (prop.temp    .exists) { storage.setLayoutTemp    (getPath(updated_root, TAG_TEMP    , prop.temp    , ENVIRONMENTS)); }
    if (prop.keystore.exists) { storage.setLayoutKeystore(getPath(updated_root, TAG_KEYSTORE, prop.keystore, ENVIRONMENTS)); }
    if (prop.lua     .exists) { storage.setLayoutLua     (getPath(updated_root, TAG_LUA     , prop.lua     , ENVIRONMENTS)); }
    if (prop.luarocks.exists) { storage.setLayoutLuaRocks(getPath(updated_root, TAG_LUAROCKS, prop.luarocks, ENVIRONMENTS)); }
    // @formatter:on

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
        if (prop.lua.libs) {
            storage.loadLibraries();
            storage.appendLuaPath();
        }
        if (!prop.lua.name.empty()) {
            storage.runLuaScriptFile(prop.lua.name);
        }
    }
    if (prop.luarocks.exists) {
        storage.appendLuaPath();
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

    return storage;
}

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

