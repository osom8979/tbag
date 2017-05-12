/**
 * @file   SafetyWriteAsync.cpp
 * @brief  SafetyWriteAsync class implementation.
 * @author zer0
 * @date   2017-05-06
 */

#include <libtbag/uvpp/ex/SafetyWriteAsync.hpp>
#include <libtbag/log/Log.hpp>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

// -------------------------------
// WriterInterface implementation.
// -------------------------------

SafetyWriteAsync::WriterInterface::WriterInterface() : id(id::generator::genTimeId())
{
    state.store(WriteState::READY);
    result.store(uerr::UVPP_UNKNOWN);
}

SafetyWriteAsync::WriterInterface::~WriterInterface()
{
    // EMPTY.
}

bool SafetyWriteAsync::WriterInterface::cancel()
{
    WriteState EXPECTED = WriteState::READY;
    return state.compare_exchange_weak(EXPECTED, WriteState::CANCEL);
}

// ----------------------------
// StreamWriter implementation.
// ----------------------------

void SafetyWriteAsync::StreamWriter::run(SafetyAsync * UNUSED_PARAM(handle))
{
    WriteState EXPECTED = WriteState::READY;
    if (state.compare_exchange_weak(EXPECTED, WriteState::WRITE)) {
        if (auto shared = stream.lock()) {
            result = shared->write(req, infos.data(), infos.size());
        } else {
            tDLogE("SafetyWriteAsync::StreamWriter::run() Stream is nullptr.");
        }
    } else {
        tDLogD("SafetyWriteAsync::StreamWriter::run() Not ready state.");
    }
}

// -------------------------
// UdpWriter implementation.
// -------------------------

void SafetyWriteAsync::UdpWriter::run(SafetyAsync * UNUSED_PARAM(handle))
{
    WriteState EXPECTED = WriteState::READY;
    if (state.compare_exchange_weak(EXPECTED, WriteState::WRITE)) {
        if (auto shared = udp.lock()) {
            result = shared->send(req, infos.data(), infos.size(), addr.get());
        } else {
            tDLogE("SafetyWriteAsync::UdpWriter::run() Stream is nullptr.");
        }
    } else {
        tDLogD("SafetyWriteAsync::UdpWriter::run() Not ready state.");
    }
}

// --------------------------------
// SafetyWriteAsync implementation.
// --------------------------------

SafetyWriteAsync::SafetyWriteAsync(Loop & loop) : SafetyAsync(loop)
{
    // EMPTY.
}

SafetyWriteAsync::~SafetyWriteAsync()
{
    // EMPTY.
}

bool SafetyWriteAsync::updateWriterInfo(WriterInterface * writer, binf const * infos, std::size_t size)
{
    assert(writer != nullptr);
    assert(infos != nullptr);
    assert(size >= 1U);

    writer->infos.resize(size);
    writer->bufs.resize(size);

    assert(writer->infos.size() == size);
    assert(writer->bufs.size() == size);

    for (std::size_t i = 0; i < size; ++i) {
        binf const * info = (infos + i);
        writer->infos[i].buffer = info->buffer;
        writer->infos[i].size   = info->size;
        writer->bufs[i].assign(info->buffer, info->buffer + info->size);
    }

    return true;
}

SafetyWriteAsync::SharedWriter SafetyWriteAsync::createWrite(WeakStream stream, binf const * infos, std::size_t size)
{
    StreamWriter * writer = new (std::nothrow) StreamWriter;
    assert(writer != nullptr);
    writer->stream = stream;

    if (updateWriterInfo(writer, infos, size) == false) {
        delete writer;
        writer = nullptr;
        return SharedWriter();
    }
    return SharedWriter(writer);
}

SafetyWriteAsync::SharedWriter SafetyWriteAsync::createWrite(WeakStream stream, char const * buffer, std::size_t size)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return createWrite(stream, &info, 1U);
}

SafetyWriteAsync::SharedWriter SafetyWriteAsync::createWrite(
        WeakUdp udp,
        binf const * infos,
        std::size_t size,
        sockaddr const * addr)
{
    UdpWriter * writer = new (std::nothrow) UdpWriter;
    assert(writer != nullptr);
    assert(addr != nullptr);

    if (addr->sa_family == AF_INET) {
        writer->addr.reset((sockaddr*)new sockaddr_in);
    } else if (addr->sa_family == AF_INET6) {
        writer->addr.reset((sockaddr*)new sockaddr_in6);
    }
    writer->udp = udp;

    if (updateWriterInfo(writer, infos, size) == false) {
        delete writer;
        writer = nullptr;
        return SharedWriter();
    }
    return SharedWriter(writer);
}

SafetyWriteAsync::SharedWriter SafetyWriteAsync::createWrite(
        WeakUdp udp,
        char const * buffer,
        std::size_t size,
        sockaddr const * addr)
{
    binf info;
    info.buffer = const_cast<char*>(buffer);
    info.size   = size;
    return createWrite(udp, &info, 1U, addr);
}

bool SafetyWriteAsync::asyncWrite(SharedWriter writer)
{
    return sendJob(writer) == uerr::UVPP_SUCCESS;
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

