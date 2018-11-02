/**
 * @file   TbuiApp.hpp
 * @brief  TbuiApp class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBUI_APP_TBUIAPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBUI_APP_TBUIAPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/app/ex/DefaultApp.hpp>
#include <libtbui/export.h>

namespace libtbui {
namespace app     {

/**
 * TbuiApp class prototype.
 *
 * @author zer0
 * @date   2018-10-30
 */
class TBUI_API TbuiApp : private libtbag::Noncopyable
{
public:
    using DefaultApp    = libtbag::app::ex::DefaultApp;
    using Params        = DefaultApp::Params;
    using RunnerParams  = DefaultApp::RunnerParams;

private:
    Params      _params;
    DefaultApp  _app;

public:
    TbuiApp(int argc = 0, char ** argv = nullptr, char ** envs = nullptr);
    virtual ~TbuiApp();

public:
    int run();

private:
    virtual int run(RunnerParams const & params);
};

} // namespace app
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_APP_TBUIAPP_HPP__

