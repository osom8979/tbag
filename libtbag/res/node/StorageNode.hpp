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
 *     <config>dir</config>
 *
 *     <!-- A set of dynamic modules. (e.g. '*.dll') -->
 *     <module>dir</module>
 *
 *     <!-- String for localization. -->
 *     <!-- If 'name' attribute is exists, Apply only those files. -->
 *     <text name='en.xml'>dir</text>
 *
 *     <!-- PNG/JPG/BMP image storage. -->
 *     <image>dir</image>
 *
 *     <!-- Obtain individual images referring to <image>. -->
 *     <drawable>dir</drawable>
 *
 *     <!-- A set of animation definition files. -->
 *     <animation>dir</animation>
 *
 *     <!-- A set of sprite definition files. -->
 *     <sprite>dir</sprite>
 *
 *     <!-- Single LMDB storage. -->
 *     <lmdb>dir</lmdb>
 *
 *     <!-- sqlite database storages. -->
 *     <!-- If 'name' attribute is exists, Apply only those files. -->
 *     <sqlite name='file.sqlite'>dir</sqlite>
 *
 *     <!-- Temporary directory. -->
 *     <!-- If 'autoclear' attribute is 'true', All files are removed when the object is deleted. -->
 *     <temp autoclear='true'>dir</sqlite>
 *
 *     <!-- The file to which the encryption applies. A password is required. -->
 *     <!-- If 'name' attribute is exists, Apply only those files.            -->
 *     <keystore name='file.key'>dir</keystore>
 *
 *     <!-- Directory containing the lua script package. -->
 *     <!-- If 'dynasm' attribute is 'true', Install DynASM(LuaJIT) package. -->
 *     <lua dynasm='true'>dir</lua>
 *
 *     <!-- A set of data from which raw buffers can be obtained. -->
 *     <raw>dir</raw>
 *
 *     <!-- BagEx 'key(filename)'/'value(BagEx)' serialization. -->
 *     <bagex>dir</bagex>
 *
 *     <!-- Executable/Libraries files. -->
 *     <exe>dir</exe>
 *
 *     <!-- Font files. -->
 *     <font>dir</font>
 *
 *     <!-- Music files. -->
 *     <music>dir</music>
 *
 *     <!-- Sound (effect) files. -->
 *     <sound>dir</sound>
 *
 *     <!-- OpenGL Shader files. -->
 *     <shader>dir</shader>
 *
 *     <!-- Layout(Widgets) files. -->
 *     <layout>dir</layout>
 *
 *     <!-- Style/Theme files. -->
 *     <style>dir</style>
 *
 *     <!-- List color names and values. -->
 *     <color>dir</color>
 *
 *     <!-- Specify the Layout name and path to hold the data. -->
 *     <user name='name1' absolute='true' raw='true'>dir1</user>
 *     <user name='name2' absolute='false'>dir2</user>
 *     <user name='name3'>dir3</layout>
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
    TBAG_CONSTEXPR static char const * const TAG_STORAGE   = "storage";

public:
    // @formatter:off
    TBAG_CONSTEXPR static char const * const TAG_ENV       = Storage::LAYOUT_ENV      ;
    TBAG_CONSTEXPR static char const * const TAG_CONFIG    = Storage::LAYOUT_CONFIG   ;
    TBAG_CONSTEXPR static char const * const TAG_MODULE    = Storage::LAYOUT_MODULE   ;
    TBAG_CONSTEXPR static char const * const TAG_TEXT      = Storage::LAYOUT_TEXT     ;
    TBAG_CONSTEXPR static char const * const TAG_IMAGE     = Storage::LAYOUT_IMAGE    ;
    TBAG_CONSTEXPR static char const * const TAG_DRAWABLE  = Storage::LAYOUT_DRAWABLE ;
    TBAG_CONSTEXPR static char const * const TAG_ANIMATION = Storage::LAYOUT_ANIMATION;
    TBAG_CONSTEXPR static char const * const TAG_SPRITE    = Storage::LAYOUT_SPRITE   ;
    TBAG_CONSTEXPR static char const * const TAG_LMDB      = Storage::LAYOUT_LMDB     ;
    TBAG_CONSTEXPR static char const * const TAG_SQLITE    = Storage::LAYOUT_SQLITE   ;
    TBAG_CONSTEXPR static char const * const TAG_TEMP      = Storage::LAYOUT_TEMP     ;
    TBAG_CONSTEXPR static char const * const TAG_KEYSTORE  = Storage::LAYOUT_KEYSTORE ;
    TBAG_CONSTEXPR static char const * const TAG_LUA       = Storage::LAYOUT_LUA      ;
    TBAG_CONSTEXPR static char const * const TAG_RAW       = Storage::LAYOUT_RAW      ;
    TBAG_CONSTEXPR static char const * const TAG_BAGEX     = Storage::LAYOUT_BAGEX    ;
    TBAG_CONSTEXPR static char const * const TAG_EXE       = Storage::LAYOUT_EXE      ;
    TBAG_CONSTEXPR static char const * const TAG_FONT      = Storage::LAYOUT_FONT     ;
    TBAG_CONSTEXPR static char const * const TAG_MUSIC     = Storage::LAYOUT_MUSIC    ;
    TBAG_CONSTEXPR static char const * const TAG_SOUND     = Storage::LAYOUT_SOUND    ;
    TBAG_CONSTEXPR static char const * const TAG_SHADER    = Storage::LAYOUT_SHADER   ;
    TBAG_CONSTEXPR static char const * const TAG_LAYOUT    = Storage::LAYOUT_LAYOUT   ;
    TBAG_CONSTEXPR static char const * const TAG_STYLE     = Storage::LAYOUT_STYLE    ;
    TBAG_CONSTEXPR static char const * const TAG_COLOR     = Storage::LAYOUT_COLOR    ;
    TBAG_CONSTEXPR static char const * const TAG_RLOG      = Storage::LAYOUT_RLOG     ;
    TBAG_CONSTEXPR static char const * const TAG_USER      = Storage::LAYOUT_USER     ;
    // @formatter:on

    TBAG_CONSTEXPR static char const * const ATT_ROOT      = "root";
    TBAG_CONSTEXPR static char const * const ATT_ABSOLUTE  = "absolute";
    TBAG_CONSTEXPR static char const * const ATT_RAW       = "raw";
    TBAG_CONSTEXPR static char const * const ATT_NAME      = "name";
    TBAG_CONSTEXPR static char const * const ATT_DEFAULT   = "default";
    TBAG_CONSTEXPR static char const * const ATT_SYSTEM    = "system";
    TBAG_CONSTEXPR static char const * const ATT_AUTOCLEAR = "autoclear";
    TBAG_CONSTEXPR static char const * const ATT_DYNASM    = "dynasm";

    TBAG_CONSTEXPR static char const * const VAL_TRUE  = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE = "false";

public:
    struct Property
    {
        std::string root;

        std::string env_text;
        std::string env_name;
        bool        env_default;
        bool        env_system;
        bool        env_absolute;

        std::string config_text;
        bool        config_absolute;
        bool        config_raw;

        std::string module_text;
        bool        module_absolute;
        bool        module_raw;

        std::string text_text;
        std::string text_name;
        bool        text_absolute;
        bool        text_raw;

        std::string image_text;
        bool        image_absolute;
        bool        image_raw;

        std::string drawable_text;
        bool        drawable_absolute;
        bool        drawable_raw;

        std::string animation_text;
        bool        animation_absolute;
        bool        animation_raw;

        std::string sprite_text;
        bool        sprite_absolute;
        bool        sprite_raw;

        std::string lmdb_text;
        bool        lmdb_absolute;
        bool        lmdb_raw;

        std::string sqlite_text;
        std::string sqlite_name;
        bool        sqlite_absolute;
        bool        sqlite_raw;

        std::string temp_text;
        bool        temp_autoclear;
        bool        temp_absolute;
        bool        temp_raw;

        std::string keystore_text;
        std::string keystore_name;
        bool        keystore_absolute;
        bool        keystore_raw;

        std::string lua_text;
        bool        lua_dynasm;
        bool        lua_absolute;
        bool        lua_raw;

        std::string raw_text;
        bool        raw_absolute;
        bool        raw_raw;

        std::string bagex_text;
        bool        bagex_absolute;
        bool        bagex_raw;

        std::string exe_text;
        bool        exe_absolute;
        bool        exe_raw;

        std::string font_text;
        bool        font_absolute;
        bool        font_raw;

        std::string music_text;
        bool        music_absolute;
        bool        music_raw;

        std::string sound_text;
        bool        sound_absolute;
        bool        sound_raw;

        std::string shader_text;
        bool        shader_absolute;
        bool        shader_raw;

        std::string layout_text;
        bool        layout_absolute;
        bool        layout_raw;

        std::string style_text;
        bool        style_absolute;
        bool        style_raw;

        std::string color_text;
        bool        color_absolute;
        bool        color_raw;
    };

private:
    char ** _envs;
    Storage _storage;

private:
    Property _property;

public:
    StorageNode(char ** envs = nullptr);
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
    inline Property       & property()       TBAG_NOEXCEPT { return _property; }
    inline Property const & property() const TBAG_NOEXCEPT { return _property; }

    inline Storage       & storage()       TBAG_NOEXCEPT { return _storage; }
    inline Storage const & storage() const TBAG_NOEXCEPT { return _storage; }

public:
    std::string getPath(Element const & element, std::string const & tag);
    std::string getPath(Element const & element, std::string const & tag, Environments const & env);

public:
    static std::string getPath(Element const & element, std::string const & root, std::string const & tag);
    static std::string getPath(Element const & element, std::string const & root, std::string const & tag, Environments const & env);

public:
    virtual std::string name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;
};

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_NODE_STORAGENODE_HPP__

