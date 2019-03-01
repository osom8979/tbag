/**
 * @file   Archive.cpp
 * @brief  Archive class implementation.
 * @author zer0
 * @date   2019-02-25
 */

#include <libtbag/archive/Archive.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/debug/Assert.hpp>
#include <libtbag/system/SysInfo.hpp>

#include <archive.h>
#include <archive_entry.h>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_7ZIP          = COMPRESS_FORMAT_7ZIP;
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_AR            = "ar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ARBSD         = "arbsd";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ARGNU         = "argnu";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ARSVR4        = "arsvr4";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_BSDTAR        = "bsdtar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_CD9660        = "cd9660";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_CPIO          = "cpio";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_GNUTAR        = "gnutar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ISO           = "iso";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ISO9660       = "iso9660";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_MTREE         = "mtree";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_MTREE_CLASSIC = "mtree-classic";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_NEWC          = "newc";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ODC           = "odc";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_OLDTAR        = "oldtar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_PAX           = COMPRESS_FORMAT_PAX;
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_PAXR          = COMPRESS_FORMAT_PAXR;
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_POSIX         = "posix";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_RAW           = "raw";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_RPAX          = "rpax";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_SHAR          = "shar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_SHARDUMP      = "shardump";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_USTAR         = "ustar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_V7TAR         = "v7tar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_V7            = "v7";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_WARC          = "warc";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_XAR           = "xar";
TBAG_CONSTEXPR char const * const ARCHIVE_COMPRESS_FORMAT_ZIP           = COMPRESS_FORMAT_ZIP;

TBAG_CONSTEXPR static char const * const EMPTY_ERROR = "Empty error message";

template <typename T>
inline struct archive * cast_archive(T * v) TBAG_NOEXCEPT
{
    return static_cast<struct archive *>(v);
}

static char const * get_error_string(struct archive * a) TBAG_NOEXCEPT
{
    auto const * e = archive_error_string(a);
    return e ? e : EMPTY_ERROR;
}

static void entry_internal_to_external(struct archive_entry /*const*/ * in, ArchiveEntry * out) TBAG_NOEXCEPT
{
    // @formatter:off
    switch (archive_entry_filetype(in)) {
    case AE_IFREG:  out->type = ArchiveEntry::TYPE_FILE;    break;
    case AE_IFDIR:  out->type = ArchiveEntry::TYPE_DIR;     break;
    case AE_IFLNK:  out->type = ArchiveEntry::TYPE_LINK;    break;
    case AE_IFIFO:  out->type = ArchiveEntry::TYPE_FIFO;    break;
    case AE_IFSOCK: out->type = ArchiveEntry::TYPE_SOCKET;  break;
    case AE_IFCHR:  out->type = ArchiveEntry::TYPE_CHAR;    break;
    case AE_IFBLK:  out->type = ArchiveEntry::TYPE_BLOCK;   break;
    default:        out->type = ArchiveEntry::TYPE_UNKNOWN; break;
    }
    // @formatter:on

    char const * source = archive_entry_sourcepath(in);
    if (source != nullptr) {
        out->source_path = source;
    } else {
        out->source_path.clear();
    }

    char const * name = archive_entry_pathname(in);
    if (name != nullptr) {
        out->path_name = name;
    } else {
        out->path_name.clear();
    }

    out->size = static_cast<std::size_t>(archive_entry_size(in));
    out->uid = static_cast<uint64_t>(archive_entry_uid(in));
    out->gid = static_cast<uint64_t>(archive_entry_gid(in));
    //archive_entry_uname(in);
    //archive_entry_gname(in);
    out->permission = archive_entry_perm(in);
}

static void entry_external_to_internal(ArchiveEntry const * in, struct archive_entry * out) TBAG_NOEXCEPT
{
    unsigned int file_type;
    // @formatter:off
    switch (in->type) {
    case ArchiveEntry::TYPE_FILE:   file_type = AE_IFREG;  break;
    case ArchiveEntry::TYPE_DIR:    file_type = AE_IFDIR;  break;
    case ArchiveEntry::TYPE_LINK:   file_type = AE_IFLNK;  break;
    case ArchiveEntry::TYPE_FIFO:   file_type = AE_IFIFO;  break;
    case ArchiveEntry::TYPE_SOCKET: file_type = AE_IFSOCK; break;
    case ArchiveEntry::TYPE_CHAR:   file_type = AE_IFCHR;  break;
    case ArchiveEntry::TYPE_BLOCK:  file_type = AE_IFBLK;  break;
    case ArchiveEntry::TYPE_UNKNOWN:
    default: return;
    }
    // @formatter:on

    archive_entry_copy_sourcepath(out, in->source_path.c_str());
    archive_entry_copy_pathname(out, in->path_name.c_str());

    archive_entry_set_size(out, static_cast<la_int64_t>(in->size));
    archive_entry_set_filetype(out, file_type);

    archive_entry_set_uid(out, in->uid);
    archive_entry_set_gid(out, in->gid);

    //archive_entry_set_uname(out, uname);
    //archive_entry_set_gname(out, gname);

    archive_entry_set_perm(out, (__LA_MODE_T)in->permission);
}

template <typename ReaderCb>
static Err write_single(struct archive * a, ArchiveEntry const & info, std::string const & format, ReaderCb reader)
{
    auto const BLOCK_SIZE = BaseArchive::getBlockSize();
    libtbag::util::Buffer buffer(BLOCK_SIZE);

    struct archive_entry * entry = archive_entry_new();
    assert(entry != nullptr);

    entry_external_to_internal(&info, entry);

    archive_entry_acl_clear(entry);
    archive_entry_xattr_clear(entry);
    archive_entry_set_fflags(entry, 0, 0);

    if (format == COMPRESS_FORMAT_PAX || format == COMPRESS_FORMAT_PAXR) {
        // Sparse files are a GNU tar extension.
        // Do not use them in standard tar files.
        archive_entry_sparse_clear(entry);
    }

    Err last_err = Err::E_UNKNOWN;
    int code = archive_write_header(a, entry);
    if (code == ARCHIVE_OK) {
        auto left = info.size;

        while (left > 0) {
            auto next = (left > BLOCK_SIZE ? BLOCK_SIZE : left);
            auto read = reader(buffer.data(), next);
            if (read != next) {
                tDLogE("write() read size mismatch: {}/{}", read, next);
                last_err = Err::E_RDERR;
                break;
            }

            auto written = archive_write_data(a, buffer.data(), next);
            if (written != next) {
                tDLogE("write() written size mismatch: {}/{}", written, next);
                last_err = Err::E_WRERR;
                break;
            }
            left -= next;
        }

        if (left == 0) {
            code = archive_write_finish_entry(a);
            if (code != ARCHIVE_OK) {
                tDLogE("write() entry finish error: {}", get_error_string(a));
            }
            last_err = BaseArchive::getErrFromArchiveCode(code);
        }
    } else {
        tDLogE("write() header write error: {}", get_error_string(a));
        last_err = BaseArchive::getErrFromArchiveCode(code);
    }

    archive_entry_free(entry);
    return last_err;
}

template <typename Predicated>
static void read_for_each(struct archive * a, Predicated predicated)
{
    struct archive_entry * entry = nullptr;
    int code = 0;

    while (true) {
        code = archive_read_next_header(a, &entry);
        if (code == ARCHIVE_EOF) {
            //tDLogI("read_for_each() End Of File");
            break;
        }
        if (code < ARCHIVE_WARN) {
            tDLogI("read_for_each() next header error: {}", get_error_string(a));
            break;
        }
        assert(code >= ARCHIVE_WARN);
        if (code < ARCHIVE_OK) {
            tDLogW("read_for_each() next header warning: {}", get_error_string(a));
        }

        bool const CONSUMED_DATA = predicated(a, entry);
        if (!CONSUMED_DATA) {
            code = archive_read_data_skip(a);
            if (code != ARCHIVE_OK) {
                tDLogE("read_for_each() skip data error: {}", get_error_string(a));
            }
        }
    }
}

// --------------------------
// BaseArchive implementation
// --------------------------

BaseArchive::BaseArchive(bool verbose) : _archive(nullptr), _verbose(verbose)
{
    // EMPTY.
}

BaseArchive::~BaseArchive()
{
    // EMPTY.
}

std::size_t BaseArchive::getBlockSize() TBAG_NOEXCEPT
{
    return static_cast<std::size_t>(libtbag::system::getPageSize());
}

Err BaseArchive::getErrFromArchiveCode(int code) TBAG_NOEXCEPT
{
    if (code == ARCHIVE_OK) {
        return Err::E_SUCCESS;
    } else if (code == ARCHIVE_EOF) {
        return Err::E_EOF;
    } else if (code == ARCHIVE_RETRY) {
        return Err::E_RETRY;
    } else if (code >= ARCHIVE_WARN) {
        return Err::E_WARNING;
    }
    assert(code < ARCHIVE_WARN);
    return Err::E_UNKNOWN;
}

char const * BaseArchive::getErrorString() const TBAG_NOEXCEPT
{
    return get_error_string(cast_archive(_archive));
}

Err BaseArchive::getErrAndPrintDefaultLog(char const * name, int code) const TBAG_NOEXCEPT
{
    assert(name != nullptr);
    if (code == ARCHIVE_OK) {
        // SKIP.
    } else if (code == ARCHIVE_EOF) {
        tDLogIfN(_verbose, "{} EOF: {}", name, getErrorString());
    } else if (code == ARCHIVE_RETRY) {
        tDLogIfW(_verbose, "{} Retry: {}", name, getErrorString());
    } else if (code >= ARCHIVE_WARN) {
        tDLogIfW(_verbose, "{} Unknown warning: {}", name, getErrorString());
    } else {
        assert(code < ARCHIVE_WARN);
        tDLogE("{} Error: {}", name, getErrorString());
    }
    return getErrFromArchiveCode(code);
}

Err BaseArchive::prefix(char const * name, int code) const TBAG_NOEXCEPT
{
    return getErrAndPrintDefaultLog(name, code);
}

// -------------
// ArchiveWriter
// -------------

ArchiveWriter::ArchiveWriter(std::string const & format, CompressType compress)
        : BaseArchive(), _format(format), _compress(compress), _open(false)
{
    _archive = archive_write_new();
    assert(_archive != nullptr);

    int code = ARCHIVE_FATAL;
    switch (compress) {
    case CompressType::CT_NONE:
        code = archive_write_add_filter_none(cast_archive(_archive));
        break;
    case CompressType::CT_GZIP:
        code = archive_write_add_filter_gzip(cast_archive(_archive));
        break;
    case CompressType::CT_BZIP2:
        code = archive_write_add_filter_bzip2(cast_archive(_archive));
        break;
    case CompressType::CT_LZMA:
        code = archive_write_add_filter_lzma(cast_archive(_archive));
        break;
    case CompressType::CT_XZ:
        code = archive_write_add_filter_xz(cast_archive(_archive));
        break;
    default:
        tDLogE("ArchiveWriter::ArchiveWriter() Unknown compress type: {}", (int)compress);
        throw ErrException(Err::E_ILLARGS);
    }

    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveWriter::ArchiveWriter()", code));
    }

    if (compress == CompressType::CT_GZIP) {
        // We're not able to specify an arbitrary timestamp for gzip.
        // The next best thing is to omit the timestamp entirely.
        code = archive_write_set_filter_option(cast_archive(_archive), "gzip", "timestamp", nullptr);
        if (code != ARCHIVE_OK) {
            tDLogE("ArchiveWriter::ArchiveWriter() filter option(gzip/timestamp) error: {}", getErrorString());
            throw ErrException(getErrFromArchiveCode(code));
        }
    }

    code = archive_write_set_format_by_name(cast_archive(_archive), format.c_str());
    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveWriter::ArchiveWriter()", code));
    }

    // do not pad the last block!!
    code = archive_write_set_bytes_in_last_block(cast_archive(_archive), 1);
    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveWriter::ArchiveWriter()", code));
    }
}

ArchiveWriter::~ArchiveWriter()
{
    auto const CODE = archive_write_free(cast_archive(_archive));
    prefix("ArchiveWriter::~ArchiveWriter()", CODE);
}

Err ArchiveWriter::openFile(std::string const & path)
{
    if (_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_write_open_filename(cast_archive(_archive), path.c_str());
    _open = (CODE == ARCHIVE_OK);
    return prefix("ArchiveWriter::openFile()", CODE);
}

Err ArchiveWriter::openMemory(char * buffer, std::size_t size, std::size_t * used)
{
    if (_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_write_open_memory(cast_archive(_archive), buffer, size, used);
    _open = (CODE == ARCHIVE_OK);
    return prefix("ArchiveWriter::openFile()", CODE);
}

Err ArchiveWriter::close()
{
    if (!_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_write_close(cast_archive(_archive));
    _open = false;
    return prefix("ArchiveWriter::close()", CODE);
}

Err ArchiveWriter::writeFromFile(std::string const & path)
{
    libtbag::filesystem::Path const FILESYSTEM_PATH(path);
    libtbag::filesystem::File file(path);

    ArchiveEntry info;
    if (FILESYSTEM_PATH.isRegularFile()) {
        info.type = ArchiveEntry::TYPE_FILE;
    } else if (FILESYSTEM_PATH.isDirectory()) {
        info.type = ArchiveEntry::TYPE_DIR;
    } else {
        return Err::E_ILLARGS;
    }

    auto const STATE = file.getState();
    auto const PERMISSION = libtbag::filesystem::details::getPermission(path);
    auto const FIXED_PERMISSION = libtbag::filesystem::details::getFixedPermission(PERMISSION);

    info.source_path = path;
    info.path_name = FILESYSTEM_PATH.getName();
    info.size = STATE.size;
    info.uid = STATE.uid;
    info.gid = STATE.gid;
    info.permission = FIXED_PERMISSION;

    return write_single(cast_archive(_archive), info, _format, [&](char * data, std::size_t size) -> std::size_t {
        return static_cast<std::size_t>(file.read(data, size));
    });
}

Err ArchiveWriter::writeFromMemory(ArchiveEntry const & entry, char const * buffer, std::size_t buffer_size)
{
    std::size_t offset = 0;
    return write_single(cast_archive(_archive), entry, _format, [&](char * data, std::size_t size) -> std::size_t {
        if (offset >= buffer_size) {
            return 0;
        }

        std::size_t copy_size;
        if ((offset + size) <= buffer_size) {
            copy_size = size;
        } else {
            assert((offset + size) > buffer_size);
            copy_size = buffer_size - offset;
        }

        assert(copy_size <= size);
        memcpy(data, buffer + offset, copy_size);

        offset += copy_size;
        return copy_size;
    });
}

Err ArchiveWriter::writeFromMemory(ArchiveEntry const & entry, Buffer const & buffer)
{
    return writeFromMemory(entry, buffer.data(), buffer.size());
}

Err ArchiveWriter::writeFromMemory(ArchiveMemoryEntry const & entry)
{
    return writeFromMemory(entry, entry.data);
}

// -------------
// ArchiveReader
// -------------

ArchiveReader::ArchiveReader() : BaseArchive(), _open(false)
{
    _archive = archive_read_new();
    assert(_archive != nullptr);

    int code = archive_read_support_filter_all(cast_archive(_archive));
    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveReader::ArchiveReader()", code));
    }

    code = archive_read_support_format_all(cast_archive(_archive));
    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveReader::ArchiveReader()", code));
    }

    code = archive_read_support_compression_all(cast_archive(_archive));
    if (code != ARCHIVE_OK) {
        throw ErrException(prefix("ArchiveReader::ArchiveReader()", code));
    }
}

ArchiveReader::~ArchiveReader()
{
    auto const CODE = archive_read_free(cast_archive(_archive));
    prefix("ArchiveReader::~ArchiveReader()", CODE);
}

Err ArchiveReader::openFile(std::string const & path)
{
    return openFile(path, getBlockSize());
}

Err ArchiveReader::openFile(std::string const & path, std::size_t block_size)
{
    if (_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_read_open_filename(cast_archive(_archive), path.c_str(), block_size);
    _open = (CODE == ARCHIVE_OK);
    return prefix("ArchiveReader::openFile()", CODE);
}

Err ArchiveReader::openMemory(char const * buffer, std::size_t size)
{
    if (_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_read_open_memory(cast_archive(_archive), buffer, size);
    _open = (CODE == ARCHIVE_OK);
    return prefix("ArchiveReader::openFile()", CODE);
}

Err ArchiveReader::close()
{
    if (!_open) {
        return Err::E_ALREADY;
    }
    auto const CODE = archive_read_close(cast_archive(_archive));
    _open = false;
    return prefix("ArchiveReader::close()", CODE);
}

std::size_t ArchiveReader::readToFile(std::string const & prefix)
{
    std::size_t success_count = 0;
    read_for_each(cast_archive(_archive), [&](struct archive * a, struct archive_entry * entry) -> bool {
        char const * name = archive_entry_pathname(entry);
        if (name == nullptr) {
            tDLogW("ArchiveReader::readToFile() empty path name.");
            return false;
        }

        using namespace libtbag::filesystem;
        File file((Path(prefix) / name).toString(), File::Flags().clear().creat().wronly());

        void const * buffer;
        size_t size;
        la_int64_t offset;
        int code;

        while (true) {
            code = archive_read_data_block(a, &buffer, &size, &offset);
            if (code == ARCHIVE_EOF) {
                tDLogIfI(_verbose, "ArchiveReader::readToFile() current({}) data block EOF.", name);
                ++success_count;
                break;
            }
            if (code < ARCHIVE_WARN) {
                tDLogE("ArchiveReader::readToFile() current({}) data block error: {}",
                       name, getErrorString());
                break;
            }
            // assert(code >= ARCHIVE_WARN);
            if (code < ARCHIVE_OK) {
                tDLogW("ArchiveReader::readToFile() current({}) data block warning: {}",
                       name, getErrorString());
            }

            auto written = file.write((char const *)buffer, size, static_cast<std::int64_t>(offset));
            if (written != size) {
                tDLogE("ArchiveReader::readToFile() current({}) written size mismatch: {}/{}",
                       name, written, size);
                break;
            }
        }

        return true;
    });
    return success_count;
}

std::size_t ArchiveReader::readToMemory(MemoryEntries & entries)
{
    entries.clear();
    read_for_each(cast_archive(_archive), [&](struct archive * a, struct archive_entry * entry) -> bool {
        char const * name = archive_entry_pathname(entry);
        if (name == nullptr) {
            tDLogW("ArchiveReader::readToMemory() empty path name.");
            return false;
        }

        ArchiveMemoryEntry info;
        entry_internal_to_external(entry, &info);
        if (info.size >= 1) {
            info.data.resize(info.size);

            void const * buffer;
            size_t size;
            la_int64_t offset;
            int code;

            while (true) {
                code = archive_read_data_block(a, &buffer, &size, &offset);
                if (code == ARCHIVE_EOF) {
                    tDLogIfI(_verbose, "ArchiveReader::readToMemory() current({}) data block EOF.", name);
                    break;
                }
                if (code < ARCHIVE_WARN) {
                    tDLogE("ArchiveReader::readToMemory() current({}) data block error, {}",
                           name, getErrorString());
                    break;
                }
                // assert(code >= ARCHIVE_WARN);
                if (code < ARCHIVE_OK) {
                    tDLogW("ArchiveReader::readToMemory() current({}) data block warning: {}",
                           name, getErrorString());
                }
                memcpy(info.data.data() + offset, buffer, size);
            }
        }

        entries.push_back(info);
        return true;
    });
    return entries.size();
}

std::size_t ArchiveReader::readHeader(Entries & entries)
{
    entries.clear();
    read_for_each(cast_archive(_archive), [&](struct archive * a, struct archive_entry * entry) -> bool {
        ArchiveEntry info;
        entry_internal_to_external(entry, &info);
        entries.push_back(info);
        return false;
    });
    return entries.size();
}

std::size_t ArchiveReader::readName(std::vector<std::string> & names)
{
    names.clear();
    read_for_each(cast_archive(_archive), [&](struct archive * a, struct archive_entry * entry) -> bool {
        char const * name = archive_entry_pathname(entry);
        if (name != nullptr) {
            names.emplace_back(name);
        } else {
            tDLogW("ArchiveReader::readName() Empty path name found.");
        }
        return false;
    });
    return names.size();
}

// -----------------
// FileArchiveWriter
// -----------------

FileArchiveWriter::FileArchiveWriter(std::string const & path, std::string const & format, CompressType compress)
        : ArchiveWriter(format, compress)
{
    auto const CODE = openFile(path);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

FileArchiveWriter::~FileArchiveWriter()
{
    // EMPTY.
}

// -------------------
// MemoryArchiveWriter
// -------------------

MemoryArchiveWriter::MemoryArchiveWriter(std::size_t capacity, std::string const & format, CompressType compress)
        : ArchiveWriter(format, compress), _archive_memory(capacity)
{
    auto const CODE = openMemory(_archive_memory.data(), _archive_memory.size(), &_used);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

MemoryArchiveWriter::MemoryArchiveWriter(std::string const & format, CompressType compress)
        : MemoryArchiveWriter(DEFAULT_CAPACITY, format, compress)
{
    // EMPTY.
}

MemoryArchiveWriter::~MemoryArchiveWriter()
{
    // EMPTY.
}

// -----------------
// FileArchiveReader
// -----------------

FileArchiveReader::FileArchiveReader(std::string const & path, std::size_t block_size)
        : ArchiveReader()
{
    auto const CODE = openFile(path, block_size);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

FileArchiveReader::FileArchiveReader(std::string const & path)
        : FileArchiveReader(path, getBlockSize())
{
    // EMPTY.
}

FileArchiveReader::~FileArchiveReader()
{
    // EMPTY.
}

// -------------------
// MemoryArchiveReader
// -------------------

MemoryArchiveReader::MemoryArchiveReader(char const * buffer, std::size_t buffer_size)
        : ArchiveReader(), _archive_memory(buffer, buffer + buffer_size)
{
    auto const CODE = openMemory(buffer, buffer_size);
    if (isFailure(CODE)) {
        throw ErrException(CODE);
    }
}

MemoryArchiveReader::MemoryArchiveReader(Buffer const & buffer)
        : MemoryArchiveReader(buffer.data(), buffer.size())
{
    // EMPTY.
}

MemoryArchiveReader::~MemoryArchiveReader()
{
    // EMPTY.
}

// -----------------------
// Miscellaneous utilities
// -----------------------

std::size_t compressArchive(std::string const & output_filename,
                            std::vector<std::string> const & input_filenames,
                            std::string const & format,
                            CompressType compress)
{
    std::size_t success_count = 0;
    FileArchiveWriter writer(output_filename, format, compress);
    for (auto & file : input_filenames) {
        if (isSuccess(writer.writeFromFile(file))) {
            ++success_count;
        }
    }
    return success_count;
}

std::size_t compressArchive(std::string const & output_filename,
                            BaseArchive::MemoryEntries & entries,
                            std::string const & format,
                            CompressType compress)
{
    std::size_t success_count = 0;
    FileArchiveWriter writer(output_filename, format, compress);
    for (auto & entry : entries) {
        if (isSuccess(writer.writeFromMemory(entry))) {
            ++success_count;
        }
    }
    return success_count;
}

std::size_t decompressArchive(std::string const & filename, std::string const & output_prefix)
{
    return FileArchiveReader(filename).readToFile(output_prefix);
}

std::size_t decompressArchive(std::string const & filename, BaseArchive::MemoryEntries & entries)
{
    return FileArchiveReader(filename).readToMemory(entries);
}

std::size_t decompressMemoryArchive(char const * buffer, std::size_t size, std::string const & output_prefix)
{
    return MemoryArchiveReader(buffer, size).readToFile(output_prefix);
}

std::size_t decompressMemoryArchive(char const * buffer, std::size_t size, BaseArchive::MemoryEntries & entries)
{
    return MemoryArchiveReader(buffer, size).readToMemory(entries);
}

std::vector<std::string> getArchiveFileList(std::string const & filename)
{
    std::vector<std::string> result;
    FileArchiveReader(filename).readName(result);
    return result;
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

