/**
 * @file   XmlModel.hpp
 * @brief  XmlModel class prototype.
 * @author zer0
 * @date   2017-04-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_XMLMODEL_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_XMLMODEL_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/3rd/tinyxml2/tinyxml2.h>
#include <libtbag/Type.hpp>

#include <map>
#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom {

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
    struct NodeInterface
    {
        using Document = XmlModel::Document;
        using Element  = XmlModel::Element;
        using Node     = XmlModel::Node;

        NodeInterface() { /* EMPTY. */ }
        virtual ~NodeInterface() { /* EMPTY. */ }

        virtual std::string name() const = 0;

        virtual void setup() = 0;
        virtual void teardown() = 0;

        virtual void load(Element const & element) = 0;
        virtual void save(Element & element) const = 0;
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
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_nodes.clear()))
    { _nodes.clear(); }
    inline std::size_t size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_nodes.size()))
    { return _nodes.size(); }
    inline bool empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_nodes.empty()))
    { return _nodes.empty(); }

public:
    bool add(SharedNode model);
    bool remove(std::string const & name);

public:
    WeakNode get(std::string const & name);

public:
    template <typename Up>
    Up * getPointer()
    {
        STATIC_ASSERT_CHECK_IS_BASE_OF(NodeInterface, Up);
        if (auto shared = get(Up().name()).lock()) {
            return static_cast<Up*>(shared.get());
        }
        return nullptr;
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
    bool loadOrDefaultSave();
    bool loadOrDefaultSave(Scope scope);
    bool loadOrDefaultSave(Path const & path);
};

} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_XMLMODEL_HPP__

