/**
 * @file   DurationString.cpp
 * @brief  DurationString class implementation.
 * @author zer0
 * @date   2019-07-15
 */

#include <libtbag/time/DurationString.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cctype>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

template <typename DefaultDurationT>
static std::size_t __to_chrono_duration(std::string const & str)
{
    auto const lower_str = libtbag::string::trim(libtbag::string::lower(str));
    auto const size = lower_str.size();

    std::size_t i = 0;
    for (; i < size; ++i) {
        if (!std::isdigit(lower_str[i])) {
            break;
        }
    }
    // Skip space;
    for (; i < size; ++i) {
        if (lower_str[i] != ' ') {
            break;
        }
    }

    auto const value = libtbag::string::toValue<std::size_t>(lower_str.substr(0, i));
    auto const suffix = lower_str.substr(i);
    auto const suffix_size = suffix.size();

    using namespace std::chrono;
    if (suffix_size <= 0) {
        return value;
    }

    assert(suffix_size >= 1);
    if (suffix[0] == 'h') {
        return duration_cast<DefaultDurationT>(hours(value)).count();
    } else if (suffix[0] == 's') {
        return duration_cast<DefaultDurationT>(seconds(value)).count();
    } else if (suffix[0] == 'n') {
        return duration_cast<DefaultDurationT>(nanoseconds(value)).count();
    } else if (suffix[0] == 'm') {
        if (suffix_size >= 2 && suffix[1] == 'i') {
            if (suffix_size >= 3) {
                if (suffix[2] == 'c') { // mic
                    return duration_cast<DefaultDurationT>(microseconds(value)).count();
                } else if (suffix[2] == 'l') { // mil
                    return duration_cast<DefaultDurationT>(milliseconds(value)).count();
                } else if (suffix[2] == 'n') { // min
                    return duration_cast<DefaultDurationT>(minutes(value)).count();
                }
            }
        }
    }

    return value;
}

std::size_t toNanoseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::nanoseconds>(str);
}

std::size_t toMicroseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::microseconds>(str);
}

std::size_t toMilliseconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::milliseconds>(str);
}

std::size_t toSeconds(std::string const & str)
{
    return __to_chrono_duration<std::chrono::seconds>(str);
}

std::size_t toMinutes(std::string const & str)
{
    return __to_chrono_duration<std::chrono::minutes>(str);
}

std::size_t toHours(std::string const & str)
{
    return __to_chrono_duration<std::chrono::hours>(str);
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

