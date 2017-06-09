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
    struct Property
    {
        int argc;
        char ** argv;
        char ** envs;
    };

    using SharedProperty = std::shared_ptr<Property>;
    using   WeakProperty =   std::weak_ptr<Property>;

    using Global = container::Global;

public:
    TBAG_CONSTEXPR static char const * const PROPERTY_NAME
            = "libtbag.app.Application.Property";

private:
    SharedProperty _property;

public:
    Application(int argc, char ** argv, char ** envs);
    Application(int argc, char ** argv);
    Application();
    virtual ~Application();

public:
    static WeakProperty getProperty();

public:
    static int getArgc();
    static char ** getArgv();
    static char ** getEnvs();

public:
    virtual int run();

public:
    virtual bool onCreate () { return true;         }
    virtual int  onRunning() { return EXIT_FAILURE; }
    virtual void onDestroy() { /* EMPTY. */         }
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
    bool create_result;

public:
    ApplicationGuard(ApplicationType & app) : _app(app)
    {
        try {
            create_result = _app.onCreate();
        } catch (...) {
            create_result = false;
        }
    }

    virtual ~ApplicationGuard()
    {
        if (create_result) {
            _app.onDestroy();
        }
    }

public:
    inline bool isCreateSuccess() const TBAG_NOEXCEPT
    {
        return create_result;
    }
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_APPLICATION_HPP__

