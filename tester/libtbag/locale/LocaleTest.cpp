/**
 * @file   LocaleTest.cpp
 * @brief  Locale tester.
 * @author zer0
 * @date   2016-12-05
 */

#include <gtest/gtest.h>
#include <libtbag/locale/Locale.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::locale;

static char const * const TEST_LOCALE0 = "de";
static char const * const TEST_LOCALE1 = "zh_CN";
static char const * const TEST_LOCALE2 = "_CN";
static char const * const TEST_LOCALE3 = "ko_KR.cp949";
static char const * const TEST_LOCALE4 = "_KR.cp949";
static char const * const TEST_LOCALE5 = "ko.cp949";
static char const * const TEST_LOCALE6 = ".cp949";
static char const * const TEST_LOCALE7  = "en_US.UTF-8@zer0";
static char const * const TEST_LOCALE8  = "_US.UTF-8@zer0";
static char const * const TEST_LOCALE9  = "en.UTF-8@zer0";
static char const * const TEST_LOCALE10 = "en_US@zer0";
static char const * const TEST_LOCALE11 = ".UTF-8@zer0";
static char const * const TEST_LOCALE12 = "_US@zer0";
static char const * const TEST_LOCALE13 = "en@zer0";
static char const * const TEST_LOCALE14 = "@zer0";

//TEST(LocaleTest, getLanguage)
//{
//    ASSERT_STREQ(  "", getLanguage("").c_str());
//    ASSERT_STREQ("de", getLanguage(TEST_LOCALE0).c_str());
//    ASSERT_STREQ("zh", getLanguage(TEST_LOCALE1).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE2).c_str());
//    ASSERT_STREQ("ko", getLanguage(TEST_LOCALE3).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE4).c_str());
//    ASSERT_STREQ("ko", getLanguage(TEST_LOCALE5).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE6).c_str());
//    ASSERT_STREQ("en", getLanguage(TEST_LOCALE7).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE8).c_str());
//    ASSERT_STREQ("en", getLanguage(TEST_LOCALE9).c_str());
//    ASSERT_STREQ("en", getLanguage(TEST_LOCALE10).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE11).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE12).c_str());
//    ASSERT_STREQ("en", getLanguage(TEST_LOCALE13).c_str());
//    ASSERT_STREQ(  "", getLanguage(TEST_LOCALE14).c_str());
//}
//
//TEST(LocaleTest, getCountry)
//{
//    ASSERT_STREQ(  "", getCountry("").c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE0).c_str());
//    ASSERT_STREQ("CN", getCountry(TEST_LOCALE1).c_str());
//    ASSERT_STREQ("CN", getCountry(TEST_LOCALE2).c_str());
//    ASSERT_STREQ("KR", getCountry(TEST_LOCALE3).c_str());
//    ASSERT_STREQ("KR", getCountry(TEST_LOCALE4).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE5).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE6).c_str());
//    ASSERT_STREQ("US", getCountry(TEST_LOCALE7).c_str());
//    ASSERT_STREQ("US", getCountry(TEST_LOCALE8).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE9).c_str());
//    ASSERT_STREQ("US", getCountry(TEST_LOCALE10).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE11).c_str());
//    ASSERT_STREQ("US", getCountry(TEST_LOCALE12).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE13).c_str());
//    ASSERT_STREQ(  "", getCountry(TEST_LOCALE14).c_str());
//}
//
//TEST(LocaleTest, getEncoding)
//{
//    ASSERT_STREQ(     "", getEncoding("").c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE0).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE1).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE2).c_str());
//    ASSERT_STREQ("cp949", getEncoding(TEST_LOCALE3).c_str());
//    ASSERT_STREQ("cp949", getEncoding(TEST_LOCALE4).c_str());
//    ASSERT_STREQ("cp949", getEncoding(TEST_LOCALE5).c_str());
//    ASSERT_STREQ("cp949", getEncoding(TEST_LOCALE6).c_str());
//    ASSERT_STREQ("UTF-8", getEncoding(TEST_LOCALE7).c_str());
//    ASSERT_STREQ("UTF-8", getEncoding(TEST_LOCALE8).c_str());
//    ASSERT_STREQ("UTF-8", getEncoding(TEST_LOCALE9).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE10).c_str());
//    ASSERT_STREQ("UTF-8", getEncoding(TEST_LOCALE11).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE12).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE13).c_str());
//    ASSERT_STREQ(     "", getEncoding(TEST_LOCALE14).c_str());
//}
//
//TEST(LocaleTest, getModifier)
//{
//    ASSERT_STREQ(    "", getModifier("").c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE0).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE1).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE2).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE3).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE4).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE5).c_str());
//    ASSERT_STREQ(    "", getModifier(TEST_LOCALE6).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE7).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE8).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE9).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE10).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE11).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE12).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE13).c_str());
//    ASSERT_STREQ("zer0", getModifier(TEST_LOCALE14).c_str());
//}

TEST(LocaleTest, Information)
{
    std::cout << "Default locale name: " << getDefaultIcuLocaleName() << std::endl;
}

