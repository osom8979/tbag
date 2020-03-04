/**
 * @file   NtpClient.cpp
 * @brief  NtpClient class implementation.
 * @author zer0
 * @date   2020-03-03
 */

#include <libtbag/time/NtpClient.hpp>
#include <libtbag/bitwise/Endian.hpp>
#include <libtbag/uvxx/UxLoop.hpp>
#include <libtbag/uvxx/UxUdp.hpp>
#include <libtbag/uvxx/UxIdle.hpp>
#include <libtbag/uvxx/UxTimer.hpp>
#include <libtbag/net/SocketAddress.hpp>
#include <libtbag/macro/attributes.hpp>

#include <cstdint>
#include <cassert>
#include <ctime>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace time {

/**
 * NTP Packet Header structure.
 *
 * @author zer0
 * @date   2020-03-03
 *
 * @see <https://tools.ietf.org/html/rfc5905#section-7.3>
 *
 * Packet Header Variables:
 * <pre>
 *  +-----------+------------+-----------------------+
 *  | Name      | Formula    | Description           |
 *  +-----------+------------+-----------------------+
 *  | leap      | leap       | leap indicator (LI)   |
 *  | version   | version    | version number (VN)   |
 *  | mode      | mode       | mode                  |
 *  | stratum   | stratum    | stratum               |
 *  | poll      | poll       | poll exponent         |
 *  | precision | rho        | precision exponent    |
 *  | rootdelay | delta_r    | root delay            |
 *  | rootdisp  | epsilon_r  | root dispersion       |
 *  | refid     | refid      | reference ID          |
 *  | reftime   | reftime    | reference timestamp   |
 *  | org       | T1         | origin timestamp      |
 *  | rec       | T2         | receive timestamp     |
 *  | xmt       | T3         | transmit timestamp    |
 *  | dst       | T4         | destination timestamp |
 *  | keyid     | keyid      | key ID                |
 *  | dgst      | dgst       | message digest        |
 *  +-----------+------------+-----------------------+
 * </pre>
 *
 * Packet Header Format:
 * <pre>
 *   0                   1                   2                   3
 *   0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |LI | VN  |Mode |    Stratum     |     Poll      |  Precision   |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Root Delay                            |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                         Root Dispersion                       |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          Reference ID                         |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                     Reference Timestamp (64)                  +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                      Origin Timestamp (64)                    +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                      Receive Timestamp (64)                   +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  +                      Transmit Timestamp (64)                  +
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  .                                                               .
 *  .                    Extension Field 1 (variable)               .
 *  .                                                               .
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  .                                                               .
 *  .                    Extension Field 2 (variable)               .
 *  .                                                               .
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                          Key Identifier                       |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *  |                                                               |
 *  |                            dgst (128)                         |
 *  |                                                               |
 *  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * </pre>
 */
TBAG_ALIGNED_STRUCT_BEGIN(1) NtpPacketHeader
{
    /**
     * Bitfield information:
     * - [LI] (2 bits) Leap indicator.
     * - [VN] (3 bits) Version number of the protocol. (currently 4)
     * - [MODE] (3 bits) Client will pick mode 3 for client.
     *
     * Leap Indicator:
     * <pre>
     *  +-------+----------------------------------------+
     *  | Value | Meaning                                |
     *  +-------+----------------------------------------+
     *  | 0     | no warning                             |
     *  | 1     | last minute of the day has 61 seconds  |
     *  | 2     | last minute of the day has 59 seconds  |
     *  | 3     | unknown (clock unsynchronized)         |
     *  +-------+----------------------------------------+
     * </pre>
     *
     * Association Modes:
     * <pre>
     *  +-------+--------------------------+
     *  | Value | Meaning                  |
     *  +-------+--------------------------+
     *  | 0     | reserved                 |
     *  | 1     | symmetric active         |
     *  | 2     | symmetric passive        |
     *  | 3     | client                   |
     *  | 4     | server                   |
     *  | 5     | broadcast                |
     *  | 6     | NTP control message      |
     *  | 7     | reserved for private use |
     *  +-------+--------------------------+
     * </pre>
     */
    uint8_t li_vn_mode;
    uint8_t stratum;            ///< 8 bits. Stratum level of the local clock.
    uint8_t poll;               ///< 8 bits. Maximum interval between successive messages.
    uint8_t precision;          ///< 8 bits. Precision of the local clock.

    uint32_t root_delay;        ///< 32 bits. Total round trip delay time.
    uint32_t root_dispersion;   ///< 32 bits. Max error aloud from primary clock source.
    uint32_t reference_id;      ///< 32 bits. Reference clock identifier.

    uint32_t ref_tm_s;          ///< 32 bits. Reference time-stamp seconds.
    uint32_t ref_tm_f;          ///< 32 bits. Reference time-stamp fraction of a second.

    uint32_t origin_tm_s;       ///< 32 bits. Originate time-stamp seconds.
    uint32_t origin_tm_f;       ///< 32 bits. Originate time-stamp fraction of a second.

    uint32_t rx_tm_s;           ///< 32 bits. Received time-stamp seconds.
    uint32_t rx_tm_f;           ///< 32 bits. Received time-stamp fraction of a second.

    // --[[ The most important field the client cares about.
    uint32_t tx_tm_s;           ///< 32 bits. Transmit time-stamp seconds.
    uint32_t tx_tm_f;           ///< 32 bits. Transmit time-stamp fraction of a second.
    // ]]--

    // Skip more ...
};
TBAG_ALIGNED_STRUCT_END(NtpPacketHeader, 48);

// ---------
// NtpClient
// ---------

using TimeResult = NtpClient::TimeResult;

// Set the first byte's bits to [00,011,011] for li = 0, vn = 3, and mode = 3.
// The rest will be left set to zero.
// Represents 27 in base 10 or [00011011] in base 2.
TBAG_CONSTEXPR static uint8_t const NTP_SEND_FLAGS = 0x1b;

#define LI(p)   (uint8_t)((p.li_vn_mode & 0xC0) >> 6)
#define VN(p)   (uint8_t)((p.li_vn_mode & 0x38) >> 3)
#define MODE(p) (uint8_t)((p.li_vn_mode & 0x07) >> 0)

NtpClient::NtpClient(std::string const & server, int port)
        : _server(server), _port(port)
{
    // EMPTY.
}

NtpClient::~NtpClient()
{
    // EMPTY.
}

TimeResult NtpClient::request(int timeout_ms) const
{
    using namespace libtbag::uvxx;
    UxLoop loop;

    UxIdle idle;
    auto const idle_init_code = idle.init(loop);
    if (isFailure(idle_init_code)) {
        return idle_init_code;
    }
    assert(idle.isInit());
    assert(loop.size() == 1u);

    UxUdp udp;
    auto const udp_init_code = udp.init(loop);
    if (isFailure(udp_init_code)) {
        return udp_init_code;
    }
    assert(udp.isInit());
    assert(loop.size() == 2u);

    UxTimer timer;
    auto const timer_init_code = timer.init(loop);
    if (isFailure(timer_init_code)) {
        return timer_init_code;
    }
    assert(timer.isInit());
    assert(loop.size() == 3u);

    libtbag::net::SocketAddress remote_address;
    auto const remote_init_code = remote_address.init(_server, _port);
    if (isFailure(remote_init_code)) {
        return remote_init_code;
    }

    NtpPacketHeader send_packet = {};
    memset(&send_packet, 0x00, sizeof(send_packet));
    send_packet.li_vn_mode = NTP_SEND_FLAGS;

    Err result = E_UNKNOWN;

    UxUdp::UdpSendRequest req;
    idle.setOnIdle([&](){
        idle.close();

        auto const timer_start_code = timer.start(timeout_ms);
        if (isFailure(timer_start_code)) {
            result = timer_start_code;
            timer.close();
            udp.close();
            return;
        }

        auto const send_code = udp.send(req, (char const *)&send_packet,
                                        sizeof(send_packet), remote_address.getCommon());
        if (isFailure(send_code)) {
            result = send_code;
            timer.stop();
            timer.close();
            udp.close();
        }
    });

    NtpPacketHeader recv_packet = {};
    memset(&recv_packet, 0x00, sizeof(recv_packet));

    std::vector<char> recv_buffer;
    udp.setOnAlloc([&](std::size_t suggested_size) -> libtbag::uvpp::binf {
        return libtbag::uvpp::defaultOnAlloc(recv_buffer, suggested_size);
    });
    udp.setOnRecv([&](Err code, char const * buffer, std::size_t size, sockaddr const * addr, unsigned int flags){
        if (size == sizeof(recv_packet)) {
            result = E_SUCCESS;
            memcpy(&recv_packet, buffer, size);
        } else {
            result = E_VERIFIER;
        }
        timer.close();
        udp.close();
    });

    timer.setOnTimer([&](){
        result = E_TIMEOUT;
        timer.close();
        udp.close();
    });

    auto const recv_code = udp.startRecv();
    if (isFailure(recv_code)) {
        return recv_code;
    }

    auto const idle_start_code = idle.start();
    if (isFailure(idle_start_code)) {
        return idle_start_code;
    }

    auto const run_code = loop.run();
    assert(isSuccess(run_code));
    assert(loop.empty());

    if (isFailure(result)) {
        return result;
    }

    using namespace libtbag::bitwise;
    recv_packet.tx_tm_s = toHost(recv_packet.tx_tm_s); // Time-stamp seconds.
    recv_packet.tx_tm_f = toHost(recv_packet.tx_tm_f); // Time-stamp fraction of a second.
    auto const tx_tm = (time_t)(recv_packet.tx_tm_s - TIMESTAMP_DELTA);
    using namespace std::chrono;
    return { E_SUCCESS, TimePoint(system_clock::from_time_t(tx_tm)) };
}

} // namespace time

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

