/**
 * @file   PropertyTest.cpp
 * @brief  Property class tester.
 * @author zer0
 * @date   2016-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/dom/xml/Property.hpp>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::xml;

class DemoProperty : public Property
{
public:
    DemoProperty() = default;
    virtual ~DemoProperty() = default;

public:
    virtual void updateDefault() override
    {
        set_IntegerKey();
        set_StringKey();
    }

public:
    CREATE_PROPERTY(int, IntegerKey, 100);
    CREATE_PROPERTY(std::string, StringKey, "TEST");
};

TEST(PropertyTest, Default)
{
    DemoProperty property;
    property.loadOrCreate("__tbag_dom_property_test__.xml");

    ASSERT_EQ(   100, property.get_IntegerKey());
    ASSERT_EQ("TEST", property.get_StringKey());
}

