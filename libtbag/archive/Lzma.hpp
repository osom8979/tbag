/**
 * @file   Lzma.hpp
 * @brief  Lzma class prototype.
 * @author zer0
 * @date   2016-10-30
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_LZMA_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_LZMA_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <memory>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

/**
 * Lzma class prototype.
 *
 * @author zer0
 * @date   2016-10-30
 */
class TBAG_API Lzma : private Noncopyable
{
public:
    enum class ResultCode
    {
        UNKNOWN = 0,
        SUCCESS,
        FAILURE,
    };

    struct Param
    {
        uint8_t * input_data;
        uint8_t * output_data;

        std::size_t input_size;
        std::size_t output_size;
    };

public:
    struct LzmaPimpl;
    using UniqueLzmaPimpl = std::unique_ptr<LzmaPimpl>;

private:
    UniqueLzmaPimpl _lzma;

public:
    Lzma();
    ~Lzma();

public:
    bool write(std::string const & path);
    bool read(std::string const & path);

public:
    static ResultCode encode(Param const & param);
    static ResultCode decode(Param const & param);
};

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_LZMA_HPP__

