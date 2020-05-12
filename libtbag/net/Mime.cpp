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

Mime::Mime(std::string const & t, std::string const & s, std::string const & p)
        : type(t), subtype(s), parameter(p)
{
    // EMPTY.
}

Mime::Mime(Mime const & obj)
        : type(obj.type),
          subtype(obj.subtype),
          parameter(obj.parameter)
{
    // EMPTY.
}

Mime::Mime(Mime && obj) TBAG_NOEXCEPT
        : type(std::move(obj.type)),
          subtype(std::move(obj.subtype)),
          parameter(std::move(obj.parameter))
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
        parameter = obj.parameter;
    }
    return *this;
}

Mime & Mime::operator =(Mime && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        type = std::move(obj.type);
        subtype = std::move(obj.subtype);
        parameter = std::move(obj.parameter);
    }
    return *this;
}

void Mime::swap(Mime & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        type.swap(obj.type);
        subtype.swap(obj.subtype);
        parameter.swap(obj.parameter);
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
    auto type_subtype = divideTwo(mime, TYPE_DELIMITER);
    type = std::move(type_subtype.first);

    if (type_subtype.second.empty()) {
        subtype.clear();
        parameter.clear();
    } else {
        auto subtype_parameter = divideTwo(type_subtype.second, PARAMETER_DELIMITER);
        subtype = std::move(subtype_parameter.first);
        parameter = std::move(subtype_parameter.second);
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
    if (!parameter.empty()) {
        ss << PARAMETER_DELIMITER << parameter;
    }
    return ss.str();
}

} // namespace net

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

