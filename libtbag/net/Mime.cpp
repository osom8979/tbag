/**
 * @file   Mime.cpp
 * @brief  Mime class implementation.
 * @author zer0
 * @date   2020-05-11
 */

#include <libtbag/net/Mime.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <sstream>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace net {

Mime::Mime()
{
    // EMPTY.
}

Mime::Mime(std::string const & mime)
{
    fromString(mime);
}

Mime::Mime(std::string const & t, std::string const & s)
        : type(t), subtype(s)
{
    // EMPTY.
}

Mime::Mime(std::string const & t, std::string const & s, Parameters const & p)
        : type(t), subtype(s), parameters(p)
{
    // EMPTY.
}

Mime::Mime(Mime const & obj)
        : type(obj.type),
          subtype(obj.subtype),
          parameters(obj.parameters)
{
    // EMPTY.
}

Mime::Mime(Mime && obj) TBAG_NOEXCEPT
        : type(std::move(obj.type)),
          subtype(std::move(obj.subtype)),
          parameters(std::move(obj.parameters))
{
    // EMPTY.
}

Mime::~Mime()
{
    // EMPTY.
}

Mime & Mime::operator =(Mime const & obj)
{
    if (this != &obj) {
        type = obj.type;
        subtype = obj.subtype;
        parameters = obj.parameters;
    }
    return *this;
}

Mime & Mime::operator =(Mime && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        type = std::move(obj.type);
        subtype = std::move(obj.subtype);
        parameters = std::move(obj.parameters);
    }
    return *this;
}

void Mime::swap(Mime & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        type.swap(obj.type);
        subtype.swap(obj.subtype);
        parameters.swap(obj.parameters);
    }
}

bool Mime::isCompatible(Mime const & obj) const
{
    using namespace libtbag::string;
    auto const self_type = lower(trim(type));
    auto const comp_type = lower(trim(obj.type));
    if (self_type == ALL) {
        return true;
    }
    if (comp_type == ALL) {
        return true;
    }
    if (self_type != comp_type) {
        return false;
    }

    auto const self_subtype = lower(trim(subtype));
    auto const comp_subtype = lower(trim(obj.subtype));
    if (self_subtype == ALL) {
        return true;
    }
    if (comp_subtype == ALL) {
        return true;
    }
    if (self_subtype != comp_subtype) {
        return false;
    }
    return true;
}

bool Mime::operator ==(Mime const & obj) const
{
    return isCompatible(obj);
}

bool Mime::operator !=(Mime const & obj) const
{
    return !isCompatible(obj);
}

void Mime::fromString(std::string const & mime)
{
    using namespace libtbag::string;
    auto const tokens = splitTokens(mime, PARAMETER_DELIMITER);
    if (tokens.empty()) {
        type.clear();
        subtype.clear();
        parameters.clear();
        return;
    }

    auto const types = divideTwo(tokens[0], TYPE_DELIMITER);
    type = trim(types.first);
    subtype = trim(types.second);

    auto itr = tokens.begin()+1;
    auto const end = tokens.end();
    for (; itr != end; ++itr) {
        auto const key_value = divideTwo(*itr, PARAMETER_KEY_VALUE_SPLITTER);
        parameters.emplace(trim(key_value.first), trim(key_value.second));
    }
}

std::string Mime::toString() const
{
    std::stringstream ss;
    if (type.empty()) {
        return {};
    }
    ss << type;
    if (!subtype.empty()) {
        ss << TYPE_DELIMITER << subtype;
    }
    for (auto const & param : parameters) {
        ss << PARAMETER_DELIMITER << param.first
           << PARAMETER_KEY_VALUE_SPLITTER << param.second;
    }
    return ss.str();
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

