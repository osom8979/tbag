/**
 * @file   Aes128.cpp
 * @brief  Aes128 class implementation.
 * @author zer0
 * @date   2016-11-18
 */

#include <libtbag/encrypt/Aes128.hpp>
#include <libtbag/3rd/tinyaes128c/aes.h>
#include <libtbag/filesystem/Path.hpp>

#include <cassert>
#include <cstring>

#include <fstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace encrypt {

Aes128::Aes128()
{
    // EMPTY.
}

Aes128::~Aes128()
{
    // EMPTY.
}

std::size_t Aes128::coding(Buffer & output, Key const & key, uint8_t const * input, std::size_t size, bool encode)
{
    uint8_t  in[KEY_SIZE] = {0,};
    uint8_t out[KEY_SIZE] = {0,};

    std::size_t read_index = 0;
    std::size_t avail_in   = 0;
    int eof = false;

    output.clear();

    while (eof == false) {
        if (size - read_index > KEY_SIZE) {
            avail_in = KEY_SIZE;
            eof = false;
        } else {
            avail_in = size - read_index;
            eof = true; // EOF.
        }

        memset(in, 0x00, sizeof(in));
        memcpy(in, input + read_index, avail_in);
        read_index += avail_in;

        if (encode) {
            AES128_ECB_encrypt(in, &key[0], out);
        } else {
            AES128_ECB_decrypt(in, &key[0], out);
        }
        output.insert(output.end(), out, out + KEY_SIZE);
    }

    return output.size();
}

std::size_t Aes128::predictSize(std::size_t size)
{
    if (size % KEY_SIZE == 0) {
        return size;
    }
    return size + KEY_SIZE - (size % KEY_SIZE);
}

std::size_t Aes128::encrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size)
{
    return coding(output, key, input, size, AES_ENCODE);
}

std::size_t Aes128::decrypt(Buffer & output, Key const & key, uint8_t const * input, std::size_t size)
{
    return coding(output, key, input, size, AES_DECODE);
}

bool Aes128::encryptFile(std::string const & output, Key const & key, std::string const & input)
{
    using Path = filesystem::Path;
    Path  input_path(input);
    Path output_path(output);

    if (output_path.exists() == true || input_path.exists() == false) {
        return false;
    }
    if (output_path.getCanonical().getParent().isDirectory() == false) {
        if (output_path.getCanonical().getParent().createDirWithRecursive() == false) {
            return false;
        }
    }

    std::size_t const  INPUT_SIZE = input_path.size();
    std::size_t const REMOVE_SIZE = predictSize(input_path.size()) - INPUT_SIZE;

    assert(REMOVE_SIZE >= 0);
    assert(REMOVE_SIZE <  KEY_SIZE);
    char header = static_cast<char>(REMOVE_SIZE);

    Buffer  input_buffer;
    Buffer output_buffer;

    std::ifstream input_file(input, std::ios_base::binary);
    if (input_file.eof() == false && input_file.fail() == false) {
        input_buffer.resize(INPUT_SIZE);
        input_file.seekg(0, std::ios_base::beg);
        input_file.read((char*)&input_buffer[0], INPUT_SIZE);
    } else {
        input_file.close();
        return false;
    }

    std::size_t const OUTPUT_SIZE = encrypt(output_buffer, key, &input_buffer[0], input_buffer.size());
    if (OUTPUT_SIZE == 0) {
        return false;
    }

    std::ofstream output_file(output, std::ios_base::binary);
    output_file.write(&header, 1);
    output_file.write((char*)&output_buffer[0], output_buffer.size());
    output_file.close();

    return true;
}

bool Aes128::decryptFile(std::string const & output, Key const & key, std::string const & input)
{
    using Path = filesystem::Path;
    Path  input_path(input);
    Path output_path(output);

    if (output_path.exists() == true || input_path.exists() == false) {
        return false;
    }
    if (output_path.getCanonical().getParent().isDirectory() == false) {
        if (output_path.getCanonical().getParent().createDirWithRecursive() == false) {
            return false;
        }
    }

    std::size_t const INPUT_SIZE = input_path.size();

    Buffer  input_buffer;
    Buffer output_buffer;

    char header = 0;

    std::ifstream input_file(input, std::ios_base::binary);
    if (input_file.eof() == false && input_file.fail() == false) {
        input_buffer.resize(INPUT_SIZE - 1);
        input_file.seekg(0, std::ios_base::beg);
        input_file.read(&header, 1);
        input_file.read((char*)&input_buffer[0], INPUT_SIZE - 1);
    } else {
        input_file.close();
        return false;
    }

    assert(header >= 0);
    assert(header <  KEY_SIZE);
    std::size_t const REMOVE_SIZE = static_cast<std::size_t>(header);

    std::size_t const OUTPUT_SIZE = decrypt(output_buffer, key, &input_buffer[0], input_buffer.size());
    if (OUTPUT_SIZE == 0) {
        return false;
    }

    std::ofstream output_file(output, std::ios_base::binary);
    output_file.write((char*)&output_buffer[0], output_buffer.size() - REMOVE_SIZE);
    output_file.close();

    return true;
}

} // namespace encrypt

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

