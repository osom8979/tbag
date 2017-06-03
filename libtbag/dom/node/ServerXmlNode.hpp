/**
 * @file   ServerXmlNode.hpp
 * @brief  ServerXmlNode class prototype.
 * @author zer0
 * @date   2017-06-03
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_DOM_NODE_SERVERXMLNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_DOM_NODE_SERVERXMLNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/XmlModel.hpp>
#include <libtbag/lock/RwLock.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace node {

/**
 * ServerXmlNode class prototype.
 *
 * @author zer0
 * @date   2017-06-03
 */
class TBAG_API ServerXmlNode : public XmlModel::NodeInterface
{
public:
    using Lock       = lock::RwLock;
    using ReadGuard  = lock::ReadLockGuard;
    using WriteGuard = lock::WriteLockGuard;

private:
    mutable Lock _lock;

private:
    std::string _default_var;
    std::string _default_ip;
    int _default_port;

private:
    std::string _var;
    std::string _ip;
    int _port;

public:
    ServerXmlNode();
    ServerXmlNode(std::string const & var, std::string const & ip, int port);
    ServerXmlNode(ServerXmlNode const & obj);
    ServerXmlNode(ServerXmlNode && obj);
    virtual ~ServerXmlNode();

public:
    ServerXmlNode & operator =(ServerXmlNode const & obj);
    ServerXmlNode & operator =(ServerXmlNode && obj);

public:
    virtual std::string name() const override;

public:
    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    std::string getVar() const;
    std::string getIp() const;
    int getPort() const;
};

} // namespace node
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_NODE_SERVERXMLNODE_HPP__

