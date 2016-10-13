/**
 * @file   DynamicAsset.cpp
 * @brief  DynamicAsset class implementation.
 * @author zer0
 * @date   2016-10-13
 */

#include <libtbag/res/DynamicAsset.hpp>
#include <libtbag/filesystem/Common.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

bool DynamicAsset::init()
{
    for (auto & path : _paths) {
        if (createDir(path.second) == false) {
            return false;
        }
    }
    return true;
}

bool DynamicAsset::addPath(String const & key, Path const & path)
{
    return _paths.insert(PathMap::value_type(key, path)).second;
}

DynamicAsset::Path DynamicAsset::getPath(String const & key)
{
    return _paths.at(key);
}

bool DynamicAsset::exists(String const & key)
{
    return filesystem::common::isDirectory(_paths.at(key));
}

bool DynamicAsset::create(String const & key)
{
    return createDir(_paths.at(key));
}

bool DynamicAsset::remove(String const & key)
{
    return removeDir(_paths.at(key));
}

DynamicAsset::PathVector DynamicAsset::scan(String const & key)
{
    return scanDir(_paths.at(key));
}

bool DynamicAsset::existsFile(Path const & path)
{
    return filesystem::common::existsFile(path);
}

bool DynamicAsset::removeFile(Path const & path)
{
    return filesystem::common::remove(path);
}

bool DynamicAsset::existsDir(Path const & path)
{
    return filesystem::common::isDirectory(path);
}

bool DynamicAsset::createDir(Path const & path)
{
    if (filesystem::common::existsFile(path)) {
        return false;
    }
    return filesystem::common::createDir(path);
}

bool DynamicAsset::removeDir(Path const & path)
{
    if (filesystem::common::isDirectory(path)) {
        return filesystem::common::removeDir(path);
    }
    return false;
}

DynamicAsset::PathVector DynamicAsset::scanDir(Path const & path)
{
    PathVector result;
    if (existsDir(path)) {
        for (auto & cursor : filesystem::common::scanDir(path)) {
            result.push_back(Path(cursor));
        }
    }
    return result;
}

DynamicAsset::Path DynamicAsset::getHomeDirPath()
{
    return Path(filesystem::common::getHomeDir());
}

DynamicAsset::Path DynamicAsset::getExeDirPath()
{
    return Path(filesystem::common::getExeDir());
}

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

