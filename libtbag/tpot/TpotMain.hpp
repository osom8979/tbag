/**
 * @file   TpotMain.hpp
 * @brief  TpotMain class prototype.
 * @author zer0
 * @date   2017-05-13
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

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

/**
 * TpotMain class prototype.
 *
 * @author zer0
 * @date   2017-05-13
 */
class TBAG_API TpotMain final : public app::ex::ServiceApp
{
private:
    bool _enable_test;

public:
    TpotMain(int argc, char ** argv, char ** envs);
    virtual ~TpotMain();

// Event methods.
public:
    virtual bool onCreate    () override;
    virtual bool onLoadConfig() override;
    virtual int  onRunning   () override;
    virtual void onDestroy   () override;
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

