/**
 * @file   SingletonUtils.cpp
 * @brief  SingletonUtils class implementation.
 * @author zer0
 * @date   2016-11-25
 */

#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/log/Log.hpp>

// Singleton classes.
#include <libtbag/log/details/LoggerManager.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

void initSingletonObjects()
{
    __tbag_debug("Call initSingletonObjects()");
    log::details::LoggerManager::createInstance();
}

void releaseSingletonObjects()
{
    __tbag_debug("Call releaseSingletonObjects()");
    log::details::LoggerManager::releaseInstance();
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

