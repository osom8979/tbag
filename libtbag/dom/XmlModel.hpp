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

    using Path   = filesystem::Path;
    using String = std::string;
    using Size   = std::size_t;

public:
    enum class Scope
    {
        WORK,
        EXE,
        HOME,
        GLOBAL,
    };

public:
    struct Interface
    {
        virtual String name() const = 0;

        virtual void teardown() = 0;
        virtual void setup() = 0;

        virtual void load(Element const & doc) = 0;
        virtual void save(Element & doc) const = 0;
    };

public:
    using SharedModel = std::shared_ptr<Interface>;
    using WeakModel   = std::weak_ptr<Interface>;
    using ModelMap    = std::map<String, SharedModel>;
    using ModelPair   = ModelMap::value_type;

private:
    ModelMap _models;

public:
    XmlModel();
    XmlModel(XmlModel const & obj);
    XmlModel(XmlModel && obj);
    virtual ~XmlModel();

public:
    XmlModel & operator =(XmlModel const & obj);
    XmlModel & operator =(XmlModel && obj);

public:
    inline void clear() TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_models.clear()))
    { _models.clear(); }
    inline Size size() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_models.size()))
    { return _models.size(); }
    inline Size empty() const TBAG_NOEXCEPT_EXPR(TBAG_NOEXCEPT_EXPR(_models.empty()))
    { return _models.empty(); }

public:
    bool add(SharedModel model);
    bool remove(String const & name);

public:
    WeakModel get(String const & name);

public:
    virtual String getRootName() const;
    virtual String getFileName() const;

    virtual Path getWorkDir() const;
    virtual Path getExeDir() const;
    virtual Path getHomeDir() const;
    virtual Path getGlobalDir() const;

public:
    Path getFilePath(Scope scope) const;
    Path findExistsFilePathOfNearest() const;

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

