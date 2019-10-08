/**
 * @file   StackTraceTest.cpp
 * @brief  StackTrace class tester.
 * @author zer0
 * @date   2017-05-28
 */

#include <gtest/gtest.h>
#include <libtbag/debug/st/ExecInfoStackTrace.hpp>
#include <libtbag/debug/st/LibunwindStackTrace.hpp>
#include <libtbag/debug/st/UnwindStackTrace.hpp>
#include <libtbag/debug/st/WindbgStackTrace.hpp>
#include <libtbag/debug/StackTrace.hpp>

#include <iostream>
#include <functional>

using namespace libtbag;
using namespace libtbag::debug;

//#define PRINT_SYMBOLIZE_RESULT

TBAG_CONSTEXPR bool isPrintSymbolizeResult() TBAG_NOEXCEPT
{
#if defined(PRINT_SYMBOLIZE_RESULT)
    return true;
#else
    return false;
#endif
}

TEST(StackTraceTest, ExecInfo)
{
    if (st::isExecInfoSupport()) {
        auto stack = st::getExecInfoStackTrace();
        ASSERT_FALSE(stack.empty());
        if (isPrintSymbolizeResult() && st::isExecInfoSupport()) {
            for (auto & str : st::getExecInfoSymbolize(stack)) {
                std::cout << str << std::endl;
            }
        }
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Libunwind)
{
    if (st::isLibunwindSupport()) {
        auto stack = st::getLibunwindStackTrace();
        ASSERT_FALSE(stack.empty());
        if (isPrintSymbolizeResult() && st::isExecInfoSupport()) {
            for (auto & str : st::getExecInfoSymbolize(stack)) {
                std::cout << str << std::endl;
            }
        }
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Unwind)
{
    if (st::isUnwindSupport()) {
        auto stack = st::getUnwindStackTrace();
        ASSERT_FALSE(stack.empty());
        if (isPrintSymbolizeResult() && st::isExecInfoSupport()) {
            for (auto & str : st::getExecInfoSymbolize(stack)) {
                std::cout << str << std::endl;
            }
        }
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Windbg)
{
    if (st::isWindbgSupport()) {
        auto stack = st::getWindbgStackTrace();
        ASSERT_FALSE(stack.empty());
        if (isPrintSymbolizeResult() && st::isExecInfoSupport()) {
            for (auto & str : st::getExecInfoSymbolize(stack)) {
                std::cout << str << std::endl;
            }
        }
    } else {
        std::cout << "Skip this test.\n";
    }
}

TEST(StackTraceTest, Default)
{
    std::cout << getStackTraceString() << std::endl;
}

template <typename T>
struct TestLambda
{
    using Callback = std::function<std::string(T, std::string const &)>;

    Callback callback;

    TestLambda()
    {
        callback = std::bind(&TestLambda::test_lambda, this, std::placeholders::_1, std::placeholders::_2);
    }

    std::string test_lambda(T temp1, std::string const & temp2)
    {
        std::cout << getStackTraceString() << std::endl;
        return temp2;
    }
};

TEST(StackTraceTest, ComplexStackTrace)
{
    TestLambda<int> obj;
    obj.callback(0, std::string());
}

TEST(StackTraceTest, ClangSymbolize)
{
    char const * const TEST_STACK = "5   libtbag.0.dylib   0x000000010de59128 _ZN7testing8TestInfo3RunEv + 216";
    auto frame = st::StFrame::parseClangSymbolize(TEST_STACK);
    ASSERT_EQ(5, frame.index);
    ASSERT_EQ(0x10de59128, frame.diff());
    ASSERT_STREQ("libtbag.0.dylib", frame.name);
    ASSERT_STREQ("testing::TestInfo::Run()", frame.source);
    ASSERT_EQ(216, frame.offset);
}

TEST(StackTraceTest, GccSymbolize)
{
    char const * const TEST_STACK1 = "./tester(_Z13RUN_ALL_TESTSv+0x11) [0x5fcbfe]";
    char const * const TEST_STACK2 = "./tester(main+0x124) [0x5fc53c]";
    char const * const TEST_STACK3 = "/lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xf5) [0x7f07667e9f45]";
    char const * const TEST_STACK4 = "./tester() [0x5f9699]";
    char const * const TEST_STACK5 = "[(nil)]";

    auto frame1 = st::StFrame::parseGccSymbolize(TEST_STACK1);
    auto frame2 = st::StFrame::parseGccSymbolize(TEST_STACK2);
    auto frame3 = st::StFrame::parseGccSymbolize(TEST_STACK3);
    auto frame4 = st::StFrame::parseGccSymbolize(TEST_STACK4);
    auto frame5 = st::StFrame::parseGccSymbolize(TEST_STACK5);

    EXPECT_EQ(0, frame1.index);
    EXPECT_EQ(0x5fcbfe, frame1.diff());
    EXPECT_STREQ("./tester", frame1.name);
    EXPECT_STREQ("RUN_ALL_TESTS()", frame1.source);
    EXPECT_EQ(0x11, frame1.offset);

    EXPECT_EQ(0, frame2.index);
    EXPECT_EQ(0x5fc53c, frame2.diff());
    EXPECT_STREQ("./tester", frame2.name);
    EXPECT_STREQ("main", frame2.source);
    EXPECT_EQ(0x124, frame2.offset);

    EXPECT_EQ(0, frame3.index);
    EXPECT_EQ(0x7f07667e9f45, frame3.diff());
    EXPECT_STREQ("/lib/x86_64-linux-gnu/libc.so.6", frame3.name);
    EXPECT_STREQ("__libc_start_main", frame3.source);
    EXPECT_EQ(0xf5, frame3.offset);

    EXPECT_EQ(0, frame4.index);
    EXPECT_EQ(0x5f9699, frame4.diff());
    EXPECT_STREQ("./tester", frame4.name);
    EXPECT_STREQ("", frame4.source);
    EXPECT_EQ(0, frame4.offset);

    EXPECT_EQ(0, frame5.index);
    EXPECT_EQ(0, frame5.diff());
    EXPECT_STREQ("", frame5.name);
    EXPECT_STREQ("", frame5.source);
    EXPECT_EQ(0, frame5.offset);
}

