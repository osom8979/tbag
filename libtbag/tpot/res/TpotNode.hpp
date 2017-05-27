/**
 * @file   TpotNode.hpp
 * @brief  TpotNode class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/tpot/res/TpotConfig.hpp>
#include <libtbag/lock/RwLock.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

/**
 * TpotNode class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API TpotNode : public TpotConfig::NodeInterface
{
public:
    using Lock       = lock::RwLock;
    using ReadGuard  = lock::ReadLockGuard;
    using WriteGuard = lock::WriteLockGuard;
    using String     = TpotConfig::String;

private:
    mutable Lock _lock;

private:
    String _var;
    String _bind;
    int    _port;

public:
    TpotNode();
    TpotNode(TpotNode const & obj);
    TpotNode(TpotNode && obj);
    virtual ~TpotNode();

public:
    TpotNode & operator =(TpotNode const & obj);
    TpotNode & operator =(TpotNode && obj);

public:
    virtual String name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;

public:
    String getVar() const;
    String getBind() const;
    int getPort() const;
};

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTNODE_HPP__

