/**
 * @file   FormatTest.cpp
 * @brief  Format class tester.
 * @author zer0
 * @date   2017-10-21
 */

#include <gtest/gtest.h>
#include <libtbag/string/Format.hpp>

using namespace libtbag;
using namespace libtbag::string;

TEST(FormatTest, Format)
{
    ASSERT_STREQ("TEST", format("%s", "TEST").c_str());
    ASSERT_STREQ("1234", format("%d", 1234).c_str());
    ASSERT_STREQ("1234", format("%.0Lf", static_cast<long double>(1234.123)).c_str());
    ASSERT_STREQ("1.34", format("%.2f", static_cast<double>(1.344555)).c_str());
    ASSERT_STREQ("TEST/1234", format("%s/%d", "TEST", 1234).c_str());
    ASSERT_STREQ("TEST/1234", fformat("{}/{}", "TEST", 1234).c_str());
}

TEST(FormatTest, FormatClass)
{
    Format format;
    ASSERT_STREQ("TEST $TEMP A", format.fformat("TEST $${} $A", "TEMP").c_str());
}

struct CustomFormat : public Format
{
    virtual std::string onEscape(char input) const override
    {
        if (input == 'Z') {
            return "TEMP";
        } else {
            return std::string(1, input);
        }
    }
};

TEST(FormatTest, CustomFormatClass)
{
    CustomFormat format;
    ASSERT_STREQ("A $ 100 TEMP", format.fformat("$A $$ {} $Z", 100).c_str());
}

