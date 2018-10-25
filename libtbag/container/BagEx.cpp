/**
 * @file   BagEx.cpp
 * @brief  BagEx class implementation.
 * @author zer0
 * @date   2018-10-23
 */

#include <libtbag/container/BagEx.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/string/Format.hpp>

#include <utility>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

BagEx::BagEx() TBAG_NOEXCEPT : _type(TypeTable::TT_UNKNOWN), _bag(nullptr), _user(nullptr)
{
    // EMPTY.
}

BagEx::BagEx(std::string const & content) : BagEx()
{
    if (isFailure(fromString(content))) {
        throw std::bad_alloc();
    }
}

BagEx::BagEx(BagEx const & obj) TBAG_NOEXCEPT : BagEx()
{
    (*this) = obj;
}

BagEx::BagEx(BagEx && obj) TBAG_NOEXCEPT : BagEx()
{
    (*this) = std::move(obj);
}

BagEx::~BagEx()
{
    clear();
}

BagEx & BagEx::operator =(BagEx const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

BagEx & BagEx::operator =(BagEx && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

BagEx & BagEx::operator =(std::string const & content)
{
    fromString(content);
    return *this;
}

void BagEx::copy(BagEx const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _type = obj._type;
        _bag  = obj._bag;
        _user = obj._user;
    }
}

void BagEx::swap(BagEx & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_type, obj._type);
        _bag.swap(obj._bag);
        _user.swap(obj._user);
    }
}

void BagEx::clear()
{
    _type = TypeTable::TT_UNKNOWN;
    _bag.reset();
    _user.reset();
}

Err BagEx::create(TypeTable type)
{
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

Err BagEx::resize(std::size_t i0, std::size_t i1, std::size_t i2, std::size_t i3,
                  std::size_t i4, std::size_t i5, std::size_t i6, std::size_t i7)
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

void * BagEx::data()
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

void const * BagEx::data() const
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

std::size_t BagEx::size() const
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

std::size_t BagEx::size(std::size_t index) const
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

std::size_t BagEx::dims() const
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

bool BagEx::empty() const
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

std::string BagEx::toString() const
{
    auto const * BUFFER = castData<char>();
    return std::string(BUFFER, BUFFER + size());
}

std::string BagEx::toHexString() const
{
    using namespace libtbag::string;
    return convertByteArrayToHexString(castData<uint8_t>(), size(), STRING_EMPTY);
}

std::string BagEx::toHexBoxString(int line_width) const
{
    using namespace libtbag::string;
    return convertByteArrayToHexStringBox(castData<uint8_t>(), size(), line_width);
}

std::string BagEx::toInfoString() const
{
    if (!_bag) {
        return "BagEx[0x00]{NULL}(0)";
    }

    std::stringstream ss;
    ss << "BagEx[" << string::convertAddressToString(data())
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

std::string BagEx::toAutoString() const
{
    if (exists() && dims() == 1 && _type == TypeTable::TT_CHAR) {
        return toString();
    }
    return toInfoString();
}

Err BagEx::fromString(std::string const & content)
{
    auto code = create<char>();
    if (isFailure(code)) {
        return code;
    }
    code = resize(content.size());
    if (isFailure(code)) {
        return code;
    }
    std::copy(content.begin(), content.end(), castData<char>());
    return Err::E_SUCCESS;
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

