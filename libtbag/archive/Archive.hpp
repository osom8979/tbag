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
#include <libtbag/Err.hpp>
#include <libtbag/filesystem/details/FsTypes.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/util/BufferInfo.hpp>
#include <libtbag/Unit.hpp>

#include <cstdint>
#include <string>
#include <vector>
#include <memory>

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

TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_7ZIP = "7zip";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_PAX  = "pax";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_PAXR = "paxr";
TBAG_CONSTEXPR char const * const COMPRESS_FORMAT_ZIP  = "zip";

/**
 * ArchiveEntry class prototype.
 *
 * @author zer0
 * @date   2019-02-27
 */
struct ArchiveEntry
{
    using DirentType = libtbag::filesystem::details::DirentType;

    TBAG_CONSTEXPR static DirentType const TYPE_UNKNOWN = libtbag::filesystem::details::DIRENT_UNKNOWN;
    TBAG_CONSTEXPR static DirentType const TYPE_FILE    = libtbag::filesystem::details::DIRENT_FILE   ;
    TBAG_CONSTEXPR static DirentType const TYPE_DIR     = libtbag::filesystem::details::DIRENT_DIR    ;
    TBAG_CONSTEXPR static DirentType const TYPE_LINK    = libtbag::filesystem::details::DIRENT_LINK   ;
    TBAG_CONSTEXPR static DirentType const TYPE_FIFO    = libtbag::filesystem::details::DIRENT_FIFO   ;
    TBAG_CONSTEXPR static DirentType const TYPE_SOCKET  = libtbag::filesystem::details::DIRENT_SOCKET ;
    TBAG_CONSTEXPR static DirentType const TYPE_CHAR    = libtbag::filesystem::details::DIRENT_CHAR   ;
    TBAG_CONSTEXPR static DirentType const TYPE_BLOCK   = libtbag::filesystem::details::DIRENT_BLOCK  ;

    /** Entry file type. */
    DirentType type = TYPE_FILE;

    /** Original source file path. */
    std::string source_path;

    /** Path name. */
    std::string path_name;

    /** File size. */
    std::size_t size = 0;

    /** User ID of owner. */
    uint64_t uid = 0;

    /** Group ID of owner. */
    uint64_t gid = 0;

    /** File permission. */
    uint64_t permission = 0644;

    ArchiveEntry() { /* EMPTY. */ }
    virtual ~ArchiveEntry() { /* EMPTY. */ }
};

struct ArchiveMemoryEntry : public ArchiveEntry
{
    using Buffer = libtbag::util::Buffer;

    /** File content. */
    Buffer data;

    ArchiveMemoryEntry() { /* EMPTY. */ }
    virtual ~ArchiveMemoryEntry() { /* EMPTY. */ }
};

/**
 * BaseArchive class prototype.
 *
 * @author zer0
 * @date   2019-02-27
 */
class TBAG_API BaseArchive : private Noncopyable
{
public:
    using Entries = std::vector<ArchiveEntry>;
    using MemoryEntries = std::vector<ArchiveMemoryEntry>;
    using Buffer = libtbag::util::Buffer;

protected:
    void * _archive;
    bool _verbose;

public:
    BaseArchive(bool verbose = false);
    virtual ~BaseArchive();

public:
    inline bool exists() const TBAG_NOEXCEPT
    { return _archive != nullptr; }

public:
    static std::size_t getBlockSize() TBAG_NOEXCEPT;
    static Err getErrFromArchiveCode(int code) TBAG_NOEXCEPT;

public:
    char const * getErrorString() const TBAG_NOEXCEPT;

public:
    Err getErrAndPrintDefaultLog(char const * name, int code) const TBAG_NOEXCEPT;
    Err prefix(char const * name, int code) const TBAG_NOEXCEPT;
};

class TBAG_API ArchiveWriter : public BaseArchive
{
private:
    std::string _format;
    CompressType _compress;
    bool _open;

public:
    ArchiveWriter(std::string const & format = COMPRESS_FORMAT_PAXR,
                  CompressType compress = CompressType::CT_NONE);
    virtual ~ArchiveWriter();

public:
    Err openFile(std::string const & path);
    Err openMemory(char * buffer, std::size_t size, std::size_t * used);

public:
    Err close();

public:
    Err writeFromFile(std::string const & path);
    Err writeFromMemory(ArchiveEntry const & entry, char const * buffer, std::size_t buffer_size);
    Err writeFromMemory(ArchiveEntry const & entry, Buffer const & buffer);
    Err writeFromMemory(ArchiveMemoryEntry const & entry);
};

class TBAG_API ArchiveReader : public BaseArchive
{
private:
    bool _open;

public:
    ArchiveReader();
    virtual ~ArchiveReader();

public:
    Err openFile(std::string const & path);
    Err openFile(std::string const & path, std::size_t block_size);
    Err openMemory(char const * buffer, std::size_t size);

public:
    Err close();

public:
    std::size_t readToFile(std::string const & prefix);
    std::size_t readToMemory(MemoryEntries & entries);
    std::size_t readHeader(Entries & entries);
    std::size_t readName(std::vector<std::string> & names);
};

class TBAG_API FileArchiveWriter : public ArchiveWriter
{
public:
    FileArchiveWriter(std::string const & path,
                      std::string const & format = COMPRESS_FORMAT_PAXR,
                      CompressType compress = CompressType::CT_NONE);
    virtual ~FileArchiveWriter();
};

class TBAG_API MemoryArchiveWriter : public ArchiveWriter
{
public:
    TBAG_CONSTEXPR static std::size_t const DEFAULT_CAPACITY = 10 * libtbag::MEGA_BYTE_TO_BYTE;

private:
    Buffer _archive_memory;
    std::size_t _used;

public:
    MemoryArchiveWriter(std::size_t capacity,
                        std::string const & format = COMPRESS_FORMAT_PAXR,
                        CompressType compress = CompressType::CT_NONE);
    MemoryArchiveWriter(std::string const & format,
                        CompressType compress = CompressType::CT_NONE);
    MemoryArchiveWriter(CompressType compress);
    MemoryArchiveWriter();
    virtual ~MemoryArchiveWriter();

public:
    inline char * data() TBAG_NOEXCEPT_SP_OP(_archive_memory.data())
    {
        return _archive_memory.data();
    }

    inline char const * data() const TBAG_NOEXCEPT_SP_OP(_archive_memory.data())
    {
        return _archive_memory.data();
    }

    inline std::size_t capacity() const TBAG_NOEXCEPT_SP_OP(_archive_memory.size())
    {
        return _archive_memory.size();
    }

    inline std::size_t used() const TBAG_NOEXCEPT
    {
        return _used;
    }
};

class TBAG_API FileArchiveReader : public ArchiveReader
{
public:
    FileArchiveReader(std::string const & path);
    FileArchiveReader(std::string const & path, std::size_t block_size);
    virtual ~FileArchiveReader();
};

class TBAG_API MemoryArchiveReader : public ArchiveReader
{
private:
    Buffer _archive_memory;

public:
    MemoryArchiveReader(char const * buffer, std::size_t buffer_size);
    MemoryArchiveReader(Buffer const & buffer);
    virtual ~MemoryArchiveReader();
};

// -----------------------
// Miscellaneous utilities
// -----------------------

TBAG_API std::size_t compressArchive(std::string const & output_filename,
                                     std::vector<std::string> const & input_filenames,
                                     std::string const & format,
                                     CompressType compress = CompressType::CT_NONE);
TBAG_API std::size_t compressArchive(std::string const & output_filename,
                                     BaseArchive::MemoryEntries & entries,
                                     std::string const & format,
                                     CompressType compress = CompressType::CT_NONE);

TBAG_API std::string getCompressFormatFromOutputFileName(std::string const & output_filename);

TBAG_API std::size_t compressArchive(std::string const & output_filename,
                                     std::vector<std::string> const & input_filenames,
                                     CompressType compress = CompressType::CT_NONE);
TBAG_API std::size_t compressArchive(std::string const & output_filename,
                                     BaseArchive::MemoryEntries & entries,
                                     CompressType compress = CompressType::CT_NONE);

TBAG_API std::size_t decompressArchive(std::string const & filename,
                                       std::string const & output_prefix);
TBAG_API std::size_t decompressArchive(std::string const & filename,
                                       BaseArchive::MemoryEntries & entries);
TBAG_API BaseArchive::MemoryEntries decompressArchive(std::string const & filename);

TBAG_API std::size_t decompressMemoryArchive(char const * buffer, std::size_t size,
                                             std::string const & output_prefix);
TBAG_API std::size_t decompressMemoryArchive(char const * buffer, std::size_t size,
                                             BaseArchive::MemoryEntries & entries);
TBAG_API BaseArchive::MemoryEntries decompressMemoryArchive(char const * buffer, std::size_t size);

TBAG_API std::vector<std::string> getArchiveFileList(std::string const & filename);

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_ARCHIVE_HPP__

