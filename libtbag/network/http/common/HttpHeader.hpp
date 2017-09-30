/**
 * @file   HttpHeader.hpp
 * @brief  HttpHeader class prototype.
 * @author zer0
 * @date   2017-09-29
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPHEADER_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPHEADER_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/network/http/common/HttpCommon.hpp>

#include <string>
#include <map>
#include <initializer_list>
#include <utility>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {
namespace http    {
namespace common  {

/**
 * HttpHeader class prototype.
 *
 * @author zer0
 * @date   2017-09-29
 */
class TBAG_API HttpHeader
{
public:
    using HeaderMultiMap = std::multimap<std::string, std::string>;

public:
    using key_type               = HeaderMultiMap::key_type;
    using mapped_type            = HeaderMultiMap::mapped_type;
    using value_type             = HeaderMultiMap::value_type;
    using key_compare            = HeaderMultiMap::key_compare;
    using value_compare          = HeaderMultiMap::value_compare;
    using allocator_type         = HeaderMultiMap::allocator_type;
    using reference              = HeaderMultiMap::reference;
    using const_reference        = HeaderMultiMap::const_reference;
    using pointer                = HeaderMultiMap::pointer;
    using const_pointer          = HeaderMultiMap::const_pointer;
    using size_type              = HeaderMultiMap::size_type;
    using difference_type        = HeaderMultiMap::difference_type;
    using iterator               = HeaderMultiMap::iterator;
    using const_iterator         = HeaderMultiMap::const_iterator;
    using reverse_iterator       = HeaderMultiMap::reverse_iterator;
    using const_reverse_iterator = HeaderMultiMap::const_reverse_iterator;

public:
    STATIC_ASSERT_CHECK_IS_SAME(   key_type, std::string);
    STATIC_ASSERT_CHECK_IS_SAME(mapped_type, std::string);

private:
    HeaderMultiMap _headers;

public:
    HttpHeader();
    HttpHeader(std::initializer_list<value_type> list);
    HttpHeader(HttpHeader const & obj);
    HttpHeader(HttpHeader && obj);

    template <class InputIterator>
    HttpHeader(InputIterator first, InputIterator last) : _headers(first, last)
    { /* EMPTY. */ }

public:
    virtual ~HttpHeader();

public:
    HttpHeader & operator =(HttpHeader const & obj);
    HttpHeader & operator =(HttpHeader && obj);

public:
    mapped_type operator [](key_type const & key);

public:
    inline       iterator begin()       TBAG_NOEXCEPT_SP_OP(_headers.begin()) { return _headers.begin(); }
    inline const_iterator begin() const TBAG_NOEXCEPT_SP_OP(_headers.begin()) { return _headers.begin(); }
    inline       iterator   end()       TBAG_NOEXCEPT_SP_OP(_headers.  end()) { return _headers.  end(); }
    inline const_iterator   end() const TBAG_NOEXCEPT_SP_OP(_headers.  end()) { return _headers.  end(); }

    inline       reverse_iterator rbegin()       TBAG_NOEXCEPT_SP_OP(_headers.rbegin()) { return _headers.rbegin(); }
    inline const_reverse_iterator rbegin() const TBAG_NOEXCEPT_SP_OP(_headers.rbegin()) { return _headers.rbegin(); }
    inline       reverse_iterator   rend()       TBAG_NOEXCEPT_SP_OP(_headers.  rend()) { return _headers.  rend(); }
    inline const_reverse_iterator   rend() const TBAG_NOEXCEPT_SP_OP(_headers.  rend()) { return _headers.  rend(); }

    inline         const_iterator  cbegin() const TBAG_NOEXCEPT_SP_OP(_headers. cbegin()) { return _headers. cbegin(); }
    inline         const_iterator    cend() const TBAG_NOEXCEPT_SP_OP(_headers.   cend()) { return _headers.   cend(); }
    inline const_reverse_iterator crbegin() const TBAG_NOEXCEPT_SP_OP(_headers.crbegin()) { return _headers.crbegin(); }
    inline const_reverse_iterator   crend() const TBAG_NOEXCEPT_SP_OP(_headers.  crend()) { return _headers.  crend(); }

    inline bool         empty() const TBAG_NOEXCEPT_SP_OP(_headers.   empty()) { return _headers.   empty(); }
    inline size_type     size() const TBAG_NOEXCEPT_SP_OP(_headers.    size()) { return _headers.    size(); }
    inline size_type max_size() const TBAG_NOEXCEPT_SP_OP(_headers.max_size()) { return _headers.max_size(); }

    inline   key_compare   key_comp() const { return _headers.  key_comp(); }
    inline value_compare value_comp() const { return _headers.value_comp(); }

    inline void clear() { _headers.clear(); }

    inline void swap(HttpHeader & obj) { _headers.swap(obj._headers); }

    inline iterator insert(value_type const & pair) { return _headers.insert(pair); }
    inline iterator insert(value_type && pair)      { return _headers.insert(std::move(pair)); }

    inline iterator  erase(const_iterator itr)                        { return _headers.erase(itr); }
    inline iterator  erase(iterator itr)                              { return _headers.erase(itr); }
    inline iterator  erase(const_iterator first, const_iterator last) { return _headers.erase(first, last); }
    inline size_type erase(key_type const & key)                      { return _headers.erase(key); }

    inline       iterator find(key_type const & key)       { return _headers.find(key); }
    inline const_iterator find(key_type const & key) const { return _headers.find(key); }

    inline       iterator lower_bound(key_type const & key)       { return _headers.lower_bound(key); }
    inline const_iterator lower_bound(key_type const & key) const { return _headers.lower_bound(key); }

    inline       iterator upper_bound(key_type const & key)       { return _headers.upper_bound(key); }
    inline const_iterator upper_bound(key_type const & key) const { return _headers.upper_bound(key); }

    using       range = std::pair<      iterator,       iterator>;
    using const_range = std::pair<const_iterator, const_iterator>;

    inline       range equal_range(key_type const & key)       { return _headers.equal_range(key); }
    inline const_range equal_range(key_type const & key) const { return _headers.equal_range(key); }

public:
    /**
     * Lowercase and trim of all tokens.
     */
    HttpHeader getRegularization() const;
    void regularization();

public:
    mapped_type get(key_type const & key) const;
    mapped_type getIgnoreCase(key_type const & key) const;

public:
    iterator  insert(key_type const & key, mapped_type const & val);
    size_type insert(HttpHeader const & header);

    iterator  insertIfNotExists(key_type const & key, mapped_type const & val);
    size_type insertIfNotExists(HttpHeader const & header);

public:
    bool exists(key_type const & key) const;
    bool exists(key_type const & key, mapped_type const & val) const;

    bool existsInSplitValues(key_type const & key, mapped_type const & val) const;
    bool existsInSplitValues(key_type const & key, mapped_type const & val, mapped_type const & delimiter) const;

public:
    void updateDefaultRequest(std::size_t body_size = 0);
    void updateDefaultResponse(std::size_t body_size = 0);

public:
    /**
     * Generate HTTP Message Headers.
     *
     * @remarks
     *  @code
     *   message-header = field-name ":" [ field-value ]
     *   field-name     = token
     *   field-value    = *( field-content | LWS )
     *   field-content  = <the OCTETs making up the field-value
     *                    and consisting of either *TEXT or combinations
     *                    of token, separators, and quoted-string>
     *  @endcode
     *
     * @see <https://tools.ietf.org/html/rfc2616#section-4.2>
     */
    std::string toString() const;
};

} // namespace common
} // namespace http
} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_HTTP_COMMON_HTTPHEADER_HPP__

