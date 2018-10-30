/**
 * @file   tbuiex.cpp
 * @brief  Tbag-UI External project entry-point.
 * @author zer0
 * @date   2018-10-29
 */

#if defined(USE_TBAG_UI)
# include <libtbui/libtbui.h>
int main(int argc, char ** argv, char ** envs)
{
    return tbuiExternalMain(argc, argv, envs);
}
#else // defined(USE_TBAG_UI)
# include <cstdlib>
# include <iostream>
int main()
{
    std::cerr << "Disable USE_TBAG_UI macro." << std::endl;
    return EXIT_FAILURE;
}
#endif


