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
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/time/Time.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

namespace __impl {

// -------
// LEVEL1.
// -------

static void initLevel1()
{
    log::mgr::LoggerManager::createInstance();
    signal::__impl::createInstance();
}

static void releaseLevel1()
{
    signal::__impl::releaseInstance();
    log::mgr::LoggerManager::releaseInstance();
}

// -------
// LEVEL2.
// -------

static void initLevel2()
{
    container::Global::createInstance();
    time::__impl::createInstance();
}

static void releaseLevel2()
{
    time::__impl::releaseInstance();
    container::Global::releaseInstance();
}

} // namespace __impl

void initSingletonObjects()
{
    __impl::initLevel1();
    __impl::initLevel2();
}

void releaseSingletonObjects()
{
    __impl::releaseLevel2();
    __impl::releaseLevel1();
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

