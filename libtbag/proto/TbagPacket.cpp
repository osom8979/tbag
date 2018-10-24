/**
 * @file   TbagPacket.cpp
 * @brief  TbagPacket class implementation.
 * @author zer0
 * @date   2018-10-24
 */

#if defined(TBAG_COMP_MSVC)
# if defined(min)
TBAG_PUSH_MACRO(min);
# undef min
# define __RESTORE_MIN__
# endif // defined(min)
# if defined(max)
TBAG_PUSH_MACRO(max);
# undef max
# define __RESTORE_MAX__
# endif // defined(max)
#endif // defined(TBAG_COMP_MSVC)

// FlatBuffers generated files.
#include <flatbuffers/flatbuffers.h>
#include <flatbuffers/idl.h>
#include <libtbag/proto/fbs/tbag_generated.h>
#include <libtbag/proto/fbs/tbag_t2s.h>

#if defined(TBAG_COMP_MSVC)
# if defined(__RESTORE_MIN__)
TBAG_POP_MACRO(min);
# undef __RESTORE_MIN__
# endif // defined(__RESTORE_MIN__)
# if defined(__RESTORE_MAX__)
TBAG_POP_MACRO(max);
# undef __RESTORE_MAX__
# endif // defined(__RESTORE_MAX__)
#endif // defined(TBAG_COMP_MSVC)

#include <libtbag/proto/TbagPacket.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/Type.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

using namespace proto::fbs;

using FlatBufferBuilder = flatbuffers::FlatBufferBuilder;
using FlatBufferParser  = flatbuffers::Parser;

using StringOffset       = flatbuffers::Offset<flatbuffers::String>;
using StringOffsetVector = flatbuffers::Vector<StringOffset>;

/**
 * FlatBuffer builder implementation.
 *
 * @author zer0
 * @date   2018-10-24
 */
struct TbagPacketBuilder::Impl
{
private:
    TbagPacketBuilder * _parent = nullptr;

private:
    FlatBufferBuilder builder;
    FlatBufferParser  parser;

public:
    Impl(TbagPacketBuilder * parent, std::size_t capacity) : _parent(parent), builder(capacity, nullptr)
    {
        if (parser.Parse(__get_text_to_cpp11_string__tbag__()) == false) {
            tDLogA("TbagPacketBuilder::Impl() Parse fail.");
            throw std::bad_alloc();
        }
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    uint8_t * point() const
    {
        return builder.GetBufferPointer();
    }

    std::size_t size() const
    {
        return builder.GetSize();
    }

    void clear()
    {
        builder.Clear();
    }

    std::string toJsonString() const
    {
        std::string result;
        if (flatbuffers::GenerateText(parser, builder.GetBufferPointer(), &result)) {
            return result;
        }
        return std::string();
    }

    template <typename T>
    void finish(flatbuffers::Offset<T> root, char const * file_identifier = nullptr)
    {
        builder.Finish(root, file_identifier);
    }
};

// ---------------------------------
// TbagPacketBuilder implementation.
// ---------------------------------

TbagPacketBuilder::TbagPacketBuilder(std::size_t capacity) : _impl(std::make_unique<Impl>(this, capacity))
{
    assert(static_cast<bool>(_impl));
}

TbagPacketBuilder::~TbagPacketBuilder()
{
    // EMPTY.
}

uint8_t * TbagPacketBuilder::point() const
{
    assert(exists());
    return _impl->point();
}

std::size_t TbagPacketBuilder::size() const
{
    assert(exists());
    return _impl->size();
}

std::string TbagPacketBuilder::toJsonString() const
{
    assert(exists());
    return _impl->toJsonString();
}

void TbagPacketBuilder::clear()
{
    assert(exists());
    _impl->clear();
}

void TbagPacketBuilder::finish()
{
    assert(exists());
}

/**
 * FlatBuffer parser implementation.
 *
 * @author zer0
 * @date   2018-10-24
 */
struct TbagPacketParser::Impl
{
private:
    TbagPacketParser * _parent = nullptr;

public:
    Impl(TbagPacketParser * parent) : _parent(parent)
    {
        // EMPTY.
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    Err parse(char const * buffer, std::size_t size, void * arg)
    {
        return Err::E_SUCCESS;
    }
};

// --------------------------------
// TbagPacketParser implementation.
// --------------------------------

TbagPacketParser::TbagPacketParser() : _impl(std::make_unique<Impl>(this))
{
    assert(static_cast<bool>(_impl));
}

TbagPacketParser::~TbagPacketParser()
{
    // EMPTY.
}

Err TbagPacketParser::parse(char const * buffer, std::size_t size, void * arg)
{
    return Err::E_SUCCESS;
}

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

