/**
 * @file   ResourceNodeInterface.hpp
 * @brief  ResourceNodeInterface class prototype.
 * @author zer0
 * @date   2017-11-18
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_NODE_RESOURCENODEINTERFACE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_NODE_RESOURCENODEINTERFACE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/XmlModel.hpp>
#include <libtbag/dom/xml/Resource.hpp>
#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace xml  {
namespace node {

/**
 * ResourceNodeInterface class prototype.
 *
 * @author zer0
 * @date   2017-11-18
 */
struct TBAG_API ResourceNodeInterface : public XmlModel::NodeInterface
{
    TBAG_CONSTEXPR static char const * const DEFAULT_RESOURCE_NAME = TBAG_DYNAMIC_ASSERT_LAYOUT_VALUE;
    TBAG_CONSTEXPR static char const * const RESOURCE_TAG_NAME = "value";

    using Path = filesystem::Path;
    using Resource = xml::Resource;
    using DynamicAsset = res::DynamicAsset;

    std::string root_directory;
    std::string resource_name;
    std::vector<std::string> layouts;

    ResourceNodeInterface();
    virtual ~ResourceNodeInterface();

    inline void setRootDirectory(std::string const & dir) { root_directory = dir; }
    inline void setResourceName(std::string const & name) { resource_name = name; }

    DynamicAsset getDynamicAsset() const;
    Path getResourceDirectory() const;
    Resource readResource(std::string const & tag) const;

    template <typename T>
    T readValue(std::string const & tag, std::string const & key, T const & default_value = T()) const
    {
        return readResource(tag).get(key, default_value);
    }

    template <typename T>
    bool saveValue(std::string const & tag, std::string const & key, T const & value) const
    {
        auto res = readResource(tag);
        res.set(key, value);
        return res.save((getResourceDirectory() / tag).toString());
    }

    // ---------------
    // Static methods.
    // ---------------

    static res::DynamicAsset getDynamicAsset(std::string const & root_dir, std::vector<std::string> const & layouts);

    static Resource readResource(std::string const & directory, std::string const & filename, std::string const & tag);
    static Resource readResource(std::string const & resource_path, std::string const & tag);
};

} // namespace node
} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_NODE_RESOURCENODEINTERFACE_HPP__

