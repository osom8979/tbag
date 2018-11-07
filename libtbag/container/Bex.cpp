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

Bex::Bex() TBAG_NOEXCEPT : _type(BexType::BT_NONE), _bag(nullptr), _user(nullptr)
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
    _type = BexType::BT_NONE;
    _bag.reset();
    _user.reset();
}

Err Bex::create(BexType type)
{
    if (_type == type) {
        return Err::E_ALREADY;
    }

    try {
        switch (type) {
        // @formatter:off
        case BexType::BT_INT8   : _bag.reset((FakeBag*)(new BagInt8   ), [](FakeBag * b){ delete reinterpret_cast<BagInt8   *>(b); }); break;
        case BexType::BT_UINT8  : _bag.reset((FakeBag*)(new BagUint8  ), [](FakeBag * b){ delete reinterpret_cast<BagUint8  *>(b); }); break;
        case BexType::BT_INT16  : _bag.reset((FakeBag*)(new BagInt16  ), [](FakeBag * b){ delete reinterpret_cast<BagInt16  *>(b); }); break;
        case BexType::BT_UINT16 : _bag.reset((FakeBag*)(new BagUint16 ), [](FakeBag * b){ delete reinterpret_cast<BagUint16 *>(b); }); break;
        case BexType::BT_INT32  : _bag.reset((FakeBag*)(new BagInt32  ), [](FakeBag * b){ delete reinterpret_cast<BagInt32  *>(b); }); break;
        case BexType::BT_UINT32 : _bag.reset((FakeBag*)(new BagUint32 ), [](FakeBag * b){ delete reinterpret_cast<BagUint32 *>(b); }); break;
        case BexType::BT_INT64  : _bag.reset((FakeBag*)(new BagInt64  ), [](FakeBag * b){ delete reinterpret_cast<BagInt64  *>(b); }); break;
        case BexType::BT_UINT64 : _bag.reset((FakeBag*)(new BagUint64 ), [](FakeBag * b){ delete reinterpret_cast<BagUint64 *>(b); }); break;
        case BexType::BT_FLOAT32: _bag.reset((FakeBag*)(new BagFloat32), [](FakeBag * b){ delete reinterpret_cast<BagFloat32*>(b); }); break;
        case BexType::BT_FLOAT64: _bag.reset((FakeBag*)(new BagFloat64), [](FakeBag * b){ delete reinterpret_cast<BagFloat64*>(b); }); break;
        // @formatter:on
        default:
            assert(false && "Unknown type assertion.");
            _bag.reset();
            _type = BexType::BT_NONE;
            return Err::E_ILLARGS;
        }
    } catch (std::exception & e) {
        _type = BexType::BT_NONE;
        return Err::E_BADALLOC;
    } catch (...) {
        _type = BexType::BT_NONE;
        return Err::E_UNKEXCP;
    }

    assert(static_cast<bool>(_bag));
    _type = type;
    return Err::E_SUCCESS;
}

Err Bex::resize(unsigned i0, unsigned i1, unsigned i2, unsigned i3,
                unsigned i4, unsigned i5, unsigned i6, unsigned i7)
{
    if (!_bag) {
        return Err::E_NREADY;
    }

    try {
        switch (_type) {
        // @formatter:off
        case BexType::BT_INT8   : reinterpret_cast<BagInt8   *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_UINT8  : reinterpret_cast<BagUint8  *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_INT16  : reinterpret_cast<BagInt16  *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_UINT16 : reinterpret_cast<BagUint16 *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_INT32  : reinterpret_cast<BagInt32  *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_UINT32 : reinterpret_cast<BagUint32 *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_INT64  : reinterpret_cast<BagInt64  *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_UINT64 : reinterpret_cast<BagUint64 *>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_FLOAT32: reinterpret_cast<BagFloat32*>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        case BexType::BT_FLOAT64: reinterpret_cast<BagFloat64*>(_bag.get())->resize(i0, i1, i2, i3, i4, i5, i6, i7); break;
        // @formatter:on
        default:
            assert(false && "Unknown type assertion.");
            return Err::E_ILLSTATE;
        }
    } catch (...) {
        return Err::E_UNKEXCP;
    }
    return Err::E_SUCCESS;
}

void * Bex::data()
{
    if (!_bag) {
        return nullptr;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8   *>(_bag.get())->data();
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8  *>(_bag.get())->data();
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16  *>(_bag.get())->data();
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16 *>(_bag.get())->data();
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32  *>(_bag.get())->data();
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32 *>(_bag.get())->data();
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64  *>(_bag.get())->data();
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64 *>(_bag.get())->data();
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32*>(_bag.get())->data();
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64*>(_bag.get())->data();
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return nullptr;
    }
}

void const * Bex::data() const
{
    if (!_bag) {
        return nullptr;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8    const *>(_bag.get())->data();
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8   const *>(_bag.get())->data();
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16   const *>(_bag.get())->data();
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16  const *>(_bag.get())->data();
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32   const *>(_bag.get())->data();
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32  const *>(_bag.get())->data();
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64   const *>(_bag.get())->data();
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64  const *>(_bag.get())->data();
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32 const *>(_bag.get())->data();
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64 const *>(_bag.get())->data();
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return nullptr;
    }
}

std::size_t Bex::size() const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8    const *>(_bag.get())->size();
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8   const *>(_bag.get())->size();
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16   const *>(_bag.get())->size();
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16  const *>(_bag.get())->size();
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32   const *>(_bag.get())->size();
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32  const *>(_bag.get())->size();
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64   const *>(_bag.get())->size();
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64  const *>(_bag.get())->size();
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32 const *>(_bag.get())->size();
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64 const *>(_bag.get())->size();
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return 0;
    }
}

std::size_t Bex::size(std::size_t index) const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8    const *>(_bag.get())->size(index);
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8   const *>(_bag.get())->size(index);
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16   const *>(_bag.get())->size(index);
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16  const *>(_bag.get())->size(index);
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32   const *>(_bag.get())->size(index);
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32  const *>(_bag.get())->size(index);
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64   const *>(_bag.get())->size(index);
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64  const *>(_bag.get())->size(index);
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32 const *>(_bag.get())->size(index);
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64 const *>(_bag.get())->size(index);
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return 0;
    }
}

std::size_t Bex::dims() const
{
    if (!_bag) {
        return 0;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8    const *>(_bag.get())->dims();
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8   const *>(_bag.get())->dims();
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16   const *>(_bag.get())->dims();
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16  const *>(_bag.get())->dims();
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32   const *>(_bag.get())->dims();
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32  const *>(_bag.get())->dims();
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64   const *>(_bag.get())->dims();
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64  const *>(_bag.get())->dims();
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32 const *>(_bag.get())->dims();
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64 const *>(_bag.get())->dims();
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return 0;
    }
}

bool Bex::empty() const
{
    if (!_bag) {
        return true;
    }

    switch (_type) {
    // @formatter:off
    case BexType::BT_INT8   : return reinterpret_cast<BagInt8    const *>(_bag.get())->empty();
    case BexType::BT_UINT8  : return reinterpret_cast<BagUint8   const *>(_bag.get())->empty();
    case BexType::BT_INT16  : return reinterpret_cast<BagInt16   const *>(_bag.get())->empty();
    case BexType::BT_UINT16 : return reinterpret_cast<BagUint16  const *>(_bag.get())->empty();
    case BexType::BT_INT32  : return reinterpret_cast<BagInt32   const *>(_bag.get())->empty();
    case BexType::BT_UINT32 : return reinterpret_cast<BagUint32  const *>(_bag.get())->empty();
    case BexType::BT_INT64  : return reinterpret_cast<BagInt64   const *>(_bag.get())->empty();
    case BexType::BT_UINT64 : return reinterpret_cast<BagUint64  const *>(_bag.get())->empty();
    case BexType::BT_FLOAT32: return reinterpret_cast<BagFloat32 const *>(_bag.get())->empty();
    case BexType::BT_FLOAT64: return reinterpret_cast<BagFloat64 const *>(_bag.get())->empty();
    // @formatter:on
    default:
        assert(false && "Unknown type assertion.");
        return true;
    }
}

std::string Bex::toString() const
{
    auto const * BUFFER = cast<char>();
    return std::string(BUFFER, BUFFER + size());
}

std::string Bex::toHexString() const
{
    using namespace libtbag::string;
    return convertByteArrayToHexString(cast<uint8_t>(), size(), STRING_EMPTY);
}

std::string Bex::toHexBoxString(int line_width) const
{
    using namespace libtbag::string;
    return convertByteArrayToHexStringBox(cast<uint8_t>(), size(), line_width);
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
    if (exists() && dims() == 1 && (_type == BexType::BT_INT8 || _type == BexType::BT_UINT8)) {
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
    assert(_type == BexType::BT_INT8 || _type == BexType::BT_UINT8);
    std::copy(content.begin(), content.end(), cast<char>());
    return Err::E_SUCCESS;
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

