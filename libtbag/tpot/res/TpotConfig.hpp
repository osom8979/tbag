/**
 * @file   TpotConfig.hpp
 * @brief  TpotConfig class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTCONFIG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTCONFIG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/XmlModel.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

TBAG_CONSTEXPR char const * const TPOT_CONFIG_XML_FILE_NAME = "tpot.xml";
TBAG_CONSTEXPR char const * const TPOT_CONFIG_XML_ROOT_NAME = "tpot";
TBAG_CONSTEXPR char const * const TPOT_CONFIG_GLOBAL_NAME   = "tpot.config";

/**
 * TpotConfig class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API TpotConfig : public libtbag::dom::XmlModel
{
public:
    using Parent = libtbag::dom::XmlModel;

    using NodeInterface = Parent::NodeInterface;
    using SharedNode    = Parent::SharedNode;

    using String = Parent::String;
    using Path   = Parent::Path;
    using Scope  = Parent::Scope;

public:
    TpotConfig();
    virtual ~TpotConfig();

public:
    virtual String getRootName () const override;
    virtual String getFileName () const override;
    virtual Path   getWorkDir  () const override;
    virtual Path   getExeDir   () const override;
    virtual Path   getHomeDir  () const override;
    virtual Path   getGlobalDir() const override;
};

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTCONFIG_HPP__

