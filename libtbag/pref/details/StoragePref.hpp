/**
 * @file   StoragePref.hpp
 * @brief  StoragePref class prototype.
 * @author zer0
 * @date   2018-11-03
 * @date   2019-11-03 (Rename: libtbag/res/node/StorageNode -> libtbag/pref/details/StoragePref)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/pref/Preferences.hpp>
#include <libtbag/res/Storage.hpp>
#include <libtbag/string/Environments.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref    {
namespace details {

/**
 * StoragePref class prototype.
 *
 * XML format:
 * @code{.xml}
 *  <!-- If 'root' attribute is exists, all layouts are sub-paths.         -->
 *  <!-- If 'root' attribute is not exists, It is set a working directory. -->
 *  <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *  <!-- Special exchange variables:         -->
 *  <!-- ${EXE_PATH} : EXE file path         -->
 *  <!-- ${EXE_NAME} : EXE file name         -->
 *  <!-- ${EXE_DIR}  : EXE directory         -->
 *  <!-- ${WORK_DIR} : Working directory     -->
 *  <!-- ${HOME_DIR} : User's HOME directory -->
 *  <storage root='dir' mkdir='true'>
 *
 *    <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *    <!-- Used for special purposes.                                  -->
 *    <!-- The special tag uses the tag name if there is no path.      -->
 *    <!-- If 'absolute' attribute is 'true', Use an absolute path.    -->
 *    <!-- If 'absolute' attribute is not exists, It is set a 'false'. -->
 *    <!-- You can set the 'absolute' attribute on any <tag>.          -->
 *    <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *    <!-- Set environment variables that affect the all XML text.                    -->
 *    <!-- If 'name' attribute is exists, apply only those files.                     -->
 *    <!-- If the 'default' attribute is 'true', update 'special exchange variables'. -->
 *    <!-- If 'default' attribute is not exists, it is set a 'true'.                  -->
 *    <!-- If the 'system' attribute is 'true', use the '_envs' member.               -->
 *    <!-- If 'system' attribute is not exists, it is set a 'true'.                   -->
 *    <!-- [IMPORTANT] The 'env' tag does not have a 'raw' attribute.                 -->
 *    <env name='env1.xml' default='true' system='true' absolute='false'>dir</env>
 *
 *    <!-- If 'raw' attribute is 'true', Do not apply environment variables. -->
 *    <!-- If 'raw' attribute is not exists, It is set a 'false'.            -->
 *    <!-- You can set the 'env' attribute on any <tag>.                     -->
 *
 *    <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *    <!-- Contains simple 'group(filename)'/'key'/'value' information for configuration. -->
 *    <config>dir</config>
 *
 *    <!-- A set of dynamic modules. (e.g. '*.dll' files)          -->
 *    <!-- If 'ext' attribute is exists, File extension filtering. -->
 *    <!-- If 'ext' attribute is not exists, No filtering.         -->
 *    <module ext='.so'>dir</module>
 *
 *    <!-- String for localization. -->
 *    <!-- If 'name' attribute is exists, Apply only those files. -->
 *    <text name='en.xml'>dir</text>
 *
 *    <!-- sqlite database storages. -->
 *    <!-- If 'name' attribute is exists, Apply only those files. -->
 *    <sqlite name='file.sqlite'>dir</sqlite>
 *
 *    <!-- Temporary directory. -->
 *    <!-- If 'autoclear' attribute is 'true', All files are removed when the object is deleted. -->
 *    <temp autoclear='true'>dir</temp>
 *
 *    <!-- The file to which the encryption applies. A password is required. -->
 *    <!-- If 'name' attribute is exists, Apply only those files.            -->
 *    <keystore name='file.key'>dir</keystore>
 *
 *    <!-- Directory containing the LuaRosks root directory. -->
 *    <luarocks>dir</luarocks>
 *
 *    <!-- Directory containing the lua script package.                    -->
 *    <!-- If the 'init' attribute is 'true', update 'default initialize'. -->
 *    <!-- If 'init' attribute is not exists, it is set a 'true'.          -->
 *    <!-- If 'name' attribute is exists, Apply only those files.          -->
 *    <lua init='true' name='file.lua'>dir</lua>
 *
 *    <!-- Specify the Layout name and path to hold the data. -->
 *    <user name='name1' absolute='true' raw='true'>dir1</user>
 *    <user name='name2' absolute='false'>dir2</user>
 *    <user name='name3'>dir3</user>
 *    <user>dir4</user> <!-- name='dir4' -->
 *
 *    <!-- ... -->
 *  </storage>
 * @endcode
 *
 * @author zer0
 * @date   2018-11-03
 * @date   2019-11-03 (Rename: libtbag/res/node/StorageNode -> libtbag/pref/details/StoragePref)
 */
class TBAG_API StoragePref : public libtbag::pref::Preferences::NodeInterface
{
public:
    using Storage = libtbag::res::Storage;
    using Environments = libtbag::string::Environments;

public:
    TBAG_CONSTEXPR static char const * const DEFAULT_STORAGE_ROOT = "${WORK_DIR}/storage";
    TBAG_CONSTEXPR static char const * const ENV_KEY_STORAGE_ROOT = "TBAG_STORAGE_ROOT";

public:
    TBAG_CONSTEXPR static char const * const TAG_STORAGE = "storage";

public:
    // clang-format off
    TBAG_CONSTEXPR static char const * const TAG_ENV      = Storage::LAYOUT_ENV;
    TBAG_CONSTEXPR static char const * const TAG_CONFIG   = Storage::LAYOUT_CONFIG;
    TBAG_CONSTEXPR static char const * const TAG_MODULE   = Storage::LAYOUT_MODULE;
    TBAG_CONSTEXPR static char const * const TAG_TEXT     = Storage::LAYOUT_TEXT;
    TBAG_CONSTEXPR static char const * const TAG_SQLITE   = Storage::LAYOUT_SQLITE;
    TBAG_CONSTEXPR static char const * const TAG_TEMP     = Storage::LAYOUT_TEMP;
    TBAG_CONSTEXPR static char const * const TAG_KEYSTORE = Storage::LAYOUT_KEYSTORE;
    TBAG_CONSTEXPR static char const * const TAG_LUA      = Storage::LAYOUT_LUA;
    TBAG_CONSTEXPR static char const * const TAG_LUAROCKS = Storage::LAYOUT_LUAROCKS;
    TBAG_CONSTEXPR static char const * const TAG_USER     = Storage::LAYOUT_USER;
    // clang-format on

    TBAG_CONSTEXPR static char const * const ATT_ROOT       = "root";
    TBAG_CONSTEXPR static char const * const ATT_MKDIR      = "mkdir";
    TBAG_CONSTEXPR static char const * const ATT_ABSOLUTE   = "absolute";
    TBAG_CONSTEXPR static char const * const ATT_RAW        = "raw";
    TBAG_CONSTEXPR static char const * const ATT_EXTENSION  = "ext";
    TBAG_CONSTEXPR static char const * const ATT_NAME       = "name";
    TBAG_CONSTEXPR static char const * const ATT_DEFAULT    = "default";
    TBAG_CONSTEXPR static char const * const ATT_SYSTEM     = "system";
    TBAG_CONSTEXPR static char const * const ATT_AUTO_CLEAR = "autoclear";
    TBAG_CONSTEXPR static char const * const ATT_INIT       = "init";

    TBAG_CONSTEXPR static char const * const VAL_TRUE  = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE = "false";

    TBAG_CONSTEXPR static char const * const VAL_DEFAULT_ENV_NAME      = "default.xml";
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
            // EMPTY.
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

        str root;
        bool mkdir = false;

        env_layout env;
        cfg_layout config;
        mod_layout module;
        txt_layout text;
        sql_layout sqlite;
        tmp_layout temp;
        key_layout keystore;
        lua_layout lua;
        def_layout lua_rocks;

        using usr_layouts = std::vector<usr_layout>;
        usr_layouts users;
    };

private:
    Property _prop;

public:
    StoragePref();
    virtual ~StoragePref();

public:
    inline Property       & property()       TBAG_NOEXCEPT { return _prop; }
    inline Property const & property() const TBAG_NOEXCEPT { return _prop; }

public:
    std::string name() const override;
    bool init() override;
    void clear() override;
    void load(Element const & element) override;
    void save(Element & element) const override;

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

public:
    Storage loadStorage(Environments const & default_envs) const;
    Storage loadStorage() const;
};

} // namespace details
} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PREF_DETAILS_STORAGEPREF_HPP__

