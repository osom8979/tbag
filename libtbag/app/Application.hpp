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
class TBAG_API Application : public Noncopyable
{
public:
    struct Property
    {
        int argc;
        char ** argv;
        char ** envs;

        int exit_code;
    };

    using SharedProperty = std::shared_ptr<Property>;
    using   WeakProperty =   std::weak_ptr<Property>;

    using Global = container::Global;

public:
    TBAG_CONSTEXPR static char const * const PROPERTY_NAME = "libtbag::app::Application::Property";

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
    void setExitCode(int code);
    int  getExitCode();

public:
    int run();

public:
    virtual void onCreate() { /* EMPTY. */ }
    virtual void onRunning() { /* EMPTY. */ }
    virtual void onDestroy() { /* EMPTY. */ }
};

} // namespace app

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_APP_APPLICATION_HPP__

