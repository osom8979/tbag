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

std::string RotateFileSink::getDefaultArguments(std::string const & path)
{
    using namespace libtbag::filesystem;
    std::stringstream ss;
    ss << "size=10240m ";
    ss << "archive=.zip ";
    ss << "time=" << TimeFormatUpdater::getDefaultTimeFormatString(Path(path));
    return ss.str();
}

RotateFileSink::RotateFileSink()
        : _rotate(std::make_shared<libtbag::filesystem::MaxSizeWriter>(),
                  std::make_shared<libtbag::filesystem::TimeFormatUpdater>())
{
    // EMPTY.
}

RotateFileSink::RotateFileSink(default_init_t, std::string const & path)
        : _rotate(getDefaultArguments(path))
{
    // EMPTY.
}

RotateFileSink::RotateFileSink(std::string const & arguments)
        : _rotate(arguments)
{
    // EMPTY.
}

RotateFileSink::~RotateFileSink()
{
    // EMPTY.
}

bool RotateFileSink::write(char const * message, int size)
{
    return _rotate.write(message, size) == E_SUCCESS;
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

