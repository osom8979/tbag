/**
 * @file   NumberOfArgumentsTest.cpp
 * @brief  NumberOfArguments class tester.
 * @author zer0
 * @date   2019-05-16
 */

#include <gtest/gtest.h>
#include <libtbag/tmp/NumberOfArguments.hpp>

#include <type_traits>
#include <sstream>

using namespace libtbag;
using namespace libtbag::tmp;

template <typename ... Args>
static int NumberOfArgumentsTest_Argument1(Args && ... args) TBAG_NOEXCEPT
{
    static_assert(NumberOfTemplateArguments<Args ...>::value == 1, "");
    return get_number_of_arguments(std::forward<Args>(args) ...);
}

template <typename ... Args>
static int NumberOfArgumentsTest_Argument2(Args && ... args) TBAG_NOEXCEPT
{
    static_assert(NumberOfTemplateArguments<Args ...>::value == 2, "");
    return get_number_of_arguments(std::forward<Args>(args) ...);
}

TEST(NumberOfArgumentsTest, Default)
{
    ASSERT_EQ(1, get_number_of_arguments(1));
    ASSERT_EQ(2, get_number_of_arguments("test", "string"));
    ASSERT_EQ(3, get_number_of_arguments("test", "string", 100));
}

TEST(NumberOfArgumentsTest, ParameterPack)
{
    std::stringstream ss;
    ss << "test" << 100 << std::endl;
    ASSERT_EQ(1, NumberOfArgumentsTest_Argument1(ss.str()));
    ASSERT_EQ(2, NumberOfArgumentsTest_Argument2(ss.str(), ss.str()));
}

