/**
 * @file   tlua.cpp
 * @brief  tlua entry-point.
 * @author zer0
 * @date   2019-12-27
 */

#include <libtbag/libtbag.h>
#include <libtbag/script/LuaMain.hpp>

int main(int argc, char ** argv)
{
    tbInitialize();
    auto const code = libtbag::script::runLuaJit(argc, argv);
    tbRelease();
    return code;
}

