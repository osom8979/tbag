/**
 * @file   MqPipeline.cpp
 * @brief  MqPipeline class implementation.
 * @author zer0
 * @date   2018-12-07
 */

#include <libtbag/mq/MqPipeline.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/type/TypeTable.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace mq {

MqPipeline::MqPipeline(MqNode * recv, MqNode * send, uint64_t wait_nano)
        : _recv(recv), _send(send), _wait_nano(wait_nano), _exit(false)
{
    // EMPTY.
}

MqPipeline::~MqPipeline()
{
    // EMPTY.
}

void MqPipeline::run(bool validate, bool verbose)
{
    MqMsg recv_msg;
    MqMsg send_msg;

    MqNode * recv;
    MqNode * send;

    Err code;

    while (!_exit) {
        recv = _recv.load();
        assert(recv != nullptr);

        code = recv->recvWait(recv_msg, static_cast<uint64_t>(_wait_nano.load()));
        if (code == Err::E_TIMEOUT) {
            tDLogIfD(verbose, "MqPipeline::run() Node recv timeout.");
            onError(Err::E_TIMEOUT);
            continue;
        }

        if (code == Err::E_ECANCELED) {
            tDLogIfI(verbose, "MqPipeline::run() Recv canceled.");
            break;
        }

        assert(isSuccess(code));
        assert(recv_msg.stream != 0);

        if (validate && recv_msg.size() > libtbag::type::TypeInfo<int>::maximum()) {
            tDLogW("MqPipeline::run() The size of the data is too large, Cancel this message.");
            onError(Err::E_VERIFIER);
            continue;
        }

        if (onMsg(recv_msg, send_msg)) {
            send = _send.load();
            assert(send != nullptr);
            send->send(send_msg);
        }
    }
}

void MqPipeline::onError(Err code)
{
    // EMPTY.
}

bool MqPipeline::onMsg(MqMsg const & recv_msg, MqMsg & send_msg)
{
    return true;
}

} // namespace mq

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

