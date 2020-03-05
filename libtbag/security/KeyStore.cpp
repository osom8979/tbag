/**
 * @file   KeyStore.cpp
 * @brief  KeyStore class implementation.
 * @author zer0
 * @date   2017-12-06
 */

#include <libtbag/security/KeyStore.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

#include <libtbag/security/store/FileKeyStore.hpp>
#include <libtbag/security/store/GnomeKeyStore.hpp>
#include <libtbag/security/store/MacKeyStore.hpp>
#include <libtbag/security/store/WindowsKeyStore.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace security {

using ErrString = KeyStore::ErrString;

KeyStore::KeyStore()
{
    // EMPTY.
}

KeyStore::~KeyStore()
{
    // EMPTY.
}

bool KeyStore::open(Type type, std::string const & params)
{
    try {
        KeyStoreInterface * interface = nullptr;
        using namespace libtbag::security::store;
        switch (type) {
        case Type::T_FILE:    interface = new FileKeyStore(params);    break;
        case Type::T_GNOME:   interface = new GnomeKeyStore(params);   break;
        case Type::T_MAC:     interface = new MacKeyStore(params);     break;
        case Type::T_WINDOWS: interface = new WindowsKeyStore(params); break;
        default:
            TBAG_INACCESSIBLE_BLOCK_ASSERT();
            break;
        }
        _store.reset(interface);
    } catch (...) {
        return false;
    }
    assert(static_cast<bool>(_store));
    return true;
}

void KeyStore::close()
{
    _store.reset();
}

bool KeyStore::isOpen() const
{
    return static_cast<bool>(_store);
}

bool KeyStore::create(std::string const & key)
{
    if (!_store) {
        return false;
    }
    return _store->create(key);
}

bool KeyStore::remove(std::string const & key)
{
    if (!_store) {
        return false;
    }
    return _store->remove(key);
}

bool KeyStore::get(std::string const & key, std::string & result) const
{
    if (!_store) {
        return false;
    }
    return _store->get(key, result);
}

bool KeyStore::set(std::string const & key, std::string const & value, bool encrypt)
{
    if (!_store) {
        return false;
    }
    return _store->set(key, value, encrypt);
}

bool KeyStore::cmp(std::string const & key, std::string const & value, bool encrypt) const
{
    if (!_store) {
        return false;
    }
    return _store->cmp(key, value, encrypt);
}

std::vector<std::string> KeyStore::list() const
{
    if (!_store) {
        return {};
    }
    return _store->list();
}

std::set<std::string> KeyStore::listSet() const
{
    auto const keys = list();
    return std::set<std::string>(keys.cbegin(), keys.cend());
}

ErrString KeyStore::get(std::string const & key) const
{
    if (!_store) {
        return E_EXPIRED;
    }
    std::string value;
    if (get(key, value)) {
        return { E_SUCCESS, value };
    } else {
        return E_GET;
    }
}

Err KeyStore::setSafe(std::string const & key, std::string const & value, bool encrypt)
{
    if (!_store) {
        return E_EXPIRED;
    }
    auto const keys = listSet();
    auto const itr = keys.find(key);
    if (itr == keys.cend()) {
        if (!create(key)) {
            return E_CREATE;
        }
    }
    return set(key, value, encrypt) ? E_SUCCESS : E_SET;
}

} // namespace security

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

