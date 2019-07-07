/**
 * @file   RotateFileSink.cpp
 * @brief  RotateFileSink class implementation.
 * @author zer0
 * @date   2019-07-02
 */

#include <libtbag/log/sink/RotateFileSink.hpp>
#include <iostream>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace log  {
namespace sink {

RotateFileSink::RotateFileSink(std::string const & arguments, bool verbose)
        : _rotate(arguments), _file(), _verbose(verbose)
{
    if (!update()) {
        throw std::bad_exception();
    }
    if (!reopen()) {
        throw std::bad_exception();
    }
}

RotateFileSink::~RotateFileSink()
{
    close();
}

bool RotateFileSink::write(char const * message, int size)
{
    if (_rotate.next(message, size)) {
        if (!reopen()) {
            if (_verbose) {
                std::cerr << "RotateFileSink::write() Next open error!"
                          << std::endl;
            }
        }
    }

    if (!isOpen()) {
        update(); // Retry the open.
        reopen();

        if (!isOpen()) {
            if (_verbose) {
                std::cerr << "RotateFileSink::write() Retry open error!"
                          << std::endl;
            }
            return false;
        }
    }

    assert(isOpen());
    return _file.write(message, size) > 0;
}

void RotateFileSink::flush()
{
    // EMPTY.
}

void RotateFileSink::close()
{
    if (_file.isOpen()) {
        _file.close();
    }
}

bool RotateFileSink::reopen(std::string const & path)
{
    close();
    assert(!_file.isOpen());
    Path parent_dir = Path(path).getParent();
    if (!parent_dir.exists()) {
        parent_dir.createDir();
    }
    if (!parent_dir.isDirectory()) {
        if (_verbose) {
            std::cerr << "RotateFileSink::reopen() Parent is not a directory: "
                      << parent_dir.toString()
                      << std::endl;
        }
        return false;
    }
    if (!parent_dir.isWritable()) {
        if (_verbose) {
            std::cerr << "RotateFileSink::reopen() Parent is not a writable: "
                      << parent_dir.toString()
                      << std::endl;
        }
        return false;
    }
    if (!_file.open(path, File::Flags().clear().creat().rdwr().append())) {
        if (_verbose) {
            std::cerr << "RotateFileSink::reopen() File open error: "
                      << path
                      << std::endl;
        }
        return false;
    }
    assert(_file.isOpen());
    return true;
}

bool RotateFileSink::reopen()
{
    return reopen(_rotate.path.toString());
}

bool RotateFileSink::update()
{
    return _rotate.update();
}

bool RotateFileSink::isOpen() const
{
    return _file.isOpen();
}

} // namespace sink
} // namespace log

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

