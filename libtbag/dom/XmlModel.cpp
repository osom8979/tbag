/**
 * @file   XmlModel.cpp
 * @brief  XmlModel class implementation.
 * @author zer0
 * @date   2017-04-21
 */

#include <libtbag/dom/XmlModel.hpp>
#include <libtbag/log/Log.hpp>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

TBAG_CONSTEXPR char const * const DEFAULT_ROOT_NAME = "root";
TBAG_CONSTEXPR char const * const DEFAULT_FILE_NAME = "config.xml";

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

bool XmlModel::add(SharedNode model)
{
    if (static_cast<bool>(model) == false) {
        return false;
    }
    return _nodes.insert(NodePair(model->name(), model)).second;
}

bool XmlModel::remove(String const & name)
{
    return _nodes.erase(name) == 1U;
}

XmlModel::WeakNode XmlModel::get(String const & name)
{
    auto itr = _nodes.find(name);
    if (itr == _nodes.end()) {
        return WeakNode();
    }
    return WeakNode(itr->second);
}

XmlModel::String XmlModel::getRootName() const
{
    return DEFAULT_ROOT_NAME;
}

XmlModel::String XmlModel::getFileName() const
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
    return Path();
}

XmlModel::Path XmlModel::getFilePath(Scope scope) const
{
    // @formatter:off
    if (scope == Scope::WORK  ) { return getWorkDir  () / getFileName(); }
    if (scope == Scope::EXE   ) { return getWorkDir  () / getFileName(); }
    if (scope == Scope::HOME  ) { return getExeDir   () / getFileName(); }
    if (scope == Scope::GLOBAL) { return getGlobalDir() / getFileName(); }
    return Path::getHomeDir() / getFileName();
    // @formatter:on
}

XmlModel::Path XmlModel::findExistsFilePathOfNearest() const
{
    // @formatter:off
    Path result;
    if (result = getFilePath(Scope::WORK  ), result.exists()) { return result; }
    if (result = getFilePath(Scope::EXE   ), result.exists()) { return result; }
    if (result = getFilePath(Scope::HOME  ), result.exists()) { return result; }
    if (result = getFilePath(Scope::GLOBAL), result.exists()) { return result; }
    return Path();
    // @formatter:on
}

XmlModel::Path XmlModel::findWritablePathOfNearest() const
{
    // @formatter:off
    Path result;
    if (result = getFilePath(Scope::WORK  ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::EXE   ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::HOME  ), result.getParent().isWritable()) { return result; }
    if (result = getFilePath(Scope::GLOBAL), result.getParent().isWritable()) { return result; }
    return Path();
    // @formatter:on
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
    return doc.SaveFile(path.getString().c_str(), false) == tinyxml2::XML_NO_ERROR;
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
    if (doc.LoadFile(path.getString().c_str()) != tinyxml2::XML_NO_ERROR) {
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

bool XmlModel::loadOrDefaultSave()
{
    Path path = findExistsFilePathOfNearest();
    if (path.empty()) {
        path = findWritablePathOfNearest();
    }
    return loadOrDefaultSave(path);
}

bool XmlModel::loadOrDefaultSave(Scope scope)
{
    return loadOrDefaultSave(getFilePath(scope));
}

bool XmlModel::loadOrDefaultSave(Path const & path)
{
    if (path.exists()) {
        if (path.isReadable()) {
            return load(path);
        } else {
            return false;
        }
    }

    if (path.getParent().isWritable() == false) {
        return false;
    }

    teardown();
    setup();

    return save(path);
}

} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

