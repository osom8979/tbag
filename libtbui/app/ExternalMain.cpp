/**
 * @file   ExternalMain.cpp
 * @brief  ExternalMain class implementation.
 * @author zer0
 * @date   2018-10-30
 */

#include <libtbui/app/ExternalMain.hpp>
#include <libtbui/log/TbuiLog.hpp>

#include <cstdlib>

namespace libtbui {
namespace app     {

ExternalMain::ExternalMain(int argc, char ** argv, char ** envs)
        : TbuiApp(argc, argv, envs)
{
    // EMPTY.
}

ExternalMain::~ExternalMain()
{
    // EMPTY.
}

} // namespace app
} // namespace libtbui

