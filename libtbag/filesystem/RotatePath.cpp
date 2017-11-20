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

RotatePath::RotatePath()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path) : RotatePath(path, SharedChecker(), SharedUpdater())
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path, SharedChecker const & checker, SharedUpdater const & updater)
        : _path(path), _checker(checker), _updater(updater)
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path, default_setup const & UNUSED_PARAM(val))
        : RotatePath(path, SharedChecker(new SizeChecker()), SharedUpdater(new TimeFormatUpdater()))
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
        _updater = obj._updater;
        _path = obj._path;
    }
    return *this;
}

RotatePath & RotatePath::operator =(RotatePath && obj)
{
    if (this != &obj) {
        _checker.swap(obj._checker);
        _updater.swap(obj._updater);
        _path.swap(obj._path);
    }
    return *this;
}

RotatePath::FileState RotatePath::getState() const
{
    FileState state = {0};
    if (details::getState(_path, &state) == false) {
        tDLogE("RotatePath::getState() result error.");
    }
    return state;
}

bool RotatePath::update()
{
    if (static_cast<bool>(_updater)) {
        Path next = _updater->update(_path);
        if (next.empty()) {
            tDLogE("RotatePath::update() prev({}) -> next({})", _path.toString(), next.toString());
            _path.swap(next);
            return true;
        }
    }
    return false;
}

bool RotatePath::testIfRead(Path const & prev) const
{
    return static_cast<bool>(_checker) && _checker->test(prev, nullptr, 0);
}

bool RotatePath::testIfRead() const
{
    return testIfRead(_path);
}

bool RotatePath::testIfWrite(Path const & prev, char const * buffer, std::size_t size) const
{
    return static_cast<bool>(_checker) && _checker->test(prev, buffer, size);
}

bool RotatePath::testIfWrite(char const * buffer, std::size_t size) const
{
    return testIfWrite(_path, buffer, size);
}

bool RotatePath::next(char const * buffer, std::size_t size)
{
    if (testIfWrite(buffer, size)) {
        return update();
    }
    return false;
}

bool RotatePath::next()
{
    if (testIfRead()) {
        return update();
    }
    return false;
}

RotatePath RotatePath::createDefault(Path const & path)
{
    return RotatePath(path, default_setup{});
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

