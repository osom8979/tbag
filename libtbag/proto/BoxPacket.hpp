/**
 * @file   BoxPacket.hpp
 * @brief  BoxPacket class prototype.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_BOXPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_BOXPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/Type.hpp>
#include <libtbag/container/Box.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <cstdint>
#include <string>
#include <map>
#include <unordered_map>
#include <memory>
#include <atomic>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

struct BoxPacketTypes : private Noncopyable
{
    using TypeTable = libtbag::type::TypeTable;
    using Buffer    = libtbag::util::Buffer;
    using Box       = libtbag::container::Box;
#if !defined(NDEBUG) && defined(TBAG_PLATFORM_MACOS)
    using BoxMap  = std::map<std::string, Box>;
#else
    using BoxMap  = std::unordered_map<std::string, Box>;
#endif
    static_assert(Box::getMaxDims() == 8, "FlatBuffers compatibility check. (table Pair)");
};

/**
 * BoxPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacketBuilder -> BoxPacketBuilder)
 */
class TBAG_API BoxPacketBuilder : public BoxPacketTypes
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
    BoxPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~BoxPacketBuilder();

public:
    uint8_t * point() const;
    std::size_t size() const;

public:
    Err assign(uint8_t const * buffer, std::size_t size);
    Err assign(Buffer const & buffer);

public:
    std::string toJsonString() const;

public:
    Err build(uint64_t id, int32_t type = 0, int32_t code = 0);
    Err build(BoxMap const & boxes, uint64_t id = 0, int32_t type = 0, int32_t code = 0);
    Err build(std::string const & content, uint64_t id = 0, int32_t type = 0, int32_t code = 0);
    Err build(std::string const & key, std::string const & val, uint64_t id = 0, int32_t type = 0, int32_t code = 0);

public:
    Buffer toBuffer() const;
};

/**
 * BoxPacketParser class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacketParser -> BoxPacketParser)
 */
class TBAG_API BoxPacketParser : public BoxPacketTypes
{
public:
    class Impl;
    friend class Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

private:
    UniqueImpl _impl;

public:
    BoxPacketParser();
    virtual ~BoxPacketParser();

public:
    Err parse(char const * buffer, std::size_t size, void * arg, std::size_t * computed_size);
    Err parse(Buffer const & buffer, void * arg, std::size_t * computed_size);

public:
    Err parseOnlyHeader(char const * buffer, std::size_t size, void * arg);
    Err parseFindKey(char const * buffer, std::size_t size, std::string const & key, void * arg);

protected:
    virtual void onHeader(uint64_t id, int32_t type, int32_t code, void * arg) { /* EMPTY. */ }
    virtual void onPairSize(std::size_t size, void * arg) { /* EMPTY. */ }
    virtual void onPair(std::string && key, Box && val, void * arg) { /* EMPTY. */ }
};

/**
 * BoxPacket class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 */
class TBAG_API BoxPacket : public BoxPacketBuilder, public BoxPacketParser
{
private:
    uint64_t _id;
    int32_t  _type;
    int32_t  _code;
    BoxMap   _bags;

public:
    enum class UserArgType : int
    {
        UAT_UNKNOWN = -1,
        UAT_BAG_EX = -2,
    };

public:
    struct UserArg
    {
        int type = static_cast<int>(UserArgType::UAT_UNKNOWN);
        void * user = nullptr;
    };

public:
    BoxPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~BoxPacket();

protected:
    virtual void onHeader(uint64_t id, int32_t type, int32_t code, void * arg) override;
    virtual void onPair(std::string && key, Box && val, void * arg) override;

public:
    inline uint64_t   id() const TBAG_NOEXCEPT { return   _id; }
    inline uint32_t type() const TBAG_NOEXCEPT { return _type; }
    inline uint32_t code() const TBAG_NOEXCEPT { return _code; }

public:
    inline void   setId(uint64_t value) TBAG_NOEXCEPT {   _id = value; }
    inline void setType(uint32_t value) TBAG_NOEXCEPT { _type = value; }
    inline void setCode(uint32_t value) TBAG_NOEXCEPT { _code = value; }

public:
    inline BoxMap       & boxes()       TBAG_NOEXCEPT { return _bags; }
    inline BoxMap const & boxes() const TBAG_NOEXCEPT { return _bags; }

public:
    void clear();

public:
    Err parse(char const * buffer, std::size_t size, std::size_t * computed_size = nullptr);
    Err parse(Buffer const & buffer, std::size_t * computed_size = nullptr);
    Err parseSelf(std::size_t * computed_size = nullptr);

public:
    Box findKey(char const * buffer, std::size_t size, std::string const & key, Err * code = nullptr);
    Box findKey(Buffer const & buffer, std::string const & key, Err * code = nullptr);
    Box findKey(std::string const & key, Err * code = nullptr);

public:
    Err buildSelf();

public:
    Err saveFile(std::string const & path);
    Err loadFile(std::string const & path);
};

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_BOXPACKET_HPP__

