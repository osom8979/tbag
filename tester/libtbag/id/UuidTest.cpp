/**
 * @file   UuidTest.cpp
 * @brief  Uuid class tester.
 * @author zer0
 * @date   2017-07-01
 */

#include <gtest/gtest.h>
#include <libtbag/id/Uuid.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <iostream>

using namespace libtbag;
using namespace libtbag::id;

TEST(UuidTest, Default)
{
    Uuid uuid1;
    Uuid uuid2 = Uuid::nil();

    ASSERT_EQ(uuid1, uuid2);

    std::string const TEST_UUID_STR = "123e4567-e89b-12d3-a456-426655440000";
    Uuid uuid3;
    ASSERT_EQ(Err::E_SUCCESS, uuid3.fromString(TEST_UUID_STR));

    std::string const UUID3_STRING = uuid3.toString();
    ASSERT_EQ(TEST_UUID_STR, string::lower(UUID3_STRING));
    ASSERT_NE(Uuid::nil(), uuid3);

    Uuid uuid4 = Uuid::ver4();
    Uuid uuid5 = Uuid::ver4();

    ASSERT_NE(Uuid::nil(), uuid4);
    ASSERT_NE(Uuid::nil(), uuid5);
    ASSERT_NE(uuid4, uuid5);
    std::cout << "UUID Version4 [1]: " << uuid4.toString() << std::endl;
    std::cout << "UUID Version4 [2]: " << uuid5.toString() << std::endl;
}

