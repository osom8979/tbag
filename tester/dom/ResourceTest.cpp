/**
 * @file   ResourceTest.cpp
 * @brief  Resource class tester.
 * @author zer0
 * @date   2016-04-12
 */

#include <gtest/gtest.h>
#include <libtbag/dom/Resource.hpp>

using namespace libtbag;
using namespace libtbag::dom;

TEST(ResourceTest, utf8)
{
    unsigned char hangul [] = {
            0xea, 0xb0, 0x80, 0xeb, 0x82, 0x98, 0xeb, 0x8b, 0xa4, 0xeb
          , 0x9d, 0xbc, 0xeb, 0xa7, 0x88, 0xeb, 0xb0, 0x94, 0xec, 0x82
          , 0xac, 0xec, 0x95, 0x84, 0xec, 0x9e, 0x90, 0xec, 0xb0, 0xa8
          , 0xec, 0xb9, 0xb4, 0xed, 0x83, 0x80, 0xed, 0x8c, 0x8c, 0xed
          , 0x95, 0x98, '\0' };
    std::string const HANGUL_UTF8 = u8"가나다라마바사아자차카타파하";

    char const * const TEST_NAME = "hangul";
    char const * const TEST_TAG = "utf8";
    char const * const FILE_NAME = "__resource_test_utf8.xml";

    Resource res;
    res.set(TEST_NAME, (char*)hangul);
    res.set_tag(TEST_TAG);
    ASSERT_TRUE(res.save(FILE_NAME));

    res.clear();
    ASSERT_TRUE(res.readFile(FILE_NAME, TEST_TAG));
    ASSERT_EQ(res.getString(TEST_NAME), HANGUL_UTF8);
}

class ResourceFixtureTest : public ::testing::Test
{
public:
    std::string xml;
    std::string tag;

    std::string attribute1;
    std::string attribute2;
    std::string attribute3;
    std::string attribute4;
    std::string attribute5;

    std::string value1;
    std::string value2;
    std::string value3;
    std::string value4;
    std::string value5;

    std::string value1_2;
    std::string value2_2;
    int         value3_2;
    int         value4_2;
    float       value5_2;

    Resource res;

public:
    ResourceFixtureTest() = default;
    ~ResourceFixtureTest() = default;

public:
    virtual void SetUp() override {
        attribute1 = "title1";
        attribute2 = "title2";
        attribute3 = "number";
        attribute4 = "number";
        attribute5 = "floating";

        value1 = "test&amp;title1";
        value2 = "test&amp;title2";
        value3 = "100";
        value4 = "50";
        value5 = "55.555555";

        value1_2 = "test&title1";
        value2_2 = "test&title2";
        value3_2 = 100;
        value4_2 = 50;
        value5_2 = 55.555555;

        xml = std::string() + "<?xml version=\"1.0\"?>" + "<resource>"
              + "<property name=\"" + attribute1 + "\">" + value1 + "</property>"
              + "<property name=\"" + attribute2 + "\">" + value2 + "</property>"
              + "<property name=\"" + attribute3 + "\">" + value3 + "</property>"
              + "<property name=\"" + attribute4 + "\">" + value4 + "</property>"
              + "<property name=\"" + attribute5 + "\">" + value5 + "</property>"
              + "</resource>";
        tag = "property";

        res.readString(xml, tag);
    }

    virtual void TearDown() override {
        // EMPTY.
    }
};

TEST_F(ResourceFixtureTest, clear_and_size)
{
    ASSERT_GT(res.size(), 0U);

    res.clear();
    ASSERT_EQ(res.size(), 0U);
}

TEST_F(ResourceFixtureTest, get_tag)
{
    ASSERT_EQ(res.get_tag(), tag);
}

TEST_F(ResourceFixtureTest, readFromXmlString)
{
    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);

    ASSERT_EQ(map.size(), 4U);
    ASSERT_EQ(map.find(attribute1)->second, value1_2);
    ASSERT_EQ(map.find(attribute2)->second, value2_2);
    ASSERT_EQ(map.find(attribute3)->second, value3);
}

TEST_F(ResourceFixtureTest, save)
{
    char const * const FILE_NAME = "__resource_test_save.xml";

    Resource::Map map;
    map = Resource::readFromXmlString(xml, tag);
    ASSERT_TRUE(Resource::save(FILE_NAME, tag, map));
}

TEST_F(ResourceFixtureTest, getValue)
{
    /* TODO: Valgrind memory leak:
     * ==38325== 272 bytes in 1 blocks are definitely lost in loss record 86 of 107
     * ==38325==    at 0x100277EBB: malloc (in /usr/local/Cellar/valgrind/3.11.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
     * ==38325==    by 0x1005E4B58: getPerThreadBufferFor_dlerror(unsigned long) (in /usr/lib/system/libdyld.dylib)
     * ==38325==    by 0x7FFF5FC0BFA4: dlerrorSet(char const*) (in /usr/lib/dyld)
     * ==38325==    by 0x7FFF5FC0C2D2: dlopen (in /usr/lib/dyld)
     * ==38325==    by 0x1005E479B: dlopen (in /usr/lib/system/libdyld.dylib)
     * ==38325==    by 0x100E4661C: std::logic_error::~logic_error() (in /usr/lib/libc++abi.dylib)
     * ==38325==    by 0x100E43D6A: __cxa_decrement_exception_refcount (in /usr/lib/libc++abi.dylib)
     * ==38325==    by 0x1001157BF: bool libtbag::Resource::getValue<int, libtbag::Resource::getInteger(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, int*) const::{lambda(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)#1}>(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, int*, libtbag::Resource::getInteger(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, int*) const::{lambda(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)#1}) const (in ./tester)
     * ==38325==    by 0x100115554: libtbag::Resource::getInteger(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, int*) const (in ./tester)
     * ==38325==    by 0x1001154FD: libtbag::Resource::getInteger(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, int) const (in ./tester)
     * ==38325==    by 0x10011A91A: ResourceFixtureTest_getValue_Test::TestBody() (in ./tester)
     * ==38325==    by 0x1001786C9: void testing::internal::HandleExceptionsInMethodIfSupported<testing::Test, void>(testing::Test*, void (testing::Test::*)(), char const*) (gtest.cc:2078)
     *
     * ==38325== 508 (80 direct, 428 indirect) bytes in 1 blocks are definitely lost in loss record 92 of 107
     * ==38325==    at 0x100277EBB: malloc (in /usr/local/Cellar/valgrind/3.11.0/lib/valgrind/vgpreload_memcheck-amd64-darwin.so)
     * ==38325==    by 0x1006608D6: __Balloc_D2A (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x10066121F: __d2b_D2A (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100662F29: __rvOK_D2A (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100663D27: __strtodg (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100664EAA: strtof_l (in /usr/lib/system/libsystem_c.dylib)
     * ==38325==    by 0x100441A21: std::__1::stof(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, unsigned long*) (in /usr/lib/libc++.1.dylib)
     * ==38325==    by 0x1001262CF: libtbag::Resource::getFloat(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float*) const::{lambda(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)#1}::operator()(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&) const (in ./tester)
     * ==38325==    by 0x100126211: bool libtbag::Resource::getValue<float, libtbag::Resource::getFloat(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float*) const::{lambda(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)#1}>(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float*, libtbag::Resource::getFloat(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float*) const::{lambda(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&)#1}) const (in ./tester)
     * ==38325==    by 0x100125FF4: libtbag::Resource::getFloat(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float*) const (in ./tester)
     * ==38325==    by 0x10011FA6D: libtbag::Resource::getFloat(std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char> > const&, float) const (in ./tester)
     * ==38325==    by 0x10011B589: ResourceFixtureTest_getValue_Test::TestBody() (in ./tester)
     */

    ASSERT_EQ(res.getString(attribute1), value1_2);

    ASSERT_EQ(res.getInteger(attribute3), value3_2);
    ASSERT_EQ(res.getInteger(attribute1), 0);

    ASSERT_EQ(res.getUnInteger(attribute3), static_cast<unsigned int>(value3_2));
    ASSERT_EQ(res.getUnInteger(attribute1), 0U);

    ASSERT_EQ(res.getLongLong(attribute3), value3_2);
    ASSERT_EQ(res.getLongLong(attribute1), 0);

    ASSERT_EQ(res.getUnLongLong(attribute3), static_cast<unsigned long long>(value3_2));
    ASSERT_EQ(res.getUnLongLong(attribute1), 0U);

    ASSERT_FLOAT_EQ(res.getFloat(attribute5), value5_2);
    ASSERT_FLOAT_EQ(res.getFloat(attribute1), 0.0);

    float abs_error = 0.0001;

    ASSERT_NEAR(res.getFloat(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getFloat(attribute1),      0.0, abs_error);

    ASSERT_NEAR(res.getDouble(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getDouble(attribute1),      0.0, abs_error);

    ASSERT_NEAR(res.getLongDouble(attribute5), value5_2, abs_error);
    ASSERT_NEAR(res.getLongDouble(attribute1),      0.0, abs_error);
}

TEST_F(ResourceFixtureTest, get)
{
    ASSERT_EQ(res.get(attribute1, ""), value1_2);

    ASSERT_EQ(res.get(attribute3, static_cast<int>(0)), value3_2);
    ASSERT_EQ(res.get(attribute3, static_cast<unsigned int>(0)), static_cast<unsigned int>(value3_2));
    ASSERT_EQ(res.get(attribute3, static_cast<long long>(0)), value3_2);
    ASSERT_EQ(res.get(attribute3, static_cast<unsigned long long>(0)), static_cast<unsigned long long>(value3_2));

    float abs_error = 0.0001;
    ASSERT_NEAR(res.get(attribute5, static_cast<float>(0)), value5_2, abs_error);
    ASSERT_NEAR(res.get(attribute5, static_cast<double>(0)), value5_2, abs_error);
    ASSERT_NEAR(res.get(attribute5, static_cast<long double>(0)), value5_2, abs_error);
}

TEST_F(ResourceFixtureTest, set)
{
    Resource res;
    res.set(attribute3, value3_2);
    ASSERT_EQ(res.getString(attribute3), value3);

    res.set(attribute4, value4_2);
    ASSERT_EQ(res.getString(attribute4), value4);
}

TEST_F(ResourceFixtureTest, at)
{
    ASSERT_EQ(res.at(attribute1), value1_2);

    std::string const TEST = "TEST";
    res.at(attribute1) = TEST;
    ASSERT_EQ(res.at(attribute1), TEST);
}

