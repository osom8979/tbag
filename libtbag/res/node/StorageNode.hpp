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
#include <libtbag/res/Storage.hpp>

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
 * @remarks
 *  XML format:
 *  @code{.xml}
 *   <!-- If 'default_root' attribute is exists, all layouts are sub-paths.       -->
 *   <!-- If 'absolute' attribute is not exists, It is set a working directory.   -->
 *   <!-- If 'archive' attribute is exists, The file system works with that type. -->
 *   <!--  'dir': Default filesystem. [DEFAULT]                                   -->
 *   <!--  'zip': ZIP archive.                                                    -->
 *   <!-- If 'absolute' attribute is 'true', Use an absolute path.                -->
 *   <!-- If 'absolute' attribute is not exists, It is set a 'false'.             -->
 *   <!-- You can set the 'absolute' attribute on any <tag>.                      -->
 *   <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *   <!-- Special exchange variable:          -->
 *   <!-- ${EXE_PATH} : EXE file path         -->
 *   <!-- ${EXE_NAME} : EXE file name         -->
 *   <!-- ${EXE_DIR}  : EXE directory         -->
 *   <!-- ${WORK_DIR} : Working directory     -->
 *   <!-- ${HOME_DIR} : User's HOME directory -->
 *   <storage default_root='directory_path' archive='dir' absolute='true'>
 *
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *     <!-- Used for special purposes.                             -->
 *     <!-- The special tag uses the tag name if there is no path. -->
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *     <!-- Set environment variables that affect the all XML text. -->
 *     <!-- If 'set' attribute is exists, Apply only those files.   -->
 *     <!-- If the 'set' attribute is '*', load all files.          -->
 *     <!-- If the 'system' attribute is 'true', load system env.   -->
 *     <env set='env1' system='true' absolute='false'>path</env>
 *
 *     <!-- Contains simple 'namespace(filename)'/'key'/'value' information for configuration. -->
 *     <config absolute='false'>path</config>
 *
 *     <!-- A set of dynamic modules. (e.g. '*.dll') -->
 *     <module absolute='false'>path</module>
 *
 *     <!-- String for localization. -->
 *     <!-- If 'default' attribute is exists, Apply only those files. (e.g. 'en' -> 'en.xml') -->
 *     <text default='en' absolute='false'>path</text>
 *
 *     <!-- PNG/JPG/BMP image storage. -->
 *     <image absolute='false'>path</image>
 *
 *     <!-- Obtain individual images referring to <image>. -->
 *     <drawable absolute='false'>path</drawable>
 *
 *     <!-- A set of animation definition files. -->
 *     <animation absolute='false'>path</animation>
 *
 *     <!-- A set of sprite definition files. -->
 *     <sprite absolute='false'>path</sprite>
 *
 *     <!-- Single LMDB storage. -->
 *     <lmdb absolute='false'>path</lmdb>
 *
 *     <!-- sqlite database storages. -->
 *     <!-- If 'set' attribute is exists, Apply only those files. -->
 *     <sqlite set='file.sqlite' absolute='false'>path</sqlite>
 *
 *     <!-- Temporary directory. -->
 *     <!-- If 'autoclear' attribute is 'true', All files are removed when the object is deleted. -->
 *     <temp autoclear='true' absolute='false'>path</sqlite>
 *
 *     <!-- The file to which the encryption applies. A password is required. -->
 *     <!-- If 'set' attribute is exists, Apply only those files.             -->
 *     <keystore set='file.key' absolute='false'>path</keystore>
 *
 *     <!-- Directory containing the lua script package. -->
 *     <!-- If 'dynasm' attribute is 'true', Install DynASM(LuaJIT) package. -->
 *     <lua dynasm='true' absolute='false'>path</lua>
 *
 *     <!-- A set of data from which raw buffers can be obtained. -->
 *     <raw absolute='false'>path</raw>
 *
 *     <!-- BagEx 'key(filename)'/'value(BagEx)' serialization. -->
 *     <bagex absolute='false'>path</bagex>
 *
 *     <!-- Executable/Libraries files. -->
 *     <exe absolute='false'>path</exe>
 *
 *     <!-- Font files. -->
 *     <font absolute='false'>path</font>
 *
 *     <!-- Music files. -->
 *     <music absolute='false'>path</music>
 *
 *     <!-- Sound (effect) files. -->
 *     <sound absolute='false'>path</sound>
 *
 *     <!-- OpenGL Shader files. -->
 *     <shader absolute='false'>path</shader>
 *
 *     <!-- Layout(Widgets) files. -->
 *     <layout absolute='false'>path</layout>
 *
 *     <!-- Style/Theme files. -->
 *     <style absolute='false'>path</style>
 *
 *     <!-- List color names and values. -->
 *     <color absolute='false'>path</color>
 *
 *     <!-- Log files. -->
 *     <!-- If 'name' attribute is exists, A rotation-logger is created. -->
 *     <rlog name='logger_name' absolute='false'>path</rlog>
 *
 *     <!-- Specify the Layout name and path to hold the data. -->
 *     <user name='name1' absolute='true'>path1</user>
 *     <user name='name2' absolute='false'>path2</user>
 *     <user name='name3'>path3</layout>
 *     <!-- ... -->
 *   </storage>
 *  @endcode
 */
class TBAG_API StorageNode : public libtbag::dom::xml::XmlModel::NodeInterface
{
public:
    using Base    = libtbag::dom::xml::XmlModel::NodeInterface;
    using Storage = libtbag::res::Storage;

public:
    TBAG_CONSTEXPR static char const * const TAG_STORAGE   = "storage";
    TBAG_CONSTEXPR static char const * const TAG_ENV       = "env";
    TBAG_CONSTEXPR static char const * const TAG_CONFIG    = "config";
    TBAG_CONSTEXPR static char const * const TAG_MODULE    = "module";
    TBAG_CONSTEXPR static char const * const TAG_TEXT      = "text";
    TBAG_CONSTEXPR static char const * const TAG_IMAGE     = "image";
    TBAG_CONSTEXPR static char const * const TAG_DRAWABLE  = "drawable";
    TBAG_CONSTEXPR static char const * const TAG_ANIMATION = "animation";
    TBAG_CONSTEXPR static char const * const TAG_SPRITE    = "sprite";
    TBAG_CONSTEXPR static char const * const TAG_LMDB      = "lmdb";
    TBAG_CONSTEXPR static char const * const TAG_SQLITE    = "sqlite";
    TBAG_CONSTEXPR static char const * const TAG_TEMP      = "temp";
    TBAG_CONSTEXPR static char const * const TAG_KEYSTORE  = "keystore";
    TBAG_CONSTEXPR static char const * const TAG_LUA       = "lua";
    TBAG_CONSTEXPR static char const * const TAG_RAW       = "raw";
    TBAG_CONSTEXPR static char const * const TAG_BAGEX     = "bagex";
    TBAG_CONSTEXPR static char const * const TAG_EXE       = "exe";
    TBAG_CONSTEXPR static char const * const TAG_FONT      = "font";
    TBAG_CONSTEXPR static char const * const TAG_MUSIC     = "music";
    TBAG_CONSTEXPR static char const * const TAG_SOUND     = "sound";
    TBAG_CONSTEXPR static char const * const TAG_SHADER    = "shader";
    TBAG_CONSTEXPR static char const * const TAG_LAYOUT    = "layout";
    TBAG_CONSTEXPR static char const * const TAG_STYLE     = "style";
    TBAG_CONSTEXPR static char const * const TAG_COLOR     = "color";
    TBAG_CONSTEXPR static char const * const TAG_RLOG      = "rlog";
    TBAG_CONSTEXPR static char const * const TAG_USER      = "user";

    TBAG_CONSTEXPR static char const * const ATT_DEFAULT_ROOT = "default_root";
    TBAG_CONSTEXPR static char const * const ATT_ARCHIVE      = "archive";
    TBAG_CONSTEXPR static char const * const ATT_ABSOLUTE     = "absolute";
    TBAG_CONSTEXPR static char const * const ATT_SET          = "set";
    TBAG_CONSTEXPR static char const * const ATT_SYSTEM       = "system";
    TBAG_CONSTEXPR static char const * const ATT_DEFAULT      = "default";
    TBAG_CONSTEXPR static char const * const ATT_AUTOCLEAR    = "autoclear";
    TBAG_CONSTEXPR static char const * const ATT_DYNASM       = "dynasm";
    TBAG_CONSTEXPR static char const * const ATT_NAME         = "name";

    TBAG_CONSTEXPR static char const * const VAL_DIR   = "dir";
    TBAG_CONSTEXPR static char const * const VAL_ZIP   = "zip";
    TBAG_CONSTEXPR static char const * const VAL_TRUE  = "true";
    TBAG_CONSTEXPR static char const * const VAL_FALSE = "false";
    TBAG_CONSTEXPR static char const * const VAL_ALL   = "*";

public:
    StorageNode();
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

