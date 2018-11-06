/**
 * @file   SharedLibrary.cpp
 * @brief  SharedLibrary class implementation.
 * @author zer0
 * @date   2018-11-06
 */

#include <libtbag/lib/SharedLibrary.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace lib {

SharedLibrary::SharedLibrary() : _module(std::make_shared<DynamicModule>())
{
    assert(static_cast<bool>(_module));
}

SharedLibrary::SharedLibrary(std::string const & path) : _module(std::make_shared<DynamicModule>(path))
{
    assert(static_cast<bool>(_module));
}

SharedLibrary::SharedLibrary(std::nullptr_t) TBAG_NOEXCEPT : _module(nullptr)
{
    // EMPTY.
}

SharedLibrary::SharedLibrary(SharedLibrary const & obj) TBAG_NOEXCEPT : SharedLibrary(nullptr)
{
    (*this) = obj;
}

SharedLibrary::SharedLibrary(SharedLibrary && obj) TBAG_NOEXCEPT : SharedLibrary(nullptr)
{
    (*this) = std::move(obj);
}

SharedLibrary::~SharedLibrary()
{
    // EMPTY.
}

SharedLibrary & SharedLibrary::operator =(SharedLibrary const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

SharedLibrary & SharedLibrary::operator =(SharedLibrary && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void SharedLibrary::copy(SharedLibrary const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _module = obj._module;
    }
}

void SharedLibrary::swap(SharedLibrary & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _module.swap(obj._module);
    }
}

void SharedLibrary::reset()
{
    _module.reset();
}

bool SharedLibrary::open(std::string const & path)
{
    if (_module) {
        return _module->open(path);
    }
    return false;
}

void SharedLibrary::close()
{
    if (_module) {
        _module->close();
    }
}

void * SharedLibrary::symbol(char const * name) const
{
    if (_module) {
        return _module->symbol(name);
    }
    return nullptr;
}

void * SharedLibrary::symbol(std::string const & name) const
{
    if (_module) {
        return _module->symbol(name);
    }
    return nullptr;
}

std::string SharedLibrary::getError() const
{
    if (_module) {
        return _module->getError();
    }
    return std::string();
}

bool SharedLibrary::isOpen() const TBAG_NOEXCEPT
{
    if (_module) {
        return _module->isOpen();
    }
    return false;
}

} // namespace lib

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

