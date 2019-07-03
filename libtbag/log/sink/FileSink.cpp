/**
 * @file   FileSink.cpp
 * @brief  FileSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/FileSink.hpp>
#include <exception>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

FileSink::FileSink(char const * path)
        :  _file(::fopen(path, "a+"))
{
    if (_file == nullptr) {
        throw std::bad_alloc();
    }
}

FileSink::FileSink(std::string const & path)
        : FileSink(path.c_str())
{
    // EMPTY.
}

FileSink::~FileSink()
{
    ::fclose(_file);
}

bool FileSink::write(char const * message, int size)
{
    return ::fwrite(message, size, 1, _file) == 1;
}

void FileSink::flush()
{
    ::fflush(_file);
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

