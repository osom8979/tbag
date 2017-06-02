/**
 * @file   DefaultLogXmlNode.hpp
 * @brief  DefaultLogXmlNode class prototype.
 * @author zer0
 * @date   2017-06-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_DEFAULTLOGXMLNODE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_DEFAULTLOGXMLNODE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/log/node/LogXmlNode.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace node {

/**
 * DefaultLogXmlNode class prototype.
 *
 * @author zer0
 * @date   2017-06-02
 *
 * @remarks
 *  Default Environments variables:
 *  - ${EXE_PATH}
 *  - ${EXE_DIR}
 *  - ${WORK_DIR}
 *  - ${HOME_DIR}
 */
class TBAG_API DefaultLogXmlNode : public LogXmlNode
{
public:
    struct InitParams
    {
        std::string name;        ///< logger name.
        std::string sink;        ///< cout, file, rotate_file
        std::string destination; ///< file path.
        std::string generator;   ///< default, default_color
        std::string severity;    ///< debug, info, error, ...
        std::string auto_flush;  ///< true or false.
        std::string multithread; ///< true or false.
        std::string mutex;       ///< true or false.
    };

public:
    InitParams _params;

public:
    DefaultLogXmlNode();
    DefaultLogXmlNode(std::string const & name);
    DefaultLogXmlNode(std::string const & name, std::string const & file_name);
    DefaultLogXmlNode(InitParams const & params);
    virtual ~DefaultLogXmlNode();

public:
    std::string getLoggerName() const;

public:
    virtual std::string name() const override;
    virtual void setup() override;

public:
    static InitParams getDefaultParams();
    static InitParams getDefaultParams(std::string const & name, std::string const & file_name);
};

} // namespace node
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_LOG_NODE_DEFAULTLOGXMLNODE_HPP__

