/**
 * @file   XmlModel.hpp
 * @brief  XmlModel class prototype.
 * @author zer0
 * @date   2017-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLMODEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLMODEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/filesystem/Path.hpp>
#include <libtbag/dom/tinyxml2/tinyxml2.h>
#include <libtbag/dom/xml/XmlHelper.hpp>

#include <cassert>
#include <memory>
#include <string>
#include <map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {
namespace xml {

/**
 * XmlModel class prototype.
 *
 * @author zer0
 * @date   2017-04-21
 */
class TBAG_API XmlModel
{
public:
    using Document = tinyxml2::XMLDocument;
    using Element  = tinyxml2::XMLElement;
    using Node     = tinyxml2::XMLNode;
    using Path     = filesystem::Path;

public:
    enum class Scope
    {
        WORK,   ///< First step.
        EXE,    ///< Second step.
        HOME,   ///< Third step.
        GLOBAL, ///< Fourth step.
    };

public:
    /**
     * NodeInterface class prototype.
     *
     * @author zer0
     * @date   2017-04-21
     */
    struct NodeInterface : public XmlHelper
    {
        using Document = XmlModel::Document;
        using Element  = XmlModel::Element;
        using Node     = XmlModel::Node;

        STATIC_ASSERT_CHECK_IS_SAME(Document, XmlHelper::Document);
        STATIC_ASSERT_CHECK_IS_SAME(Element , XmlHelper::Element );

        NodeInterface() { /* EMPTY. */ }
        virtual ~NodeInterface() { /* EMPTY. */ }

        virtual std::string name() const = 0;

        virtual void setup() { /* EMPTY. */ }
        virtual void teardown() { /* EMPTY. */ }

        virtual void load(Element const & element) { /* EMPTY. */ };
        virtual void save(Element & element) const { /* EMPTY. */ };

        bool loadFromXmlFile(std::string const & path)
        {
            Document doc;
            if (doc.LoadFile(path.c_str()) != tinyxml2::XML_SUCCESS) {
                return false;
            }
            Element const * root = doc.FirstChildElement(name().c_str());
            if (root == nullptr) {
                return false;
            }
            load(*root);
            return true;
        }

        bool loadFromXmlText(std::string const & xml)
        {
            Document doc;
            if (doc.Parse(xml.c_str()) != tinyxml2::XML_SUCCESS) {
                return false;
            }
            Element const * root = doc.FirstChildElement(name().c_str());
            if (root == nullptr) {
                return false;
            }
            load(*root);
            return true;
        }

        bool writeToXmlFile(std::string const & path, bool compact = false) const
        {
            Document doc;
            auto * element = newElement(doc, name());
            assert(element != nullptr);
            save(*element);
            doc.InsertFirstChild(element);
            return doc.SaveFile(path.c_str(), compact) == tinyxml2::XML_SUCCESS;
        }

        bool writeToXmlText(std::string & xml, bool compact = false) const
        {
            Document doc;
            auto * element = newElement(doc, name());
            assert(element != nullptr);
            save(*element);
            doc.InsertFirstChild(element);
            return writeToXml(doc, xml, compact) == E_SUCCESS;
        }
    };

public:
    using SharedNode = std::shared_ptr<NodeInterface>;
    using WeakNode   = std::weak_ptr<NodeInterface>;
    using NodeMap    = std::map<std::string, SharedNode>;
    using NodePair   = NodeMap::value_type;

private:
    NodeMap _nodes;

public:
    XmlModel();
    XmlModel(XmlModel const & obj);
    XmlModel(XmlModel && obj);
    virtual ~XmlModel();

public:
    XmlModel & operator =(XmlModel const & obj);
    XmlModel & operator =(XmlModel && obj);

public:
    inline void clear() TBAG_NOEXCEPT_SP_OP(_nodes.clear())
    { _nodes.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_SP_OP(_nodes.size())
    { return _nodes.size(); }
    inline bool empty() const TBAG_NOEXCEPT_SP_OP(_nodes.empty())
    { return _nodes.empty(); }

public:
    bool add(NodeInterface * node);
    bool add(SharedNode node);
    bool remove(std::string const & name);

public:
    WeakNode get(std::string const & name);

public:
    /** Create(new) & add node. */
    template <typename Up, typename ... Args>
    inline std::shared_ptr<typename remove_cr<Up>::type> newAdd(Args && ... args)
    {
        typedef typename remove_cr<Up>::type ResultObjectType;
        SharedNode shared(new (std::nothrow) ResultObjectType(std::forward<Args>(args) ...));
        if (shared != nullptr && add(shared)) {
            return std::static_pointer_cast<ResultObjectType>(shared);
        }
        return std::shared_ptr<ResultObjectType>();
    }

    template <typename Up>
    Up * getPointer()
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(NodeInterface, Up);
        if (auto shared = get(Up().name()).lock()) {
            return static_cast<Up*>(shared.get());
        }
        return nullptr;
    }

    template <typename Up>
    std::weak_ptr<Up> getWeak()
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(NodeInterface, Up);
        if (auto shared = get(Up().name()).lock()) {
            return std::weak_ptr<Up>(std::static_pointer_cast<Up>(shared));
        }
        return std::weak_ptr<Up>();
    }

public:
    virtual std::string getRootName() const;
    virtual std::string getFileName() const;

    virtual Path getWorkDir() const;
    virtual Path getExeDir() const;
    virtual Path getHomeDir() const;
    virtual Path getGlobalDir() const;

public:
    Path getFilePath(Scope scope) const;
    Path findExistsFilePathOfNearest() const;
    Path findWritablePathOfNearest() const;

public:
    void teardown();
    void setup();

public:
    bool save() const;
    bool save(Scope scope) const;
    bool save(Path const & path) const;

public:
    bool load();
    bool load(Scope scope);
    bool load(Path const & path);

public:
    bool loadOrDefaultSave(bool create_dir = false);
    bool loadOrDefaultSave(Scope scope, bool create_dir = false);
    bool loadOrDefaultSave(Path const & path, bool create_dir = false);
};

} // namespace xml
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XML_XMLMODEL_HPP__

