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
#include <libtbag/Err.hpp>
#include <libtbag/Unit.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>

#include <cstdint>
#include <vector>
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
class TBAG_API File : private Noncopyable
{
public:
    using ufile = details::ufile;
    using binf  = details::binf;
    using Flags = details::FileOpenFlags;

private:
    ufile _file;

public:
    File();
    File(std::string const & path, Flags flags = Flags(), int mode = 0664);
    ~File();

public:
    inline bool isOpen() const TBAG_NOEXCEPT
    { return _file != 0; }

    inline ufile getFd() const TBAG_NOEXCEPT
    { return _file; }

public:
    bool open(std::string const & path, Flags flags = Flags(), int mode = 0664);
    bool close();

public:
    int read(binf const * infos, std::size_t infos_size, int64_t offset);
    int read(char * buffer, std::size_t size, int64_t offset);

public:
    int write(binf const * infos, std::size_t infos_size, int64_t offset);
    int write(char const * buffer, std::size_t size, int64_t offset);

public:
    using TimeSpec  = details::TimeSpec;
    using FileState = details::FileState;

public:
    FileState getState() const;
};

TBAG_API Err readFile(std::string const & path, std::string & result, uint64_t limit_size = 2 * MEGA_BYTE_TO_BYTE);
TBAG_API Err readFile(std::string const & path, std::vector<char> & result, uint64_t limit_size = 2 * MEGA_BYTE_TO_BYTE);

TBAG_API Err writeFile(std::string const & path, std::string const & content);
TBAG_API Err writeFile(std::string const & path, std::vector<char> const & content);

} // namespace filesystem

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_FILESYSTEM_FILE_HPP__

