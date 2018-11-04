/**
 * @file   Storage.cpp
 * @brief  Storage class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/Storage.hpp>
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

void Storage::clear()
{
    _impl = std::make_shared<Impl>();
}

bool Storage::updateEnvSet(std::string const & set)
{
    if (set.empty()) {
        return false;
    }
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    auto const ENV_DIR = asset().get(LAYOUT_ENV);
    if (set == ENV_UPDATE_ALL) {
        for (auto & file_path : ENV_DIR.scanDir()) {
            _impl->envs.readResourceXmlFile(file_path);
        }
    } else {
        _impl->envs.readResourceXmlFile(ENV_DIR / set);
    }
    return true;
}

void Storage::updateDefaultEnv()
{
    _impl->envs = Environments::createDefaultEnvironments();
}

void Storage::updateEnvParams(char ** envs)
{
    if (envs != nullptr) {
        _impl->envs.parse(envs);
    }
}

void Storage::clearEnv()
{
    _impl->envs.clear();
}

void Storage::setEnv(std::string const & key, std::string const & value)
{
    _impl->envs.set(key, value);
}

bool Storage::getEnv(std::string const & key, std::string & value) const
{
    return _impl->envs.get(key, value);
}

std::string Storage::convert(std::string const & value) const
{
    return _impl->envs.convert(value);
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

std::vector<std::string> Storage::getConfigGroups() const
{
    std::vector<std::string> result;
    if (asset().exists(LAYOUT_CONFIG)) {
        for (auto & cursor : asset().get(LAYOUT_CONFIG).scanDir()) {
            result.push_back(cursor);
        }
    }
    return result;
}

std::vector<std::string> Storage::getConfigKeys(std::string const & group) const
{
    std::vector<std::string> result;
    if (asset().exists(LAYOUT_CONFIG)) {
        Resource res;
        if (res.readFile(asset().get(LAYOUT_CONFIG) / group)) {
            return res.keys();
        }
    }
    return result;
}

void Storage::removeConfig(std::string const & group)
{
    if (asset().exists(LAYOUT_CONFIG)) {
        (asset().get(LAYOUT_CONFIG) / group).remove();
    }
}

void Storage::removeAllConfig()
{
    for (auto & group : getConfigGroups()) {
        libtbag::filesystem::Path(group).remove();
    }
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

