/**
 * @file   StringQueueSink.cpp
 * @brief  StringQueueSink class implementation.
 * @author zer0
 * @date   2019-07-03
 */

#include <libtbag/log/sink/StringQueueSink.hpp>
#include <libtbag/string/StringUtils.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

std::size_t StringQueueSink::parseArguments(std::string const & arguments)
{
    if (arguments.empty()) {
        return DEFAULT_SIZE;
    }
    using namespace libtbag::string;
    auto const lower_arguments = lower(trim(arguments));
    if (/**/lower_arguments == INFINITY_ARGUMENTS_01 ||
            lower_arguments == INFINITY_ARGUMENTS_02 ||
            lower_arguments == INFINITY_ARGUMENTS_03) {
        return INFINITY_SIZE;
    }
    return libtbag::string::toValue<int>(arguments, DEFAULT_SIZE);
}

StringQueueSink::StringQueueSink(std::size_t max_size)
        : MAX_SIZE(max_size == 0U ? INFINITY_SIZE : max_size)
{
    assert(MAX_SIZE >= 1);
}

StringQueueSink::StringQueueSink(std::string const & arguments)
        : StringQueueSink(parseArguments(arguments))
{
    // EMPTY.
}

StringQueueSink::~StringQueueSink()
{
    // EMPTY.
}

bool StringQueueSink::write(int level, char const * message, int size)
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

