/**
 * @file   PropertyTest.cpp
 * @brief  Property class tester.
 * @author zer0
 * @date   2016-04-13
 */

#include <gtest/gtest.h>
#include <libtbag/Property.hpp>

using namespace libtbag;

class DemoProperty : public Property
{
public:
    DemoProperty() = default;
    virtual ~DemoProperty() = default;

public:
    virtual void updateDefault() override {
        set_IntegerKey();
        set_StringKey();
    }

public:
    CREATE_PROPERTY(int, IntegerKey, 100);
    CREATE_PROPERTY(std::string, StringKey, "TEST");
};

// Fixture.

class PropertyTest : public ::testing::Test
{
public:
    DemoProperty property;

public:
    PropertyTest() {
        this->property.loadOrCreate();
    }
    virtual ~PropertyTest() = default;

public:
    virtual void SetUp() override {
    }

    virtual void TearDown() override {
    }
};

TEST_F(PropertyTest, test)
{
    ASSERT_EQ(this->property.get_IntegerKey(), 100);
    ASSERT_EQ(this->property.get_StringKey(), "TEST");
}

