/**
 * @file   Storage.cpp
 * @brief  Storage class implementation.
 * @author zer0
 * @date   2018-11-03
 */

#include <libtbag/res/Storage.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/string/StringUtils.hpp>

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

std::vector<std::string> Storage::getFilenames(std::string const & key) const
{
    std::vector<std::string> result;
    if (asset().exists(key)) {
        for (auto & cursor : asset().get(key).scanNameOnly()) {
            result.push_back(cursor);
        }
    }
    return result;
}

void Storage::setEnvFilename(std::string const & filename)
{
    _impl->envs_filename = filename;
}

void Storage::setLayoutEnv(std::string const & dir, std::string const & filename, bool auto_load)
{
    setLayoutEnv(dir);
    setEnvFilename(filename);
    if (auto_load && !readEnv()) {
        tDLogE("Storage::setLayoutEnv() auto load failure: {}", libtbag::filesystem::Path(dir) / filename);
    }
}

bool Storage::readEnv()
{
    if (_impl->envs_filename.empty()) {
        return false;
    }
    if (!asset().exists(LAYOUT_ENV)) {
        return false;
    }
    _impl->envs.readResourceXmlFile(asset().get(LAYOUT_ENV) / _impl->envs_filename);
    return true;
}

void Storage::readEnvDefault()
{
    _impl->envs = Environments::createDefaultEnvironments();
}

void Storage::readEnvParams(char ** envs)
{
    if (envs != nullptr) {
        _impl->envs.parse(envs);
    }
}

bool Storage::saveEnv()
{
    if (_impl->envs_filename.empty()) {
        return false;
    }
    if (!asset().exists(LAYOUT_ENV)) {
        return false;
    }
    auto const ENV_DIR = asset().get(LAYOUT_ENV);
    if (!ENV_DIR.exists()) {
        ENV_DIR.createDir();
    }
    if (!ENV_DIR.isDirectory() || !ENV_DIR.isWritable()) {
        return false;
    }
    return _impl->envs.saveResourceXmlFile(ENV_DIR / _impl->envs_filename);
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

bool Storage::readConfig(std::string const & filename, std::string const & key, std::string & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    Resource res;
    if (!res.readFile(asset().get(LAYOUT_CONFIG) / filename)) {
        return false;
    }
    return res.getString(key, &value);
}

bool Storage::saveConfig(std::string const & filename, std::string const & key, std::string const & value)
{
    if (!asset().exists(LAYOUT_CONFIG)) {
        return false;
    }
    auto const CONFIG_DIR = asset().get(LAYOUT_CONFIG);
    if (!CONFIG_DIR.exists()) {
        CONFIG_DIR.createDir();
    }
    if (!CONFIG_DIR.isDirectory() || !CONFIG_DIR.isWritable()) {
        return false;
    }
    auto const PATH = CONFIG_DIR / filename;
    Resource res;
    res.readFile(PATH);
    res.set(key, value);
    return res.saveFile(PATH);
}

std::vector<std::string> Storage::getConfigFilenames() const
{
    return getFilenames(LAYOUT_CONFIG);
}

std::vector<std::string> Storage::getConfigKeys(std::string const & filename) const
{
    std::vector<std::string> result;
    if (asset().exists(LAYOUT_CONFIG)) {
        Resource res;
        if (res.readFile(asset().get(LAYOUT_CONFIG) / filename)) {
            return res.keys();
        }
    }
    return result;
}

void Storage::removeConfig(std::string const & filename)
{
    if (asset().exists(LAYOUT_CONFIG)) {
        (asset().get(LAYOUT_CONFIG) / filename).remove();
    }
}

void Storage::removeAllConfig()
{
    for (auto const & filename : getConfigFilenames()) {
        removeConfig(filename);
    }
}

void Storage::setModuleExtension(std::string const & extension)
{
    _impl->module_extension = extension;
}

void Storage::setLayoutModule(std::string const & dir, std::string const & extension)
{
    setLayoutModule(dir);
    setModuleExtension(extension);
}

std::vector<std::string> Storage::getModuleFilenames() const
{
    using namespace libtbag::filesystem;
    using namespace libtbag::string;

    auto const LOWER_EXTENSION = lower(_impl->module_extension);
    if (LOWER_EXTENSION.empty()) {
        return getFilenames(LAYOUT_MODULE);
    }

    std::vector<std::string> result;
    for (auto & cursor : getFilenames(LAYOUT_MODULE)) {
        if (LOWER_EXTENSION == lower(Path(cursor).getExtensionName())) {
            result.push_back(cursor);
        }
    }
    return result;
}

Storage::SharedLibrary Storage::getModule(std::string const & filename) const
{
    if (asset().exists(LAYOUT_MODULE)) {
        return SharedLibrary(asset().get(LAYOUT_MODULE) / filename);
    }
    return SharedLibrary(nullptr);
}

void Storage::setTextLanguage(std::string const & language)
{
    _impl->text.setLanguage(language);
}

void Storage::setLayoutText(std::string const & dir, std::string const & language)
{
    setLayoutText(dir);
    setTextLanguage(language);
}

std::vector<std::string> Storage::getTextFilenames() const
{
    return getFilenames(LAYOUT_TEXT);
}

std::string Storage::getText(std::string const & language, std::string const & name) const
{
    return _impl->text.get(language, name);
}

std::string Storage::getText(std::string const & name) const
{
    return _impl->text.get(name);
}

void Storage::setText(std::string const & language, std::string const & name, std::string const & text)
{
    _impl->text.set(language, name, text);
}

void Storage::setText(std::string const & name, std::string const & text)
{
    _impl->text.set(name, text);
}

bool Storage::loadText()
{
    return _impl->text.load(asset().get(LAYOUT_TEXT));
}

bool Storage::saveText()
{
    return _impl->text.save(asset().get(LAYOUT_TEXT));
}

std::vector<std::string> Storage::getImageFilenames() const
{
    return getFilenames(LAYOUT_IMAGE);
}

bool Storage::loadImage(std::string const & filename, Image & image)
{
    return libtbag::graphic::readImage(asset().get(LAYOUT_IMAGE) / filename, image) == Err::E_SUCCESS;
}

bool Storage::saveImage(std::string const & filename, Image const & image)
{
    return libtbag::graphic::writeImage(asset().get(LAYOUT_IMAGE) / filename, image) == Err::E_SUCCESS;
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

