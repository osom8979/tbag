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

StorageNode::StorageNode(char ** envs) : _envs(envs)
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
    prop.env      .text = TAG_ENV      ;
    prop.config   .text = TAG_CONFIG   ;
    prop.module   .text = TAG_MODULE   ;
    prop.text     .text = TAG_TEXT     ;
    prop.image    .text = TAG_IMAGE    ;
    prop.drawable .text = TAG_DRAWABLE ;
    prop.animation.text = TAG_ANIMATION;
    prop.sprite   .text = TAG_SPRITE   ;
    prop.lmdb     .text = TAG_LMDB     ;
    prop.sqlite   .text = TAG_SQLITE   ;
    prop.temp     .text = TAG_TEMP     ;
    prop.keystore .text = TAG_KEYSTORE ;
    prop.lua      .text = TAG_LUA      ;
    prop.raw      .text = TAG_RAW      ;
    prop.bagex    .text = TAG_BAGEX    ;
    prop.exe      .text = TAG_EXE      ;
    prop.font     .text = TAG_FONT     ;
    prop.music    .text = TAG_MUSIC    ;
    prop.sound    .text = TAG_SOUND    ;
    prop.shader   .text = TAG_SHADER   ;
    prop.layout   .text = TAG_LAYOUT   ;
    prop.style    .text = TAG_STYLE    ;
    prop.color    .text = TAG_COLOR    ;
    // @formatter:on

    // @formatter:off
    prop.env      .exists = true;
    prop.config   .exists = true;
    prop.module   .exists = true;
    prop.text     .exists = true;
    prop.image    .exists = true;
    prop.drawable .exists = true;
    prop.animation.exists = true;
    prop.sprite   .exists = true;
    prop.lmdb     .exists = true;
    prop.sqlite   .exists = true;
    prop.temp     .exists = true;
    prop.keystore .exists = true;
    prop.lua      .exists = true;
    prop.raw      .exists = true;
    prop.bagex    .exists = true;
    prop.exe      .exists = true;
    prop.font     .exists = true;
    prop.music    .exists = true;
    prop.sound    .exists = true;
    prop.shader   .exists = true;
    prop.layout   .exists = true;
    prop.style    .exists = true;
    prop.color    .exists = true;
    // @formatter:on

    update(libtbag::filesystem::Path::getWorkDir(), prop);
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
    readElement(element, TAG_IMAGE    , prop.image);
    readElement(element, TAG_DRAWABLE , prop.drawable);
    readElement(element, TAG_ANIMATION, prop.animation);
    readElement(element, TAG_SPRITE   , prop.sprite);
    readElement(element, TAG_LMDB     , prop.lmdb);
    readElement(element, TAG_SQLITE   , prop.sqlite);
    readElement(element, TAG_TEMP     , prop.temp);
    readElement(element, TAG_KEYSTORE , prop.keystore);
    readElement(element, TAG_LUA      , prop.lua);
    readElement(element, TAG_RAW      , prop.raw);
    readElement(element, TAG_BAGEX    , prop.bagex);
    readElement(element, TAG_EXE      , prop.exe);
    readElement(element, TAG_FONT     , prop.font);
    readElement(element, TAG_MUSIC    , prop.music);
    readElement(element, TAG_SOUND    , prop.sound);
    readElement(element, TAG_SHADER   , prop.shader);
    readElement(element, TAG_LAYOUT   , prop.layout);
    readElement(element, TAG_STYLE    , prop.style);
    readElement(element, TAG_COLOR    , prop.color);
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
    addNewElement(element, TAG_IMAGE    , _prop.image);
    addNewElement(element, TAG_DRAWABLE , _prop.drawable);
    addNewElement(element, TAG_ANIMATION, _prop.animation);
    addNewElement(element, TAG_SPRITE   , _prop.sprite);
    addNewElement(element, TAG_LMDB     , _prop.lmdb);
    addNewElement(element, TAG_SQLITE   , _prop.sqlite);
    addNewElement(element, TAG_TEMP     , _prop.temp);
    addNewElement(element, TAG_KEYSTORE , _prop.keystore);
    addNewElement(element, TAG_LUA      , _prop.lua);
    addNewElement(element, TAG_RAW      , _prop.raw);
    addNewElement(element, TAG_BAGEX    , _prop.bagex);
    addNewElement(element, TAG_EXE      , _prop.exe);
    addNewElement(element, TAG_FONT     , _prop.font);
    addNewElement(element, TAG_MUSIC    , _prop.music);
    addNewElement(element, TAG_SOUND    , _prop.sound);
    addNewElement(element, TAG_SHADER   , _prop.shader);
    addNewElement(element, TAG_LAYOUT   , _prop.layout);
    addNewElement(element, TAG_STYLE    , _prop.style);
    addNewElement(element, TAG_COLOR    , _prop.color);
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
        optAttr(*child, ATT_DYNASM  , layout.dynasm);
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
            setAttr(child, ATT_DYNASM  , layout.dynasm);
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
    if (prop.config   .exists) { storage.setLayoutConfig   (getPath(updated_root, TAG_CONFIG   , prop.config   , ENVIRONMENTS)); }
    if (prop.module   .exists) { storage.setLayoutModule   (getPath(updated_root, TAG_MODULE   , prop.module   , ENVIRONMENTS)); }
    if (prop.text     .exists) { storage.setLayoutText     (getPath(updated_root, TAG_TEXT     , prop.text     , ENVIRONMENTS)); }
    if (prop.image    .exists) { storage.setLayoutImage    (getPath(updated_root, TAG_IMAGE    , prop.image    , ENVIRONMENTS)); }
    if (prop.drawable .exists) { storage.setLayoutDrawable (getPath(updated_root, TAG_DRAWABLE , prop.drawable , ENVIRONMENTS)); }
    if (prop.animation.exists) { storage.setLayoutAnimation(getPath(updated_root, TAG_ANIMATION, prop.animation, ENVIRONMENTS)); }
    if (prop.sprite   .exists) { storage.setLayoutSprite   (getPath(updated_root, TAG_SPRITE   , prop.sprite   , ENVIRONMENTS)); }
    if (prop.lmdb     .exists) { storage.setLayoutLmdb     (getPath(updated_root, TAG_LMDB     , prop.lmdb     , ENVIRONMENTS)); }
    if (prop.sqlite   .exists) { storage.setLayoutSqlite   (getPath(updated_root, TAG_SQLITE   , prop.sqlite   , ENVIRONMENTS)); }
    if (prop.temp     .exists) { storage.setLayoutTemp     (getPath(updated_root, TAG_TEMP     , prop.temp     , ENVIRONMENTS)); }
    if (prop.keystore .exists) { storage.setLayoutKeystore (getPath(updated_root, TAG_KEYSTORE , prop.keystore , ENVIRONMENTS)); }
    if (prop.lua      .exists) { storage.setLayoutLua      (getPath(updated_root, TAG_LUA      , prop.lua      , ENVIRONMENTS)); }
    if (prop.raw      .exists) { storage.setLayoutRaw      (getPath(updated_root, TAG_RAW      , prop.raw      , ENVIRONMENTS)); }
    if (prop.bagex    .exists) { storage.setLayoutBagex    (getPath(updated_root, TAG_BAGEX    , prop.bagex    , ENVIRONMENTS)); }
    if (prop.exe      .exists) { storage.setLayoutExe      (getPath(updated_root, TAG_EXE      , prop.exe      , ENVIRONMENTS)); }
    if (prop.font     .exists) { storage.setLayoutFont     (getPath(updated_root, TAG_FONT     , prop.font     , ENVIRONMENTS)); }
    if (prop.music    .exists) { storage.setLayoutMusic    (getPath(updated_root, TAG_MUSIC    , prop.music    , ENVIRONMENTS)); }
    if (prop.sound    .exists) { storage.setLayoutSound    (getPath(updated_root, TAG_SOUND    , prop.sound    , ENVIRONMENTS)); }
    if (prop.shader   .exists) { storage.setLayoutShader   (getPath(updated_root, TAG_SHADER   , prop.shader   , ENVIRONMENTS)); }
    if (prop.layout   .exists) { storage.setLayoutLayout   (getPath(updated_root, TAG_LAYOUT   , prop.layout   , ENVIRONMENTS)); }
    if (prop.style    .exists) { storage.setLayoutStyle    (getPath(updated_root, TAG_STYLE    , prop.style    , ENVIRONMENTS)); }
    if (prop.color    .exists) { storage.setLayoutColor    (getPath(updated_root, TAG_COLOR    , prop.color    , ENVIRONMENTS)); }
    // @formatter:on

    if (prop.module.exists) {
        storage.setModuleExtension(prop.module.ext);
    }
    if (prop.text.exists) {
        storage.setTextLanguage(prop.text.name);
        storage.loadText();
    }

    // drawable
    // animation
    // sprite

    // lmdb
    // sqlite
    // temp
    // keystore
    // lua
    // raw
    // bagex
    // exe
    // font
    // music
    // sound
    // shader
    // layout
    // style
    // color

    return storage;
}

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

