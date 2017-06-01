/**
 * @file   SingletonUtils.cpp
 * @brief  SingletonUtils class implementation.
 * @author zer0
 * @date   2016-11-25
 */

#include <libtbag/util/SingletonUtils.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/log/Log.hpp>

// Singleton classes.
#include <libtbag/log/mgr/LoggerManager.hpp>
#include <libtbag/signal/SignalHandler.hpp>
#include <libtbag/container/Global.hpp>
#include <libtbag/time/Time.hpp>

#include <functional>
#include <initializer_list>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace util {

namespace __impl {

/**
 * Create or Release buffer class.
 *
 * @author zer0
 * @date   2017-05-18
 */
class CreateOrRelease : private Noncopyable
{
public:
    using Callback = std::function<void(void)>;

public:
    struct Func
    {
        Callback create;
        Callback release;

        Func() { /* EMPTY. */ }
        Func(Callback const & c, Callback const & r) : create(c), release(r) { /* EMPTY. */ }
    };

public:
    std::vector<Func> _list;

public:
    CreateOrRelease(std::initializer_list<Func> const & init) : _list(init)
    {
        // EMPTY.
    }

    ~CreateOrRelease()
    {
        // EMPTY.
    }

public:
    void create()
    {
        for (auto itr = _list.begin(); itr != _list.end(); ++itr) {
            itr->create();
        }
    }

    void release()
    {
        for (auto itr = _list.rbegin(); itr != _list.rend(); ++itr) {
            itr->release();
        }
    }
};

static void runCreateOrRelease(bool is_create = true)
{
    using Func = CreateOrRelease::Func;

    // @formatter:off
    Func    log([](){ log::mgr::LoggerManager::createInstance(); }, [](){ log::mgr::LoggerManager::releaseInstance(); });
    Func signal([](){          signal::__impl::createInstance(); }, [](){          signal::__impl::releaseInstance(); });
    Func   time([](){            time::__impl::createInstance(); }, [](){            time::__impl::releaseInstance(); });
    Func global([](){       container::Global::createInstance(); }, [](){       container::Global::releaseInstance(); });
    // @formatter:on

    CreateOrRelease init({log, signal, time, global});

    if (is_create) {
        init.create();
    } else {
        init.release();
    }
}

} // namespace __impl

void createSingletonObjects()
{
    __impl::runCreateOrRelease(true);
}

void releaseSingletonObjects()
{
    __impl::runCreateOrRelease(false);
}

} // namespace util

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

