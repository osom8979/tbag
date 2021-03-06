/**
 * @file   NullSink.cpp
 * @brief  NullSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/NullSink.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

NullSink::NullSink()
{
    // EMPTY.
}

NullSink::NullSink(std::string const & UNUSED_PARAM(arguments))
{
    // EMPTY.
}

NullSink::~NullSink()
{
    // EMPTY.
}

bool NullSink::write(int level, char const * message, int size)
{
    return true;
}

void NullSink::flush()
{
    // EMPTY.
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

