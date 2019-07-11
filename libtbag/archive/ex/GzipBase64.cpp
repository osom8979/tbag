/**
 * @file   GzipBase64.cpp
 * @brief  GzipBase64 class implementation.
 * @author zer0
 * @date   2019-07-11
 */

#include <libtbag/archive/ex/GzipBase64.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {
namespace ex      {

bool encodeGzipBase64(char const * input, std::size_t size, std::string & output)
{
    return false;
}

bool encodeGzipBase64(util::Buffer const & input, std::string & output)
{
    return false;
}

bool decodeGzipBase64(std::string  const & input, util::Buffer & output)
{
    return false;
}

bool encodeGzipBase64(std::string const & input, std::string & output)
{
    return false;
}

bool decodeGzipBase64(std::string const & input, std::string & output)
{
    return false;
}

bool encodeGzipBase64FromFile(std::string const & input_file_path, std::string & output)
{
    return false;
}

bool decodeGzipBase64FromFile(std::string const & input_file_path, util::Buffer & output)
{
    return false;
}

bool encodeGzipBase64ToFile(util::Buffer const & input, std::string const & output_file_path)
{
    return false;
}

bool decodeGzipBase64ToFile(std::string  const & input, std::string const & output_file_path)
{
    return false;
}

bool encodeGzipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path)
{
    return false;
}

bool decodeGzipBase64FileToFile(std::string const & input_file_path, std::string const & output_file_path)
{
    return false;
}

} // namespace ex
} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

