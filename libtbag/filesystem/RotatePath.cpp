/**
 * @file   RotatePath.cpp
 * @brief  RotatePath class implementation.
 * @author zer0
 * @date   2017-07-31
 */

#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/log/Log.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

// ---------------------------------
// RotatePathUpdater implementation.
// ---------------------------------

RotatePathUpdater::RotatePathUpdater(std::string const & rule) : update_rule(rule)
{
    // EMPTY.
}

RotatePathUpdater::~RotatePathUpdater()
{
    // EMPTY.
}

Path RotatePathUpdater::obtain() const
{
    return Path();
}

// -------------------------------------
// RotatePathSizeChecker implementation.
// -------------------------------------

RotatePathSizeChecker::RotatePathSizeChecker(std::string const & rule, std::size_t size)
        : RotatePathUpdater(rule), max_size(size)
{
    // EMPTY.
}

RotatePathSizeChecker::~RotatePathSizeChecker()
{
    // EMPTY.
}

bool RotatePathSizeChecker::test(Path const & prev, Path & next, char const * buffer, std::size_t size) const
{
    RotatePath::FileState state = {0};
    if (details::getState(prev, &state) == false) {
        return false;
    }

    if (state.size + size < max_size) {
        return false;
    }

    // TODO: Update next path.
    return true;
}

// --------------------------
// RotatePath implementation.
// --------------------------

RotatePath::RotatePath()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path) : _path(path)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path, SharedChecker const & checker) : _path(path), _checker(checker)
{
    // EMPTY.
}

RotatePath::RotatePath(RotatePath const & obj)
{
    (*this) = obj;
}

RotatePath::RotatePath(RotatePath && obj)
{
    (*this) = std::move(obj);
}

RotatePath::~RotatePath()
{
    // EMPTY.
}

RotatePath & RotatePath::operator =(RotatePath const & obj)
{
    if (this != &obj) {
        _checker = obj._checker;
        _path = obj._path;
    }
    return *this;
}

RotatePath & RotatePath::operator =(RotatePath && obj)
{
    if (this != &obj) {
        _checker.swap(obj._checker);
        _path.swap(obj._path);
    }
    return *this;
}

bool RotatePath::testIfRead(Path const & prev, Path & next) const
{
    if (static_cast<bool>(_checker)) {
        return _checker->test(prev, next, nullptr, 0);
    }
    return false;
}

bool RotatePath::testIfRead(Path & next) const
{
    return testIfRead(_path, next);
}

bool RotatePath::testIfWrite(Path const & prev, Path & next, char const * buffer, std::size_t size) const
{
    if (static_cast<bool>(_checker)) {
        return _checker->test(prev, next, buffer, size);
    }
    return false;
}

bool RotatePath::testIfWrite(Path & next, char const * buffer, std::size_t size) const
{
    return testIfWrite(_path, next, buffer, size);
}

bool RotatePath::next(char const * buffer, std::size_t size)
{
    Path next;
    if (testIfWrite(next, buffer, size)) {
        _path = next;
        return true;
    }
    return false;
}

bool RotatePath::next()
{
    Path next;
    if (testIfRead(next)) {
        _path = next;
        return true;
    }
    return false;
}

RotatePath::FileState RotatePath::getState() const
{
    FileState state = {0};
    if (details::getState(_path, &state) == false) {
        tDLogE("RotatePath::getState() result error.");
    }
    return state;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

