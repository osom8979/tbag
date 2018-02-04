/**
 * @file   X509.cpp
 * @brief  X509 class implementation.
 * @author zer0
 * @date   2018-01-31
 */

#include <libtbag/crypto/X509.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace crypto {

struct X509::Impl : private Noncopyable
{
    X509 * parent;

    Impl(X509 * p) : parent(p)
    {
        // EMPTY.
    }

    ~Impl()
    {
        // EMPTY.
    }
};

// --------------------
// X509 implementation.
// --------------------

X509::X509() : _impl(new Impl(this)), _infos()
{
    // EMPTY.
}

X509::~X509()
{
    // EMPTY.
}

void X509::clear()
{
    _infos.clear();
}

std::string X509::get(std::string const & key) const
{
    auto itr = _infos.find(key);
    if (itr != _infos.end()) {
        return itr->second;
    }
    return std::string();
}

bool X509::insert(std::string const & key, std::string const & value)
{
    return _infos.insert(Informations::value_type(key, value)).second;
}

} // namespace crypto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

