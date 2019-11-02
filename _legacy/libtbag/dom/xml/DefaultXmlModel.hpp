/**
 * @file   DefaultXmlModel.hpp
 * @brief  DefaultXmlModel class prototype.
 * @author zer0
 * @date   2017-06-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_DEFAULTXMLMODEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_DEFAULTXMLMODEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/XmlModel.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/**
 * DefaultXmlModel class prototype.
 *
 * @author zer0
 * @date   2017-06-02
 */
class TBAG_API DefaultXmlModel : public XmlModel
{
public:
    using NodeInterface = XmlModel::NodeInterface;
    using SharedNode    = XmlModel::SharedNode;

    using Path  = XmlModel::Path;
    using Scope = XmlModel::Scope;

public:
    struct InitParams
    {
        std::string root_name;
        std::string file_name;
        std::string work_dir;
        std::string exe_dir;
        std::string home_dir;
        std::string global_dir;
    };

private:
    InitParams _params;

public:
    DefaultXmlModel();
    DefaultXmlModel(std::string const & root_name);
    DefaultXmlModel(std::string const & root_name, std::string const & file_name);
    DefaultXmlModel(InitParams const & params);
    virtual ~DefaultXmlModel();

public:
    inline InitParams getParams() const { return _params; }
    inline void setParams(InitParams const params) { _params = params; }

public:
    virtual std::string getRootName() const override;
    virtual std::string getFileName() const override;

public:
    virtual Path getWorkDir  () const override;
    virtual Path getExeDir   () const override;
    virtual Path getHomeDir  () const override;
    virtual Path getGlobalDir() const override;

public:
    static InitParams getDefaultParams();
    static InitParams getDefaultParams(std::string const & root_name);
    static InitParams getDefaultParams(std::string const & root_name, std::string const & file_name);
    static InitParams getDefaultParams(std::string const & root_name, std::string const & app_dir_name, std::string const & file_name);
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_DEFAULTXMLMODEL_HPP__

