/**
 * @file   Format.cpp
 * @brief  Format class implementation.
 * @author zer0
 * @date   2017-10-21
 */

#include <libtbag/string/Format.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>

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
    swap(obj);
    return *this;
}

void Format::swap(Format & obj)
{
    if (this != &obj) {
        std::swap(_escape, obj._escape);
    }
}

std::string Format::convert(std::string const & source) const
{
    std::stringstream ss;
    std::size_t const SIZE = source.size();
    FormatMode mode = FormatMode::FM_NORMAL;

    char cursor = 0;
    for (std::size_t i = 0; i < SIZE; ++i) {
        cursor = source[i];
        switch (mode) {
            case FormatMode::FM_NORMAL:
                if (cursor == _escape) {
                    // First escape.
                    mode = FormatMode::FM_ESCAPE;
                } else {
                    ss << cursor; // Normal character.
                }
                break;

            case FormatMode::FM_ESCAPE:
                if (cursor == _escape) {
                    // Double escape.
                    ss << _escape;
                } else {
                    // Special character.
                    std::string output;
                    int const CONSUME_SIZE = onEscape(source, i, output);
                    if (CONSUME_SIZE >= 1) {
                        i += (CONSUME_SIZE - 1);
                        ss << output;
                    }
                }
                mode = FormatMode::FM_NORMAL;
                break;

            default:
                TBAG_INACCESSIBLE_BLOCK_ASSERT();
                break;
        }
    }

    return ss.str();
}

int Format::onEscape(std::string const & source, std::size_t index, std::string & output) const
{
    output = source[index];
    return 1;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

