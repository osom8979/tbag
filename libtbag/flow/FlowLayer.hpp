/**
 * @file   FlowLayer.hpp
 * @brief  FlowLayer class prototype.
 * @author zer0
 * @date   2017-12-21
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWLAYER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWLAYER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/container/Pointer.hpp>

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace flow {

/**
 * FlowLayer class prototype.
 *
 * @author zer0
 * @date   2017-12-21
 */
template <typename DataType, typename KeyType = std::string>
class FlowLayer : private Noncopyable
{
public:
    using Data = DataType;
    using Key  = KeyType;

    using Keys     = std::vector<Key>;
    using DataPtr  = container::Pointer<Data>;
    using DataMap  = std::unordered_map<Key, DataPtr>;
    using DataPair = typename DataMap::value_type;

public:
    Key   key;
    Data  data;
    Keys  prev;
    Keys  next;

public:
    FlowLayer(Key const & k) : key(k), data(), prev(), next() { /* EMPTY. */ }
    virtual ~FlowLayer() { /* EMPTY. */ }

public:
    virtual bool open(std::string const & data) { return true; }
    virtual void close() { /* EMPTY. */ }
    virtual void clear() { /* EMPTY. */ }

public:
    virtual bool  forward(DataMap const & prev) { return true; }
    virtual bool backward(DataMap const & next) { return true; }
};

} // namespace flow

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FLOW_FLOWLAYER_HPP__

