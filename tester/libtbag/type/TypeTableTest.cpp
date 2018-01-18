/**
 * @file   TypeTableTest.cpp
 * @brief  TypeTable class tester.
 * @author zer0
 * @date   2018-01-16
 */

#include <gtest/gtest.h>
#include <libtbag/type/TypeTable.hpp>

using namespace libtbag;
using namespace libtbag::type;

struct CustomTestType { /* EMPTY. */ };

template <typename T>
TypeTable coverageOnly()
{
    TypeInfo<T>::name();
    TypeInfo<T>::isArithmetic();
    TypeInfo<T>::isSpecialized();
    TypeInfo<T>::isPointer();
    TypeInfo<T>::size();
    TypeInfo<T>::index();
    TypeInfo<T>::maximum();
    TypeInfo<T>::minimum();
    TypeInfo<T>::lowest();
    TypeInfo<T>::epsilon();
    TypeInfo<T>::round_error();
    TypeInfo<T>::infinity();
    TypeInfo<T>::quiet_NaN();
    TypeInfo<T>::signaling_NaN();
    TypeInfo<T>::denorm_min();
    return TypeInfo<T>::table();
}

TEST(TypeTableTest, Default)
{
    ASSERT_EQ(TypeTable::TT_UNKNOWN, coverageOnly<    CustomTestType>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<              bool>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<              char>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<       signed char>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<     unsigned char>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<           wchar_t>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<          char16_t>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<          char32_t>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<             short>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<    unsigned short>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<               int>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<      unsigned int>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<              long>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<     unsigned long>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<         long long>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<unsigned long long>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<             float>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<            double>());
    ASSERT_NE(TypeTable::TT_UNKNOWN, coverageOnly<       long double>());
}

TEST(TypeTableTest, IntegerTypes)
{
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly<  int8_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly< uint8_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly< int16_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly<uint16_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly< int32_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly<uint32_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly< int64_t>());
    EXPECT_NE(TypeTable::TT_UNKNOWN, coverageOnly<uint64_t>());
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

