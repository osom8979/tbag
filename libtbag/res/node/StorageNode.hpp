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
 *   <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *   <!-- Special exchange variables:         -->
 *   <!-- ${EXE_PATH} : EXE file path         -->
 *   <!-- ${EXE_NAME} : EXE file name         -->
 *   <!-- ${EXE_DIR}  : EXE directory         -->
 *   <!-- ${WORK_DIR} : Working directory     -->
 *   <!-- ${HOME_DIR} : User's HOME directory -->
 *   <storage default_root='dir' archive='dir'>
 *
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *     <!-- Used for special purposes.                                  -->
 *     <!-- The special tag uses the tag name if there is no path.      -->
 *     <!-- If 'absolute' attribute is 'true', Use an absolute path.    -->
 *     <!-- If 'absolute' attribute is not exists, It is set a 'false'. -->
 *     <!-- You can set the 'absolute' attribute on any <tag>.          -->
 *     <!-- ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ -->
 *
 *     <!-- Set environment variables that affect the all XML text.                        -->
 *     <!-- If 'set' attribute is exists, Apply only those files.                          -->
 *     <!-- If the 'set' attribute is '*', load all files.                                 -->
 *     <!-- If the 'default_set' attribute is 'true', Update 'Special exchange variables'. -->
 *     <!-- If 'default_set' attribute is not exists, It is set a 'true'.                  -->
 *     <env set='env1.xml' default_set='true' absolute='false'>dir</env>
 *
 *     <!-- If 'raw' attribute is 'true', Do not apply environment variables. -->
 *     <!-- If 'raw' attribute is not exists, It is set a 'false'.            -->
 *     <!-- You can set the 'env' attribute on any <tag>.                     -->
 *
 *     <!-- Contains simple 'group(filename)'/'key'/'value' information for configuration. -->
 *     <config absolute='false' raw='true'>dir</config>
 *
 *     <!-- A set of dynamic modules. (e.g. '*.dll') -->
 *     <module absolute='false'>dir</module>
 *
 *     <!-- String for localization. -->
 *     <!-- If 'default' attribute is exists, Apply only those files. (e.g. 'en' -> 'en.xml') -->
 *     <text default='en' absolute='false'>dir</text>
 *
 *     <!-- PNG/JPG/BMP image storage. -->
 *     <image absolute='false'>dir</image>
 *
 *     <!-- Obtain individual images referring to <image>. -->
 *     <drawable absolute='false'>dir</drawable>
 *
 *     <!-- A set of animation definition files. -->
 *     <animation absolute='false'>dir</animation>
 *
 *     <!-- A set of sprite definition files. -->
 *     <sprite absolute='false'>dir</sprite>
 *
 *     <!-- Single LMDB storage. -->
 *     <lmdb absolute='false'>dir</lmdb>
 *
 *     <!-- sqlite database storages. -->
 *     <!-- If 'set' attribute is exists, Apply only those files. -->
 *     <sqlite set='file.sqlite' absolute='false'>dir</sqlite>
 *
 *     <!-- Temporary directory. -->
 *     <!-- If 'autoclear' attribute is 'true', All files are removed when the object is deleted. -->
 *     <temp autoclear='true' absolute='false'>dir</sqlite>
 *
 *     <!-- The file to which the encryption applies. A password is required. -->
 *     <!-- If 'set' attribute is exists, Apply only those files.             -->
 *     <keystore set='file.key' absolute='false'>dir</keystore>
 *
 *     <!-- Directory containing the lua script package. -->
 *     <!-- If 'dynasm' attribute is 'true', Install DynASM(LuaJIT) package. -->
 *     <lua dynasm='true' absolute='false'>dir</lua>
 *
 *     <!-- A set of data from which raw buffers can be obtained. -->
 *     <raw absolute='false'>dir</raw>
 *
 *     <!-- BagEx 'key(filename)'/'value(BagEx)' serialization. -->
 *     <bagex absolute='false'>dir</bagex>
 *
 *     <!-- Executable/Libraries files. -->
 *     <exe absolute='false'>dir</exe>
 *
 *     <!-- Font files. -->
 *     <font absolute='false'>dir</font>
 *
 *     <!-- Music files. -->
 *     <music absolute='false'>dir</music>
 *
 *     <!-- Sound (effect) files. -->
 *     <sound absolute='false'>dir</sound>
 *
 *     <!-- OpenGL Shader files. -->
 *     <shader absolute='false'>dir</shader>
 *
 *     <!-- Layout(Widgets) files. -->
 *     <layout absolute='false'>dir</layout>
 *
 *     <!-- Style/Theme files. -->
 *     <style absolute='false'>dir</style>
 *
 *     <!-- List color names and values. -->
 *     <color absolute='false'>dir</color>
 *
 *     <!-- Log files. -->
 *     <!-- If 'name' attribute is exists, A rotation-logger is created. -->
 *     <rlog name='logger_name' absolute='false'>dir</rlog>
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

    TBAG_CONSTEXPR static char const * const ATT_DEFAULT_ROOT = "default_root";
    TBAG_CONSTEXPR static char const * const ATT_ARCHIVE      = "archive";
    TBAG_CONSTEXPR static char const * const ATT_ABSOLUTE     = "absolute";
    TBAG_CONSTEXPR static char const * const ATT_RAW          = "raw";
    TBAG_CONSTEXPR static char const * const ATT_SET          = "set";
    TBAG_CONSTEXPR static char const * const ATT_DEFAULT_SET  = "default_set";
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
    static std::string getPath(Element const & element,
                               std::string const & default_root);
    static std::string getPath(Element const & element,
                               std::string const & default_root,
                               Environments const & env);

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

