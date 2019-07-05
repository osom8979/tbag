/**
 * @file   RotatePath.cpp
 * @brief  RotatePath class implementation.
 * @author zer0
 * @date   2017-07-31
 */

#include <libtbag/filesystem/RotatePath.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

RotatePath::RotatePath()
        : path(), checker(), updater()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path)
        : path(path), checker(), updater()
{
    // EMPTY.
}

RotatePath::RotatePath(Path const & path, SharedChecker const & checker, SharedUpdater const & updater)
        : path(path), checker(checker), updater(updater)
{
    // EMPTY.
}

RotatePath::RotatePath(SharedChecker const & checker, SharedUpdater const & updater)
        : path(), checker(checker), updater(updater)
{
    // EMPTY.
}

bool RotatePath::update()
{
    if (!updater) {
        return false;
    }

    Path next = updater->update(path);
    if (!next.empty()) {
        path.swap(next);
        return true;
    }
    return false;
}

bool RotatePath::testIfRead(Path const & prev) const
{
    return checker && checker->test(prev, nullptr, 0);
}

bool RotatePath::testIfRead() const
{
    return testIfRead(path);
}

bool RotatePath::testIfWrite(Path const & prev, char const * buffer, std::size_t size) const
{
    return checker && checker->test(prev, buffer, size);
}

bool RotatePath::testIfWrite(char const * buffer, std::size_t size) const
{
    return testIfWrite(path, buffer, size);
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

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

