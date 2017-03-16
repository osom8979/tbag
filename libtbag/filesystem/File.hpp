/**
 * @file   File.hpp
 * @brief  File class prototype.
 * @author zer0
 * @date   2017-03-16
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FILE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FILE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>
#include <libtbag/filesystem/details/FsIo.hpp>

#include <string>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace filesystem {

/**
 * File class prototype.
 *
 * @author zer0
 * @date   2017-03-16
 */
class TBAG_API File : public Noncopyable
{
public:
    using ufile = details::ufile;
    using binf  = details::binf;

public:
    TBAG_CONSTEXPR static uint32_t const FILE_READ_ONLY    = _WIN_OR_POSIX(O_RDONLY); ///< open for reading only.
    TBAG_CONSTEXPR static uint32_t const FILE_WRITE_ONLY   = _WIN_OR_POSIX(O_WRONLY); ///< open for writing only.
    TBAG_CONSTEXPR static uint32_t const FILE_READ_WRITE   = _WIN_OR_POSIX(O_RDWR);   ///< open for reading and writing.
    TBAG_CONSTEXPR static uint32_t const FILE_APPEND       = _WIN_OR_POSIX(O_APPEND); ///< set append mode.
    TBAG_CONSTEXPR static uint32_t const FILE_CREATE       = _WIN_OR_POSIX(O_CREAT);  ///< create if nonexistant.
    TBAG_CONSTEXPR static uint32_t const FILE_TRUNCATE     = _WIN_OR_POSIX(O_TRUNC);  ///< truncate to zero length.
    TBAG_CONSTEXPR static uint32_t const FILE_EXISTS_ERROR = _WIN_OR_POSIX(O_EXCL);   ///< error if already exists.

private:
    ufile _file;

public:
    File();
    File(std::string const & path);
    ~File();

public:
    bool open(std::string const & path, int flags = FILE_CREATE | FILE_READ_WRITE, int mode = 0664);
    bool close();

public:
    int  read(binf const * infos, std::size_t infos_size, int64_t offset);
    int write(binf const * infos, std::size_t infos_size, int64_t offset);

public:
    int  read2(char const * buffer, std::size_t size, int64_t offset);
    int write2(char const * buffer, std::size_t size, int64_t offset);

public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

public:
    FileState getState() const;
};

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FILE_HPP__

