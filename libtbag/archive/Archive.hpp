/**
 * @file   Archive.hpp
 * @brief  Archive class prototype.
 * @author zer0
 * @date   2019-02-25
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <libtbag/config.h>
#include <libtbag/predef.hpp>
#include <libtbag/Noncopyable.hpp>

#include <string>
#include <vector>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

/** Compression type.  */
enum class CompressType
{
    CT_NONE,
    CT_GZIP,
    CT_BZIP2,
    CT_LZMA,
    CT_XZ
};

TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_7ZIP          = "7zip";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_AR            = "ar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ARBSD         = "arbsd";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ARGNU         = "argnu";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ARSVR4        = "arsvr4";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_BSDTAR        = "bsdtar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_CD9660        = "cd9660";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_CPIO          = "cpio";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_GNUTAR        = "gnutar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ISO           = "iso";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ISO9660       = "iso9660";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_MTREE         = "mtree";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_MTREE_CLASSIC = "mtree-classic";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_NEWC          = "newc";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ODC           = "odc";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_OLDTAR        = "oldtar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_PAX           = "pax";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_PAXR          = "paxr";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_POSIX         = "posix";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_RAW           = "raw";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_RPAX          = "rpax";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_SHAR          = "shar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_SHARDUMP      = "shardump";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_USTAR         = "ustar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_V7TAR         = "v7tar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_V7            = "v7";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_WARC          = "warc";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_XAR           = "xar";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ZIP           = "zip";

TBAG_API std::size_t compressArchive(std::string const & output_filename,
                                     std::vector<std::string> const & input_filenames,
                                     std::string const & format = COMPRESS_FORMAT_PAXR,
                                     CompressType compress = CompressType::CT_NONE);
TBAG_API std::vector<std::string> decompressArchive(std::string const & input_filename);

/**
 * Archive class prototype.
 *
 * @author zer0
 * @date   2019-02-25
 */
class TBAG_API Archive : private Noncopyable
{
public:
    Archive();
    virtual ~Archive();
};

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__

