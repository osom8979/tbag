/**
 * @file   RotateFileSink.cpp
 * @brief  RotateFileSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/Err.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

RotateFileSink::RotateFileSink(std::string const & arguments)
        : _rotate(arguments)
{
    // EMPTY.
}

RotateFileSink::~RotateFileSink()
{
    // EMPTY.
}

bool RotateFileSink::write(char const * message, int size)
{
    return _rotate.write(message, size) == E_SUCCESS;
}

void RotateFileSink::flush()
{
    _rotate.flush();
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

