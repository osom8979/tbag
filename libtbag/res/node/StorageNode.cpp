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
        _envs = obj._envs;
    }
}

void StorageNode::swap(StorageNode & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_envs, obj._envs);
    }
}

std::string StorageNode::name() const
{
    return std::string(TAG_STORAGE);
}

void StorageNode::setup()
{
}

void StorageNode::teardown()
{
}

std::string StorageNode::getPath(Element const & element, std::string const & default_root)
{
    return getPath(element, default_root, Environments());
}

std::string StorageNode::getPath(Element const & element, std::string const & default_root, Environments const & env)
{
    bool absolute = false;
    optAttr(element, ATT_ABSOLUTE, absolute, false);

    bool raw = false;
    optAttr(element, ATT_RAW, raw, false);

    std::string path;
    if (raw) {
        path = text(element);
    } else {
        path = env.convert(text(element));
    }

    using namespace libtbag::filesystem;
    if (path.empty()) {
        path = Path(default_root) / std::string(TAG_ENV);
    } else if (!absolute) {
        path = Path(default_root) / path;
    }
    return path;
}

void StorageNode::load(Element const & element)
{
    using namespace libtbag::filesystem;
    using namespace libtbag::string;

    Storage result;

    std::string default_root;
    if (isSuccess(optAttr(element, ATT_DEFAULT_ROOT, default_root, Path::getWorkDir().toString()))) {
        default_root = Environments::createDefaultEnvironments().convert(default_root);
    }

    std::string archive;
    if (isSuccess(optAttr(element, ATT_ARCHIVE, archive, VAL_DIR))) {
        archive = lower(archive);
        if (archive != VAL_DIR && archive != VAL_ZIP) {
            tDLogW("StorageNode::load() {} attribute error: {}", std::string(ATT_ARCHIVE), archive);
            archive = VAL_DIR;
        }
    }

    Environments envs;
    if (auto * env = element.FirstChildElement(TAG_ENV)) {
        result.setLayoutEnv(getPath(*env, default_root));

        std::string name;
        optAttr(*env, ATT_NAME, name);
        if (!name.empty()) {
            result.setEnvFilename(name);
            result.readEnv();
        }

        bool default_flag;
        optAttr(*env, ATT_DEFAULT, default_flag, false);
        if (default_flag) {
            result.readEnvDefault();
        }

        bool system_flag;
        optAttr(*env, ATT_SYSTEM, system_flag, false);
        if (system_flag && _envs != nullptr) {
            result.readEnvParams(_envs);
        }

        envs = result.envs();
    } else {
        envs = Environments::createDefaultEnvironments();
    }

    if (auto * config = element.FirstChildElement(TAG_CONFIG)) {
        result.setLayoutConfig(getPath(*config, default_root, envs));
    }

    /*
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
     */
}

void StorageNode::save(Element & element) const
{
}

} // namespace node
} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------
