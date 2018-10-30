/**
 * @file   TbuiApp.cpp
 * @brief  TbuiApp class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbui/app/TbuiApp.hpp>
#include <libtbui/util/TbuiVersion.hpp>

namespace libtbui {
namespace app     {

TbuiApp::TbuiApp(int argc, char ** argv, char ** envs, bool init, char const * name)
        : Base(argc, argv, envs, init, name)
{
    // EMPTY.
}

TbuiApp::~TbuiApp()
{
    // EMPTY.
}

bool TbuiApp::onCreate()
{
    return Base::onCreate();
}

void TbuiApp::onDestroy()
{
    Base::onDestroy();
}

TbuiApp::Version TbuiApp::getVersion()
{
    return libtbui::util::getVersion();
}

int TbuiApp::onMain(Commands const & commands)
{
    return 0;
}

} // namespace app
} // namespace libtbui

