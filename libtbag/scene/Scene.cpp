/**
 * @file   Scene.cpp
 * @brief  Scene class implementation.
 * @author zer0
 * @date   2019-05-29
 */

#include <libtbag/scene/Scene.hpp>
#include <libtbag/log/Log.hpp>

#include <algorithm>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace scene {

Scene::Scene()
{
    // EMPTY.
}

Scene::Scene(Scene const & obj) : Scene()
{
    (*this) = obj;
}

Scene::Scene(Scene && obj) TBAG_NOEXCEPT : Scene()
{
    (*this) = std::move(obj);
}

Scene::~Scene()
{
    // EMPTY.
}

Scene & Scene::operator =(Scene const & obj)
{
    copy(obj);
    return *this;
}

Scene & Scene::operator =(Scene && obj) TBAG_NOEXCEPT
{
    swap(obj);
    return *this;
}

void Scene::copy(Scene const & obj)
{
    if (this != &obj) {
        // TODO: Copy object.
    }
}

void Scene::swap(Scene & obj) TBAG_NOEXCEPT
{
    if (this != &obj) {
        // TODO: Swap object.
    }
}

} // namespace scene

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

