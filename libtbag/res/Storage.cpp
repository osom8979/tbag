/**
 * @file   Storage.cpp
 * @brief  Storage class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/Storage.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

Storage::Storage() : _impl(std::make_shared<Impl>())
{
    assert(static_cast<bool>(_impl));
}

Storage::Storage(std::nullptr_t) TBAG_NOEXCEPT : _impl(nullptr)
{
    // EMPTY.
}

Storage::Storage(Storage const & obj) TBAG_NOEXCEPT : Storage(nullptr)
{
    (*this) = obj;
}

Storage::Storage(Storage && obj) TBAG_NOEXCEPT : Storage(nullptr)
{
    (*this) = std::move(obj);
}

Storage::~Storage()
{
    // EMPTY.
}

Storage & Storage::operator =(Storage const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

Storage & Storage::operator =(Storage && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Storage::copy(Storage const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void Storage::swap(Storage & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

void Storage::reset()
{
    _impl.reset();
}

bool Storage::setEnv(std::string const & dir, std::string const & set, bool default_set)
{
    if (asset().exists(LAYOUT_ENV)) {
        return false;
    }
    if (!asset().add(LAYOUT_ENV, libtbag::filesystem::Path(dir))) {
        return false;
    }

    Environments temp_envs;
    if (default_set) {
        temp_envs = Environments::createDefaultEnvironments();
    }

    if (!set.empty()) {
        if (set == ENV_UPDATE_ALL) {
            for (auto & file_path : libtbag::filesystem::Path(dir).scanDir()) {
                temp_envs.readResourceXmlFile(file_path);
            }
        } else {
            temp_envs.readResourceXmlFile(libtbag::filesystem::Path(dir) / set);
        }
    }

    envs() = temp_envs;
    return true;
}

bool Storage::setConfig(std::string const & dir)
{
    if (asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    return asset().add(LAYOUT_CONFIG, libtbag::filesystem::Path(dir));
}

bool Storage::readConfig(std::string const & group, std::string const & key, std::string & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    auto const PATH = asset().get(LAYOUT_CONFIG) / group;
    Resource res;
    if (!res.readFile(PATH)) {
        return false;
    }
    return res.getString(key, &value);
}

bool Storage::saveConfig(std::string const & group, std::string const & key, std::string const & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    auto const PATH = asset().get(LAYOUT_CONFIG) / group;
    Resource res;
    res.readFile(PATH);
    res.set(key, value);
    return res.saveFile(PATH);
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

