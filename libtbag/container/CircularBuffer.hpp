/**
 * @file   CircularBuffer.hpp
 * @brief  CircularBuffer class prototype.
 * @author zer0
 * @date   2016-12-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CIRCULARBUFFER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CIRCULARBUFFER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Type.hpp>

#include <cassert>

#include <iterator>
#include <algorithm>
#include <vector>
#include <type_traits>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace container {

/**
 * CircularBuffer class prototype.
 *
 * @author zer0
 * @date   2016-12-29
 */
template <typename ValueType>
class CircularBuffer
{
public:
    using Value  = ValueType;
    using Size   = std::size_t;
    using Buffer = std::vector<Value>;

public:
    STATIC_ASSERT_CHECK_IS_SAME(Size, typename Buffer::size_type);

public:
    TBAG_CONSTEXPR static Size const CIRCULAR_BUFFER_DEFAULT_SIZE = 1024;

    static_assert(CIRCULAR_BUFFER_DEFAULT_SIZE > 0, "Too small default size.");

public:
    /**
     * Iterator of CircularBuffer.
     *
     * @author zer0
     * @date   2016-12-29
     */
    template <typename Type>
    struct Iterator : public std::iterator<std::forward_iterator_tag, Type>
    {
        using Parent = std::iterator<std::forward_iterator_tag, Type>;

        typedef typename Parent::value_type        value_type;
        typedef typename Parent::difference_type   difference_type;
        typedef typename Parent::pointer           pointer;
        typedef typename Parent::reference         reference;
        typedef typename Parent::iterator_category iterator_category;

        pointer buffer;
        Size    position;
        Size    read_index;
        Size    max_size;

        Iterator(pointer buf, Size pos, Size index, Size max) TBAG_NOEXCEPT
                : buffer(buf), position(pos), read_index(index), max_size(max)
        { /* EMPTY. */ }
        Iterator(Iterator const & itr) TBAG_NOEXCEPT
                : buffer(itr.buffer), position(itr.position), read_index(itr.read_index), max_size(itr.max_size)
        { /* EMPTY. */ }

        inline Iterator & operator =(Iterator const & itr) TBAG_NOEXCEPT
        {
            if (this != &itr) {
                buffer     = itr.buffer;
                position   = itr.position;
                read_index = itr.read_index;
                max_size   = itr.max_size;
            }
            return *this;
        }

        inline Iterator & operator ++() TBAG_NOEXCEPT
        {
            ++read_index;
            position = (position + 1 == max_size ? 0 : position + 1);
            return *this;
        }

        inline Iterator operator ++(int) TBAG_NOEXCEPT
        {
            Iterator result = *this;
            ++(*this);
            return result;
        }

        inline bool operator == (Iterator const & itr) const TBAG_NOEXCEPT
        { return buffer == itr.buffer && position == itr.position && read_index == itr.read_index; }
        inline bool operator != (Iterator const & itr) const TBAG_NOEXCEPT
        { return !(*this == itr); }

        inline reference operator *() TBAG_NOEXCEPT
        { return *(buffer + position); }
        inline const reference operator *() const TBAG_NOEXCEPT
        { return *(buffer + position); }

        inline pointer operator ->() TBAG_NOEXCEPT
        { return (buffer + position); }
        inline const pointer operator ->() const TBAG_NOEXCEPT
        { return (buffer + position); }
    };

public:
    typedef Iterator<Value      >       iterator;
    typedef Iterator<Value const> const_iterator;

    STATIC_ASSERT_CHECK_IS_SAME(typename       iterator::iterator_category, std::forward_iterator_tag);
    STATIC_ASSERT_CHECK_IS_SAME(typename const_iterator::iterator_category, std::forward_iterator_tag);

private:
    Buffer _buffer;
    Size   _read_index;
    Size   _written_size;

private:
    static TBAG_CONSTEXPR bool const buffer_is_nothrow_constructible
            = std::is_nothrow_constructible<Buffer, Size>::value;

public:
    CircularBuffer(Size size = CIRCULAR_BUFFER_DEFAULT_SIZE)
            TBAG_NOEXCEPT_EXPR(buffer_is_nothrow_constructible)
            : _buffer(size), _read_index(0), _written_size(0)
    { /* EMPTY. */ }

    ~CircularBuffer()
    { /* EMPTY. */ }

public:
    CircularBuffer(CircularBuffer const & obj)
            TBAG_NOEXCEPT_EXPR(std::is_nothrow_copy_constructible<Buffer>::value)
    {
        if (this != &obj) {
            _buffer       = obj._buffer;
            _read_index   = obj._read_index;
            _written_size = obj._written_size;
        }
    }

    CircularBuffer(CircularBuffer && obj)
            TBAG_NOEXCEPT_EXPR(std::is_nothrow_move_assignable<Buffer>::value)
    {
        if (this != &obj) {
            _buffer       = std::move(obj._buffer);
            _read_index   = obj._read_index;
            _written_size = obj._written_size;

            obj._buffer.clear();
            obj._read_index   = 0;
            obj._written_size = 0;
        }
    }

public:
    CircularBuffer & operator =(CircularBuffer const & obj)
            TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(copy(obj)))
    {
        return copy(obj);
    }

    CircularBuffer & operator =(CircularBuffer && obj)
            TBAG_NOEXCEPT_EXPR(std::is_nothrow_move_assignable<Buffer>::value)
    {
        if (this != &obj) {
            _buffer       = std::move(obj._buffer);
            _read_index   = obj._read_index;
            _written_size = obj._written_size;

            obj._buffer.clear();
            obj._read_index   = 0;
            obj._written_size = 0;
        }
        return *this;
    }

public:
    CircularBuffer & copy(CircularBuffer const & obj) TBAG_NOEXCEPT_EXPR(std::is_nothrow_copy_assignable<Buffer>::value)
    {
        if (this != &obj) {
            _buffer       = obj._buffer;
            _read_index   = obj._read_index;
            _written_size = obj._written_size;
        }
        return *this;
    }

public:
    void swap(CircularBuffer & obj) TBAG_NOEXCEPT_EXPR(std::is_nothrow_move_assignable<Buffer>::value)
    {
        if (this != &obj) {
            _buffer.swap(obj._buffer);
            std::swap(_read_index  , obj._read_index  );
            std::swap(_written_size, obj._written_size);
        }
    }

public:
    friend void swap(CircularBuffer & obj1, CircularBuffer & obj2)
    { obj1.swap(obj2); }

public:
    inline Buffer       & atBuffer()       TBAG_NOEXCEPT { return _buffer; }
    inline Buffer const & atBuffer() const TBAG_NOEXCEPT { return _buffer; }

public:
    inline Value       & operator [](Size index)       { return _buffer[index]; }
    inline Value const & operator [](Size index) const { return _buffer[index]; }

public:
    void clear()
    {
        _buffer.clear();
        _read_index   = 0;
        _written_size = 0;
    }

public:
    // @formatter:off
    inline bool empty() const TBAG_NOEXCEPT { return _written_size == 0;         }
    inline Size  size() const TBAG_NOEXCEPT { return _written_size;              }
    inline Size start() const TBAG_NOEXCEPT { return _read_index;                }
    inline Size   max() const TBAG_NOEXCEPT { return _buffer.size();             }
    inline Size  free() const TBAG_NOEXCEPT { return max() - size();             }
    inline Size  last() const TBAG_NOEXCEPT { return (start() + size()) % max(); }
    // @formatter:on

private:
    // @formatter:off
    inline Size moreCapacity(Size size) TBAG_NOEXCEPT
    {
        return ((size - free()) / CIRCULAR_BUFFER_DEFAULT_SIZE)
               * CIRCULAR_BUFFER_DEFAULT_SIZE
               + CIRCULAR_BUFFER_DEFAULT_SIZE;
    }
    inline Size next(Size index) TBAG_NOEXCEPT
    { return (index + 1 == max() ? 0 : index + 1); }
    // @formatter:on

public:
    // @formatter:off
    inline       iterator  begin()       { return       iterator(&_buffer[0], start(),      0, max()); }
    inline       iterator  end  ()       { return       iterator(&_buffer[0],  last(), size(), max()); }
    inline const_iterator cbegin() const { return const_iterator(&_buffer[0], start(),      0, max()); }
    inline const_iterator cend  () const { return const_iterator(&_buffer[0],  last(), size(), max()); }
    // @formatter:on

public:
    Size push(Value const * buffer, Size size)
    {
        Size index = 0U;

        for (; index < size; ++index) {
            if (_written_size + 1 > max()) {
                break;
            }

            assert(last() >= 0);
            assert(last() <  max());

            _buffer[last()] = *(buffer + index); // WRITE!!
            ++_written_size;
        }

        return index;
    }

    Size pop(Value * buffer, Size size)
    {
        Size index = 0;

        for (; index < size && _written_size > 0; ++index) {
            *(buffer + index) = _buffer[start()];

            _read_index = next(_read_index);
            --_written_size;
        }

        return index;
    }

public:
    bool extendCapacity(Size more_size)
    {
        Size const ORIGINAL_SIZE = _buffer.size();
        Size const EXTEND_SIZE   = ORIGINAL_SIZE + more_size;

        if (EXTEND_SIZE == ORIGINAL_SIZE) {
            __tbag_error("Size must be greater than 0.");
            return false;
        } else if (EXTEND_SIZE >= _buffer.max_size()) {
            __tbag_error("Too large size: {}", EXTEND_SIZE);
            return false;
        }

        assert(EXTEND_SIZE > ORIGINAL_SIZE);
        CircularBuffer newbie(EXTEND_SIZE);
        //auto itr = std::copy(begin(), end(), newbie._buffer.begin());

        auto __result = newbie._buffer.begin();
        auto __first = begin();
        auto __last  = end();
        for (; __first != __last; ++__first, (void) ++__result)
            *__result = *__first;
        auto itr = __result;

        newbie._read_index   = 0;
        newbie._written_size = std::distance(newbie._buffer.begin(), itr);

        *this = std::move(newbie);

        return true;
    }

public:
    Size extendPush(Value const * buffer, Size size)
    {
        if (free() < size) {
            if (extendCapacity(moreCapacity(size)) == false) {
                return 0;
            }
        }

        assert(free() >= size);
        return push(buffer, size);
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CIRCULARBUFFER_HPP__

