/**
 * @file   ModelData.hpp
 * @brief  ModelData class prototype.
 * @author zer0
 * @date   2018-10-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELDATA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELDATA_HPP__

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
 * ModelData class prototype.
 *
 * @author zer0
 * @date   2018-10-22
 */
class TBAG_API ModelData
{
public:
    struct Impl;
    friend struct Impl;
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;

public:
    ModelData() TBAG_NOEXCEPT;
    ModelData(ModelData const & obj) TBAG_NOEXCEPT;
    ModelData(ModelData && obj) TBAG_NOEXCEPT;
    ~ModelData();

public:
    ModelData & operator =(ModelData const & obj) TBAG_NOEXCEPT;
    ModelData & operator =(ModelData && obj) TBAG_NOEXCEPT;

public:
    void copy(ModelData const & obj) TBAG_NOEXCEPT;
    void swap(ModelData & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(ModelData & lh, ModelData & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    std::string toString() const;
};

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_GRAPH_MODELDATA_HPP__

