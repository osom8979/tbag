/**
 * @file   ModelNet.hpp
 * @brief  ModelNet class prototype.
 * @author zer0
 * @date   2018-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>

#include <memory>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

/**
 * ModelNet class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 */
class TBAG_API ModelNet
{
public:
    struct Impl;
    friend struct Impl;
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    ModelNet() TBAG_NOEXCEPT;
    ModelNet(ModelNet const & obj) TBAG_NOEXCEPT;
    ModelNet(ModelNet && obj) TBAG_NOEXCEPT;
    ~ModelNet();

public:
    ModelNet & operator =(ModelNet const & obj) TBAG_NOEXCEPT;
    ModelNet & operator =(ModelNet && obj) TBAG_NOEXCEPT;

public:
    void copy(ModelNet const & obj) TBAG_NOEXCEPT;
    void swap(ModelNet & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ModelNet & lh, ModelNet & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    std::string toString() const;
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELNET_HPP__

