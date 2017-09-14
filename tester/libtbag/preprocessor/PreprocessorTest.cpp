/**
 * @file   PreprocessorTest.cpp
 * @brief  Preprocessor class tester.
 * @author zer0
 * @date   2017-09-14
 */

#include <gtest/gtest.h>
#include <libtbag/preprocessor/Preprocessor.hpp>

#include <vector>
#include <string>

TEST(PreprocessorTest, Cat)
{
    std::string const TEST_STR = "TEST";
    std::string const STR = TBAG_PP_CAT(TEST_, STR);
    ASSERT_EQ(TEST_STR, STR);
}

TEST(PreprocessorTest, Stringify)
{
    std::string const STR = TBAG_PP_STRINGIFY(test_string);
    ASSERT_EQ("test_string", STR);
}

TEST(PreprocessorTest, Boolean)
{
    bool const TEST1 = TBAG_PP_BOOL(0);
    bool const TEST2 = TBAG_PP_BOOL(1);
    bool const TEST3 = TBAG_PP_BOOL(100);

    ASSERT_FALSE(TEST1);
    ASSERT_TRUE(TEST2);
    ASSERT_TRUE(TEST3);
}

TEST(PreprocessorTest, If)
{
    std::string const OK = "OK";
    std::string const NO = "NO";

    std::string const TEST1 = TBAG_PP_IF(0, OK, NO);
    std::string const TEST2 = TBAG_PP_IF(8, OK, NO);

    ASSERT_EQ(NO, TEST1);
    ASSERT_EQ(OK, TEST2);
}

TEST(PreprocessorTest, CommaIf)
{
    auto testAddFunc = [](int lh, int rh) -> decltype(lh+rh) { return lh + rh; };
    auto result = testAddFunc(TBAG_PP_COMMA_IF(0) 10 TBAG_PP_COMMA_IF(1) 5 TBAG_PP_COMMA_IF(0));
    ASSERT_EQ(15, result);
}

TEST(PreprocessorTest, OddEvenIf)
{
    std::string const OK = "OK";
    std::string const NO = "NO";

    std::string const TEST1 = TBAG_PP_ODD_IF(4, OK, NO);
    std::string const TEST2 = TBAG_PP_ODD_IF(5, OK, NO);
    std::string const TEST3 = TBAG_PP_EVEN_IF(6, OK, NO);
    std::string const TEST4 = TBAG_PP_EVEN_IF(7, OK, NO);

    ASSERT_EQ(NO, TEST1);
    ASSERT_EQ(OK, TEST2);
    ASSERT_EQ(OK, TEST3);
    ASSERT_EQ(NO, TEST4);
}

TEST(PreprocessorTest, Repeat)
{
    std::string const SUFFIX = "i";

#ifndef __TBAG_PREPROCESSOR_TEST__REPEAT_MACRO
#define __TBAG_PREPROCESSOR_TEST__REPEAT_MACRO(n, d) (std::to_string(n)+d) TBAG_PP_COMMA()
#endif
    std::vector<std::string> const TEST = { TBAG_PP_REPEAT(5, __TBAG_PREPROCESSOR_TEST__REPEAT_MACRO, SUFFIX) };
#undef __TBAG_PREPROCESSOR_TEST__REPEAT_MACRO

    ASSERT_EQ(5, TEST.size());
    ASSERT_EQ("0i", TEST[0]);
    ASSERT_EQ("1i", TEST[1]);
    ASSERT_EQ("2i", TEST[2]);
    ASSERT_EQ("3i", TEST[3]);
    ASSERT_EQ("4i", TEST[4]);
}

TEST(PreprocessorTest, Variadic)
{
#ifndef __TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO
#define __TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO "a", "b", "c", "d"
#endif
    std::string const TEST1 = TBAG_PP_VARIADIC_ELEM(0, __TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO);
    std::string const TEST2 = TBAG_PP_VARIADIC_ELEM(3, __TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO);
    std::size_t TEST_SIZE   = TBAG_PP_VARIADIC_SIZE(__TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO);
#undef __TBAG_PREPROCESSOR_TEST__VARIADIC_MACRO

    ASSERT_STREQ("a", TEST1.c_str());
    ASSERT_STREQ("d", TEST2.c_str());
    ASSERT_EQ(4, TEST_SIZE);
}

