/**
 * @file   TbagPacket.hpp
 * @brief  TbagPacket class prototype.
 * @author zer0
 * @date   2018-10-24
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

struct TbagPacketTypes : private Noncopyable
{
    // EMPTY.
};

/**
 * TbagPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 */
class TBAG_API TbagPacketBuilder : public TbagPacketTypes
{
public:
    struct Impl;
    friend struct Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;
    TBAG_CONSTEXPR static char const * const DEFAULT_ECHO_MESSAGE = "TBAG";

    TBAG_CONSTEXPR static unsigned int const TBAG_VER_MAJOR = LIBTBAG_VERSION_PACKET_MAJOR;
    TBAG_CONSTEXPR static unsigned int const TBAG_VER_MINOR = LIBTBAG_VERSION_PACKET_MINOR;

private:
    UniqueImpl _impl;

public:
    TbagPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~TbagPacketBuilder();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return static_cast<bool>(_impl); }

    inline operator bool() const TBAG_NOEXCEPT
    { return exists(); }

public:
    uint8_t * point() const;
    std::size_t size() const;

public:
    std::string toJsonString() const;

public:
    void clear();
    void finish();
};

/**
 * TbagPacketParser class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 */
class TBAG_API TbagPacketParser : public TbagPacketTypes
{
public:
    class Impl;
    friend class Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    TbagPacketParser();
    virtual ~TbagPacketParser();

public:
    Err parse(char const * buffer, std::size_t size, void * arg = nullptr);
};

/**
 * TbagPacket class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 */
class TBAG_API TbagPacket : public TbagPacketBuilder, public TbagPacketParser
{
public:
    TbagPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY)
            : TbagPacketBuilder(capacity), TbagPacketParser()
    {
        // EMPTY.
    }

    virtual ~TbagPacket()
    {
        // EMPTY.
    }
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_TBAGPACKET_HPP__

