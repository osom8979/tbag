/**
 * @file   DynamicPython.cpp
 * @brief  DynamicPython class implementation.
 * @author zer0
 * @date   2018-10-31
 */

#include <libtbag/script/DynamicPython.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/lib/SharedLibrary.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace script {

/**
 * DynamicPython::Impl class implementation.
 *
 * @author zer0
 * @date   2018-10-31
 */
struct DynamicPython::Impl : private Noncopyable
{
public:
    using SharedLibrary = libtbag::lib::SharedLibrary;

public:
    SharedLibrary lib;
    std::string   path;

public:
    Impl() { /* EMPTY. */ }
    ~Impl() { /* EMPTY. */ }
};

// ----------------------------
// DynamicPython implementation.
// ----------------------------

DynamicPython::DynamicPython() : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

DynamicPython::DynamicPython(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

DynamicPython::DynamicPython(DynamicPython const & obj) TBAG_NOEXCEPT : DynamicPython(nullptr)
{
    (*this) = obj;
}

DynamicPython::DynamicPython(DynamicPython && obj) TBAG_NOEXCEPT : DynamicPython(nullptr)
{
    (*this) = std::move(obj);
}

DynamicPython::~DynamicPython()
{
    // EMPTY.
}

DynamicPython & DynamicPython::operator =(DynamicPython const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

DynamicPython & DynamicPython::operator =(DynamicPython && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void DynamicPython::copy(DynamicPython const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void DynamicPython::swap(DynamicPython & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

bool DynamicPython::isOpen() const
{
    assert(exists());
    return _impl->lib.isOpen();
}

std::string DynamicPython::getPath() const
{
    assert(exists());
    return _impl->path;
}

bool DynamicPython::open(std::string const & path)
{
    assert(exists());
    bool const RESULT = _impl->lib.open(path);
    if (RESULT) {
        _impl->path = path;
    }
    return RESULT;
}

void DynamicPython::close()
{
    assert(exists());
    _impl->lib.close();
    _impl->path.clear();
}

void DynamicPython::Py_Initialize()
{
    assert(exists());
    _impl->lib.call<void>("Py_Initialize");
}

void DynamicPython::Py_Finalize()
{
    assert(exists());
    _impl->lib.call<void>("Py_Finalize");
}

} // namespace script

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

