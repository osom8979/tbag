/**
 * @file   ProcessStateTest.cpp
 * @brief  ProcessState class tester.
 * @author zer0
 * @date   2019-10-20
 */

#include <gtest/gtest.h>
#include <libtbag/system/linux/ProcessState.hpp>
#include <libtbag/system/ProcessId.hpp>

using namespace libtbag;
using namespace libtbag::system;
using namespace libtbag::system::linux;

TEST(ProcessStateTest, Default)
{
    if (!isLinuxPlatform()) {
        std::cout << "Skip this test. (windows only)" << std::endl;
        return;
    }

    auto const process_id = getProcessId();
    ASSERT_GT(process_id, 1);
    auto const result = getProcessState(process_id);
    ASSERT_EQ(E_SUCCESS, result);
    auto const & state = result.val;
    ASSERT_EQ(process_id, state.pid);

    auto const text = getPrintableTextOfProcessState(state);
    std::cout << "Process state (" << process_id << "):\n" << text << std::endl;
}

