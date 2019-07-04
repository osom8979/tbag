/**
 * @file   StringQueueSink.cpp
 * @brief  StringQueueSink class implementation.
 * @author zer0
 * @date   2019-07-03
 */

#include <libtbag/log/sink/StringQueueSink.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

StringQueueSink::StringQueueSink(std::size_t max_size)
        : MAX_SIZE(max_size == 0U ? INFINITY_SIZE : max_size)
{
    assert(MAX_SIZE >= 1);
}

StringQueueSink::~StringQueueSink()
{
    // EMPTY.
}

bool StringQueueSink::write(char const * message, int size)
{
    while (_deque.size() >= MAX_SIZE) {
        _deque.pop_front();
    }
    _deque.push_back(std::string(message, message + size));
    return true;
}

void StringQueueSink::flush()
{
    // EMPTY.
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

