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
    using String = std::string;

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
    String _uri;

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
    Uri(String const & uri);
    Uri(Uri const & obj);
    Uri(Uri && obj);
    ~Uri();

public:
    Uri & operator =(Uri const & obj);
    Uri & operator =(Uri && obj);

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
    String getSchema  () const { return getFieldString(_uri, _schema  ); }
    String getHost    () const { return getFieldString(_uri, _host    ); }
    String getPort    () const { return getFieldString(_uri, _port    ); }
    String getPath    () const { return getFieldString(_uri, _path    ); }
    String getQuery   () const { return getFieldString(_uri, _query   ); }
    String getFragment() const { return getFieldString(_uri, _fragment); }
    String getUserinfo() const { return getFieldString(_uri, _userinfo); }

public:
    int getPortNumber() const;

public:
    void clear();
    bool parse(String const & uri, bool is_connect = false);

// Alias methods.
public:
    inline bool set(String const & uri, bool is_connect = false)
    { return parse(uri, is_connect); }

    inline String get() const
    { return _uri; }

public:
    enum class AddrFlags
    {
        MOST_IPV4,
        MOST_IPV6,
    };

    Err requestAddrInfo(String & host, int & port, AddrFlags flags = AddrFlags::MOST_IPV4);

// Static methods.
public:
    static String getFieldString(String const & original, FieldInfo const & info);
};

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_NETWORK_URI_HPP__

