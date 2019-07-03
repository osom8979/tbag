/**
 * @file   XmlModel.cpp
 * @brief  XmlModel class implementation.
 * @author zer0
 * @date   2017-04-21
 */

#include <libtbag/dom/xml/XmlModel.hpp>

#include <cassert>
#include <functional>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

TBAG_CONSTEXPR static char const * const DEFAULT_ROOT_NAME = "root";
TBAG_CONSTEXPR static char const * const DEFAULT_FILE_NAME = "config.xml";

// ------------------------
// XmlModel implementation.
// ------------------------

XmlModel::XmlModel()
{
    // EMPTY.
}

XmlModel::XmlModel(XmlModel const & obj)
{
    (*this) = obj;
}

XmlModel::XmlModel(XmlModel && obj)
{
    (*this) = std::move(obj);
}

XmlModel::~XmlModel()
{
    // EMPTY.
}

XmlModel & XmlModel::operator =(XmlModel const & obj)
{
    if (this != &obj) {
        _nodes = obj._nodes;
    }
    return *this;
}

XmlModel & XmlModel::operator =(XmlModel && obj)
{
    if (this != &obj) {
        _nodes.swap(obj._nodes);
    }
    return *this;
}

bool XmlModel::add(NodeInterface * node)
{
    assert(node != nullptr);
    return add(SharedNode(node));
}

bool XmlModel::add(SharedNode node)
{
    if (static_cast<bool>(node) == false) {
        return false;
    }
    return _nodes.insert(NodePair(node->name(), node)).second;
}

bool XmlModel::remove(std::string const & name)
{
    return _nodes.erase(name) == 1U;
}

XmlModel::WeakNode XmlModel::get(std::string const & name)
{
    auto itr = _nodes.find(name);
    if (itr == _nodes.end()) {
        return WeakNode();
    }
    return WeakNode(itr->second);
}

std::string XmlModel::getRootName() const
{
    return DEFAULT_ROOT_NAME;
}

std::string XmlModel::getFileName() const
{
    return DEFAULT_FILE_NAME;
}

XmlModel::Path XmlModel::getWorkDir() const
{
    return Path::getWorkDir();
}

XmlModel::Path XmlModel::getExeDir() const
{
    return Path::getExeDir();
}

XmlModel::Path XmlModel::getHomeDir() const
{
    return Path::getHomeDir();
}

XmlModel::Path XmlModel::getGlobalDir() const
{
#if defined(TBAG_INSTALL_PREFIX)
    return Path(TBAG_INSTALL_PREFIX);
#elif defined(TBAG_PLATFORM_WINDOWS)
    return Path("C:\\Program Files");
#else
    return Path("/usr/local");
#endif
}

XmlModel::Path XmlModel::getFilePath(Scope scope) const
{
    // clang-format off
    if (scope == Scope::WORK  ) { return getWorkDir  () / getFileName(); }
    if (scope == Scope::EXE   ) { return getExeDir   () / getFileName(); }
    if (scope == Scope::HOME  ) { return getHomeDir  () / getFileName(); }
    if (scope == Scope::GLOBAL) { return getGlobalDir() / getFileName(); }
    return Path::getHomeDir() / getFileName();
    // clang-format on
}

XmlModel::Path XmlModel::findExistsFilePathOfNearest() const
{
    // clang-format off
    Path result;
    if (result = getFilePath(Scope::WORK  ), result.exists()) { return result; }
    if (result = getFilePath(Scope::EXE   ), result.exists()) { return result; }
    if (result = getFilePath(Scope::HOME  ), result.exists()) { return result; }
    if (result = getFilePath(Scope::GLOBAL), result.exists()) { return result; }
    return Path();
    // clang-format on
}

XmlModel::Path XmlModel::findWritablePathOfNearest() const
{
    // clang-format off
    Path result;
    if (result = getFilePath(Scope::WORK  ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::EXE   ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::HOME  ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::GLOBAL), result.getParent().isWritable()) { return result; }
    return Path();
    // clang-format on
}

void XmlModel::teardown()
{
    for (auto & cursor : _nodes) {
        cursor.second->teardown();
    }
}

void XmlModel::setup()
{
    for (auto & cursor : _nodes) {
        cursor.second->setup();
    }
}

bool XmlModel::save() const
{
    return save(findWritablePathOfNearest());
}

bool XmlModel::save(Scope scope) const
{
    return save(getFilePath(scope));
}

bool XmlModel::save(Path const & path) const
{
    Document doc;
    Node * root = doc.NewElement(getRootName().c_str());
    for (auto & cursor : _nodes) {
        auto name = cursor.second->name();
        if (name.empty() == false) {
            Element * element = doc.NewElement(name.c_str());
            if (element != nullptr) {
                cursor.second->save(*element);
            }
            root->InsertEndChild(element);
        }
    }
    doc.InsertFirstChild(root);
    return doc.SaveFile(path.getString().c_str(), false) == tinyxml2::XML_SUCCESS;
}

bool XmlModel::load()
{
    return load(findExistsFilePathOfNearest());
}

bool XmlModel::load(Scope scope)
{
    return load(getFilePath(scope));
}

bool XmlModel::load(Path const & path)
{
    using namespace tinyxml2;
    Document doc;
    if (doc.LoadFile(path.getString().c_str()) != tinyxml2::XML_SUCCESS) {
        return false;
    }

    Element const * root = doc.FirstChildElement(getRootName().c_str());
    if (root == nullptr) {
        return false;
    }

    for (auto & cursor : _nodes) {
        auto name = cursor.second->name();
        if (name.empty() == false) {
            Element const * element = root->FirstChildElement(name.c_str());
            if (element != nullptr) {
                cursor.second->load(*element);
            }
        }
    }
    return true;
}

bool XmlModel::loadOrDefaultSave(bool create_dir)
{
    Path path = findExistsFilePathOfNearest();
    if (path.empty()) {
        path = findWritablePathOfNearest();
    }
    return loadOrDefaultSave(path, create_dir);
}

bool XmlModel::loadOrDefaultSave(Scope scope, bool create_dir)
{
    return loadOrDefaultSave(getFilePath(scope), create_dir);
}

bool XmlModel::loadOrDefaultSave(Path const & path, bool create_dir)
{
    if (path.exists()) {
        if (path.isReadable()) {
            return load(path);
        } else {
            return false;
        }
    }

    auto const PARENT_DIR = path.getParent();
    if (create_dir && PARENT_DIR.isDirectory() == false && PARENT_DIR.createDir() == false) {
        return false;
    }

    if (PARENT_DIR.isWritable() == false) {
        return false;
    }

    teardown();
    setup();

    return save(path);
}

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

