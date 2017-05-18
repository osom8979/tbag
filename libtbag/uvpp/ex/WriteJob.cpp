/**
 * @file   WriteJob.cpp
 * @brief  WriteJob class implementation.
 * @author zer0
 * @date   2017-05-05
 */

#include <libtbag/uvpp/ex/WriteJob.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace uvpp {
namespace ex   {

WriteJob::WriteJob(Stream * stream, WriteRequest * request, WriteBuffer buffer)
        : _stream(stream), _request(request), _buffer(buffer)
{
    _cancel.store(false);
    _complete.store(false);
    _result = Err::E_UNKNOWN;
}

WriteJob::~WriteJob()
{
    // EMPTY.
}

void WriteJob::run(SafetyAsync * handle)
{
    if (_cancel.load() == false) {
        _result = _stream->write(*_request, _buffer.data(), _buffer.size());
    }
    _complete.store(true);
}

} // namespace ex
} // namespace uvpp

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

