/**
 * @file   Bex.cpp
 * @brief  Bex class implementation.
 * @author zer0
 * @date   2018-10-23
 * @date   2018-11-07 (Rename: BagEx -> Bex)
 */

#include <libtbag/container/Bex.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <utility>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

Bex::Bex() TBAG_NOEXCEPT : _type(TypeTable::TT_UNKNOWN), _bag(nullptr), _user(nullptr)
{
    // EMPTY.
}

Bex::Bex(std::string const & content) : Bex()
{
    if (isFailure(fromString(content))) {
        throw std::bad_alloc();
    }
}

Bex::Bex(char const * content) : Bex(std::string(content == nullptr ? "" : content))
{
    // EMPTY.
}

Bex::Bex(Bex const & obj) TBAG_NOEXCEPT : Bex()
{
    (*this) = obj;
}

Bex::Bex(Bex && obj) TBAG_NOEXCEPT : Bex()
{
    (*this) = std::move(obj);
}

Bex::~Bex()
{
    clear();
}

Bex & Bex::operator =(Bex const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Bex & Bex::operator =(Bex && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

Bex & Bex::operator =(std::string const & content)
{
    fromString(content);
    return *this;
}

Bex & Bex::operator =(char const * content)
{
    fromString(content);
    return *this;
}

void Bex::copy(Bex const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _type = obj._type;
        _bag  = obj._bag;
        _user = obj._user;
    }
}

void Bex::swap(Bex & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_type, obj._type);
        _bag.swap(obj._bag);
        _user.swap(obj._user);
    }
}

void Bex::clear()
{
    _type = TypeTable::TT_UNKNOWN;
    _bag.reset();
    _user.reset();
}

Err Bex::create(TypeTable type)
{
    if (_type == type) {
        return Err::E_ALREADY;
    }

    switch (type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name:       \
        _bag.reset((FakeBag*)(new (std::nothrow) BAG_##name##_TYPE()), [](FakeBag * b){ \
            delete ((BAG_##name##_TYPE*)b); \
        }); \
        break;
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        _bag.reset();
        _type = TypeTable::TT_UNKNOWN;
        return Err::E_ILLARGS;
    }

    if (_bag) {
        _type = type;
        return Err::E_SUCCESS;
    }

    _type = TypeTable::TT_UNKNOWN;
    return Err::E_BADALLOC;
}

Err Bex::resize(unsigned i0, unsigned i1, unsigned i2, unsigned i3,
                  unsigned i4, unsigned i5, unsigned i6, unsigned i7)
{
    if (!_bag) {
        return Err::E_NREADY;
    }

    try {
        switch (_type) {
#define _TBAG_XX(name, symbol, type) \
        case TypeTable::TT_##name:   \
            ((BAG_##name##_TYPE*)_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); \
            break;
        TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
        case TypeTable::TT_UNKNOWN:
        default:
            return Err::E_ILLSTATE;
        }
        return Err::E_SUCCESS;
    } catch (...) {
        return Err::E_UNKEXCP;
    }
}

void * Bex::data()
{
    if (!_bag) {
        return nullptr;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE*)_bag.get())->data();
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return nullptr;
    }
}

void const * Bex::data() const
{
    if (!_bag) {
        return nullptr;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE const *)_bag.get())->data();
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return nullptr;
    }
}

std::size_t Bex::size() const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE const *)_bag.get())->size();
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return 0;
    }
}

std::size_t Bex::size(std::size_t index) const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE const *)_bag.get())->size(index);
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return 0;
    }
}

std::size_t Bex::dims() const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE const *)_bag.get())->dims();
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return 0;
    }
}

bool Bex::empty() const
{
    if (!_bag) {
        return true;
    }

    switch (_type) {
#define _TBAG_XX(name, symbol, type) \
    case TypeTable::TT_##name: return ((BAG_##name##_TYPE const *)_bag.get())->empty();
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
#undef _TBAG_XX
    case TypeTable::TT_UNKNOWN:
    default:
        return true;
    }
}

std::string Bex::toString() const
{
    auto const * BUFFER = castData<char>();
    return std::string(BUFFER, BUFFER + size());
}

std::string Bex::toHexString() const
{
    using namespace libtbag::string;
    return convertByteArrayToHexString(castData<uint8_t>(), size(), STRING_EMPTY);
}

std::string Bex::toHexBoxString(int line_width) const
{
    using namespace libtbag::string;
    return convertByteArrayToHexStringBox(castData<uint8_t>(), size(), line_width);
}

std::string Bex::toInfoString() const
{
    if (!_bag) {
        return "Bex[0x00]{NULL}(0)";
    }

    std::stringstream ss;
    ss << "Bex[" << string::convertAddressToString(data())
       << "]{" << getTypeName() << '}';

    std::size_t const DIMS = dims();
    if (DIMS >= 1) {
        ss << '(' << size(0);
        for (std::size_t i = 1; i < DIMS; ++i) {
            ss << 'x' << size(i);
        }
        ss << ')';
    } else {
        assert(DIMS == 0);
    }

    return ss.str();
}

std::string Bex::toAutoString() const
{
    if (exists() && dims() == 1 && _type == TypeTable::TT_CHAR) {
        return toString();
    }
    return toInfoString();
}

Err Bex::fromString(std::string const & content)
{
    Err const CODE = resize<char>(content.size());
    if (isFailure(CODE)) {
        return CODE;
    }
    assert(_type == TypeTable::TT_CHAR);
    std::copy(content.begin(), content.end(), castData<char>());
    return Err::E_SUCCESS;
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

