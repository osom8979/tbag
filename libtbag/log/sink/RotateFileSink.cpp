/**
 * @file   RotateFileSink.cpp
 * @brief  RotateFileSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/RotateFileSink.hpp>
#include <libtbag/Err.hpp>
#include <sstream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

std::string RotateFileSink::getDefaultArguments()
{
    using namespace libtbag::filesystem;
    std::stringstream ss;
    ss << "size=10240m ";
    ss << "archive=.zip ";
    ss << "time=" << TimeFormatUpdater::getDefaultTimeFormatString(Path::getExePath());
    return ss.str();
}

RotateFileSink::RotateFileSink()
        : _rotate(std::make_shared<libtbag::filesystem::MaxSizeWriter>(),
                  std::make_shared<libtbag::filesystem::TimeFormatUpdater>())
{
    // EMPTY.
}

RotateFileSink::RotateFileSink(std::string const & arguments)
        : _rotate(arguments.empty() ? getDefaultArguments() : arguments)
{
    // EMPTY.
}

RotateFileSink::~RotateFileSink()
{
    // EMPTY.
}

bool RotateFileSink::write(int level, char const * message, int size)
{
    return _rotate.write(message, size) == E_SUCCESS;
}

bool RotateFileSink::isRequiredFlush()
{
    return true;
}

void RotateFileSink::flush()
{
    _rotate.flush();
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

