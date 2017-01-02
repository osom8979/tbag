/**
 * @file   CommonTcp.cpp
 * @brief  CommonTcp class implementation.
 * @author zer0
 * @date   2017-01-02
 */

#include <libtbag/network/CommonTcp.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/uv/Request.hpp>
#include <libtbag/uv/Loop.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace network {

CommonTcp::CommonTcp()
{
    // EMPTY.
}

CommonTcp::CommonTcp(SharedTcp tcp) : _tcp(tcp)
{
    // EMPTY.
}

CommonTcp::CommonTcp(CallableTcp * tcp) : _tcp(tcp)
{
    // EMPTY.
}

CommonTcp::CommonTcp(uv::Loop & loop) : _tcp(new CallableTcp(loop, this))
{
    // EMPTY.
}

CommonTcp::~CommonTcp()
{
    // EMPTY.
}

CommonTcp::WriteRequest * CommonTcp::obtainWriteRequest()
{
    return static_cast<WriteRequest*>(_writers.create(_tcp.get()).lock().get());
}

void CommonTcp::releaseWriteRequest(WriteRequest * request)
{
    _writers.release(static_cast<uv::Request*>(request));
}

CommonTcp::WriteRequest * CommonTcp::asyncWrite(binf * infos, std::size_t infos_size)
{
    if (static_cast<bool>(_tcp)) {
        WriteRequest * request = obtainWriteRequest();
        if (request != nullptr) {
            if (_tcp->write(*request, infos, infos_size)) {
                return request;
            }
        }
    }
    return nullptr;
}

CommonTcp::WriteRequest * CommonTcp::asyncWrite(char const * buffer, std::size_t size)
{
    if (static_cast<bool>(_tcp)) {
        WriteRequest * request = obtainWriteRequest();
        if (request != nullptr) {
            if (_tcp->write(*request, buffer, size)) {
                return request;
            }
        }
    }
    return nullptr;
}

std::size_t CommonTcp::tryWrite(binf * infos, std::size_t infos_size, Err * result)
{
    if (static_cast<bool>(_tcp)) {
        return _tcp->tryWrite(infos, infos_size, result);
    }

    if (result != nullptr) {
        *result = Err::NULL_POINTER;
    }
    return 0U;
}

std::size_t CommonTcp::tryWrite(char const * buffer, std::size_t size, Err * result)
{
    if (static_cast<bool>(_tcp)) {
        return _tcp->tryWrite(buffer, size, result);
    }

    if (result != nullptr) {
        *result = Err::NULL_POINTER;
    }
    return 0U;
}

CommonTcp::binf CommonTcp::onAlloc(std::size_t suggested_size)
{
    // Realloc with read buffer.
    if (_read_buffer.size() < suggested_size) {
        _read_buffer.resize(suggested_size);
    }

    binf info;
    info.buffer = &_read_buffer[0];
    info.size   =  _read_buffer.size();
    return info;
}

} // namespace network

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

