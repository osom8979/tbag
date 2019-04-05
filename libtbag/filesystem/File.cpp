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
    if (!File::open(path, flags, mode)) {
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
    return write(&info, 1U);
}

File::FileState File::getState() const
{
    FileState state = {0};
    if (!details::getStateWithFile(_file, &state)) {
        tDLogE("File::getState() result error.");
    }
    return state;
}

uint64_t File::getSize() const
{
    return getState().size;
}

bool File::isEof() const
{
    return _offset >= getSize();
}

// -------------
namespace impl {
// -------------

template <typename StlContainerType>
static Err readToBuffer(std::string const & path, StlContainerType & result, uint64_t limit_size)
{
    File f(path, File::Flags().clear().rdonly());
    if (f.isOpen() == false) {
        return E_ENOENT;
    }

    uint64_t const SIZE = f.getState().size;
    if (SIZE > limit_size) {
        tDLogE("readToBuffer() limit size error: file_size({}) vs limit_siz({})", SIZE, limit_size);
        return E_SMALLBUF;
    }

    if (result.size() < SIZE) {
        result.resize(SIZE);
    }

    int const READ_SIZE = f.read((char*)&result[0], SIZE, 0);
    if (SIZE != static_cast<uint64_t>(READ_SIZE)) {
        tDLogW("readToBuffer() Read size is not the same: {}/{}.", READ_SIZE, SIZE);
    }

    return E_SUCCESS;
}

static Err writeFromBuffer(std::string const & path, char const * buffer, std::size_t size)
{
    File f(path, File::Flags().clear().creat().wronly());
    if (f.isOpen() == false) {
        return E_ENOENT;
    }

    int const WRITE_SIZE = f.write(buffer, size, 0);
    if (size != static_cast<uint64_t>(WRITE_SIZE)) {
        tDLogW("writeFromBuffer() Read size is not the same: {}/{}.", WRITE_SIZE, size);
    }

    return E_SUCCESS;
}

template <typename StlContainerType>
static Err writeFromBuffer(std::string const & path, StlContainerType const & content)
{
    return writeFromBuffer(path, (char const *)&content[0], content.size());
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

Err readFile(std::string const & path, std::string & result)
{
    Path const READ_FILE_PATH(path);
    return readFile(path, result, READ_FILE_PATH.getState().size);
}

Err readFile(std::string const & path, util::Buffer & result)
{
    Path const READ_FILE_PATH(path);
    return readFile(path, result, READ_FILE_PATH.getState().size);
}

Err writeFile(std::string const & path, char const * buffer, std::size_t size)
{
    return impl::writeFromBuffer(path, buffer, size);
}

Err writeFile(std::string const & path, std::string const & content)
{
    return impl::writeFromBuffer(path, content);
}

Err writeFile(std::string const & path, util::Buffer const & content)
{
    return impl::writeFromBuffer(path, content);
}

Err copyFile(std::string const & source_path, std::string const & destination_path)
{
    libtbag::util::Buffer buffer;
    auto const CODE = readFile(source_path, buffer);
    if (isFailure(CODE)) {
        return CODE;
    }
    return writeFile(destination_path, buffer);
}

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

