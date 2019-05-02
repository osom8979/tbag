/**
 * @file   StorageNode.hpp
 * @brief  StorageNode class prototype.
 * @author zer0
 * @date   2018-11-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_NODE_STORAGENODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_NODE_STORAGENODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/XmlModel.hpp>
#include <libtbag/string/Environments.hpp>
#include <libtbag/res/Storage.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res  {
namespace node {

/**
 * StorageNode class prototype.
 *
 * @author zer0
 * @date   2018-11-03
 *
 * @remarks
 *  XML format:
 *  @code{.xml}
 *   <!-- If 'root' attribute is exists, all layouts are sub-paths.         -->
 *   <!-- If 'root' attribute is not exists, It is set a working directory. -->
 *   <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *   <!-- Special exchange variables:         -->
 *   <!-- ${EXE_PATH} : EXE file path         -->
 *   <!-- ${EXE_NAME} : EXE file name         -->
 *   <!-- ${EXE_DIR}  : EXE directory         -->
 *   <!-- ${WORK_DIR} : Working directory     -->
 *   <!-- ${HOME_DIR} : User's HOME directory -->
 *   <storage root='dir'>
 *
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *     <!-- Used for special purposes.                                  -->
 *     <!-- The special tag uses the tag name if there is no path.      -->
 *     <!-- If 'absolute' attribute is 'true', Use an absolute path.    -->
 *     <!-- If 'absolute' attribute is not exists, It is set a 'false'. -->
 *     <!-- You can set the 'absolute' attribute on any <tag>.          -->
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *     <!-- Set environment variables that affect the all XML text.                    -->
 *     <!-- If 'name' attribute is exists, apply only those files.                     -->
 *     <!-- If the 'default' attribute is 'true', update 'special exchange variables'. -->
 *     <!-- If 'default' attribute is not exists, it is set a 'true'.                  -->
 *     <!-- If the 'system' attribute is 'true', use the '_envs' member.               -->
 *     <!-- If 'system' attribute is not exists, it is set a 'true'.                   -->
 *     <!-- [IMPORTANT] The 'env' tag does not have a 'raw' attribute.                 -->
 *     <env name='env1.xml' default='true' system='true' absolute='false'>dir</env>
 *
 *     <!-- If 'raw' attribute is 'true', Do not apply environment variables. -->
 *     <!-- If 'raw' attribute is not exists, It is set a 'false'.            -->
 *     <!-- You can set the 'env' attribute on any <tag>.                     -->
 *
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *     <!-- Contains simple 'group(filename)'/'key'/'value' information for configuration. -->
 *     <!-- If 'gui' attribute is exists, Configuration file to use in GUI mode.           -->
 *     <config gui='gui.xml'>dir</config>
 *
 *     <!-- A set of dynamic modules. (e.g. '*.dll' files)          -->
 *     <!-- If 'ext' attribute is exists, File extension filtering. -->
 *     <!-- If 'ext' attribute is not exists, No filtering.         -->
 *     <module ext='.so'>dir</module>
 *
 *     <!-- String for localization. -->
 *     <!-- If 'name' attribute is exists, Apply only those files. -->
 *     <text name='en.xml'>dir</text>
 *
 *     <!-- sqlite database storages. -->
 *     <!-- If 'name' attribute is exists, Apply only those files. -->
 *     <sqlite name='file.sqlite'>dir</sqlite>
 *
 *     <!-- Temporary directory. -->
 *     <!-- If 'autoclear' attribute is 'true', All files are removed when the object is deleted. -->
 *     <temp autoclear='true'>dir</temp>
 *
 *     <!-- The file to which the encryption applies. A password is required. -->
 *     <!-- If 'name' attribute is exists, Apply only those files.            -->
 *     <keystore name='file.key'>dir</keystore>
 *
 *     <!-- Directory containing the LuaRosks root directory. -->
 *     <luarocks>dir</luarocks>
 *
 *     <!-- Directory containing the lua script package.                    -->
 *     <!-- If the 'init' attribute is 'true', update 'default initialize'. -->
 *     <!-- If 'init' attribute is not exists, it is set a 'true'.          -->
 *     <!-- If 'name' attribute is exists, Apply only those files.          -->
 *     <lua init='true' name='file.lua'>dir</lua>
 *
 *     <!-- Specify the Layout name and path to hold the data. -->
 *     <user name='name1' absolute='true' raw='true'>dir1</user>
 *     <user name='name2' absolute='false'>dir2</user>
 *     <user name='name3'>dir3</user>
 *     <!-- ... -->
 *   </storage>
 *  @endcode
 */
class TBAG_API StorageNode : public libtbag::dom::xml::XmlModel::NodeInterface
{
public:
    using Base         = libtbag::dom::xml::XmlModel::NodeInterface;
    using Environments = libtbag::string::Environments;
    using Storage      = libtbag::res::Storage;

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_STORAGE_ROOT = "${WORK_DIR}/storage";

public:
    TBAG_CONSTEXPR static char const * const TAG_STORAGE = "storage";

public:
    // clang-format off
    TBAG_CONSTEXPR static char const * const TAG_ENV      = Storage::LAYOUT_ENV     ;
    TBAG_CONSTEXPR static char const * const TAG_CONFIG   = Storage::LAYOUT_CONFIG  ;
    TBAG_CONSTEXPR static char const * const TAG_MODULE   = Storage::LAYOUT_MODULE  ;
    TBAG_CONSTEXPR static char const * const TAG_TEXT     = Storage::LAYOUT_TEXT    ;
    TBAG_CONSTEXPR static char const * const TAG_SQLITE   = Storage::LAYOUT_SQLITE  ;
    TBAG_CONSTEXPR static char const * const TAG_TEMP     = Storage::LAYOUT_TEMP    ;
    TBAG_CONSTEXPR static char const * const TAG_KEYSTORE = Storage::LAYOUT_KEYSTORE;
    TBAG_CONSTEXPR static char const * const TAG_LUA      = Storage::LAYOUT_LUA     ;
    TBAG_CONSTEXPR static char const * const TAG_LUAROCKS = Storage::LAYOUT_LUAROCKS;
    TBAG_CONSTEXPR static char const * const TAG_USER     = Storage::LAYOUT_USER    ;
    // clang-format on

    TBAG_CONSTEXPR static char const * const ATT_ROOT       = "root";
    TBAG_CONSTEXPR static char const * const ATT_ABSOLUTE   = "absolute";
    TBAG_CONSTEXPR static char const * const ATT_RAW        = "raw";
    TBAG_CONSTEXPR static char const * const ATT_EXTENSION  = "ext";
    TBAG_CONSTEXPR static char const * const ATT_NAME       = "name";
    TBAG_CONSTEXPR static char const * const ATT_DEFAULT    = "default";
    TBAG_CONSTEXPR static char const * const ATT_SYSTEM     = "system";
    TBAG_CONSTEXPR static char const * const ATT_AUTO_CLEAR = "autoclear";
    TBAG_CONSTEXPR static char const * const ATT_GUI        = "gui";
    TBAG_CONSTEXPR static char const * const ATT_INIT       = "init";

    TBAG_CONSTEXPR static char const * const VAL_TRUE  = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE = "false";

    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_ENV_NAME      = "default.xml";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_CONFIG_GUI    = "gui.xml";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_MODULE_EXT    = ".so";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_TEXT_NAME     = "en.xml";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_SQLITE_NAME   = "default.sqlite";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_KEYSTORE_NAME = "default.key";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_LUA_NAME      = "config.lua";
    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_LUA_INIT      = VAL_TRUE;

public:
    struct Property
    {
        using str = std::string;

        struct env_layout
        {
            str  text;
            str  name;
            bool exists = false;
            bool def    = true;
            bool sys    = true;
            bool abs    = false;
        };

        struct def_layout
        {
            str  text;
            bool exists = false;
            bool abs    = false;
            bool raw    = false;
        };

        struct cfg_layout : public def_layout
        {
            str gui;
        };

        struct mod_layout : public def_layout
        {
            str ext;
        };

        struct txt_layout : public def_layout
        {
            str name;
        };

        struct sql_layout : public txt_layout
        {
            // EMPTY.
        };

        struct tmp_layout : public def_layout
        {
            bool auto_clear = true;
        };

        struct key_layout : public txt_layout
        {
            // EMPTY.
        };

        struct lua_layout : public txt_layout
        {
            bool init = true;
        };

        struct usr_layout : public txt_layout
        {
            // EMPTY.
        };

        env_layout env;
        cfg_layout config;
        mod_layout module;
        txt_layout text;
        sql_layout sqlite;
        tmp_layout temp;
        key_layout keystore;
        lua_layout lua;
        def_layout luarocks;

        using usr_layouts = std::vector<usr_layout>;
        usr_layouts users;
    };

private:
    char ** _envs;
    Storage _storage;

private:
    std::string _root;
    Property    _prop;

public:
    StorageNode(char ** envs = nullptr);
    StorageNode(std::string const & root, char ** envs = nullptr);
    StorageNode(StorageNode const & obj);
    StorageNode(StorageNode && obj) TBAG_NOEXCEPT;
    ~StorageNode();

public:
    StorageNode & operator =(StorageNode const & obj);
    StorageNode & operator =(StorageNode && obj) TBAG_NOEXCEPT;

public:
    void copy(StorageNode const & obj);
    void swap(StorageNode & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(StorageNode & lh, StorageNode & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline Storage       & storage()       TBAG_NOEXCEPT { return _storage; }
    inline Storage const & storage() const TBAG_NOEXCEPT { return _storage; }

    inline std::string       & root()       TBAG_NOEXCEPT { return _root; }
    inline std::string const & root() const TBAG_NOEXCEPT { return _root; }

    inline Property       & property()       TBAG_NOEXCEPT { return _prop; }
    inline Property const & property() const TBAG_NOEXCEPT { return _prop; }

public:
    inline void setEnvs(char ** envs) TBAG_NOEXCEPT { _envs = envs; }
    inline char ** getEnvs() const TBAG_NOEXCEPT { return _envs; }

public:
    void update();
    void update(std::string const & root);
    void update(Property const & prop);
    void update(std::string const & root, Property const & prop);

public:
    virtual std::string name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    static void readElement(Element const & element, std::string const & tag, Property::env_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::def_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::cfg_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::mod_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::txt_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::sql_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::tmp_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::key_layout & layout);
    static void readElement(Element const & element, std::string const & tag, Property::lua_layout & layout);
    static void readElement(Element const & element, Property::usr_layout & layout);

public:
    static void addNewElement(Element & element, std::string const & tag, Property::env_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::def_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::cfg_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::mod_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::txt_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::sql_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::tmp_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::key_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::lua_layout const & layout);
    static void addNewElement(Element & element, std::string const & tag, Property::usr_layout const & layout);

public:
    static std::string getPath(std::string const & root, std::string const & tag, std::string const & text,
                               Environments const & env, bool abs = false, bool raw = false);
    static std::string getPath(std::string const & root, std::string const & tag,
                               Property::def_layout const & layout, Environments const & env);
    static Storage loadStorage(std::string const & root, Property const & prop, char ** envs = nullptr);
};

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_NODE_STORAGENODE_HPP__

