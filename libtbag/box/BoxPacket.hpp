/**
 * @file   BoxPacket.hpp
 * @brief  BoxPacket class prototype.
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXPACKET_HPP__

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

#include <libtbag/box/details/box_api.hpp>
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

namespace box {

/**
 * BoxPacketBuilder class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacketBuilder -> BoxPacketBuilder)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */
class TBAG_API BoxPacketBuilder
{
public:
    struct Impl;
    friend struct Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    using box_data = libtbag::box::details::box_data;
    using Buffer = libtbag::util::Buffer;

public:
    struct Options
    {
        /**
         * 'strict_json' adds "quotes" around field names if true.
         */
        bool strict_json = true;

        /**
         * If 'ident_step' is 0, no indentation will be generated.
         * if it is less than 0, no linefeeds will be generated either.
         */
        int indent_step = -1;

        Options() { /* EMPTY. */ }
        ~Options() { /* EMPTY. */ }
    };

public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_BUILDER_CAPACITY = 1 * MEGA_BYTE_TO_BYTE;

private:
    UniqueImpl _impl;

public:
    BoxPacketBuilder(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    BoxPacketBuilder(Options const & options, std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
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
    Err build(box_data const * box);

public:
    Buffer toBuffer() const;
};

/**
 * BoxPacketParser class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacketParser -> BoxPacketParser)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */
class TBAG_API BoxPacketParser
{
public:
    class Impl;
    friend class Impl;
    using UniqueImpl = std::unique_ptr<Impl>;

public:
    using box_data = libtbag::box::details::box_data;
    using Buffer = libtbag::util::Buffer;

private:
    UniqueImpl _impl;

public:
    BoxPacketParser();
    virtual ~BoxPacketParser();

public:
    Err parse(void const * buffer, std::size_t size, box_data * box, std::size_t * computed_size) const;
};

/**
 * BoxPacket class prototype.
 *
 * @author zer0
 * @date   2018-10-24
 * @date   2018-11-07 (Rename: BoxPacket -> BoxPacket)
 * @date   2019-05-19 (Move: libtbag::proto::BoxPacket -> libtbag::box::BoxPacket)
 */
class TBAG_API BoxPacket : public BoxPacketBuilder, public BoxPacketParser
{
public:
    BoxPacket(std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    BoxPacket(Options const & options, std::size_t capacity = DEFAULT_BUILDER_CAPACITY);
    virtual ~BoxPacket();
};

} // namespace box

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_BOX_BOXPACKET_HPP__

