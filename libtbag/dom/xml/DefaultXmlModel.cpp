/**
 * @file   DefaultXmlModel.cpp
 * @brief  DefaultXmlModel class implementation.
 * @author zer0
 * @date   2017-06-02
 */

#include <libtbag/dom/xml/DefaultXmlModel.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

TBAG_CONSTEXPR static char const * const DEFAULT_ROOT_XML_NODE_NAME = "config";
TBAG_CONSTEXPR static char const * const DEFAULT_XML_FILE_EXTENSION = ".xml";

DefaultXmlModel::DefaultXmlModel()
{
    _params = getDefaultParams();
}

DefaultXmlModel::DefaultXmlModel(std::string const & root_name)
{
    _params = getDefaultParams(root_name);
}

DefaultXmlModel::DefaultXmlModel(std::string const & root_name, std::string const & file_name)
{
    _params = getDefaultParams(root_name, file_name);
}

DefaultXmlModel::DefaultXmlModel(InitParams const & params) : _params(params)
{
    // EMPTY.
}

DefaultXmlModel::~DefaultXmlModel()
{
    // EMPTY.
}

std::string DefaultXmlModel::getRootName() const
{
    return _params.root_name;
}

std::string DefaultXmlModel::getFileName() const
{
    return _params.file_name;
}

DefaultXmlModel::Path DefaultXmlModel::getWorkDir() const
{
    return Path(_params.work_dir);
}

DefaultXmlModel::Path DefaultXmlModel::getExeDir() const
{
    return Path(_params.exe_dir);
}

DefaultXmlModel::Path DefaultXmlModel::getHomeDir() const
{
    return Path(_params.home_dir);
}

DefaultXmlModel::Path DefaultXmlModel::getGlobalDir() const
{
    return Path(_params.global_dir);
}

DefaultXmlModel::InitParams DefaultXmlModel::getDefaultParams()
{
    return getDefaultParams(DEFAULT_ROOT_XML_NODE_NAME);
}

DefaultXmlModel::InitParams DefaultXmlModel::getDefaultParams(std::string const & root_name)
{
    return getDefaultParams(root_name, root_name + std::string(DEFAULT_XML_FILE_EXTENSION));
}

DefaultXmlModel::InitParams DefaultXmlModel::getDefaultParams(std::string const & root_name, std::string const & file_name)
{
    XmlModel model;
    InitParams params;
    params.root_name  = root_name;
    params.file_name  = file_name;
    // @formatter:off
    params.work_dir   = (model.getWorkDir  ()   / root_name  ).getString();
    params.exe_dir    = (model.getExeDir   () /*/ root_name*/).getString();
    params.home_dir   = (model.getHomeDir  ()   / root_name  ).getString();
    params.global_dir = (model.getGlobalDir()   / root_name  ).getString();
    // @formatter:on
    return params;
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

