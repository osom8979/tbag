/**
 * @file   Aes128Test.cpp
 * @brief  Aes128 class tester.
 * @author zer0
 * @date   2016-11-18
 */

#include <gtest/gtest.h>
#include <libtbag/encrypt/Aes128.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <string>
#include <fstream>

using namespace libtbag;
using namespace libtbag::encrypt;

TEST(Aes128Test, EncodeAndDecode_01)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(16, TEST_BODY.size());
    ASSERT_EQ(16, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16, Aes128::decrypt(decrypt, key, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(Aes128Test, EncodeAndDecode_02)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef+";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(17, TEST_BODY.size());
    ASSERT_EQ(16 * 2, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16 * 2, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16 * 2, Aes128::decrypt(decrypt, key, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_EQ(TEST_BODY, result);
}

TEST(Aes128Test, EncodeAndDecode_Fail)
{
    Aes128::Key key;
    key.fill(0x00);

    char const * const CONTENT = "0123456789abcdef";
    std::string const TEST_BODY = CONTENT;
    ASSERT_EQ(16, TEST_BODY.size());
    ASSERT_EQ(16, Aes128::predictSize(TEST_BODY.size()));

    std::vector<uint8_t> input;
    input.assign(TEST_BODY.begin(), TEST_BODY.end());

    std::vector<uint8_t> encrypt;
    ASSERT_EQ(16, Aes128::encrypt(encrypt, key, &input[0], input.size()));

    Aes128::Key key2;
    key2.fill(0x00);
    key2[0] = 1;

    std::vector<uint8_t> decrypt;
    ASSERT_EQ(16, Aes128::decrypt(decrypt, key2, &encrypt[0], encrypt.size()));

    ASSERT_NE(input, encrypt);
    ASSERT_NE(encrypt, decrypt);

    std::string result;
    result.assign(decrypt.begin(), decrypt.begin() + TEST_BODY.size());
    ASSERT_NE(TEST_BODY, result);
}

TEST(Aes128Test, EncodeAndDecode_File)
{
    std::string const TEST_FILENAME     = "__tester_encrypt_aes128test_encodeanddecode_file__";
    std::string const TEST_ENC_FILENAME = "__tester_encrypt_aes128test_encodeanddecode_file__enc__";
    std::string const TEST_DEC_FILENAME = "__tester_encrypt_aes128test_encodeanddecode_file__dec__";
    std::string const TEST_CONTENT      = "0123456789";

    using Path = libtbag::filesystem::Path;

    Path const TEST_PATH = Path(TEST_FILENAME).getCanonical();
    Path const  ENC_PATH = Path(TEST_ENC_FILENAME).getCanonical();
    Path const  DEC_PATH = Path(TEST_DEC_FILENAME).getCanonical();

    if (TEST_PATH.exists()) { TEST_PATH.remove(); }
    if ( ENC_PATH.exists()) {  ENC_PATH.remove(); }
    if ( DEC_PATH.exists()) {  DEC_PATH.remove(); }

    ASSERT_FALSE(TEST_PATH.exists());
    ASSERT_FALSE(ENC_PATH.exists());
    ASSERT_FALSE(DEC_PATH.exists());

    // Create test file.
    ASSERT_EQ(TEST_CONTENT.size(), filesystem::createSimpleTextFile(TEST_PATH.getString(), &TEST_CONTENT[0], TEST_CONTENT.size()));

    Aes128::Key key;
    key.fill(0x00);

    ASSERT_TRUE(Aes128::encryptFile(ENC_PATH, key, TEST_PATH));
    ASSERT_TRUE(ENC_PATH.isRegularFile());

    ASSERT_TRUE(Aes128::decryptFile(DEC_PATH, key, ENC_PATH));
    ASSERT_TRUE(DEC_PATH.isRegularFile());

    std::string result;

    {   // Read result file.
        std::size_t const DEC_FILE_SIZE = DEC_PATH.size();
        result.resize(DEC_FILE_SIZE);

        std::ifstream f(DEC_PATH.getString(), std::ios_base::binary);
        f.read(&result[0], DEC_FILE_SIZE);
        f.close();
    }

    ASSERT_EQ(TEST_CONTENT, result);

    if (TEST_PATH.exists()) { TEST_PATH.remove(); }
    if ( ENC_PATH.exists()) {  ENC_PATH.remove(); }
    if ( DEC_PATH.exists()) {  DEC_PATH.remove(); }
}

TEST(Aes128Test, EncodeAndDecode_Dir)
{
    std::string const TEST_DIR     = "__tester_encrypt_aes128test_encodeanddecode_dir__";
    std::string const TEST_ENC_DIR = "__tester_encrypt_aes128test_encodeanddecode_dir__enc__";
    std::string const TEST_DEC_DIR = "__tester_encrypt_aes128test_encodeanddecode_dir__dec__";
    std::string const TEST_CONTENT = "0123456789";

    std::string const TEST_SUB_DIR  = "dir";
    std::string const TEST_SUB_FILE = "file";

    using Path = libtbag::filesystem::Path;

    Path const TEST_PATH = Path(TEST_DIR).getCanonical();
    Path const  ENC_PATH = Path(TEST_ENC_DIR).getCanonical();
    Path const  DEC_PATH = Path(TEST_DEC_DIR).getCanonical();

    // @formatter:off
    if (TEST_PATH.exists()) { TEST_PATH.removeDirWithRecursive(); }
    if ( ENC_PATH.exists()) {  ENC_PATH.removeDirWithRecursive(); }
    if ( DEC_PATH.exists()) {  DEC_PATH.removeDirWithRecursive(); }
    // @formatter:on

    Path const TEST_SUB_PATH(TEST_PATH / TEST_SUB_DIR);
    TEST_SUB_PATH.getCanonical().createDirWithRecursive();

    ASSERT_TRUE(TEST_SUB_PATH.isDirectory());
    {   // Create test file.
        std::ofstream f((TEST_SUB_PATH / TEST_SUB_FILE).getString(), std::ios_base::binary);
        f.write(&TEST_CONTENT[0], TEST_CONTENT.size());
        f.close();
    }

    Aes128::Key key;
    key.fill(0x00);

    ASSERT_TRUE(Aes128::encryptDir(ENC_PATH, key, TEST_PATH));
    ASSERT_TRUE(Aes128::decryptDir(DEC_PATH, key, ENC_PATH));

    Path const TEST_DEC_PATH(DEC_PATH / TEST_SUB_DIR / TEST_SUB_FILE);
    ASSERT_TRUE(TEST_DEC_PATH.isRegularFile());

    std::string result;

    {   // Read result file.
        std::size_t const DEC_FILE_SIZE = TEST_DEC_PATH.size();
        result.resize(DEC_FILE_SIZE);

        std::ifstream f(TEST_DEC_PATH.getString(), std::ios_base::binary);
        f.read(&result[0], DEC_FILE_SIZE);
        f.close();
    }

    ASSERT_EQ(TEST_CONTENT, result);

    // @formatter:off
    if (TEST_PATH.exists()) { TEST_PATH.removeDirWithRecursive(); }
    if ( ENC_PATH.exists()) {  ENC_PATH.removeDirWithRecursive(); }
    if ( DEC_PATH.exists()) {  DEC_PATH.removeDirWithRecursive(); }
    // @formatter:on
}

