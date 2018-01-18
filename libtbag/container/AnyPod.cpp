/**
 * @file   AnyPod.cpp
 * @brief  AnyPod class implementation.
 * @author zer0
 * @date   2018-01-18
 */

#include <libtbag/container/AnyPod.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/string/StringUtils.hpp>

#include <cstdint>
#include <cstring>
#include <cassert>
#include <utility>
#include <algorithm>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

AnyPod::AnyPod() : _type(TypeTable::TT_UNKNOWN), _data()
{
    ::memset(&_data, 0x00, sizeof(_data));
}

AnyPod::AnyPod(AnyPod const & obj) : AnyPod()
{
    (*this) = obj;
}

AnyPod::AnyPod(AnyPod && obj) : AnyPod()
{
    (*this) = std::move(obj);
}

AnyPod::~AnyPod()
{
    // EMPTY.
}

AnyPod & AnyPod::operator =(AnyPod const & obj)
{
    assign(obj);
    return *this;
}

AnyPod & AnyPod::operator =(AnyPod && obj)
{
    swap(obj);
    return *this;
}

void AnyPod::assign(AnyPod const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _type = obj._type;
        _data = obj._data;
    }
}

void AnyPod::swap(AnyPod & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        std::swap(_type, obj._type);
        std::swap(_data, obj._data);
    }
}

int AnyPod::compare(AnyPod const & obj) const TBAG_NOEXCEPT
{
    if (_type == TypeTable::TT_UNKNOWN || obj._type == TypeTable::TT_UNKNOWN) {
        return ::memcmp(&_data, &obj._data, sizeof(_data));
    }
    assert(    _type != TypeTable::TT_UNKNOWN);
    assert(obj._type != TypeTable::TT_UNKNOWN);

#define _TBAG_XX(n, s, t)            \
    if (_type == TypeTable::TT_##n) {        \
        t rh = obj.cast<t>();             \
        if (_data.s > rh) { return  1; }   \
        if (_data.s < rh) { return -1; }   \
        return 0;                               \
    } /* -- END -- */
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
    TBAG_TYPE_TABLE_POINT_MAP(_TBAG_XX)
#undef _TBAG_XX

    TBAG_INACCESSIBLE_BLOCK_ASSERT();
    return ::memcmp(&_data, &obj._data, sizeof(_data));
}

bool AnyPod::operator ==(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj) == 0; }
bool AnyPod::operator !=(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj) != 0; }
bool AnyPod::operator  >(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj)  > 0; }
bool AnyPod::operator  <(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj)  < 0; }
bool AnyPod::operator >=(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj) >= 0; }
bool AnyPod::operator <=(AnyPod const & obj) const TBAG_NOEXCEPT { return compare(obj) <= 0; }

std::string AnyPod::toString() const
{
#define _TBAG_XX(n, s, t)                   \
    if (_type == TypeTable::TT_##n) {       \
        return string::toString<t>(_data.s);\
    } /* -- END -- */
#define _TBAG_POINT_XX(n, s, t)             \
    if (_type == TypeTable::TT_##n) {       \
        return string::convertAddressToString((void const *)_data.s); \
    } /* -- END -- */
    TBAG_TYPE_TABLE_MAP(_TBAG_XX)
    TBAG_TYPE_TABLE_POINT_MAP(_TBAG_POINT_XX)
#undef _TBAG_XX
    return std::string();
}

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

