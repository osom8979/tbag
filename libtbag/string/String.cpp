/**
 * @file   String.cpp
 * @brief  String class implementation.
 * @author zer0
 * @date   2016-11-23
 */

#include <libtbag/string/String.hpp>
#include <libtbag/algorithm/MinMax.hpp>
#include <libtbag/algorithm/Pack.hpp>
#include <libtbag/memory/Memory.hpp>

#include <cassert>
#include <cstdlib>
#include <cstdint>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace string {

void rstr_clear(rstr_base * s) TBAG_NOEXCEPT
{
    s->flags = 0;
    s->capacity = 0;
    s->size = 0;
    s->data = nullptr;
}

void rstr_base_malloc(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(capacity >= 1);
    if (flags & RSTR_FLAG_ALIGN) {
        s->flags = RSTR_FLAG_ALIGN;
        s->data = (char*)tbAlignedMalloc(sizeof(char)*capacity, tbDefaultAlignSize());
    } else {
        s->flags = 0;
        s->data = (char*)tbMalloc(sizeof(char)*capacity);
    }
    s->capacity = capacity;
    s->size = 0;
}

void rstr_base_free(rstr_base * s) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    if (s->capacity <= 0) {
        return;
    }

    assert(s->data);
    if (s->flags & RSTR_FLAG_ALIGN) {
        tbAlignedFree(s->data);
    } else {
        tbFree(s->data);
    }
    s->capacity = 0;
    s->size = 0;
    s->data = nullptr;
}

void rstr_base_copy_string(rstr_base * s, char const * src, int size) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(s->data != nullptr);
    assert(src != nullptr);
    assert(size >= 1);
    assert(WITH_NULLSIZE(size) <= s->capacity);

    ::memcpy(s->data, src, sizeof(char)*size);
    s->data[size] = '\0';
    s->size = size;
}

void rstr_base_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(capacity >= 1);

    if (capacity <= s->capacity) {
        return;
    }

    rstr_base temp = *s;
    assert(capacity > temp.capacity);

    rstr_base_malloc(s, capacity, flags);
    assert(s->capacity == capacity);
    assert(s->data == nullptr);
    assert(s->size == 0);

    // Restore original data.
    if (temp.size >= 1) {
        assert(temp.data != nullptr);
        assert(temp.capacity >= temp.size);
        assert(WITH_NULLSIZE(temp.size) <= capacity);
        rstr_base_copy_string(s, temp.data, temp.size);
    }

    // Free original memory.
    if (temp.capacity >= 1) {
        assert(temp.data != nullptr);
        rstr_base_free(&temp);
    }
}

int rstr_base_get_recommand_capacity(int capacity) TBAG_NOEXCEPT
{
    if (capacity <= RSTR_SMALL_CAPACITY) {
        return RSTR_SMALL_CAPACITY;
    }
    if (capacity <= RSTR_MEDIUM_CAPACITY) {
        return RSTR_MEDIUM_CAPACITY;
    }
    if (capacity <= RSTR_LARGE_CAPACITY) {
        return RSTR_LARGE_CAPACITY;
    }
    if (capacity <= RSTR_VLARGE_CAPACITY) {
        return RSTR_VLARGE_CAPACITY;
    }
    return RSTR_STEP_CAPACITY * libtbag::algorithm::getPackedSize(capacity, RSTR_STEP_CAPACITY);
}

void rstr_base_recommand_capacity_extend(rstr_base * s, int capacity, int flags) TBAG_NOEXCEPT
{
    rstr_base_capacity_extend(s, rstr_base_get_recommand_capacity(WITH_NULLSIZE(capacity)), flags);
}

void rstr_base_assign(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(src != nullptr);
    assert(size >= 1);
    s->size = 0;
    rstr_base_recommand_capacity_extend(s, size, flag);
    rstr_base_copy_string(s, src, size);
}

void rstr_base_append(rstr_base * s, char const * src, int size, int flag) TBAG_NOEXCEPT
{
    assert(s != nullptr);
    assert(src != nullptr);
    assert(size >= 1);

    if (s->size == 0) {
        rstr_base_assign(s, src, size, flag);
        return;
    }

    auto const MIN_SIZE = WITH_NULLSIZE(s->size+size);
    rstr_base_recommand_capacity_extend(s, MIN_SIZE, flag);
    assert(MIN_SIZE <= s->capacity);

    ::memcpy(s->data + s->size, src, sizeof(char)*size);
    s->size += size;
    s->data[s->size] = '\0';
}

// -------------------
// rstr implementation
// -------------------

rstr::rstr() : _base(nullptr)
{
    auto * s = (rstr_base*)::malloc(sizeof(rstr_base));
    assert(s != nullptr);
    rstr_clear(s);

    _base.reset(s, [](rstr_base * ptr){
        rstr_base_free(ptr);
        ::free(ptr);
    });
    assert(static_cast<bool>(_base));
}

rstr::rstr(std::nullptr_t) TBAG_NOEXCEPT : _base(nullptr)
{
    assert(!static_cast<bool>(_base));
}

rstr::rstr(rstr const & obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_COPY_CONSTRUCTIBLE) : _base(nullptr)
{
    (*this) = obj;
}

rstr::rstr(rstr && obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_MOVE_CONSTRUCTIBLE) : _base(nullptr)
{
    (*this) = std::move(obj);
}

rstr::~rstr()
{
    _base.reset();
}

rstr & rstr::operator =(rstr const & obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_COPY_ASSIGNABLE)
{
    _base = obj._base;
    return *this;
}

rstr & rstr::operator =(rstr && obj) TBAG_NOEXCEPT_SP_OP(NOTHROW_MOVE_ASSIGNABLE)
{
    _base.swap(obj._base);
    return *this;
}

void rstr::swap(rstr & obj) TBAG_NOEXCEPT
{
    _base.swap(obj._base);
}

rstr rstr::clone() const
{
    if (!_base) {
        return rstr(nullptr);
    }
    assert(exists());
    rstr result;
    result.reserve(_base->capacity, _base->flags);
    result.assign(data(), size());
    return result;
}

using value_type             = rstr::value_type;
using traits_type            = rstr::traits_type;
using allocator_type         = rstr::allocator_type;
using reference              = rstr::reference;
using const_reference        = rstr::const_reference;
using pointer                = rstr::pointer;
using const_pointer          = rstr::const_pointer;
using difference_type        = rstr::difference_type;
using size_type              = rstr::size_type;
using iterator               = rstr::iterator;
using const_iterator         = rstr::const_pointer;
using reverse_iterator       = rstr::reverse_iterator;
using const_reverse_iterator = rstr::const_reverse_iterator;

iterator rstr::begin() TBAG_NOEXCEPT
{
    assert(exists());
    return iterator(data());
}

iterator rstr::end() TBAG_NOEXCEPT
{
    assert(exists());
    return iterator(data() + size());
}

const_iterator rstr::begin() const TBAG_NOEXCEPT
{
    assert(exists());
    return const_iterator(data());
}

const_iterator rstr::end() const TBAG_NOEXCEPT
{
    assert(exists());
    return const_iterator(data() + size());
}

const_iterator rstr::cbegin() const TBAG_NOEXCEPT
{
    assert(exists());
    return begin();
}

const_iterator rstr::cend() const TBAG_NOEXCEPT
{
    assert(exists());
    return end();
}

reverse_iterator rstr::rbegin() TBAG_NOEXCEPT
{
    assert(exists());
    return reverse_iterator(end());
}

reverse_iterator rstr::rend() TBAG_NOEXCEPT
{
    assert(exists());
    return reverse_iterator(begin());
}

const_reverse_iterator rstr::rbegin() const TBAG_NOEXCEPT
{
    assert(exists());
    return const_reverse_iterator(end());
}

const_reverse_iterator rstr::rend() const TBAG_NOEXCEPT
{
    assert(exists());
    return const_reverse_iterator(begin());
}

const_reverse_iterator rstr::crbegin() const TBAG_NOEXCEPT
{
    assert(exists());
    return rbegin();
}

const_reverse_iterator rstr::crend() const TBAG_NOEXCEPT
{
    assert(exists());
    return rend();
}

size_type rstr::size() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->size;
}

size_type rstr::length() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->size;
}

size_type rstr::capacity() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->capacity;
}

bool rstr::empty() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->size == 0;
}

void rstr::resize(size_type n, int flags)
{
    assert(exists());
    assert(n >= 1);
    reserve(n, flags);
    assert(n <= _base->capacity);
    _base->size = n;
}

void rstr::resize(size_type n)
{
    resize(n, _base->flags);
}

void rstr::reserve(size_type n, int flags)
{
    assert(exists());
    assert(n >= 1);
    rstr_base_recommand_capacity_extend(_base.get(), n, flags);
}

void rstr::reserve(size_type n)
{
    reserve(n, _base->flags);
}

void rstr::clear() TBAG_NOEXCEPT
{
    assert(exists());
    _base->size = 0;
}

reference rstr::operator[](size_type i)
{
    assert(exists());
    assert(0 <= COMPARE_AND(i) < size());
    return _base->data[i];
}

const_reference rstr::operator[](size_type i) const
{
    assert(exists());
    assert(0 <= COMPARE_AND(i) < size());
    return _base->data[i];
}

reference rstr::at(size_type i)
{
    assert(exists());
    assert(0 <= COMPARE_AND(i) < size());
    return _base->data[i];
}

const_reference rstr::at(size_type i) const
{
    assert(exists());
    assert(0 <= COMPARE_AND(i) < size());
    return _base->data[i];
}

reference rstr::front()
{
    assert(exists());
    return at(0);
}

const_reference rstr::front() const
{
    assert(exists());
    return at(0);
}

reference rstr::back()
{
    assert(exists());
    return at(size()-1);
}

const_reference rstr::back() const
{
    assert(exists());
    return at(size()-1);
}

void rstr::assign(rstr const & obj)
{
    assert(exists());
    assert(obj.exists());
    rstr_base_assign(_base.get(), obj.data(), obj.size(), _base->flags);
}

void rstr::assign(const_pointer src, size_type n)
{
    assert(exists());
    assert(src != nullptr);
    assert(n >= 1);
    return rstr_base_assign(_base.get(), src, n, _base->flags);
}

void rstr::assign(const_pointer src)
{
    assign(src, ::strlen(src));
}

void rstr::assign(std::string const & src)
{
    assign(src.c_str(), src.size());
}

rstr & rstr::append(rstr const & obj) TBAG_NOEXCEPT
{
    assert(exists());
    assert(obj.exists());
    rstr_base_append(_base.get(), obj.data(), obj.size(), _base->flags);
    return *this;
}

rstr & rstr::append(const_pointer src, size_type n) TBAG_NOEXCEPT
{
    assert(exists());
    assert(src != nullptr);
    rstr_base_append(_base.get(), src, n, _base->flags);
    return *this;
}

rstr & rstr::append(const_pointer src) TBAG_NOEXCEPT
{
    return append(src, ::strlen(src));
}

rstr & rstr::append(std::string const & src) TBAG_NOEXCEPT
{
    return append(src.c_str(), src.size());
}

pointer rstr::c_str() TBAG_NOEXCEPT
{
    assert(exists());
    return _base->data;
}

const_pointer rstr::c_str() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->data;
}

pointer rstr::data() TBAG_NOEXCEPT
{
    assert(exists());
    return _base->data;
}

const_pointer rstr::data() const TBAG_NOEXCEPT
{
    assert(exists());
    return _base->data;
}

int rstr::compare(rstr const & obj) const TBAG_NOEXCEPT
{
    assert(exists());
    assert(obj.exists());
    return ::strncmp(data(), obj.data(), libtbag::algorithm::getMin(size(), obj.size()));
}

int rstr::compare(const_pointer s) const TBAG_NOEXCEPT
{
    assert(exists());
    assert(s != nullptr);
    return ::strncmp(data(), s, size());
}

bool rstr::operator ==(rstr const & obj) const TBAG_NOEXCEPT
{
    assert(exists());
    assert(obj.exists());
    return compare(obj) == 0;
}

bool rstr::operator ==(const_pointer s) const TBAG_NOEXCEPT
{
    assert(exists());
    assert(s != nullptr);
    return compare(s) == 0;
}

} // namespace string

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

