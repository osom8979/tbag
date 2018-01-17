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
bool printTypeInfo(std::string const & prefix)
{
    std::cout << prefix << ": "
              << TypeInfo<T>::name()          << "/"
              << TypeInfo<T>::isArithmetic()  << "/"
              << TypeInfo<T>::isSpecialized() << "/"
              << TypeInfo<T>::size()          << "/"
              << TypeInfo<T>::index()         << "/"
              << TypeInfo<T>::maximum()       << "/"
              << TypeInfo<T>::minimum()       << "/"
              << TypeInfo<T>::lowest()        << "/"
              << TypeInfo<T>::epsilon()       << "/"
              << TypeInfo<T>::round_error()   << "/"
              << TypeInfo<T>::infinity()      << "/"
              << TypeInfo<T>::quiet_NaN()     << "/"
              << TypeInfo<T>::signaling_NaN() << "/"
              << TypeInfo<T>::denorm_min()    << "/"
              << std::endl;
    return true;
}

TEST(TypeTableTest, Default)
{
    ASSERT_TRUE(printTypeInfo<    CustomTestType>("CustomTestType"));
    ASSERT_TRUE(printTypeInfo<              bool>("bool"));
    ASSERT_TRUE(printTypeInfo<              char>("char"));
    ASSERT_TRUE(printTypeInfo<       signed char>("signed char"));
    ASSERT_TRUE(printTypeInfo<     unsigned char>("unsigned char"));
    ASSERT_TRUE(printTypeInfo<           wchar_t>("wchar_t"));
    ASSERT_TRUE(printTypeInfo<          char16_t>("char16_t"));
    ASSERT_TRUE(printTypeInfo<          char32_t>("char32_t"));
    ASSERT_TRUE(printTypeInfo<             short>("short"));
    ASSERT_TRUE(printTypeInfo<    unsigned short>("unsigned short"));
    ASSERT_TRUE(printTypeInfo<               int>("int"));
    ASSERT_TRUE(printTypeInfo<      unsigned int>("unsigned int"));
    ASSERT_TRUE(printTypeInfo<              long>("long"));
    ASSERT_TRUE(printTypeInfo<     unsigned long>("unsigned long"));
    ASSERT_TRUE(printTypeInfo<         long long>("long long"));
    ASSERT_TRUE(printTypeInfo<unsigned long long>("unsigned long long"));
    ASSERT_TRUE(printTypeInfo<             float>("float"));
    ASSERT_TRUE(printTypeInfo<            double>("double"));
    ASSERT_TRUE(printTypeInfo<       long double>("long double"));
    ASSERT_TRUE(printTypeInfo<             void*>("void*"));
    ASSERT_TRUE(printTypeInfo<            int8_t>("int8_t"));
    ASSERT_TRUE(printTypeInfo<           uint8_t>("uint8_t"));
    ASSERT_TRUE(printTypeInfo<           int16_t>("int16_t"));
    ASSERT_TRUE(printTypeInfo<          uint16_t>("uint16_t"));
    ASSERT_TRUE(printTypeInfo<           int32_t>("int32_t"));
    ASSERT_TRUE(printTypeInfo<          uint32_t>("uint32_t"));
    ASSERT_TRUE(printTypeInfo<           int64_t>("int64_t"));
    ASSERT_TRUE(printTypeInfo<          uint64_t>("uint64_t"));
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
    ASSERT_STREQ( "VPOINT", getTypeName(TypeTable::TT_VPOINT ));
}

TEST(TypeTableTest, GetTypeTable)
{
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<               int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<        signed int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<         const int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<      volatile int>());
    ASSERT_EQ(TypeTable::TT_INT, getTypeTable<const volatile int>());
}

