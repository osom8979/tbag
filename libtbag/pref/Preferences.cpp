/**
 * @file   Preferences.cpp
 * @brief  Preferences class implementation.
 * @author zer0
 * @date   2019-11-03
 */

#include <libtbag/pref/Preferences.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace pref {

Preferences::Preferences() : Preferences(TAG_APPLICATION)
{
    // EMPTY.
}

Preferences::Preferences(std::string const & root) : _root(root)
{
    // EMPTY.
}

Preferences::Preferences(Preferences const & obj) : Preferences()
{
    (*this) = obj;
}

Preferences::Preferences(Preferences && obj) TBAG_NOEXCEPT : Preferences()
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
        _root = obj._root;
        _nodes = obj._nodes;
    }
}

void Preferences::swap(Preferences & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _root.swap(obj._root);
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
    return _nodes.erase(name) == 1U;
}

Preferences::WeakNode Preferences::get(std::string const & name)
{
    auto itr = _nodes.find(name);
    if (itr == _nodes.end()) {
        return WeakNode();
    }
    return WeakNode(itr->second);
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

    auto const * root = doc.FirstChildElement(_root.c_str());
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
    auto * root = doc.NewElement(_root.c_str());
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

} // namespace pref

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

