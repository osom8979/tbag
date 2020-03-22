/**
 * @file   InputUtils.cpp
 * @brief  InputUtils class implementation.
 * @author zer0
 * @date   2020-03-22
 */

#include <libtbag/io/InputUtils.hpp>

#include <cassert>
#include <cstring>
#include <cstdio>

#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace io {

std::string readInput(int buffer_size)
{
    using buffer_type = std::vector<char>;
    using buffer_value = buffer_type::value_type;
    buffer_type buffer(buffer_size);
    std::string result;
    auto const buffer_bytes = sizeof(buffer_value) * buffer.size() - 1/*LAST NULL*/;
    assert(buffer_bytes >= 1);
    while (fgets(buffer.data(), buffer_bytes, stdin) != nullptr) {
        result += buffer.data();
        memset(buffer.data(), 0x00, buffer_bytes);
    }
    return result;
}

} // namespace io

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

