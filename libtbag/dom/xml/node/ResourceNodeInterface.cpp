/**
 * @file   ResourceNodeInterface.cpp
 * @brief  ResourceNodeInterface class implementation.
 * @author zer0
 * @date   2017-11-18
 */

#include <libtbag/dom/xml/node/ResourceNodeInterface.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace xml  {
namespace node {

ResourceNodeInterface::ResourceNodeInterface()
        : root_directory(), resource_name(DEFAULT_RESOURCE_NAME), layouts(libtbag::res::getDefaultLayout())
{
    // EMPTY.
}

ResourceNodeInterface::~ResourceNodeInterface()
{
    // EMPTY.
}

ResourceNodeInterface::DynamicAsset ResourceNodeInterface::getDynamicAsset() const
{
    return getDynamicAsset(root_directory, layouts);
}

ResourceNodeInterface::Path ResourceNodeInterface::getResourceDirectory() const
{
    return Path(root_directory) / resource_name;
}

ResourceNodeInterface::Resource ResourceNodeInterface::readResource(std::string const & tag) const
{
    return readResource(getResourceDirectory(), tag, RESOURCE_TAG_NAME);
}

// ---------------
// Static methods.
// ---------------

ResourceNodeInterface::DynamicAsset ResourceNodeInterface::getDynamicAsset(
        std::string const & root_dir, std::vector<std::string> const & layouts)
{
    DynamicAsset asset;
    for (auto & layout : layouts) {
        asset.add(layout, (libtbag::filesystem::Path(root_dir) / layout));
    }
    return asset;
}

ResourceNodeInterface::Resource ResourceNodeInterface::readResource(
        std::string const & directory, std::string const & filename, std::string const & tag)
{
    return readResource(libtbag::filesystem::Path(directory) / filename, tag);
}

ResourceNodeInterface::Resource ResourceNodeInterface::readResource(
        std::string const & resource_path, std::string const & tag)
{
    using namespace libtbag::filesystem;
    Path path(resource_path);
    Path dir = path.getParent();

    if (dir.exists() == false) {
        dir.createDir();
    }

    Resource res;
    if (path.isRegularFile()) {
        res.readFile(path, tag);
    } else {
        res.setTag(tag);
        res.save(path);
    }
    return res;
}

} // namespace node
} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

