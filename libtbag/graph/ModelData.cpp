/**
 * @file   ModelData.cpp
 * @brief  ModelData class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelData.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

/**
 * Pointer implementation.
 *
 * @author zer0
 * @date   2018-10-22
 */
struct ModelData::Impl : private Noncopyable
{
private:
    ModelData * _parent = nullptr;

public:
    Impl(ModelData * parent) : _parent(parent)
    {
        assert(_parent != nullptr);
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// -------------------------
// ModelData implementation.
// -------------------------

ModelData::ModelData() TBAG_NOEXCEPT : _impl(std::make_shared<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

ModelData::ModelData(ModelData const & obj) TBAG_NOEXCEPT : ModelData()
{
    (*this) = obj;
}

ModelData::ModelData(ModelData && obj) TBAG_NOEXCEPT : ModelData()
{
    (*this) = std::move(obj);
}

ModelData::~ModelData()
{
    // EMPTY.
}

ModelData & ModelData::operator =(ModelData const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

ModelData & ModelData::operator =(ModelData && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ModelData::copy(ModelData const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl = obj._impl;
    }
}

void ModelData::swap(ModelData & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _impl.swap(obj._impl);
    }
}

std::string ModelData::toString() const
{
    return std::string();
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

