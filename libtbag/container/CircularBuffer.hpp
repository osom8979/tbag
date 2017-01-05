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
class CircularBuffer
{
public:
    TBAG_CONSTEXPR static std::size_t const CIRCULAR_BUFFER_DEFAULT_SIZE = 1024;

public:
    using Value = ValueType;
    using Buffer = std::vector<Value>;

public:
    /**
     * The helper of CircularBuffer indexing variables.
     *
     * @author zer0
     * @date   2016-12-29
     */
    struct Indexer
    {
        Buffer    * b; ///< Buffer point.
        std::size_t r; ///< Read index.
        std::size_t s; ///< Written size.

        // @formatter:off
        Indexer() : b(nullptr), r(0), s(0)
        { /* EMPTY. */ }
        Indexer(Buffer * buffer, std::size_t read_index = 0, std::size_t size = 0) : b(buffer), r(read_index), s(size)
        { /* EMPTY. */ }

        Indexer(Indexer const & obj) : b(obj.b), r(obj.r), s(obj.s)
        { /* EMPTY. */ }
        Indexer(Indexer && obj)
        { swap(std::move(obj)); }

        ~Indexer()
        { /* EMPTY. */ }

        Indexer & operator =(Indexer const & obj)
        { copy(obj); return *this; }
        Indexer & operator =(Indexer && obj)
        { swap(obj); return *this; }

        void copy(Indexer const & obj)
        { if (this != &obj) { b = obj.b; r = obj.r; s = obj.s; } }
        void swap(Indexer && obj)
        { if (this != &obj) { std::swap(b, obj.b); std::swap(r, obj.r); std::swap(s, obj.s); } }

        inline bool       empty() const TBAG_NOEXCEPT { return s == 0;         }
        inline std::size_t size() const TBAG_NOEXCEPT { return s;              }
        inline std::size_t  max() const TBAG_NOEXCEPT { return b->size(); }
        inline std::size_t free() const TBAG_NOEXCEPT { return max() - s;      }

        inline std::size_t last() const
        { assert(max() > 0); return (r + s) % max(); }
        inline void next()
        { if (r + 1 == max()) { r = 0; } else { ++r; } }
        // @formatter:on
    };

public:
    /**
     * Iterator of CircularBuffer.
     *
     * @author zer0
     * @date   2016-12-29
     *
     * @warning
     *  Don't use the <code>std::distance</code> method.
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
            if (position + 1 == max_size) {
                position = 0;
            } else {
                ++position;
            }
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
    Buffer  _buffer;
    Indexer _index;

public:
    CircularBuffer(std::size_t size) : _buffer(size), _index(&_buffer)
    { /* EMPTY. */ }
    CircularBuffer() : CircularBuffer(CIRCULAR_BUFFER_DEFAULT_SIZE)
    { /* EMPTY. */ }

    CircularBuffer(CircularBuffer const & cb) : _buffer(cb._buffer), _index(cb._index)
    { /* EMPTY. */ }
    CircularBuffer(CircularBuffer && cb)
    { swap(std::move(cb)); }

    ~CircularBuffer()
    { /* EMPTY. */ }

public:
    CircularBuffer & operator =(CircularBuffer const & cb) { copy(cb); return *this; }
    CircularBuffer & operator =(CircularBuffer      && cb) { swap(cb); return *this; }

public:
    void copy(CircularBuffer const & cb)
    { if (this != &cb) { _buffer = cb._buffer; _index  = cb._index; } }
    void swap(CircularBuffer && cb)
    { if (this != &cb) { _buffer.swap(cb._buffer); _index.swap(cb._index); } }

public:
    inline Buffer & atBuffer() TBAG_NOEXCEPT
    { return _buffer; }
    inline Buffer const & atBuffer() const TBAG_NOEXCEPT
    { return _buffer; }

public:
    inline Value & operator [](std::size_t index)
    { return _buffer[index]; }
    inline Value const & operator [](std::size_t index) const
    { return _buffer[index]; }

public:
    inline void resize(std::size_t size)
    { _buffer.resize(size); } // WARNING!!!

public:
    inline bool empty() const TBAG_NOEXCEPT
    { return _index.empty(); }
    inline std::size_t size() const TBAG_NOEXCEPT
    { return _index.size(); }

public:
    inline Indexer getIndex() const TBAG_NOEXCEPT
    { return _index; }
    inline Indexer & atIndex() TBAG_NOEXCEPT
    { return _index; }
    inline Indexer const & atIndex() const TBAG_NOEXCEPT
    { return _index; }

public:
    // @formatter:off
    inline iterator begin() { return iterator(&_buffer[0], _index.r     , _index.max()); }
    inline iterator end  () { return iterator(&_buffer[0], _index.last(), _index.max()); }
    // ----------
    inline const_iterator cbegin() const { return const_iterator(&_buffer[0], _index.r     , _index.max()); }
    inline const_iterator cend  () const { return const_iterator(&_buffer[0], _index.last(), _index.max()); }
    // ----------
private:
    inline iterator write_begin() { return iterator(&_buffer[0], _index.last(), _index.max()); }
    // @formatter:on

public:
    std::size_t push(Value const * buffer, std::size_t size, bool safe = true)
    {
        auto itr = write_begin();
        bool over_write = false;
        std::size_t index = 0U;

        for (; index < size; ++index, ++itr) {
            if (_index.s + 1 > _index.max()) {
                if (safe) {
                    break;
                } else {
                    over_write = true;
                }
            }

            // WRITE!!
            *itr = *(buffer + index);

            if (over_write) {
                ++_index.r;
            } else {
                ++_index.s;
            }
        }

        return index;
    }

    std::size_t pop(Value * buffer, std::size_t size)
    {
        auto itr = begin();
        std::size_t index = 0;

        for (; index < size && _index.s > 0; ++index, ++itr) {
            *(buffer + index) = *itr;
            _index.next();
            --_index.s;
        }

        return index;
    }

public:
    void extendCapacity(std::size_t more_size)
    {
        std::size_t const REQUEST_SIZE = _buffer.size() + more_size;
        Buffer new_buffer(REQUEST_SIZE, Value());

        if (_index.r <= _index.last()) {
            _buffer.resize(REQUEST_SIZE);

        } else /* if (_index.r > _index.last()) */ {
            std::size_t const BEGIN_END_DISTANCE = std::distance(_buffer.begin() + _index.r, _buffer.end());

            Buffer new_buffer(REQUEST_SIZE, Value());
            std::copy(_buffer.begin() + _index.r, _buffer.end(), new_buffer.begin());
            std::copy(_buffer.begin(), _buffer.begin() + _index.last(), new_buffer.begin() + BEGIN_END_DISTANCE);

            _buffer.swap(new_buffer);
            _index.b = &_buffer;
            _index.r = 0;
        }
    }
};

} // namespace container

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_CONTAINER_CIRCULARBUFFER_HPP__

