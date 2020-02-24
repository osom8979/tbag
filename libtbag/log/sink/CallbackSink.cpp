/**
 * @file   CallbackSink.cpp
 * @brief  CallbackSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/CallbackSink.hpp>
#include <libtbag/debug/Assert.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

CallbackSink::CallbackSink(WriteCallback const & wcb, FlushCallback const & fcb, void * user)
        : WRITE_CALLBACK(wcb), FLUSH_CALLBACK(fcb), USER_DATA(user)
{
    assert(WRITE_CALLBACK != nullptr);
}

CallbackSink::CallbackSink(std::string const & UNUSED_PARAM(arguments))
        : CallbackSink(nullptr, nullptr, nullptr)
{
    // EMPTY.
}

CallbackSink::~CallbackSink()
{
    // EMPTY.
}

bool CallbackSink::write(int level, char const * message, int size)
{
    return WRITE_CALLBACK(level, message, size, USER_DATA);
}

bool CallbackSink::isRequiredFlush()
{
    return true;
}

void CallbackSink::flush()
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

