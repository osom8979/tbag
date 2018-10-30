/**
 * @file   libtbui.cpp
 * @brief  libtbui class implementation.
 * @author zer0
 * @date   2018-10-29
 */

#include <libtbui/libtbui.h>
#include <libtbui/util/TbuiVersion.hpp>

int tbuiGetMajorVersion()
{
    return libtbui::util::getMajorVersion();
}

int tbuiGetMinorVersion()
{
    return libtbui::util::getMinorVersion();
}

#include <libtbui/app/ExternalMain.hpp>

int tbuiExternalMain(int argc, char ** argv, char ** envs)
{
    return libtbui::app::ExternalMain(argc, argv, envs).run();
}

