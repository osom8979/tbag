/**
 * @file   LogXmlNode.hpp
 * @brief  LogXmlNode class prototype.
 * @author zer0
 * @date   2017-04-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/XmlModel.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

/**
 * LogXmlNode class prototype.
 *
 * @author zer0
 * @date   2017-04-25
 */
class TBAG_API LogXmlNode : public dom::XmlModel::NodeInterface
{
public:
    LogXmlNode();
    virtual ~LogXmlNode();

protected:
    virtual String name() const override;

    virtual void setup() override;
    virtual void teardown() override;

    virtual void load(Element const & element) override;
    virtual void save(Element & element) const override;
};

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_LOGXMLNODE_HPP__

