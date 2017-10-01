/**
 * @file   FunctionalTpotPacket.hpp
 * @brief  FunctionalTpotPacket class prototype.
 * @author zer0
 * @date   2017-09-15
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_PROTO_FUNCTIONALTPOTPACKET_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_PROTO_FUNCTIONALTPOTPACKET_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Type.hpp>

#include <libtbag/proto/TpotPacket.hpp>
#include <libtbag/functional/CallbackHelper.hpp>

#include <functional>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace proto {

/**
 * FunctionalTpotPacket class prototype.
 *
 * @author zer0
 * @date   2017-09-15
 */
template <typename TpotPacketType>
class FunctionalTpotPacket : public TpotPacketType
{
public:
    using Parent = TpotPacketType;

    STATIC_ASSERT_CHECK_IS_BASE_OF(libtbag::proto::TpotPacket, Parent);

public:
    FunctionalTpotPacket(std::size_t capacity = TpotPacket::DEFAULT_BUILDER_CAPACITY) : Parent(capacity)
    { /* EMPTY. */ }
    virtual ~FunctionalTpotPacket()
    { /* EMPTY. */ }

public:
    // @formatter:off
    TBAG_VOID_CALLBACK_HELPER(onVersionRequest       , util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onVersionResponse      , util::Header const &, util::Version const &, util::Pairs const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onEchoRequest          , util::Header const &, std::string const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onEchoResponse         , util::Header const &, std::string const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onLoginRequest         , util::Header const &, std::string const &, std::string const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onLoginResponse        , util::Header const &, std::string const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onLogoutRequest        , util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onLogoutResponse       , util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onExecRequest          , util::Header const &, util::ExecParam const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onExecResponse         , util::Header const &, int, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessListRequest   , util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessListResponse  , util::Header const &, std::vector<util::ProcessInfo> const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessKillRequest   , util::Header const &, int, int, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessKillResponse  , util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessRemoveRequest , util::Header const &, int, void*);
    TBAG_VOID_CALLBACK_HELPER(onProcessRemoveResponse, util::Header const &, void*);
    TBAG_VOID_CALLBACK_HELPER(onParsingError, Err, char const *, std::size_t, void*);
    // @formatter:on
};

using FuncTpotPacket = FunctionalTpotPacket<TpotPacket>;

} // namespace proto

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_PROTO_FUNCTIONALTPOTPACKET_HPP__

