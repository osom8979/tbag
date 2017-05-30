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
#include <libtbag/app/Service.hpp>
#include <libtbag/string/HelpCommander.hpp>

#include <libtbag/tpot/res/TpotConfig.hpp>
#include <libtbag/tpot/res/TpotNode.hpp>
#include <libtbag/tpot/res/TpotLog.hpp>

#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {

TBAG_CONSTEXPR char const * const TPOT_SERVICE_NAME = "tpot";

/**
 * TpotMain class prototype.
 *
 * @author zer0
 * @date   2017-05-13
 */
class TBAG_API TpotMain final : public app::Service
{
public:
    using HelpCommander = string::HelpCommander;
    using TpotLog  = res::TpotLog;
    using TpotNode = res::TpotNode;

    using WeakTpotLog  = std::weak_ptr<res::TpotLog>;
    using WeakTpotNode = std::weak_ptr<res::TpotNode>;

public:
    enum class RunningMode
    {
        RUN_APPLICATION,
        RUN_INSTALL,
        RUN_UNINSTALL,
        RUN_START,
        RUN_STOP,
        RUN_REQUEST,
    };

private:
    HelpCommander _commander;
    RunningMode   _mode;

private:
    std::string _config_path;

private:
    bool _help;
    bool _test;
    bool _verbose;
    bool _unknown;
    bool _version;

public:
    TpotMain(int argc, char ** argv, char ** envs);
    virtual ~TpotMain();

private:
    void initCommander(int argc, char ** argv);
    void initConfig();

public:
    int autoRun();

private:
    int runRequest();
    int runApplication();
    int runServiceInstall();
    int runServiceUninstall();
    int runServiceStart();
    int runServiceStop();

// Static methods.
public:
    /**
     * Obtain the pointer of log config.
     *
     * @warning
     *  This function is not thread safe.
     */
    static TpotLog * getTpotLogPointer();

    /**
     * Obtain the pointer of node config.
     *
     * @warning
     *  This function is not thread safe.
     */
    static TpotNode * getTpotNodePointer();

// Event methods.
public:
    virtual bool onCreate () override;
    virtual int  onRunning() override;
    virtual void onDestroy() override;
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

