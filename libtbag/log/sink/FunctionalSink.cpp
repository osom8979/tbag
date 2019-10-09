/**
 * @file   FunctionalSink.cpp
 * @brief  FunctionalSink class implementation.
 * @author zer0
 * @date   2019-07-03
 */

#include <libtbag/log/sink/FunctionalSink.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

FunctionalSink::FunctionalSink(WriteCallback const & wcb, FlushCallback const & fcb, void * user)
        : WRITE_CALLBACK(wcb), FLUSH_CALLBACK(fcb), USER_DATA(user)
{
    assert(WRITE_CALLBACK);
}

FunctionalSink::FunctionalSink(std::string const & UNUSED_PARAM(arguments))
        : FunctionalSink(nullptr, nullptr, nullptr)
{
    // EMPTY.
}

FunctionalSink::~FunctionalSink()
{
    // EMPTY.
}

bool FunctionalSink::write(int level, char const * message, int size)
{
    return WRITE_CALLBACK(level, message, size, USER_DATA);
}

void FunctionalSink::flush()
{
    if (FLUSH_CALLBACK) {
        FLUSH_CALLBACK(USER_DATA);
    }
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

