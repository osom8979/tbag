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
 * SocketAddressInfo class prototype.
 *
 * @author zer0
 * @date   2017-07-28
 */
struct TBAG_API SocketAddressInfo : protected XmlHelper
{
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_ENDABLE = "enable";
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_IP      = "ip";
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_PORT    = "port";

    bool enable;
    std::string ip;
    int port;

    SocketAddressInfo();
    SocketAddressInfo(std::string const & ip, int port, bool enable = true);
    ~SocketAddressInfo();

    void setProperties(std::string const & ip, int port, bool enable = true);
    void clear();
    void swap(SocketAddressInfo & obj);

    bool load(Element const & parent, std::string const & element_name);
    bool save(Element & parent, std::string const & element_name) const;
};

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

public:
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_ROOT = "server";
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_ADDR = "addr";
    TBAG_CONSTEXPR static char const * const SERVER_XML_NODE_VAR  = "var";

private:
    mutable Lock _lock;

private:
    SocketAddressInfo _default_addr;
    std::string _default_var;

private:
    SocketAddressInfo _addr;
    std::string _var;

public:
    ServerXmlNode();
    ServerXmlNode(std::string const & var, std::string const & ip, int port, bool enable = true);
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
    bool getEnable() const;
    std::string getIp() const;
    int getPort() const;
};

} // namespace node
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_DOM_NODE_SERVERXMLNODE_HPP__

