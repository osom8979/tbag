/**
 * @file   MqCommon.cpp
 * @brief  MqCommon class implementation.
 * @author zer0
 * @date   2018-11-10
 */

#include <libtbag/mq/details/MqCommon.hpp>
#include <libtbag/network/details/NetCommon.hpp>
#include <libtbag/network/Uri.hpp>
#include <libtbag/string/StringUtils.hpp>
#include <libtbag/log/Log.hpp>
#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq      {
namespace details {

// -----------------------------
// MqMsgCopyFrom implementation.
// -----------------------------

MqMsgCopyFrom::MqMsgCopyFrom(MqMsg const & msg) : source_msg(msg)
{
    // EMPTY.
}

MqMsgCopyFrom::~MqMsgCopyFrom()
{
    // EMPTY.
}

bool MqMsgCopyFrom::operator()(MqMsg * msg)
{
    assert(msg != nullptr);
    *msg = source_msg;
    return true;
}

// ---------------------------
// MqMsgCopyTo implementation.
// ---------------------------

MqMsgCopyTo::MqMsgCopyTo(MqMsg & msg) : destination_msg(msg)
{
    // EMPTY.
}

MqMsgCopyTo::~MqMsgCopyTo()
{
    // EMPTY.
}

bool MqMsgCopyTo::operator()(MqMsg * msg)
{
    destination_msg = *msg;
    return true;
}

// -----------------------
// Miscellaneous utilities
// -----------------------

MqParams convertUriToParams(std::string const & uri_string, bool convert_windows_path)
{
    using namespace libtbag::network;
    using namespace libtbag::string;
    MqParams params;
    Uri uri;
    if (!uri.parse(uri_string)) {
        return params;
    }

    auto const SCHEMA = libtbag::string::lower(uri.getSchema());
    if (SCHEMA == PIPE_LOWER_NAME) {
        params.type = MqType::MT_PIPE;
    } else if (SCHEMA == TCP_LOWER_NAME) {
        params.type = MqType::MT_TCP;
    } else if (SCHEMA == UDP_LOWER_NAME) {
        params.type = MqType::MT_UDP;
    } else {
        tDLogW("convertUriToParams() {} applies as a TCP type.", SCHEMA);
        params.type = MqType::MT_TCP;
    }

    if (params.type == MqType::MT_PIPE) {
        if (convert_windows_path) {
            params.address = replaceRegex(uri.getPath(), "/", "\\");
        } else {
            params.address = uri.getPath();
        }
        params.port = 0;
    } else {
        if (libtbag::network::details::isIp(uri.getHost()) && uri.isPort()) {
            params.address = uri.getHost();
            params.port    = uri.getPortNumber();
        } else {
            std::string host;
            int port;
            if (isSuccess(uri.requestAddrInfo(host, port, Uri::AddrFlags::MOST_IPV4))) {
                params.address = host;
                params.port    = port;
            } else {
                params.address = uri.getHost();
                params.port    = uri.getPortNumber();
            }
        }
    }

    auto const QUERIES = uri.getQueryMap();

    auto itr = QUERIES.find(TCP_IPV6_ONLY_NAME);
    if (itr != QUERIES.end()) {
        params.tcp_ipv6_only = toValue<bool>(itr->second);
    }

    itr = QUERIES.find(SEND_QUEUE_SIZE_NAME);
    if (itr != QUERIES.end()) {
        params.send_queue_size = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(SEND_MSG_SIZE_NAME);
    if (itr != QUERIES.end()) {
        params.send_msg_size = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(RECV_QUEUE_SIZE_NAME);
    if (itr != QUERIES.end()) {
        params.recv_queue_size = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(RECV_MSG_SIZE_NAME);
    if (itr != QUERIES.end()) {
        params.recv_msg_size = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(MAX_NODES_NAME);
    if (itr != QUERIES.end()) {
        params.max_nodes = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(PACKER_SIZE_NAME);
    if (itr != QUERIES.end()) {
        params.packer_size = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(WAIT_CLOSING_NAME);
    if (itr != QUERIES.end()) {
        params.wait_closing_millisec = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(VERIFY_MSG_NAME);
    if (itr != QUERIES.end()) {
        params.verify_restore_message = toValue<bool>(itr->second);
    }

    itr = QUERIES.find(READ_ERROR_NAME);
    if (itr != QUERIES.end()) {
        params.continuous_read_error_count = toValue<unsigned>(itr->second);
    }

    itr = QUERIES.find(TRY_RECONNECT_NAME);
    if (itr != QUERIES.end()) {
        params.try_reconnect_count = toValue<unsigned>(itr->second);
    }

    return params;
}

} // namespace details
} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

