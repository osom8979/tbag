/**
 * @file   TlsReaderTest.cpp
 * @brief  TlsReader class tester.
 * @author zer0
 * @date   2019-01-27
 */

#include <gtest/gtest.h>
#include <libtbag/crypto/Tls.hpp>
#include <libtbag/util/BufferInfo.hpp>

using namespace libtbag;
using namespace libtbag::crypto;

struct TlsReaderTest : public TlsReaderInterface
{
    using mem_crt = TlsReader::mem_crt;
    using Buffer = libtbag::util::Buffer;

    TlsReader tls;

    Buffer write_buffer;
    Buffer read_buffer;

    TlsReaderTest() : tls(this)
    { /* EMPTY. */ }

    TlsReaderTest(mem_crt) : tls(this, mem_crt{})
    { /* EMPTY. */ }

    ~TlsReaderTest()
    { /* EMPTY. */ }

    virtual Err onWrite(char const * buffer, std::size_t size) override
    {
        write_buffer.assign(buffer, buffer + size);
        return E_SUCCESS;
    }

    virtual Err onRead(char const * buffer, std::size_t size) override
    {
        read_buffer.assign(buffer, buffer + size);
        return E_SUCCESS;
    }
};

TEST(TlsReaderTest, Default)
{
    TlsReaderTest client;
    TlsReaderTest server(TlsReaderTest::mem_crt{});

//    // Client connect
//    ASSERT_EQ(E_SUCCESS, client.tls.connect());
//    ASSERT_FALSE(client.write_buffer.empty());
//    ASSERT_FALSE(client.tls.isFinished());
//
//    // Server accept.
//    ASSERT_EQ(E_SUCCESS, server.tls.accept());
//    ASSERT_TRUE(client.write_buffer.empty());
//    ASSERT_FALSE(server.tls.isFinished());
//
//    auto sendClientToServer = [&]() -> Err {
//        auto const CODE = server.tls.parse(client.write_buffer.data(), client.write_buffer.size());
//        client.write_buffer.clear();
//        return CODE;
//    };
//    auto sendServerToClient = [&]() -> Err {
//        auto const CODE = client.tls.parse(server.write_buffer.data(), server.write_buffer.size());
//        server.write_buffer.clear();
//        return CODE;
//    };
//
//    // Client ~~ HELLO ~> Server
//    ASSERT_EQ(E_SUCCESS, sendClientToServer());
//    ASSERT_FALSE(server.write_buffer.empty());
//    ASSERT_FALSE(server.tls.isFinished());
//
//    // Server ~~ HELLO ~> Client
//    ASSERT_EQ(E_SUCCESS, sendServerToClient());
//    ASSERT_FALSE(client.write_buffer.empty());
//    ASSERT_FALSE(client.tls.isFinished());
//
//    // Client ~~ EXCHANGE ~> Server
//    ASSERT_EQ(E_SUCCESS, sendClientToServer());
//    ASSERT_TRUE(server.write_buffer.empty());
//    ASSERT_TRUE(server.tls.isFinished());

//    // Server[FINISH] ~~ FINISH ~> Client[FINISH]
//    ASSERT_EQ(E_SUCCESS, sendServerToClient());
//    ASSERT_TRUE(client.write_buffer.empty());
//    ASSERT_TRUE(client.tls.isFinished());

//    // Client ~~ FINISH ~> Server
//    ASSERT_EQ(E_SUCCESS, sendClientToServer());
//    ASSERT_TRUE(server.write_buffer.empty());
//    ASSERT_TRUE(server.tls.isFinished());
}

