/**
 * @file   ThreadLocalStorage.hpp
 * @brief  ThreadLocalStorage class prototype.
 * @author zer0
 * @date   2018-11-22
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADLOCALSTORAGE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADLOCALSTORAGE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <new>
#include <memory>
#include <type_traits>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

/**
 * Thread-local storage key.
 *
 * @author zer0
 * @date   2018-11-22
 *
 * @remarks
 *  The total thread-local storage size may be limited. @n
 *  That is, it may not be possible to create many TLS keys.
 */
struct TlsKey;

TBAG_API bool createKey(TlsKey * key);
TBAG_API void deleteKey(TlsKey * key);

TBAG_API void * get(TlsKey * key);
TBAG_API void set(TlsKey * key, void * value);

/**
 * Thread-local storage.
 *
 * @author zer0
 * @date   2018-11-22
 */
class TBAG_API ThreadLocalStorage : private Noncopyable
{
public:
    using UniqueKey = std::unique_ptr<TlsKey>;

private:
    UniqueKey _key;

public:
    ThreadLocalStorage();
    ThreadLocalStorage(void * value);
    ~ThreadLocalStorage();

public:
    void * get();

public:
    template <typename ValueT>
    ValueT * cast()
    {
        return (ValueT*)get();
    }

public:
    void set(void * value);

public:
    template <typename ValueT>
    void set(typename std::enable_if<!std::is_void<ValueT>::value, ValueT>::type * value)
    {
        return set((void*)value);
    }
};

/**
 * Shared Thread-local storage.
 *
 * @author zer0
 * @date   2018-11-22
 */
using SharedTls = std::shared_ptr<ThreadLocalStorage>;

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_THREAD_THREADLOCALSTORAGE_HPP__

