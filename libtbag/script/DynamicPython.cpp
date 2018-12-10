/**
 * @file   DynamicPython.cpp
 * @brief  DynamicPython class implementation.
 * @author zer0
 * @date   2018-10-31
 */

#include <libtbag/script/DynamicPython.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

DynamicPython::DynamicPython()
{
    // EMPTY.
}

DynamicPython::~DynamicPython()
{
    // EMPTY.
}

bool DynamicPython::isOpen() const
{
    return _lib.isOpen();
}

std::string DynamicPython::getPath() const
{
    return _path;
}

std::string DynamicPython::getError() const
{
    return _lib.getError();
}

bool DynamicPython::open(std::string const & path)
{
    bool const RESULT = _lib.open(path);
    if (RESULT) {
        _path = path;
    }
    return RESULT;
}

void DynamicPython::close()
{
    _lib.close();
    _path.clear();
}

void DynamicPython::Py_Initialize()
{
    _lib.call<void>("Py_Initialize");
}

void DynamicPython::Py_Finalize()
{
    _lib.call<void>("Py_Finalize");
}

std::string DynamicPython::findSharedLibrary()
{
    return "";
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

