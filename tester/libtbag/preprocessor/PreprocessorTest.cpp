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

TEST(PreprocessorTest, IncDec)
{
    int const INC = TBAG_PP_INC(50);
    ASSERT_EQ(51, INC);

    int const DEC = TBAG_PP_DEC(50);
    ASSERT_EQ(49, DEC);
}

TEST(PreprocessorTest, Cat)
{
    std::string const TEST_STR = "TEST";
    std::string const STR = TBAG_PP_CAT(TEST_, STR);
    ASSERT_EQ(TEST_STR, STR);
}

TEST(PreprocessorTest, Stringify)
{
    std::string const STR = TBAG_PP_STRINGIZE(test_string);
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

TEST(PreprocessorTest, Compl)
{
    bool const COMPL1 = TBAG_PP_COMPL(TBAG_PP_BOOL(0));
    bool const COMPL2 = TBAG_PP_COMPL(TBAG_PP_BOOL(1));
    bool const COMPL3 = TBAG_PP_COMPL(TBAG_PP_BOOL(100));

    ASSERT_TRUE(COMPL1);
    ASSERT_FALSE(COMPL2);
    ASSERT_FALSE(COMPL3);
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

#ifndef _PP_TEST_MACRO_REPEAT
#define _PP_TEST_MACRO_REPEAT(z, s, n, d) TBAG_PP_COMMA_IF(n) (std::to_string(n)+d)
#endif
    std::vector<std::string> const TEST1 = { TBAG_PP_REPEAT(5, _PP_TEST_MACRO_REPEAT, SUFFIX) };
#undef _PP_TEST_MACRO_REPEAT

    ASSERT_EQ(5, TEST1.size());
    ASSERT_EQ("0i", TEST1[0]);
    ASSERT_EQ("1i", TEST1[1]);
    ASSERT_EQ("2i", TEST1[2]);
    ASSERT_EQ("3i", TEST1[3]);
    ASSERT_EQ("4i", TEST1[4]);
}

TEST(PreprocessorTest, Tuple)
{
#define _PP_TEST_MACRO_TUPLES "OK", "NO"
    std::string const TEST1 = TBAG_PP_TUPLE_ELEM(2, 0, TBAG_PP_VARIADIC_TO_TUPLE(_PP_TEST_MACRO_TUPLES));
    std::string const TEST2 = TBAG_PP_TUPLE_ELEM(2, 1, TBAG_PP_VARIADIC_TO_TUPLE(_PP_TEST_MACRO_TUPLES));
#undef _PP_TEST_MACRO_TUPLES

    ASSERT_STREQ("OK", TEST1.c_str());
    ASSERT_STREQ("NO", TEST2.c_str());
}

TEST(PreprocessorTest, Variadic)
{
#ifndef _PP_TEST_MACRO_VARIADIC
#define _PP_TEST_MACRO_VARIADIC "a", "b", "c", "d"
#endif
    std::string const TEST1 = TBAG_PP_VARIADIC_ELEM(0, _PP_TEST_MACRO_VARIADIC);
    std::string const TEST2 = TBAG_PP_VARIADIC_ELEM(3, _PP_TEST_MACRO_VARIADIC);
    std::size_t TEST_SIZE   = TBAG_PP_VARIADIC_SIZE(_PP_TEST_MACRO_VARIADIC);
#undef _PP_TEST_MACRO_VARIADIC

    ASSERT_STREQ("a", TEST1.c_str());
    ASSERT_STREQ("d", TEST2.c_str());
    ASSERT_EQ(4, TEST_SIZE);
}

TEST(PreprocessorTest, VariadicArgs)
{
    int result0;
    std::string result1;
    auto func = [&](TBAG_PP_EXPEND_PARAMS(int, std::string)){
        result0 = v0;
        result1 = v1;
    };
    func(123456, "TEST");

    ASSERT_EQ(123456, result0);
    ASSERT_EQ("TEST", result1);
}

//TEST(PreprocessorTest, VariadicIsEmpty)
//{
//#define _EATER0(...)
//#define _EATER1(...) ,
//#define _EATER2(...) (/*empty*/)
//#define _EATER3(...) (/*empty*/),
//#define _EATER4(...) EATER1
//#define _MAC0() ()
//#define _MAC1(x) ()
//#define _MACV(...) ()
//#define _MAC2(x,y) whatever
//
//    int const i01 = TBAG_PP_VARIADIC_IS_EMPTY();
//    int const i02 = TBAG_PP_VARIADIC_IS_EMPTY(/*comment*/);
//    int const i03 = TBAG_PP_VARIADIC_IS_EMPTY(a);
//    int const i04 = TBAG_PP_VARIADIC_IS_EMPTY(a, b);
//    int const i05 = TBAG_PP_VARIADIC_IS_EMPTY(a, b, c);
//    int const i06 = TBAG_PP_VARIADIC_IS_EMPTY(a, b, c, d);
//    int const i07 = TBAG_PP_VARIADIC_IS_EMPTY(a, b, c, d, e);
//    int const i08 = TBAG_PP_VARIADIC_IS_EMPTY((void));
//    int const i09 = TBAG_PP_VARIADIC_IS_EMPTY((void), b, c, d);
//    int const i10 = TBAG_PP_VARIADIC_IS_EMPTY(TBAG_PP_VIE_TRIGGER_PARENTHESIS);
//    int const i11 = TBAG_PP_VARIADIC_IS_EMPTY(_EATER0);
//    int const i12 = TBAG_PP_VARIADIC_IS_EMPTY(_EATER1);
//    int const i13 = TBAG_PP_VARIADIC_IS_EMPTY(_EATER2);
//    int const i14 = TBAG_PP_VARIADIC_IS_EMPTY(_EATER3);
//    int const i15 = TBAG_PP_VARIADIC_IS_EMPTY(_EATER4);
//    int const i16 = TBAG_PP_VARIADIC_IS_EMPTY(_MAC0);
//    int const i17 = TBAG_PP_VARIADIC_IS_EMPTY(_MAC1);
//    int const i18 = TBAG_PP_VARIADIC_IS_EMPTY(_MACV);
//
//#undef _EATER0
//#undef _EATER1
//#undef _EATER2
//#undef _EATER3
//#undef _EATER4
//#undef _MAC0
//#undef _MAC1
//#undef _MACV
//#undef _MAC2
//
//    // This one will fail because MAC2 is not called correctly.
//    // TBAG_PP_VARIADIC_IS_EMPTY(_MAC2);
//
//    EXPECT_EQ(1, i01);
//    EXPECT_EQ(1, i02);
//    EXPECT_EQ(0, i03);
//    EXPECT_EQ(0, i04);
//    EXPECT_EQ(0, i05);
//    EXPECT_EQ(0, i06);
//    EXPECT_EQ(0, i07);
//    EXPECT_EQ(0, i08);
//    EXPECT_EQ(0, i09);
//    EXPECT_EQ(0, i10);
//    EXPECT_EQ(0, i11);
//    EXPECT_EQ(0, i12);
//    EXPECT_EQ(0, i13);
//    EXPECT_EQ(0, i14);
//    EXPECT_EQ(0, i15);
//    EXPECT_EQ(0, i16);
//    EXPECT_EQ(0, i17);
//    EXPECT_EQ(0, i18);
//
//#define _EATER5(...) TBAG_PP_IF(TBAG_PP_VARIADIC_IS_EMPTY(__VA_ARGS__), "T", "F")
//    std::string const s01 = _EATER5();
//    std::string const s02 = _EATER5(a);
//    std::string const s03 = _EATER5(a, b);
//#undef _EATER5
//
//    EXPECT_STREQ("T", s01.c_str());
//    EXPECT_STREQ("F", s02.c_str());
//    EXPECT_STREQ("F", s03.c_str());
//}

