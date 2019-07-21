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

    ASSERT_STREQ("0xA1B2C3D4", libtbag::string::fformat("0x{:0>8X}", 0xA1B2C3D4).c_str());
    ASSERT_STREQ("0x00B2C3D4", libtbag::string::fformat("0x{:0>8X}", 0xB2C3D4).c_str());
}

TEST(FormatTest, FormatClass)
{
    Format format;
    ASSERT_STREQ("TEST $TEMP A", format.fformat("TEST $${} $A", "TEMP").c_str());
}

struct CustomFormat : public Format
{
    virtual int onEscape(std::string const & source, std::size_t index, std::string & output) const override
    {
        if (source[index] == 'Z') {
            output = "TEMP";
        } else {
            output.assign(1, source[index]);
        }
        return 1;
    }
};

TEST(FormatTest, CustomFormatClass)
{
    CustomFormat format;
    ASSERT_STREQ("A $ 100 TEMP", format.fformat("$A $$ {} $Z", 100).c_str());
}

