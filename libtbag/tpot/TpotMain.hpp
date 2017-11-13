/**
 * @file   TpotMain.hpp
 * @brief  TpotMain class prototype.
 * @author zer0
 * @date   2017-09-07
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <libtbag/app/ex/ServiceApp.hpp>
#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/log/node/DefaultLogXmlNode.hpp>
#include <libtbag/dom/xml/node/ServerXmlNode.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotMain class prototype.
 *
 * @author zer0
 * @date   2017-09-07
 */
class TBAG_API TpotMain final : public app::ex::ServiceApp
{
public:
    using HelpMap  = HelpCommander::HelpMap;
    using HelpPair = HelpCommander::HelpPair;

    using DefaultLogXmlNode = log::node::DefaultLogXmlNode;
    using WeakLogNode       = std::weak_ptr<DefaultLogXmlNode>;

    using ServerXmlNode  = dom::xml::node::ServerXmlNode;
    using WeakServerNode = std::weak_ptr<ServerXmlNode>;

private:
    HelpMap _commands;

private:
    std::string _ip;
    int  _port;
    int  _timeout;
    bool _service;

public:
    TpotMain(int argc, char ** argv, char ** envs);
    virtual ~TpotMain();

// Event methods.
public:
    virtual bool onCreate() override;
    virtual bool onLoadConfig(DefaultXmlModel & config) override;
    virtual int  onDefaultCommand(StringVector const & args) override;
    virtual void onDestroy() override;

public:
    static WeakLogNode getLogNode();
    static WeakServerNode getServerNode();
};

// ------------
// Entry-point.
// ------------

TBAG_API int runTpotMain(int argc, char ** argv, char ** envs);

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__

