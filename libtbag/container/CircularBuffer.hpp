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
#include <libtbag/Type.hpp>

#include <cassert>

#include <algorithm>
#include <iterator>
#include <vector>

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
class CircularBuffer : public Noncopyable
{
public:
    TBAG_CONSTEXPR static std::size_t const CIRCULAR_BUFFER_DEFAULT_SIZE = 1024;

public:
    using Value  = ValueType;
    using Buffer = std::vector<Value>;

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

        pointer     buffer;
        std::size_t position;
        std::size_t max_size;

        Iterator(pointer buf, std::size_t pos, std::size_t max) TBAG_NOEXCEPT
                : buffer(buf), position(pos), max_size(max)
        { /* EMPTY. */ }
        Iterator(Iterator const & itr) TBAG_NOEXCEPT
                : buffer(itr.buffer), position(itr.position), max_size(itr.max_size)
        { /* EMPTY. */ }

        inline Iterator & operator =(Iterator const & itr) TBAG_NOEXCEPT
        {
            if (this != &itr) {
                buffer   = itr.buffer;
                position = itr.position;
                max_size = itr.max_size;
            }
            return *this;
        }

        inline Iterator & operator ++() TBAG_NOEXCEPT
        {
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
        { return buffer == itr.buffer && position == itr.position; }
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
    Buffer       _buffer;
    std::size_t  _read_index;
    std::size_t  _written_size;

public:
    CircularBuffer(std::size_t size) : _buffer(size), _read_index(0), _written_size(0)
    { /* EMPTY. */ }
    CircularBuffer() : CircularBuffer(CIRCULAR_BUFFER_DEFAULT_SIZE)
    { /* EMPTY. */ }
    ~CircularBuffer()
    { /* EMPTY. */ }

public:
    inline Buffer       & atBuffer()       TBAG_NOEXCEPT { return _buffer; }
    inline Buffer const & atBuffer() const TBAG_NOEXCEPT { return _buffer; }

public:
    inline Value       & operator [](std::size_t index)       { return _buffer[index]; }
    inline Value const & operator [](std::size_t index) const { return _buffer[index]; }

public:
    inline void resize(std::size_t size)
    { _buffer.resize(size); } // WARNING!!!

public:
    // @formatter:off
    inline bool        empty() const TBAG_NOEXCEPT { return _written_size == 0;         }
    inline std::size_t  size() const TBAG_NOEXCEPT { return _written_size;              }
    inline std::size_t start() const TBAG_NOEXCEPT { return _read_index;                }
    inline std::size_t   max() const TBAG_NOEXCEPT { return _buffer.size();             }
    inline std::size_t  free() const TBAG_NOEXCEPT { return max() - size();             }
    inline std::size_t  last() const TBAG_NOEXCEPT { return (start() + size()) % max(); }
    // @formatter:on

public:
    // @formatter:off
    inline       iterator  begin()       { return       iterator(&_buffer[0], start(), max()); }
    inline       iterator  end  ()       { return       iterator(&_buffer[0],  last(), max()); }
    inline const_iterator cbegin() const { return const_iterator(&_buffer[0], start(), max()); }
    inline const_iterator cend  () const { return const_iterator(&_buffer[0],  last(), max()); }
    // @formatter:on

private:
    inline std::size_t next(std::size_t index) TBAG_NOEXCEPT
    {
        if (index + 1 == max()) {
            return 0;
        }
        return index + 1;
    }

public:
    std::size_t push(Value const * buffer, std::size_t size)
    {
        std::size_t index = 0U;

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

    std::size_t pop(Value * buffer, std::size_t size)
    {
        std::size_t index = 0;

        for (; index < size && _written_size > 0; ++index) {
            *(buffer + index) = _buffer[start()];

            _read_index = next(_read_index);
            --_written_size;
        }

        return index;
    }

public:
    std::size_t extendPush(Value const * buffer, std::size_t size)
    {
        if (free() < size) {
            extendCapacity((size - free()) / CIRCULAR_BUFFER_DEFAULT_SIZE + CIRCULAR_BUFFER_DEFAULT_SIZE);
        }
        assert(free() >= size);
        return push(buffer, size);
    }

public:
    void extendCapacity(std::size_t more_size)
    {
        std::size_t const SIZE = max() + more_size;
        _buffer.resize(SIZE);

        if (start() > last()) {
//            std::copy(_buffer.begin() + _index.r, _buffer.end()                  , new_buffer.begin());
//            std::copy(_buffer.begin()           , _buffer.begin() + _index.last(), new_buffer.begin() + BEGIN_END_DISTANCE);
        }
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CIRCULARBUFFER_HPP__

