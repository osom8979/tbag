/**
 * @file   TpotLog.hpp
 * @brief  TpotLog class prototype.
 * @author zer0
 * @date   2017-05-27
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTLOG_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTLOG_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace tpot {
namespace res  {

/**
 * TpotLog class prototype.
 *
 * @author zer0
 * @date   2017-05-27
 */
class TBAG_API TpotLog : public log::node::LogXmlNode
{
public:
    using LogXmlNode = log::node::LogXmlNode;
    using String = LogXmlNode::String;

private:
    // Insert member variables.

public:
    TpotLog();
    virtual ~TpotLog();

public:
    virtual String name() const override;
    virtual void setup() override;
};

} // namespace res
} // namespace tpot

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_TPOT_RES_TPOTLOG_HPP__

