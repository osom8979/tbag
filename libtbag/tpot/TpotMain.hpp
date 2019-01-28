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
    using RunnerParams  = DefaultApp::RunnerParams;
    using HelpCommander = DefaultApp::HelpCommander;
    using Element       = DefaultApp::Element;
    using Params        = DefaultApp::Params;

private:
    DefaultApp _app;
    Params _params;

public:
    TpotMain();
    TpotMain(int argc, char ** argv);
    TpotMain(int argc, char ** argv, char ** envs);
    virtual ~TpotMain();

public:
    int run();

public:
    void onTerminateSignal(int signum);

public:
    void onOptions(HelpCommander & commander);
    void onInfo(Element const & element);

public:
    int onRun(RunnerParams const & params);
};

} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_TPOTMAIN_HPP__

