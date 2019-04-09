/**
 * @file   TpotMain.hpp
 * @brief  TpotMain class prototype.
 * @author zer0
 * @date   2019-01-28
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/app/ex/DefaultApp.hpp>
#include <libtbag/tpot/PotManager.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotMain class prototype.
 *
 * @author zer0
 * @date   2019-01-28
 */
class TBAG_API TpotMain : private Noncopyable
{
public:
    using DefaultApp    = libtbag::app::ex::DefaultApp;
    using Version       = libtbag::util::Version;
    using RunnerParams  = DefaultApp::RunnerParams;
    using HelpCommander = DefaultApp::HelpCommander;
    using Element       = DefaultApp::Element;
    using Params        = DefaultApp::Params;
    using PotManager    = libtbag::tpot::PotManager;
    using Pot           = PotManager::Pot;
    using Pots          = PotManager::Pots;

private:
    std::string _default_app;
    DefaultApp  _app;
    Params      _params;
    PotManager  _manager;

public:
    TpotMain();
    TpotMain(int argc, char ** argv);
    TpotMain(int argc, char ** argv, char ** envs);
    TpotMain(std::string const & service_name, std::string const & config_name, std::string const & synopsis,
             int argc, char ** argv, char ** envs);
    TpotMain(std::string const & service_name, std::string const & config_name, std::string const & synopsis,
             Version const & version, DefaultApp::ConfigScope scope,
             Pots const & pots, std::string const & default_app,
             int argc, char ** argv, char ** envs);
    virtual ~TpotMain();

private:
    void createDefaultLogger();

public:
    int run();

public:
    void onTerminateSignal(int signum);

public:
    void onOptions(HelpCommander & commander);
    void onInfo(Element const & element);

private:
    void printParamsInfo(RunnerParams const & params) const;

public:
    int onRun(RunnerParams const & params);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__

