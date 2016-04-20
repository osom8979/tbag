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

class PropertyFixtureTest : public ::testing::Test
{
public:
    DemoProperty property;

public:
    PropertyFixtureTest() {
        property.loadOrCreate();
    }

    ~PropertyFixtureTest() {
        __EMPTY_BLOCK__
    }

public:
    virtual void SetUp() override {
        __EMPTY_BLOCK__
    }

    virtual void TearDown() override {
        __EMPTY_BLOCK__
    }
};

TEST_F(PropertyFixtureTest, test)
{
    ASSERT_EQ(this->property.get_IntegerKey(), 100);
    ASSERT_EQ(this->property.get_StringKey(), "TEST");
}

