/**
 * @file   File.cpp
 * @brief  File class implementation.
 * @author zer0
 * @date   2017-03-16
 */

#include <libtbag/filesystem/File.hpp>
#include <libtbag/filesystem/details/FsCommon.hpp>
#include <libtbag/log/Log.hpp>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

File::File() : _file(0)
{
    // EMPTY.
}

File::File(std::string const & path, Flags flags, int mode)
{
    open(path, flags, mode);
}

File::~File()
{
    close();
}

bool File::open(std::string const & path, Flags flags, int mode)
{
    _file = details::open(path, flags.flags, mode);
    if (_file > 0) {
        return true;
    }

    tDLogE("File::open({}) error.", path);
    return false;
}

bool File::close()
{
    bool result = false;
    if (_file != 0) {
        result = details::close(_file);
        _file = 0;
    }
    return result;
}

int File::read(binf const * infos, std::size_t infos_size, int64_t offset)
{
    return details::read(_file, infos, infos_size, offset);
}

int File::read(char const * buffer, std::size_t size, int64_t offset)
{
    return details::read2(_file, buffer, size, offset);
}

int File::write(binf const * infos, std::size_t infos_size, int64_t offset)
{
    return details::write(_file, infos, infos_size, offset);
}

int File::write(char const * buffer, std::size_t size, int64_t offset)
{
    return details::write2(_file, buffer, size, offset);
}

File::FileState File::getState() const
{
    FileState state = {0};
    if (details::getStateWithFile(_file, &state) == false) {
        tDLogE("File::getState() result error.");
    }
    return state;
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

