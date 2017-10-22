/**
 * @file   Format.cpp
 * @brief  Format class implementation.
 * @author zer0
 * @date   2017-10-21
 */

#include <libtbag/string/Format.hpp>
#include <libtbag/log/Log.hpp>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::string format(char const * f, ...)
{
    std::string result;
    TBAG_FORMAT_VA_LIST(result, f, f, DEFAULT_FORMAT_BUFFER_SIZE);
    return result;
}

std::string vformat(char const * f, std::size_t buffer_size, va_list & l)
{
    std::size_t step = 1;
    while (step <= MAX_FORMAT_RESIZE_LOOP_COUNT) {
        std::vector<char> buffer(buffer_size * step);
        int const WRITE_SIZE = std::vsnprintf(&buffer[0], buffer.size(), f, l);

        if (WRITE_SIZE <= 0) {
            return std::string(); // empty or error.
        } else if (0 < COMPARE_AND(static_cast<std::size_t>(WRITE_SIZE)) < buffer.size()) {
            return std::string(buffer.data(), buffer.data() + WRITE_SIZE);
        }
        ++step;
    }
    return std::string();
}

// ----------------------
// Format implementation.
// ----------------------

Format::Format() : _escape(DEFAULT_ESCAPE)
{
    // EMPTY.
}

Format::Format(char escape) : _escape(escape)
{
    // EMPTY.
}

Format::Format(Format const & obj) : Format()
{
    (*this) = obj;
}

Format::Format(Format && obj) : Format()
{
    (*this) = std::move(obj);
}

Format::~Format()
{
    // EMPTY.
}

Format & Format::operator =(Format const & obj)
{
    if (this != &obj) {
        _escape = obj._escape;
    }
    return *this;
}

Format & Format::operator =(Format && obj)
{
    if (this != &obj) {
        std::swap(_escape, obj._escape);
    }
    return *this;
}

std::string Format::convert(std::string const & source) const
{
    std::stringstream ss;
    FormatMode mode = FormatMode::FM_NORMAL;
    for (auto & cursor : source) {
        switch (mode) {
            case FormatMode::FM_NORMAL:
                if (cursor == _escape) {
                    mode = FormatMode::FM_ESCAPE; // First escape.
                } else {
                    ss << cursor; // Normal character.
                }
                break;

            case FormatMode::FM_ESCAPE:
                if (cursor == _escape) {
                    ss << _escape; // Double escape.
                } else {
                    ss << onEscape(cursor); // Special character.
                }
                mode = FormatMode::FM_NORMAL;
                break;

            default:
                break;
        }
    }
    return ss.str();
}

std::string Format::onEscape(char input) const
{
    return std::string(1, input);
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

