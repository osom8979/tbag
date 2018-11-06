/**
 * @file   GetText.cpp
 * @brief  GetText class implementation.
 * @author zer0
 * @date   2018-11-06
 */

#include <libtbag/res/GetText.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

GetText::GetText() : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

GetText::GetText(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

GetText::GetText(GetText const & obj) TBAG_NOEXCEPT : GetText(nullptr)
{
    (*this) = obj;
}

GetText::GetText(GetText && obj) TBAG_NOEXCEPT : GetText(nullptr)
{
    (*this) = std::move(obj);
}

GetText::~GetText()
{
    // EMPTY.
}

GetText & GetText::operator =(GetText const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

GetText & GetText::operator =(GetText && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void GetText::copy(GetText const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
        _current = obj._current;
    }
}

void GetText::swap(GetText & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
        std::swap(_current, obj._current);
    }
}

void GetText::reset()
{
    _impl.reset();
    _current = nullptr;
}

void GetText::clear()
{
    assert(_impl != nullptr);
    _impl->map.clear();
    _impl->language.clear();
    _current = nullptr;
}

bool GetText::empty() const
{
    assert(_impl != nullptr);
    return _impl->map.empty();
}

std::size_t GetText::size() const
{
    assert(_impl != nullptr);
    return _impl->map.size();
}

void GetText::setLanguage(std::string const & language, bool auto_update)
{
    assert(_impl != nullptr);
    _impl->language = language;
    if (auto_update) {
        updateCache();
    }
}

std::string GetText::getLanguage() const
{
    assert(_impl != nullptr);
    return _impl->language;
}

std::vector<std::string> GetText::getLanguages() const
{
    assert(_impl != nullptr);
    std::vector<std::string> result;
    for (auto const & cursor : _impl->map) {
        result.push_back(cursor.first);
    }
    return result;
}

bool GetText::read(std::string const & dir)
{
    assert(_impl != nullptr);
    auto const ROOT_DIR = libtbag::filesystem::Path(dir);
    if (!ROOT_DIR.isDirectory() || !ROOT_DIR.isReadable()) {
        return false;
    }
    for (auto const & cursor : ROOT_DIR.scanNameOnly()) {
        _impl->map.insert(std::make_pair(cursor, Resource::createFromXmlFile(ROOT_DIR / cursor, ROOT_TAG, PROP_TAG, NAME_ATT)));
    }
    return true;
}

bool GetText::save(std::string const & dir) const
{
    assert(_impl != nullptr);
    auto const ROOT_DIR = libtbag::filesystem::Path(dir);
    if (!ROOT_DIR.isDirectory() || !ROOT_DIR.isWritable()) {
        return false;
    }
    for (auto & cursor : _impl->map) {
        if (!cursor.second.saveFile(ROOT_DIR / cursor.first)) {
            return false;
        }
    }
    return true;
}

bool GetText::updateCache()
{
    assert(_impl != nullptr);
    auto itr = _impl->map.find(_impl->language);
    if (itr != _impl->map.end()) {
        _current = &(itr->second);
        return true;
    }
    return false;
}

void GetText::set(std::string const & language, std::string const & name, std::string const & text)
{
    assert(_impl != nullptr);
    assert(!language.empty());
    assert(!name.empty());
    assert(!text.empty());

    auto itr = _impl->map.find(language);
    if (itr == _impl->map.end()) {
        Resource res(ROOT_TAG, PROP_TAG, NAME_ATT);
        res.set(name, text);
        _impl->map.insert(std::make_pair(language, std::move(res)));
    } else {
        itr->second.set(name, text);
    }
}

void GetText::set(std::string const & name, std::string const & text)
{
    assert(_impl != nullptr);
    assert(!name.empty());
    assert(!text.empty());

    if (_current == nullptr) {
        set(_impl->language, name, text);
    }
    _current->set(name, text);
}

std::string GetText::get(std::string const & language, std::string const & name) const
{
    assert(_impl != nullptr);
    if (language.empty() || name.empty()) {
        return std::string();
    }
    auto itr = _impl->map.find(language);
    if (itr != _impl->map.end()) {
        return itr->second.getString(name);
    }
    return std::string();
}

std::string GetText::get(std::string const & name) const
{
    assert(_impl != nullptr);
    if (name.empty()) {
        return std::string();
    }
    if (_current == nullptr) {
        return get(_impl->language, name);
    }
    return _current->getString(name);
}

std::string const & GetText::at(std::string const & language, std::string const & name) const
{
    assert(_impl != nullptr);
    assert(!language.empty());
    assert(!name.empty());

    return _impl->map.find(language)->second.at(name);
}

std::string const & GetText::at(std::string const & name) const
{
    assert(_impl != nullptr);
    assert(!name.empty());

    if (_current == nullptr) {
        return at(_impl->language, name);
    }
    return _current->at(name);
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

