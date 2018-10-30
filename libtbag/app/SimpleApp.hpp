/**
 * @file   SimpleApp.hpp
 * @brief  SimpleApp class prototype.
 * @author zer0
 * @date   2018-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_APP_SIMPLEAPP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_APP_SIMPLEAPP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/string/HelpCommander.hpp>
#include <libtbag/util/Version.hpp>

#include <vector>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace app {

/**
 * SimpleApp class prototype.
 *
 * @author zer0
 * @date   2018-10-30
 */
class TBAG_API SimpleApp : private Noncopyable
{
public:
    using HelpCommander = libtbag::string::HelpCommander;
    using Arguments     = libtbag::string::Arguments;
    using Version       = libtbag::util::Version;
    using Commands      = std::vector<std::string>;

public:
    TBAG_CONSTEXPR static int const DEFAULT_LOG_LEVEL = 5; // 5 is WARNING_SEVERITY

public:
    struct Param
    {
        int         argc = 0;
        char **     argv = nullptr;
        char **     envs = nullptr;
        bool        init = false;
        std::string name;

        Param(int c = 0, char ** v = nullptr, char ** e = nullptr, bool i = false, char const * n = nullptr)
                : argc(c), argv(v), envs(e), init(i), name(n == nullptr ? std::string() : n)
        { /* EMPTY. */ }
        ~Param()
        { /* EMPTY. */ }
    };

public:
    Param param;

    HelpCommander help;
    Commands      commands;

    bool print_help    = false;
    bool print_verbose = false;
    bool print_version = false;
    bool print_unknown = false;

    bool log_raw   = false;
    bool log_color = false;
    int  log_level = DEFAULT_LOG_LEVEL;

public:
    SimpleApp(int argc = 0, char ** argv = nullptr, char ** envs = nullptr,
              bool init = false, char const * name = nullptr);
    SimpleApp(Param const & params);
    virtual ~SimpleApp();

protected:
    virtual bool onCreate();
    virtual void onDestroy();

protected:
    virtual Version getVersion();

protected:
    virtual int onMain(Commands const & commands);

public:
    int run();
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_SIMPLEAPP_HPP__

