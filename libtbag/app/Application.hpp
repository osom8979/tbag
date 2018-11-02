/**
 * @file   Application.hpp
 * @brief  Application class prototype.
 * @author zer0
 * @date   2017-05-13
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_APPLICATION_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_APPLICATION_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Global.hpp>

#include <cstdlib>
#include <memory>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

/**
 * Application class prototype.
 *
 * @author zer0
 * @date   2017-05-13
 */
class TBAG_API Application : private Noncopyable
{
public:
    struct MainArguments
    {
        int argc;
        char ** argv;
        char ** envs;
    };

    using SharedMainArguments = std::shared_ptr<MainArguments>;
    using   WeakMainArguments =   std::weak_ptr<MainArguments>;

    using Global = container::Global;

public:
    TBAG_CONSTEXPR static char const * const MAIN_ARGUMENTS_NAME
            = "libtbag.app.Application.MainArguments";

private:
    bool _initialize_tbag;
    SharedMainArguments _main_arguments;

public:
    Application(int argc, char ** argv, char ** envs, bool init_tbag = false);
    Application(int argc, char ** argv, bool init_tbag = false);
    Application(bool init_tbag = false);
    virtual ~Application();

public:
    static WeakMainArguments getMainArguments();

public:
    static int getArgc();
    static char ** getArgv();
    static char ** getEnvs();

public:
    static std::vector<std::string> getCommandLineArguments();
    static std::string getCommandLineArgumentsString();

public:
    virtual int run();

public:
    virtual bool onCreate () { return true; }
    virtual void onDestroy() { /* EMPTY. */ }

public:
    virtual int onRunning(std::vector<std::string> const & args) = 0;
};

/**
 * Application event guard.
 *
 * @author zer0
 * @date   2017-06-03
 */
template <typename ApplicationType>
class ApplicationGuard
{
private:
    ApplicationType & _app;
    bool _create_result;

public:
    ApplicationGuard(ApplicationType & app) : _app(app)
    {
        try {
            _create_result = _app.onCreate();
        } catch (...) {
            _create_result = false;
        }
    }

    virtual ~ApplicationGuard()
    {
        if (_create_result) {
            _app.onDestroy();
        }
    }

public:
    inline bool isCreateSuccess() const TBAG_NOEXCEPT { return _create_result; }
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_APPLICATION_HPP__

