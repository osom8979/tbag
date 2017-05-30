/**
 * @file   Uri.hpp
 * @brief  Uri class prototype.
 * @author zer0
 * @date   2017-05-19
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_URI_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_URI_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Err.hpp>

#include <cstdint>
#include <algorithm>
#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

/**
 * Uri class prototype.
 *
 * @author zer0
 * @date   2017-05-19
 */
class TBAG_API Uri
{
public:
    struct FieldInfo
    {
        uint16_t offset; ///< Offset into buffer in which field starts.
        uint16_t length; ///< Length of run in buffer.
        bool     enable; ///< Enable flag.

        FieldInfo(uint16_t off, uint16_t len, bool en = true) : offset(off), length(len), enable(en)
        { /* EMPTY. */ }
        FieldInfo() : FieldInfo(0, 0, false)
        { /* EMPTY. */ }

        inline void set(uint16_t off, uint16_t len, bool en = true) TBAG_NOEXCEPT
        { offset = off; length = len; enable = en; }
        inline void clear() TBAG_NOEXCEPT
        { offset = 0; length = 0; enable = false; }

        inline bool operator ==(FieldInfo const & obj) const TBAG_NOEXCEPT
        { return offset == obj.offset && length == obj.length && enable == obj.enable; }
        inline bool operator !=(FieldInfo const & obj) const TBAG_NOEXCEPT
        { return !((*this).operator==(obj)); }

        void swap(FieldInfo & obj) TBAG_NOEXCEPT
        {
            if (this != &obj) {
                std::swap(offset, obj.offset);
                std::swap(length, obj.length);
                std::swap(enable, obj.enable);
            }
        }
    };

private:
    std::string _uri;

private:
    FieldInfo _schema  ;
    FieldInfo _host    ;
    FieldInfo _port    ;
    FieldInfo _path    ;
    FieldInfo _query   ;
    FieldInfo _fragment;
    FieldInfo _userinfo;

public:
    Uri();
    explicit Uri(char const * uri, bool is_connect = false);
    explicit Uri(std::string const & uri, bool is_connect = false);
    Uri(Uri const & obj);
    Uri(Uri && obj);
    ~Uri();

public:
    Uri & operator =(char const * uri);
    Uri & operator =(std::string const & uri);

    Uri & operator =(Uri const & obj);
    Uri & operator =(Uri && obj);

// Casting
public:
    operator std::string() const;
    operator char const *() const;

public:
    void swap(Uri & obj) TBAG_NOEXCEPT;

public:
    inline FieldInfo getSchemaInfo  () const TBAG_NOEXCEPT { return _schema  ; }
    inline FieldInfo getHostInfo    () const TBAG_NOEXCEPT { return _host    ; }
    inline FieldInfo getPortInfo    () const TBAG_NOEXCEPT { return _port    ; }
    inline FieldInfo getPathInfo    () const TBAG_NOEXCEPT { return _path    ; }
    inline FieldInfo getQueryInfo   () const TBAG_NOEXCEPT { return _query   ; }
    inline FieldInfo getFragmentInfo() const TBAG_NOEXCEPT { return _fragment; }
    inline FieldInfo getUserinfoInfo() const TBAG_NOEXCEPT { return _userinfo; }

public:
    inline bool isSchema  () const TBAG_NOEXCEPT { return _schema  .enable; }
    inline bool isHost    () const TBAG_NOEXCEPT { return _host    .enable; }
    inline bool isPort    () const TBAG_NOEXCEPT { return _port    .enable; }
    inline bool isPath    () const TBAG_NOEXCEPT { return _path    .enable; }
    inline bool isQuery   () const TBAG_NOEXCEPT { return _query   .enable; }
    inline bool isFragment() const TBAG_NOEXCEPT { return _fragment.enable; }
    inline bool isUserinfo() const TBAG_NOEXCEPT { return _userinfo.enable; }

public:
    std::string getSchema  () const { return getFieldString(_uri, _schema  ); }
    std::string getHost    () const { return getFieldString(_uri, _host    ); }
    std::string getPort    () const { return getFieldString(_uri, _port    ); }
    std::string getPath    () const { return getFieldString(_uri, _path    ); }
    std::string getQuery   () const { return getFieldString(_uri, _query   ); }
    std::string getFragment() const { return getFieldString(_uri, _fragment); }
    std::string getUserinfo() const { return getFieldString(_uri, _userinfo); }

public:
    int getPortNumber() const;
    std::string getUrl() const;
    std::string getRequestPath() const;

public:
    void clear();
    bool parse(std::string const & uri, bool is_connect = false);

// Alias methods.
public:
    inline bool set(std::string const & uri, bool is_connect = false)
    { return parse(uri, is_connect); }

    inline std::string getString() const
    { return _uri; }

public:
    enum class AddrFlags
    {
        MOST_IPV4,
        MOST_IPV6,
    };

    Err requestAddrInfo(std::string & host, int & port, AddrFlags flags = AddrFlags::MOST_IPV4) const;

// Static methods.
public:
    static std::string getFieldString(std::string const & original, FieldInfo const & info);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_URI_HPP__

