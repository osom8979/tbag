/**
 * @file   GetText.hpp
 * @brief  GetText class prototype.
 * @author zer0
 * @date   2018-11-06
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_RES_GETTEXT_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_RES_GETTEXT_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/dom/xml/Resource.hpp>

#include <unordered_map>
#include <string>
#include <vector>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace res {

/**
 * GetText class prototype.
 *
 * @author zer0
 * @date   2018-11-06
 */
class TBAG_API GetText
{
public:
    using Resource    = libtbag::dom::xml::Resource;
    using ResourceMap = std::unordered_map<std::string, Resource>;

public:
    TBAG_CONSTEXPR static char const * const ROOT_TAG = "resource";
    TBAG_CONSTEXPR static char const * const PROP_TAG = "text";
    TBAG_CONSTEXPR static char const * const NAME_ATT = "name";

public:
    struct Impl
    {
        ResourceMap  map;
        std::string  language;
    };

public:
    using SharedImpl = std::shared_ptr<Impl>;

private:
    SharedImpl _impl;
    Resource * _current = nullptr;

public:
    GetText();
    GetText(std::nullptr_t) TBAG_NOEXCEPT;
    GetText(GetText const & obj) TBAG_NOEXCEPT;
    GetText(GetText && obj) TBAG_NOEXCEPT;
    ~GetText();

public:
    GetText & operator =(GetText const & obj) TBAG_NOEXCEPT;
    GetText & operator =(GetText && obj) TBAG_NOEXCEPT;

public:
    void copy(GetText const & obj) TBAG_NOEXCEPT;
    void swap(GetText & obj) TBAG_NOEXCEPT;

public:
    inline friend void swap(GetText & lh, GetText & rh) TBAG_NOEXCEPT { lh.swap(rh); }

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    inline Impl       * get()       TBAG_NOEXCEPT { return _impl.get(); }
    inline Impl const * get() const TBAG_NOEXCEPT { return _impl.get(); }

    inline Impl       * operator ->()       TBAG_NOEXCEPT { return get(); }
    inline Impl const * operator ->() const TBAG_NOEXCEPT { return get(); }

    inline Impl       & operator *()       TBAG_NOEXCEPT { return *get(); }
    inline Impl const & operator *() const TBAG_NOEXCEPT { return *get(); }

public:
    /**
     * Implemented for std::less<> compatibility.
     *
     * @see std::set
     * @see std::map
     * @see std::less
     */
    friend inline bool operator <(GetText const & x, GetText const & y) TBAG_NOEXCEPT
    {
        return x.get() < y.get();
    }

    inline bool operator ==(GetText const & obj) const TBAG_NOEXCEPT
    {
        return get() == obj.get();
    }

    inline bool operator !=(GetText const & obj) const TBAG_NOEXCEPT
    {
        return get() != obj.get();
    }

public:
    void reset();
    void clear();

public:
    bool empty() const;
    std::size_t size() const;

public:
    inline Resource       * current()       TBAG_NOEXCEPT { return _current; }
    inline Resource const * current() const TBAG_NOEXCEPT { return _current; }

public:
    void setLanguage(std::string const & language, bool auto_update = true);
    std::string getLanguage() const;

public:
    std::vector<std::string> getLanguages() const;

public:
    bool load(std::string const & dir, bool auto_update = true);
    bool save(std::string const & dir) const;

public:
    bool updateCache();

public:
    void set(std::string const & language, std::string const & name, std::string const & text);
    void set(std::string const & name, std::string const & text);

public:
    std::string get(std::string const & language, std::string const & name) const;
    std::string get(std::string const & name) const;

public:
    std::string const & at(std::string const & language, std::string const & name) const;
    std::string const & at(std::string const & name) const;
};

} // namespace res

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_RES_GETTEXT_HPP__

