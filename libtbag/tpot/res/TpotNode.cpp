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
    opt(element, TPOT_NODE_VAR, _var);
    opt(element, TPOT_NODE_BIND, _bind);
    opt(element, TPOT_NODE_PORT, _port);
}

void TpotNode::save(Element & element) const
{
    ReadGuard guard(_lock);
    set(element, TPOT_NODE_VAR, _var);
    set(element, TPOT_NODE_BIND, _bind);
    set(element, TPOT_NODE_PORT, _port);
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

