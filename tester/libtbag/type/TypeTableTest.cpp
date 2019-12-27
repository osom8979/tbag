/**
 * @file   TypeTableTest.cpp
 * @brief  TypeTable class tester.
 * @author zer0
 * @date   2018-01-16
 */

#include <gtest/gtest.h>
#include <libtbag/util/TestUtils.hpp>
#include <libtbag/type/TypeTable.hpp>
#include <sstream>

using namespace libtbag;
using namespace libtbag::type;

struct CustomTestType { /* EMPTY. */ };

template <typename T>
static std::string __get_printable_information()
{
    std::stringstream ss;
    ss << "NAME: "             << TypeInfo<T>::name()          << std::endl
       << " - isArithmetic: "  << TypeInfo<T>::isArithmetic()  << std::endl
       << " - isSpecialized: " << TypeInfo<T>::isSpecialized() << std::endl
       << " - isPointer: "     << TypeInfo<T>::isPointer()     << std::endl
       << " - size: "          << TypeInfo<T>::size()          << std::endl
       << " - index: "         << TypeInfo<T>::index()         << std::endl
       << " - maximum: "       << TypeInfo<T>::maximum()       << std::endl
       << " - minimum: "       << TypeInfo<T>::minimum()       << std::endl
       << " - lowest: "        << TypeInfo<T>::lowest()        << std::endl
       << " - epsilon: "       << TypeInfo<T>::epsilon()       << std::endl
       << " - round_error: "   << TypeInfo<T>::round_error()   << std::endl
       << " - infinity: "      << TypeInfo<T>::infinity()      << std::endl
       << " - quiet_NaN: "     << TypeInfo<T>::quiet_NaN()     << std::endl
       << " - signaling_NaN: " << TypeInfo<T>::signaling_NaN() << std::endl
       << " - denorm_min: "    << TypeInfo<T>::denorm_min()    << std::endl;
    return ss.str();
}

TEST(TypeTableTest, PrintInformation)
{
    std::stringstream ss;
    ss << __get_printable_information<              bool>() << std::endl;
    ss << __get_printable_information<              char>() << std::endl;
    ss << __get_printable_information<       signed char>() << std::endl;
    ss << __get_printable_information<     unsigned char>() << std::endl;
    ss << __get_printable_information<           wchar_t>() << std::endl;
    ss << __get_printable_information<          char16_t>() << std::endl;
    ss << __get_printable_information<          char32_t>() << std::endl;
    ss << __get_printable_information<             short>() << std::endl;
    ss << __get_printable_information<    unsigned short>() << std::endl;
    ss << __get_printable_information<               int>() << std::endl;
    ss << __get_printable_information<      unsigned int>() << std::endl;
    ss << __get_printable_information<              long>() << std::endl;
    ss << __get_printable_information<     unsigned long>() << std::endl;
    ss << __get_printable_information<         long long>() << std::endl;
    ss << __get_printable_information<unsigned long long>() << std::endl;
    ss << __get_printable_information<             float>() << std::endl;
    ss << __get_printable_information<            double>() << std::endl;
    ss << __get_printable_information<       long double>() << std::endl;
    TBAG_WRITE_INFORMATION(ss.str());
}

TEST(TypeTableTest, Default)
{
    ASSERT_EQ(TypeTable::TT_UNKNOWN, TypeInfo<    CustomTestType>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<              bool>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<              char>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<       signed char>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<     unsigned char>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<           wchar_t>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<          char16_t>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<          char32_t>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<             short>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<    unsigned short>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<               int>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<      unsigned int>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<              long>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<     unsigned long>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<         long long>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<unsigned long long>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<             float>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<            double>::table());
    ASSERT_NE(TypeTable::TT_UNKNOWN, TypeInfo<       long double>::table());
}

TEST(TypeTableTest, IntegerTypes)
{
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo<  int8_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo< uint8_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo< int16_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo<uint16_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo< int32_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo<uint32_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo< int64_t>::table());
    EXPECT_NE(TypeTable::TT_UNKNOWN, TypeInfo<uint64_t>::table());
}

TEST(TypeTableTest, GetTypeName)
{
    ASSERT_STREQ(   "BOOL", getTypeName(TypeTable::TT_BOOL   ));
    ASSERT_STREQ(   "CHAR", getTypeName(TypeTable::TT_CHAR   ));
    ASSERT_STREQ(  "SCHAR", getTypeName(TypeTable::TT_SCHAR  ));
    ASSERT_STREQ(  "UCHAR", getTypeName(TypeTable::TT_UCHAR  ));
    ASSERT_STREQ(  "WCHAR", getTypeName(TypeTable::TT_WCHAR  ));
    ASSERT_STREQ( "CHAR16", getTypeName(TypeTable::TT_CHAR16 ));
    ASSERT_STREQ( "CHAR32", getTypeName(TypeTable::TT_CHAR32 ));
    ASSERT_STREQ(  "SHORT", getTypeName(TypeTable::TT_SHORT  ));
    ASSERT_STREQ( "USHORT", getTypeName(TypeTable::TT_USHORT ));
    ASSERT_STREQ(    "INT", getTypeName(TypeTable::TT_INT    ));
    ASSERT_STREQ(   "UINT", getTypeName(TypeTable::TT_UINT   ));
    ASSERT_STREQ(   "LONG", getTypeName(TypeTable::TT_LONG   ));
    ASSERT_STREQ(  "ULONG", getTypeName(TypeTable::TT_ULONG  ));
    ASSERT_STREQ(  "LLONG", getTypeName(TypeTable::TT_LLONG  ));
    ASSERT_STREQ( "ULLONG", getTypeName(TypeTable::TT_ULLONG ));
    ASSERT_STREQ(  "FLOAT", getTypeName(TypeTable::TT_FLOAT  ));
    ASSERT_STREQ( "DOUBLE", getTypeName(TypeTable::TT_DOUBLE ));
    ASSERT_STREQ("LDOUBLE", getTypeName(TypeTable::TT_LDOUBLE));
}

TEST(TypeTableTest, GetTypeTable)
{
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<               int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<        signed int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<         const int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<      volatile int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<const volatile int>());
}

