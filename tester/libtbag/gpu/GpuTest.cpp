/**
 * @file   GpuTest.cpp
 * @brief  Gpu class tester.
 * @author zer0
 * @date   2018-01-13
 */

#include <gtest/gtest.h>
#include <libtbag/gpu/Gpu.hpp>

using namespace libtbag;
using namespace libtbag::gpu;

TEST(GpuTest, Information)
{
    runAllIfSupported([](Gpu & gpu){
        auto plat_info = gpu.atPlatformInfo();
        auto  dev_info = gpu.atDeviceInfo();
        std::cout.setf(std::ios_base::boolalpha);
        std::cout << "GPU Type(" << gpu.getTypeString()
                  << ") Plt("    << gpu.getPlatformId()
                  << ") Dev("    << gpu.getDeviceId()
                  << ") Cxt("    << gpu.getContextId()
                  << ") H("      << gpu.isHost()
                  << ") D("      << gpu.isDevice()
                  << ") S("      << gpu.isStream() << ")\n";
        std::cout << "* Profile(" << plat_info.profile
                  << ") Name("    << plat_info.name
                  << ") Vender("  << plat_info.vendor
                  << ") Version(" << plat_info.version << ")\n";
        std::cout << "* Extensions: " << plat_info.extensions << "\n";
        std::cout << "* Device(" << dev_info.name
                  << ") Ver("    << dev_info.driver_version
                  << ") DevVer(" << dev_info.device_version
                  << ") GMem("   << dev_info.global_memory << ")" << std::endl;
        for (auto & prop : dev_info.properties) {
            std::cout << "** " << prop.first << ": " << prop.second << std::endl;
        }
    });
}

