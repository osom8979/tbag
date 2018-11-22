/**
 * @file   ThreadLocalStorage.cpp
 * @brief  ThreadLocalStorage class implementation.
 * @author zer0
 * @date   2018-11-22
 */

#include <libtbag/thread/ThreadLocalStorage.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>
#include <uv.h>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace thread {

struct TlsKey
{
    uv_key_t uv_key;
};

bool createKey(TlsKey * key)
{
    assert(key != nullptr);
    return ::uv_key_create(&(key->uv_key)) == 0;
}

void deleteKey(TlsKey * key)
{
    assert(key != nullptr);
    ::uv_key_delete(&(key->uv_key));
}

void * get(TlsKey * key)
{
    assert(key != nullptr);
    return ::uv_key_get(&(key->uv_key));
}

void set(TlsKey * key, void * value)
{
    assert(key != nullptr);
    assert(value != nullptr);
    ::uv_key_set(&(key->uv_key), value);
}

// ----------------------------------
// ThreadLocalStorage implementation.
// ----------------------------------

ThreadLocalStorage::ThreadLocalStorage() : _key(std::make_unique<TlsKey>())
{
    if (!createKey(_key.get())) {
        throw std::bad_alloc();
    }
}

ThreadLocalStorage::ThreadLocalStorage(void * value) : ThreadLocalStorage()
{
    this->set((void*)value);
}

ThreadLocalStorage::~ThreadLocalStorage()
{
    deleteKey(_key.get());
}

void * ThreadLocalStorage::get()
{
    return libtbag::thread::get(_key.get());
}

void ThreadLocalStorage::set(void * value)
{
    libtbag::thread::set(_key.get(), value);
}

} // namespace thread

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

