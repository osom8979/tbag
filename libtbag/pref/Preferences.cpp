/**
 * @file   Preferences.cpp
 * @brief  Preferences class implementation.
 * @author zer0
 * @date   2019-11-03
 */

#include <libtbag/pref/Preferences.hpp>
#include <libtbag/filesystem/Path.hpp>

#include <libtbag/pref/details/ExtraPref.hpp>
#include <libtbag/pref/details/LogPref.hpp>
#include <libtbag/pref/details/StoragePref.hpp>
#include <libtbag/pref/details/ValuesPref.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

Preferences::Preferences()
{
    // EMPTY.
}

Preferences::Preferences(Preferences const & obj)
{
    (*this) = obj;
}

Preferences::Preferences(Preferences && obj) TBAG_NOEXCEPT
{
    (*this) = std::move(obj);
}

Preferences::~Preferences()
{
    // EMPTY.
}

Preferences & Preferences::operator =(Preferences const & obj)
{
    copy(obj);
    return *this;
}

Preferences & Preferences::operator =(Preferences && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Preferences::copy(Preferences const & obj)
{
    if (this != &obj) {
        _nodes = obj._nodes;
    }
}

void Preferences::swap(Preferences & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _nodes.swap(obj._nodes);
    }
}

bool Preferences::add(NodeInterface * node)
{
    assert(node != nullptr);
    return add(SharedNode(node));
}

bool Preferences::add(SharedNode node)
{
    if (!node) {
        return false;
    }
    return _nodes.insert(NodePair(node->name(), node)).second;
}

bool Preferences::remove(std::string const & name)
{
    return _nodes.erase(name) == 1u;
}

Preferences::WeakNode Preferences::get(std::string const & name)
{
    auto itr = _nodes.find(name);
    if (itr == _nodes.end()) {
        return WeakNode();
    }
    return WeakNode(itr->second);
}

bool Preferences::newAddExtraPref()
{
    return static_cast<bool>(newAdd<libtbag::pref::details::ExtraPref>());
}

bool Preferences::newAddLogPref()
{
    return static_cast<bool>(newAdd<libtbag::pref::details::LogPref>());
}

bool Preferences::newAddStoragePref()
{
    return static_cast<bool>(newAdd<libtbag::pref::details::StoragePref>());
}

bool Preferences::newAddValuesPref()
{
    return static_cast<bool>(newAdd<libtbag::pref::details::ValuesPref>());
}

std::size_t Preferences::init()
{
    std::size_t count = 0u;
    for (auto const & cursor : _nodes) {
        if (cursor.second->init()) {
            ++count;
        }
    }
    return count;
}

Err Preferences::load(std::string const & path)
{
    using namespace tinyxml2;
    using namespace libtbag::dom::xml;
    Document doc;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
        return E_PARSING;
    }

    auto const * root = doc.FirstChildElement(TAG_APPLICATION);
    if (root == nullptr) {
        return E_NFOUND;
    }

    for (auto const & cursor : _nodes) {
        auto const name = cursor.second->name();
        if (name.empty()) {
            continue;
        }

        auto const * element = root->FirstChildElement(name.c_str());
        if (element) {
            cursor.second->load(*element);
        }
    }
    return E_SUCCESS;
}

Err Preferences::save(std::string const & path) const
{
    using namespace tinyxml2;
    using namespace libtbag::dom::xml;

    Document doc;
    auto * root = doc.NewElement(TAG_APPLICATION);
    for (auto & cursor : _nodes) {
        auto const name = cursor.second->name();
        if (name.empty()) {
            continue;
        }

        auto * element = doc.NewElement(name.c_str());
        if (element) {
            cursor.second->save(*element);
        }
        root->InsertEndChild(element);
    }
    doc.InsertFirstChild(root);

    if (doc.SaveFile(path.c_str(), false) == tinyxml2::XML_SUCCESS) {
        return E_SUCCESS;
    }
    return E_WRERR;
}

Err Preferences::loadOrDefaultSave(std::string const & config_path, bool create_dir)
{
    libtbag::filesystem::Path path(config_path);
    if (path.exists()) {
        if (path.isReadable()) {
            return load(config_path);
        } else {
            return E_RDERR;
        }
    }

    auto const PARENT_DIR = path.getParent();
    if (create_dir && !PARENT_DIR.isDirectory()) {
        if (!PARENT_DIR.createDir()) {
            return E_CREATE;
        }
    }
    if (!PARENT_DIR.isWritable()) {
        return E_WRERR;
    }

    init();
    return save(config_path);
}

Preferences Preferences::createDefault()
{
    Preferences result;
    result.newAddExtraPref();
    result.newAddLogPref();
    result.newAddStoragePref();
    result.newAddValuesPref();
    assert(result.size() == 4);
    return result;
}

Preferences Preferences::loadDefault(std::string const & path)
{
    auto pref = createDefault();
    auto const code = pref.load(path);
    if (isFailure(code)) {
        return {};
    }
    return pref;
}

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

