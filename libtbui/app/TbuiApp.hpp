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
#include <libtbag/app/SimpleApp.hpp>
#include <libtbui/export.h>

namespace libtbui {
namespace app     {

/**
 * TbuiApp class prototype.
 *
 * @author zer0
 * @date   2018-10-30
 */
class TBUI_API TbuiApp : public libtbag::app::SimpleApp
{
public:
    using Base    = libtbag::app::SimpleApp;
    using Param   = Base::Param;
    using Version = Base::Version;

public:
    TbuiApp(int argc = 0, char ** argv = nullptr, char ** envs = nullptr,
            bool init = true, char const * name = nullptr);
    virtual ~TbuiApp();

protected:
    virtual bool onCreate() override;
    virtual void onDestroy() override;

protected:
    virtual Version getVersion() override;

protected:
    virtual int onMain(Commands const & commands) override;
};

} // namespace app
} // namespace libtbui

#endif // __INCLUDE_LIBTBAG__LIBTBUI_APP_TBUIAPP_HPP__
