/**
 * @file   TpotNode.cpp
 * @brief  TpotNode class implementation.
 * @author zer0
 * @date   2017-05-27
 */

#include <libtbag/tpot/res/TpotNode.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

TBAG_CONSTEXPR char const * const TPOT_NODE_ROOT = "config";
TBAG_CONSTEXPR char const * const TPOT_NODE_VAR  = "var";
TBAG_CONSTEXPR char const * const TPOT_NODE_BIND = "bind";
TBAG_CONSTEXPR char const * const TPOT_NODE_PORT = "port";

TpotNode::TpotNode()
{
    // EMPTY.
}

TpotNode::TpotNode(TpotNode const & obj)
{
    (*this) = obj;
}

TpotNode::TpotNode(TpotNode && obj)
{
    (*this) = std::move(obj);
}

TpotNode::~TpotNode()
{
    // EMPTY.
}

TpotNode & TpotNode::operator =(TpotNode const & obj)
{
    if (this != &obj) {
        this->_lock.writeLock();
        obj._lock.readLock();
        {
            _var  = obj._var;
            _bind = obj._bind;
            _port = obj._port;
        }
        obj._lock.readUnlock();
        this->_lock.writeUnlock();
    }
    return *this;
}

TpotNode & TpotNode::operator =(TpotNode && obj)
{
    if (this != &obj) {
        this->_lock.writeLock();
        obj._lock.readLock();
        {
            _var.swap(obj._var);
            _bind.swap(obj._bind);
            std::swap(_port, obj._port);
        }
        obj._lock.readUnlock();
        this->_lock.writeUnlock();
    }
    return *this;
}

std::string TpotNode::name() const
{
    return TPOT_NODE_ROOT;
}

void TpotNode::setup()
{
    WriteGuard guard(_lock);
    _var  = "${EXE_DIR}";
    _bind = "0.0.0.0";
    _port = 2100;
}

void TpotNode::teardown()
{
    WriteGuard guard(_lock);
    _var.clear();
    _bind.clear();
    _port = 0;
}

void TpotNode::load(Element const & element)
{
    WriteGuard guard(_lock);

    auto * var_node = element.FirstChildElement(TPOT_NODE_VAR);
    if (var_node != nullptr && var_node->GetText() != nullptr) {
        _var.assign(var_node->GetText());
    } else {
        _var.clear();
    }

    auto * bind_node = element.FirstChildElement(TPOT_NODE_BIND);
    if (bind_node != nullptr && bind_node->GetText() != nullptr) {
        _bind.assign(bind_node->GetText());
    } else {
        _bind.clear();
    }

    int temp_port = 0;
    auto * port_node = element.FirstChildElement(TPOT_NODE_PORT);
    if (port_node != nullptr && port_node->QueryIntText(&temp_port) == tinyxml2::XML_NO_ERROR) {
        _port = temp_port;
    } else {
        _port = 0;
    }
}

void TpotNode::save(Element & element) const
{
    ReadGuard guard(_lock);

    auto * var_node = element.GetDocument()->NewElement(TPOT_NODE_VAR);
    var_node->SetText(_var.c_str());
    element.InsertEndChild(var_node);

    auto * bind_node = element.GetDocument()->NewElement(TPOT_NODE_BIND);
    bind_node->SetText(_bind.c_str());
    element.InsertEndChild(bind_node);

    auto * port_node = element.GetDocument()->NewElement(TPOT_NODE_PORT);
    port_node->SetText(_port);
    element.InsertEndChild(port_node);
}

std::string TpotNode::getVar() const
{
    ReadGuard guard(_lock);
    return _var;
}

std::string TpotNode::getBind() const
{
    ReadGuard guard(_lock);
    return _bind;
}

int TpotNode::getPort() const
{
    ReadGuard guard(_lock);
    return _port;
}

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

