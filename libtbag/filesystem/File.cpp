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

File::File() : _file(0), _offset(0)
{
    // EMPTY.
}

File::File(std::string const & path, Flags flags, int mode) : File()
{
    if (open(path, flags, mode) == false) {
        throw std::bad_alloc();
    }
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

int File::read(char * buffer, std::size_t size, int64_t offset)
{
    return details::read(_file, buffer, size, offset);
}

int File::read(binf const * infos, std::size_t infos_size)
{
    int const READ_SIZE = read(infos, infos_size, _offset);
    if (READ_SIZE > 0) {
        _offset += READ_SIZE;
    }
    return READ_SIZE;
}

int File::read(char * buffer, std::size_t size)
{
    binf const info(buffer, size);
    return read(&info, 1U);
}

int File::write(binf const * infos, std::size_t infos_size, int64_t offset)
{
    return details::write(_file, infos, infos_size, offset);
}

int File::write(char const * buffer, std::size_t size, int64_t offset)
{
    return details::write(_file, buffer, size, offset);
}

int File::write(binf const * infos, std::size_t infos_size)
{
    int const WRITE_SIZE = write(infos, infos_size, _offset);
    if (WRITE_SIZE > 0) {
        _offset += WRITE_SIZE;
    }
    return WRITE_SIZE;
}

int File::write(char const * buffer, std::size_t size)
{
    binf const info(const_cast<char*>(buffer), size);
    return read(&info, 1U);
}

File::FileState File::getState() const
{
    FileState state = {0};
    if (details::getStateWithFile(_file, &state) == false) {
        tDLogE("File::getState() result error.");
    }
    return state;
}

bool File::isEof() const
{
    return _offset >= getState().size;
}

// -------------
namespace impl {
// -------------

template <typename StlContainerType>
static Err readToBuffer(std::string const & path, StlContainerType & result, uint64_t limit_size)
{
    File f(path, File::Flags().clear().rdonly());
    if (f.isOpen() == false) {
        return Err::E_ENOENT;
    }

    uint64_t const SIZE = f.getState().size;
    if (SIZE >= limit_size) {
        tDLogE("readToBuffer() result error.");
        return Err::E_SMALLBUF;
    }

    if (result.size() < SIZE) {
        result.resize(SIZE);
    }

    int READ_SIZE = f.read((char*)&result[0], SIZE, 0);
    if (SIZE != static_cast<uint64_t>(READ_SIZE)) {
        tDLogW("readToBuffer() Read size is not the same: {}/{}.", READ_SIZE, SIZE);
    }

    return Err::E_SUCCESS;
}

template <typename StlContainerType>
static Err writeFromBuffer(std::string const & path, StlContainerType const & content)
{
    File f(path, File::Flags().clear().creat().wronly());
    if (f.isOpen() == false) {
        return Err::E_ENOENT;
    }

    int WRITE_SIZE = f.write((char*)&content[0], content.size(), 0);
    if (content.size() != static_cast<uint64_t>(WRITE_SIZE)) {
        tDLogW("writeFromBuffer() Read size is not the same: {}/{}.", WRITE_SIZE, content.size());
    }

    return Err::E_SUCCESS;
}

// ----------------
} // namespace impl
// ----------------

Err readFile(std::string const & path, std::string & result, uint64_t limit_size)
{
    return impl::readToBuffer(path, result, limit_size);
}

Err readFile(std::string const & path, util::Buffer & result, uint64_t limit_size)
{
    return impl::readToBuffer(path, result, limit_size);
}

Err writeFile(std::string const & path, std::string const & content)
{
    return impl::writeFromBuffer(path, content);
}

Err writeFile(std::string const & path, util::Buffer const & content)
{
    return impl::writeFromBuffer(path, content);
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

