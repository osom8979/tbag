/**
 * @file   BoxCursor.cpp
 * @brief  BoxCursor class implementation.
 * @author zer0
 * @date   2019-12-30
 */

#include <libtbag/box/BoxCursor.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace box {

BoxCursor::BoxCursor() TBAG_NOEXCEPT
{
    // EMPTY.
}

BoxCursor::BoxCursor(box_cursor const & c) TBAG_NOEXCEPT : _cursor(c)
{
    // EMPTY.
}

BoxCursor::BoxCursor(box_cursor && c) TBAG_NOEXCEPT : _cursor(c)
{
    // EMPTY.
}

BoxCursor::BoxCursor(BoxCursor const & obj) TBAG_NOEXCEPT : _cursor(obj._cursor)
{
    // EMPTY.
}

BoxCursor::BoxCursor(BoxCursor && obj) TBAG_NOEXCEPT : _cursor(obj._cursor)
{
    // EMPTY.
}

BoxCursor::~BoxCursor()
{
    // EMPTY.
}

BoxCursor & BoxCursor::operator =(BoxCursor const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _cursor = obj._cursor;
    }
    return *this;
}

BoxCursor & BoxCursor::operator =(BoxCursor && obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _cursor = obj._cursor;
    }
    return *this;
}

ErrPair<BoxCursor> BoxCursor::sub(box_slice const & slice) const
{
    return sub(slice.begin, slice.end, slice.step);
}

ErrPair<BoxCursor> BoxCursor::sub(int begin_index, int end_index, int step_index) const
{
    assert(_cursor.box != nullptr);
    if ((_cursor.dim_index+1) >= _cursor.box->rank) {
        return libtbag::E_OORANGE;
    }
    auto const err_cursor = _cursor.init_cursor(_cursor.dim_index+1, begin_index, end_index, step_index);
    return { err_cursor.code, err_cursor.value };
}

ErrPair<BoxCursor> BoxCursor::sub(int begin_index, int end_index) const
{
    return sub(begin_index, end_index, 1);
}

ErrPair<BoxCursor> BoxCursor::sub(int begin_index) const
{
    return sub(begin_index, nop);
}

ErrPair<BoxCursor> BoxCursor::sub() const
{
    return sub(nop);
}

void * BoxCursor::getBegin() TBAG_NOEXCEPT
{
    return _cursor.begin;
}

void const * BoxCursor::getBegin() const TBAG_NOEXCEPT
{
    return _cursor.begin;
}

void * BoxCursor::getEnd() TBAG_NOEXCEPT
{
    return _cursor.end;
}

void const * BoxCursor::getEnd() const TBAG_NOEXCEPT
{
    return _cursor.end;
}

int BoxCursor::getStrideByte() const TBAG_NOEXCEPT
{
    return _cursor.stride_byte;
}

int BoxCursor::getDiff() const TBAG_NOEXCEPT
{
    return _cursor.diff();
}

bool BoxCursor::isLastDimension() const TBAG_NOEXCEPT
{
    return _cursor.is_last_dim();
}

bool BoxCursor::isContinue() const TBAG_NOEXCEPT
{
    return _cursor.is_continue();
}

bool BoxCursor::isEnd() const TBAG_NOEXCEPT
{
    return _cursor.is_end();
}

bool BoxCursor::next() TBAG_NOEXCEPT
{
    return _cursor.next();
}

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

