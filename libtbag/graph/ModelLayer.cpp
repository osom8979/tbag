/**
 * @file   ModelLayer.cpp
 * @brief  ModelLayer class implementation.
 * @author zer0
 * @date   2018-10-22
 */

#include <libtbag/graph/ModelLayer.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace graph {

// ------------------------------------
// ModelLayer::LogLayer implementation.
// ------------------------------------

ModelLayer::LogLayer::LogLayer(bool verbose) : LayerBase("LogLayer"), _verbose(verbose)
{
    // EMPTY.
}

ModelLayer::LogLayer::~LogLayer()
{
    // EMPTY.
}

Err ModelLayer::LogLayer::setup(std::string const & data)
{
    tDLogIfD(_verbose, "LogLayer[{}@{}]::setup()", getName(), getId());
    return Err::E_SUCCESS;
}

Err ModelLayer::LogLayer::teardown()
{
    tDLogIfD(_verbose, "LogLayer[{}@{}]::teardown()", getName(), getId());
    return Err::E_SUCCESS;
}

Err ModelLayer::LogLayer::forward(Layers const & input)
{
    tDLogIfD(_verbose, "LogLayer[{}@{}]::forward(in:{})", getName(), getId(), input.size());
    return Err::E_SUCCESS;
}

Err ModelLayer::LogLayer::backward(Layers const & input)
{
    tDLogIfD(_verbose, "LogLayer[{}@{}]::backward(in:{})", getName(), getId(), input.size());
    return Err::E_SUCCESS;
}

// --------------------------
// ModelLayer implementation.
// --------------------------

ModelLayer::ModelLayer() : ModelLayer(std::make_shared<LayerBase>())
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(int id) : ModelLayer()
{
    setId(id);
}

ModelLayer::ModelLayer(std::nullptr_t) TBAG_NOEXCEPT : _base(nullptr)
{
    // EMPTY.
}

ModelLayer::ModelLayer(LayerBase * base) TBAG_NOEXCEPT : ModelLayer(SharedBase(base))
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(SharedBase const & base) TBAG_NOEXCEPT : _base(base)
{
    assert(static_cast<bool>(_base));
}

ModelLayer::ModelLayer(ModelLayer const & obj) TBAG_NOEXCEPT : ModelLayer(nullptr)
{
    (*this) = obj;
}

ModelLayer::ModelLayer(ModelLayer && obj) TBAG_NOEXCEPT : ModelLayer(nullptr)
{
    (*this) = std::move(obj);
}

ModelLayer::~ModelLayer()
{
    // EMPTY.
}

ModelLayer & ModelLayer::operator =(ModelLayer const & obj) TBAG_NOEXCEPT
{
    copy(obj);
    return *this;
}

ModelLayer & ModelLayer::operator =(ModelLayer && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void ModelLayer::copy(ModelLayer const & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base = obj._base;
    }
}

void ModelLayer::swap(ModelLayer & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        _base.swap(obj._base);
    }
}

int ModelLayer::getId() const
{
    assert(exists());
    return _base->getId();
}

void ModelLayer::setId(int id)
{
    assert(exists());
    _base->_id = id;
}

bool ModelLayer::isComplete() const
{
    assert(exists());
    return _base->isComplete();
}

std::string ModelLayer::getName() const
{
    assert(exists());
    return _base->getName();
}

void ModelLayer::complete()
{
    assert(exists());
    _base->_complete = true;
}

void ModelLayer::incomplete()
{
    assert(exists());
    _base->_complete = false;
}

Err ModelLayer::setup(std::string const & data)
{
    assert(exists());
    return _base->setup(data);
}

Err ModelLayer::teardown()
{
    assert(exists());
    return _base->teardown();
}

Err ModelLayer::forward(Layers const & input)
{
    assert(exists());
    return _base->forward(input);
}

Err ModelLayer::backward(Layers const & input)
{
    assert(exists());
    return _base->backward(input);
}

Err ModelLayer::toData(Buffer & output) const
{
    assert(exists());
    return _base->toData(output);
}

Err ModelLayer::fromData(Buffer const & input)
{
    assert(exists());
    return _base->fromData(input);
}

std::string ModelLayer::get(std::string const & key) const
{
    assert(exists());
    return _base->get(key);
}

void ModelLayer::set(std::string const & key, std::string const & val)
{
    assert(exists());
    _base->set(key, val);
}

std::string ModelLayer::toString() const
{
    return getName() + (isComplete() ? "{Y}@" : "{N}@") + std::to_string(getId());
}

} // namespace graph

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

