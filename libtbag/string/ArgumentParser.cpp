/**
 * @file   ArgumentParser.cpp
 * @brief  ArgumentParser class implementation.
 * @author zer0
 * @date   2019-10-09
 */

#include <libtbag/string/ArgumentParser.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

std::string ArgumentParser::DefaultFormatter::print()
{
    return {};
}

ArgumentParser::ArgumentParser()
{
    // EMPTY.
}

ArgumentParser::~ArgumentParser()
{
    // EMPTY.
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

