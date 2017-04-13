/**
 * @file   SingletonUtils.cpp
 * @brief  SingletonUtils class implementation.
 * @author zer0
 * @date   2016-11-25
 */

#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/log/Log.hpp>

// Singleton classes.
#include <libtbag/log/mgr/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

void initSingletonObjects()
{
    log::mgr::LoggerManager::createInstance();
}

void releaseSingletonObjects()
{
    log::mgr::LoggerManager::releaseInstance();
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

