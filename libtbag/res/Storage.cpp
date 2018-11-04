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

void Storage::clear()
{
    _impl = std::make_shared<Impl>();
}

void Storage::setEnv(std::string const & dir, std::string const & set, char ** envs, bool default_set)
{
    Environments temp;
    if (default_set) {
        temp = Environments::createDefaultEnvironments();
    }
    if (envs != nullptr) {
        temp.parse(envs);
    }

    if (!set.empty()) {
        if (set == ENV_UPDATE_ALL) {
            for (auto & file_path : libtbag::filesystem::Path(dir).scanDir()) {
                temp.readResourceXmlFile(file_path);
            }
        } else {
            temp.readResourceXmlFile(libtbag::filesystem::Path(dir) / set);
        }
    }

    _impl->envs = temp;
    asset().set(LAYOUT_ENV, libtbag::filesystem::Path(dir));
}

void Storage::setConfig(std::string const & dir)
{
    asset().set(LAYOUT_CONFIG, libtbag::filesystem::Path(dir));
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

