/**
 * @file   Zip.hpp
 * @brief  Zip class prototype.
 * @author zer0
 * @date   2016-11-17
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <cstdint>
#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

/**
 * Zip class prototype.
 *
 * @author zer0
 * @date   2016-11-17
 */
class TBAG_API Zip : private Noncopyable
{
public:
    enum class ResultCode
    {
        UNKNOWN = 0,
        SUCCESS,
        FAILURE,

        INIT_ERROR,
        OPEN_ERROR,
        GO_TO_FIRST_FILE_ERROR,
        OPEN_CURRENT_FILE_ERROR,
    };

    static int const DEFAULT_ENCODE_LEVEL = 6;
    static int const MIN_ENCODE_LEVEL = 1;
    static int const MAX_ENCODE_LEVEL = 9;
    static int const DECODE_LEVEL = 0;

    using Buffer = std::vector<uint8_t>;

public:
    Zip();
    ~Zip();

private:
    static ResultCode coding(Buffer & output, uint8_t const * input, std::size_t size, int level = DECODE_LEVEL);

public:
    static ResultCode encode(Buffer & output, uint8_t const * input, std::size_t size, int level = DEFAULT_ENCODE_LEVEL);
    static ResultCode decode(Buffer & output, uint8_t const * input, std::size_t size);

public:
    static ResultCode   zip(std::string const & file, std::string const & dir);
    static ResultCode unzip(std::string const & file, std::string const & dir);
};

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ZIP_HPP__

