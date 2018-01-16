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

