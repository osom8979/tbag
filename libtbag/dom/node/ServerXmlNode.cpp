/**
 * @file   ServerXmlNode.cpp
 * @brief  ServerXmlNode class implementation.
 * @author zer0
 * @date   2017-06-03
 */

#include <libtbag/dom/node/ServerXmlNode.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/network/details/NetCommon.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace dom  {
namespace node {

// ---------------------------------
// SocketAddressInfo implementation.
// ---------------------------------

SocketAddressInfo::SocketAddressInfo()
        : ip(network::details::ANY_IPV4), port(0), enable(false)
{
    // EMPTY.
}

SocketAddressInfo::SocketAddressInfo(std::string const & i, int p, bool e)
        : ip(i), port(p), enable(e)
{
    // EMPTY.
}

SocketAddressInfo::~SocketAddressInfo()
{
    // EMPTY.
}

void SocketAddressInfo::setProperties(std::string const & i, int p, bool e)
{
    enable = e;
    ip     = i;
    port   = p;
}

void SocketAddressInfo::clear()
{
    enable = false;
    ip.clear();
    port = 0;
}

void SocketAddressInfo::swap(SocketAddressInfo & obj)
{
    std::swap(enable, obj.enable);
    ip.swap(obj.ip);
    std::swap(port, obj.port);
}

bool SocketAddressInfo::load(Element const & parent, std::string const & element_name)
{
    Element const * cursor = parent.FirstChildElement(element_name.c_str());
    if (cursor != nullptr) {
        opt(*cursor, SERVER_XML_NODE_ENDABLE, enable);
        opt(*cursor, SERVER_XML_NODE_IP     , ip);
        opt(*cursor, SERVER_XML_NODE_PORT   , port);
        return true;
    }

    clear();
    return false;
}

bool SocketAddressInfo::save(Element & parent, std::string const & element_name) const
{
    auto * doc = parent.GetDocument();
    if (doc == nullptr) {
        return false;
    }

    auto * element = doc->NewElement(element_name.c_str());
    if (element == nullptr) {
        return false;
    }

    set(*element, SERVER_XML_NODE_ENDABLE, enable);
    set(*element, SERVER_XML_NODE_IP     , ip);
    set(*element, SERVER_XML_NODE_PORT   , port);

    return parent.InsertEndChild(element) != nullptr;
}

// -----------------------------
// ServerXmlNode implementation.
// -----------------------------

ServerXmlNode::ServerXmlNode() : _default_var(), _default_addr()
{
    // EMPTY.
}

ServerXmlNode::ServerXmlNode(std::string const & var, std::string const & ip, int port, bool enable)
        : _default_var(var), _default_addr(ip, port, enable)
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
            _default_addr = obj._default_addr;
            _default_var  = obj._default_var;
            _addr = obj._addr;
            _var  = obj._var;
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
            _default_addr.swap(obj._default_addr);
            _default_var.swap(obj._default_var);
            _addr.swap(obj._addr);
            _var.swap(obj._var);
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
    _addr = _default_addr;
    _var  = _default_var;
}

void ServerXmlNode::teardown()
{
    WriteGuard guard(_lock);
    _addr.clear();
    _var.clear();
}

void ServerXmlNode::load(Element const & element)
{
    WriteGuard guard(_lock);
    _addr.load(element, SERVER_XML_NODE_ADDR);
    opt(element, SERVER_XML_NODE_VAR, _var);
}

void ServerXmlNode::save(Element & element) const
{
    ReadGuard guard(_lock);
    _addr.save(element, SERVER_XML_NODE_ADDR);
    set(element, SERVER_XML_NODE_VAR, _var);
}

std::string ServerXmlNode::getVar() const
{
    ReadGuard guard(_lock);
    return _var;
}

bool ServerXmlNode::getEnable() const
{
    ReadGuard guard(_lock);
    return _addr.enable;
}

std::string ServerXmlNode::getIp() const
{
    ReadGuard guard(_lock);
    return _addr.ip;
}

int ServerXmlNode::getPort() const
{
    ReadGuard guard(_lock);
    return _addr.port;
}

} // namespace node
} // namespace dom

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

