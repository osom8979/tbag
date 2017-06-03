/**
 * @file   ServerXmlNode.cpp
 * @brief  ServerXmlNode class implementation.
 * @author zer0
 * @date   2017-06-03
 */

#include <libtbag/dom/node/ServerXmlNode.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace node {

TBAG_CONSTEXPR char const * const SERVER_XML_NODE_ROOT = "server";
TBAG_CONSTEXPR char const * const SERVER_XML_NODE_VAR  = "var";
TBAG_CONSTEXPR char const * const SERVER_XML_NODE_IP   = "ip";
TBAG_CONSTEXPR char const * const SERVER_XML_NODE_PORT = "port";

ServerXmlNode::ServerXmlNode() : ServerXmlNode("${EXE_DIR}", "0.0.0.0", 1001)
{
    // EMPTY.
}

ServerXmlNode::ServerXmlNode(std::string const & var, std::string const & ip, int port)
        : _default_var(var), _default_ip(ip), _default_port(port)
{
    // EMPTY.
}

ServerXmlNode::ServerXmlNode(ServerXmlNode const & obj)
{
    (*this) = obj;
}

ServerXmlNode::ServerXmlNode(ServerXmlNode && obj)
{
    (*this) = std::move(obj);
}

ServerXmlNode::~ServerXmlNode()
{
    // EMPTY.
}

ServerXmlNode & ServerXmlNode::operator =(ServerXmlNode const & obj)
{
    if (this != &obj) {
        this->_lock.writeLock();
        obj._lock.readLock();
        {
            _default_var  = obj._default_var;
            _default_ip   = obj._default_ip;
            _default_port = obj._default_port;
            _var  = obj._var;
            _ip   = obj._ip;
            _port = obj._port;
        }
        obj._lock.readUnlock();
        this->_lock.writeUnlock();
    }
    return *this;
}

ServerXmlNode & ServerXmlNode::operator =(ServerXmlNode && obj)
{
    if (this != &obj) {
        this->_lock.writeLock();
        obj._lock.readLock();
        {
            _default_var.swap(obj._default_var);
            _default_ip.swap(obj._default_ip);
            std::swap(_default_port, obj._default_port);

            _var.swap(obj._var);
            _ip.swap(obj._ip);
            std::swap(_port, obj._port);
        }
        obj._lock.readUnlock();
        this->_lock.writeUnlock();
    }
    return *this;
}

std::string ServerXmlNode::name() const
{
    return SERVER_XML_NODE_ROOT;
}

void ServerXmlNode::setup()
{
    WriteGuard guard(_lock);
    _var  = _default_var;
    _ip   = _default_ip;
    _port = _default_port;
}

void ServerXmlNode::teardown()
{
    WriteGuard guard(_lock);
    _var.clear();
    _ip.clear();
    _port = 0;
}

void ServerXmlNode::load(Element const & element)
{
    WriteGuard guard(_lock);
    opt(element, SERVER_XML_NODE_VAR , _var );
    opt(element, SERVER_XML_NODE_IP  , _ip  );
    opt(element, SERVER_XML_NODE_PORT, _port);
}

void ServerXmlNode::save(Element & element) const
{
    ReadGuard guard(_lock);
    set(element, SERVER_XML_NODE_VAR , _var );
    set(element, SERVER_XML_NODE_IP  , _ip  );
    set(element, SERVER_XML_NODE_PORT, _port);
}

std::string ServerXmlNode::getVar() const
{
    ReadGuard guard(_lock);
    return _var;
}

std::string ServerXmlNode::getIp() const
{
    ReadGuard guard(_lock);
    return _ip;
}

int ServerXmlNode::getPort() const
{
    ReadGuard guard(_lock);
    return _port;
}

} // namespace node
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

