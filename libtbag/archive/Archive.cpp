/**
 * @file   Archive.cpp
 * @brief  Archive class implementation.
 * @author zer0
 * @date   2019-02-25
 */

#include <libtbag/archive/Archive.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/system/SysInfo.hpp>

#include <archive.h>
#include <archive_entry.h>

#include <cassert>
#include <cstring>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

static char const * __get_archive_error_string(struct archive * a) TBAG_NOEXCEPT
{
    auto const * e = archive_error_string(a);
    return e ? e : "Unknown error";
}

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

std::size_t compressArchive(std::string const & output_filename,
                            std::vector<std::string> const & input_filenames,
                            std::string const & format,
                            CompressType compress)
{
    auto * a = archive_write_new();
    assert(a != nullptr);

    int code = ARCHIVE_FATAL;
    switch (compress) {
    case CompressType::CT_NONE:
        code = archive_write_add_filter_none(a);
        break;
    case CompressType::CT_GZIP:
        code = archive_write_add_filter_gzip(a);
        break;
    case CompressType::CT_BZIP2:
        code = archive_write_add_filter_bzip2(a);
        break;
    case CompressType::CT_LZMA:
        code = archive_write_add_filter_lzma(a);
        break;
    case CompressType::CT_XZ:
        code = archive_write_add_filter_xz(a);
        break;
    default:
        tDLogE("compressArchive() Unknown compress type: {}", (int)compress);
        return 0;
    }

    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() filter error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    if (compress == CompressType::CT_GZIP) {
        // We're not able to specify an arbitrary timestamp for gzip.
        // The next best thing is to omit the timestamp entirely.
        code = archive_write_set_filter_option(a, "gzip", "timestamp", nullptr);
        if (code != ARCHIVE_OK) {
            tDLogE("compressArchive() filter option(gzip/timestamp) error({}): {}", code, __get_archive_error_string(a));
            return 0;
        }
    }

    code = archive_write_set_format_by_name(a, format.c_str());
    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() format({}) error({}): {}", format, code, __get_archive_error_string(a));
        return 0;
    }

    // do not pad the last block!!
    code = archive_write_set_bytes_in_last_block(a, 1);
    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() bytes in last block error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    code = archive_write_open_filename(a, output_filename.c_str());
    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() file open error({}), {}", code, __get_archive_error_string(a));
        return 0;
    }

    auto const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
    libtbag::util::Buffer buffer(PAGE_SIZE);

    std::size_t success_count = 0;
    for (auto & item : input_filenames) {
        using namespace libtbag::filesystem;
        Path path(item);
        File file(item);

        auto const PERMISSION = details::getPermission(item);
        auto const FIXED_PERMISSION = details::getFixedPermission(PERMISSION);

        auto const FILE_STATE = file.getState();
        auto const FILE_SIZE = FILE_STATE.size;

        archive_entry * entry = archive_entry_new();
        assert(entry != nullptr);

        archive_entry_copy_sourcepath(entry, item.c_str());
        archive_entry_copy_pathname(entry, path.getName().c_str());

        archive_entry_set_size(entry, FILE_SIZE);
        archive_entry_set_filetype(entry, AE_IFREG);

        archive_entry_set_uid(entry, FILE_STATE.uid);
        archive_entry_set_gid(entry, FILE_STATE.gid);
        //archive_entry_set_uname(entry, uname);
        //archive_entry_set_gname(entry, gname);
        archive_entry_set_perm(entry, FIXED_PERMISSION);

        archive_entry_acl_clear(entry);
        archive_entry_xattr_clear(entry);
        archive_entry_set_fflags(entry, 0, 0);

        if (format == COMPRESS_FORMAT_PAX || format == COMPRESS_FORMAT_PAXR) {
            // Sparse files are a GNU tar extension.
            // Do not use them in standard tar files.
            archive_entry_sparse_clear(entry);
        }

        code = archive_write_header(a, entry);
        if (code == ARCHIVE_OK) {
            auto left = FILE_SIZE;

            while (left > 0) {
                auto next = (left > PAGE_SIZE ? PAGE_SIZE : left);
                auto read = static_cast<std::size_t>(file.read(buffer.data(), next));
                if (read != next) {
                    tDLogE("compressArchive() read size mismatch: {}/{}", read, next);
                    break;
                }

                auto written = archive_write_data(a, buffer.data(), next);
                if (written != next) {
                    tDLogE("compressArchive() written size mismatch: {}/{}", written, next);
                    break;
                }
                left -= next;
            }

            if (left == 0) {
                ++success_count;
            }
        } else {
            tDLogE("compressArchive() header write error({}), {}", code, __get_archive_error_string(a));
        }
        archive_entry_free(entry);
    }

    code = archive_write_close(a);
    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() archive close error({}): {}", code, __get_archive_error_string(a));
    }
    code = archive_write_free(a);
    if (code != ARCHIVE_OK) {
        tDLogE("compressArchive() archive free error({}): {}", code, __get_archive_error_string(a));
    }

    return success_count;
}

std::size_t decompressArchive(std::string const & filename, std::string const & output_prefix)
{
    auto * a = archive_read_new();
    assert(a != nullptr);

    int code = archive_read_support_format_all(a);
    if (code != ARCHIVE_OK) {
        tDLogE("decompressArchive() format error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    code = archive_read_support_compression_all(a);
    if (code != ARCHIVE_OK) {
        tDLogE("decompressArchive() compression error({}): {}", code, __get_archive_error_string(a));
        return 0;
    }

    auto const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
    code = archive_read_open_filename(a, filename.c_str(), PAGE_SIZE);
    if (code != ARCHIVE_OK) {
        tDLogE("decompressArchive() file({}) open error({}): {}",
               filename, code, __get_archive_error_string(a));
        return 0;
    }

    std::size_t success_count = 0;
    struct archive_entry * entry;
    while (true) {
        code = archive_read_next_header(a, &entry);
        if (code == ARCHIVE_EOF) {
            tDLogI("decompressArchive() next header EOF.");
            break;
        }
        if (code < ARCHIVE_WARN) {
            tDLogE("decompressArchive() next header error({}), {}", code, __get_archive_error_string(a));
            break;
        }
        assert(code >= ARCHIVE_WARN);
        if (code < ARCHIVE_OK) {
            tDLogW("decompressArchive() next header warning({}): {}", code, __get_archive_error_string(a));
        }

        assert(entry != nullptr);
        char const * name = archive_entry_pathname(entry);
        if (name == nullptr) {
            tDLogW("decompressArchive() empty path name.");
            continue;
        }

        using namespace libtbag::filesystem;
        File file((Path(output_prefix) / name).toString(), File::Flags().clear().creat().wronly());

        void const * buffer;
        size_t size;
        la_int64_t offset;

        while (true) {
            code = archive_read_data_block(a, &buffer, &size, &offset);
            if (code == ARCHIVE_EOF) {
                tDLogI("decompressArchive() current({}) data block EOF.", name);
                ++success_count;
                break;
            }
            if (code < ARCHIVE_WARN) {
                tDLogE("decompressArchive() current({}) data block error({}), {}",
                       name, code, __get_archive_error_string(a));
                break;
            }
            assert(code >= ARCHIVE_WARN);
            if (code < ARCHIVE_OK) {
                tDLogW("decompressArchive() current({}) data block warning({}): {}",
                       name, code, __get_archive_error_string(a));
            }

            auto written = file.write((char const *)buffer, size, static_cast<std::int64_t>(offset));
            if (written != size) {
                tDLogE("decompressArchive() written size mismatch: {}/{}", written, size);
                break;
            }
        }
    }

    code = archive_read_close(a);
    if (code != ARCHIVE_OK) {
        tDLogE("decompressArchive() archive close error({}): {}", code, __get_archive_error_string(a));
    }
    code = archive_read_free(a);
    if (code != ARCHIVE_OK) {
        tDLogE("decompressArchive() archive free error({}): {}", code, __get_archive_error_string(a));
    }

    return success_count;
}

std::vector<std::string> getArchiveFileList(std::string const & filename)
{
    using result_t = std::vector<std::string>;
    auto * a = archive_read_new();
    assert(a != nullptr);

    int code = archive_read_support_filter_all(a);
    if (code != ARCHIVE_OK) {
        tDLogE("getArchiveFileList() filter error({}): {}", code, __get_archive_error_string(a));
        return result_t();
    }

    code = archive_read_support_format_all(a);
    if (code != ARCHIVE_OK) {
        tDLogE("getArchiveFileList() format error({}): {}", code, __get_archive_error_string(a));
        return result_t();
    }

    auto const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
    code = archive_read_open_filename(a, filename.c_str(), PAGE_SIZE);
    if (code != ARCHIVE_OK) {
        tDLogE("getArchiveFileList() file({}) open error({}): {}",
               filename, code, __get_archive_error_string(a));
        return result_t();
    }

    result_t result;
    struct archive_entry * entry;
    while (true) {
        code = archive_read_next_header(a, &entry);
        if (code == ARCHIVE_EOF) {
            tDLogI("getArchiveFileList() next header EOF.");
            break;
        }
        if (code < ARCHIVE_WARN) {
            tDLogE("getArchiveFileList() next header error({}), {}", code, __get_archive_error_string(a));
            break;
        }
        assert(code >= ARCHIVE_WARN);
        if (code < ARCHIVE_OK) {
            tDLogW("getArchiveFileList() next header warning({}): {}", code, __get_archive_error_string(a));
        }

        assert(entry != nullptr);
        char const * name = archive_entry_pathname(entry);
        if (name != nullptr) {
            result.push_back(name);
        } else {
            tDLogW("getArchiveFileList() empty path name.");
        }

        code = archive_read_data_skip(a);
        if (code != ARCHIVE_OK) {
            tDLogE("getArchiveFileList() skip data error({}): {}", code, __get_archive_error_string(a));
            break;
        }
    }

    code = archive_read_close(a);
    if (code != ARCHIVE_OK) {
        tDLogE("getArchiveFileList() archive close error({}): {}", code, __get_archive_error_string(a));
    }
    code = archive_read_free(a);
    if (code != ARCHIVE_OK) {
        tDLogE("getArchiveFileList() archive free error({}): {}", code, __get_archive_error_string(a));
    }

    return result;
}

/**
 * archive implementation.
 *
 * @author zer0
 * @date   2019-02-25
 */
struct BaseArchive::Impl : private Noncopyable
{
public:
    using Buffer = libtbag::util::Buffer;

public:
    TBAG_CONSTEXPR static char const * const EMPTY_ERROR = "Empty error message";

private:
    struct archive * _archive;
    Buffer _buffer;
    bool _verbose;

public:
    Impl() : _archive(nullptr), _buffer(getBlockSize()), _verbose(false)
    {
        // EMPTY.
    }

    ~Impl()
    {
        // EMPTY.
    }

public:
    static Err getErrFromArchiveCode(int code) TBAG_NOEXCEPT
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

    static std::size_t getBlockSize() TBAG_NOEXCEPT
    {
        static std::size_t const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
        return PAGE_SIZE;
    }

    char const * getErrorString() const TBAG_NOEXCEPT
    {
        auto const * e = archive_error_string(_archive);
        return e ? e : EMPTY_ERROR;
    }

    inline struct archive       * getArchive()       TBAG_NOEXCEPT { return _archive; }
    inline struct archive const * getArchive() const TBAG_NOEXCEPT { return _archive; }

    Err getErrAndPrintDefaultLog(char const * prefix, int code) const TBAG_NOEXCEPT
    {
        assert(prefix != nullptr);
        if (code == ARCHIVE_OK) {
            // SKIP.
        } else if (code == ARCHIVE_EOF) {
            tDLogIfN(_verbose, "BaseArchive::Impl::{}() EOF: {}", prefix, getErrorString());
        } else if (code == ARCHIVE_RETRY) {
            tDLogIfW(_verbose, "BaseArchive::Impl::{}() Retry: {}", prefix, getErrorString());
        } else if (code >= ARCHIVE_WARN) {
            tDLogIfW(_verbose, "BaseArchive::Impl::{}() Unknown warning: {}", prefix, getErrorString());
        } else {
            assert(code < ARCHIVE_WARN);
            tDLogE("BaseArchive::Impl::{}() error: {}", prefix, getErrorString());
        }
        return getErrFromArchiveCode(code);
    }

    inline bool exists() const TBAG_NOEXCEPT
    {
        return _archive != nullptr;
    }

    Err __prefix(char const * prefix, int code) const TBAG_NOEXCEPT
    {
        return getErrAndPrintDefaultLog(prefix, code);
    }

    void write_new()
    {
        _archive = archive_write_new();
        assert(_archive != nullptr);
    }

    void read_new()
    {
        _archive = archive_read_new();
        assert(_archive != nullptr);
    }

    Err write_free()
    {
        auto const CODE = __prefix("write_free", archive_write_free(_archive));
        _archive = nullptr;
        return CODE;
    }

    Err read_free()
    {
        auto const CODE = __prefix("read_free", archive_read_free(_archive));
        _archive = nullptr;
        return CODE;
    }

    Err write_close()
    {
        return __prefix("write_close", archive_write_close(_archive));
    }

    Err read_close()
    {
        return __prefix("read_close", archive_read_close(_archive));
    }

    Err write_open_filename(char const * filename)
    {
        return __prefix("write_open_filename", archive_write_open_filename(_archive, filename));
    }

    Err read_open_filename(char const * filename)
    {
        return read_open_filename(filename, getBlockSize());
    }

    Err read_open_filename(char const * filename, std::size_t block_size)
    {
        return __prefix("read_open_filename", archive_read_open_filename(_archive, filename, block_size));
    }

    /**
     * @param[in] buffer
     *      Memory buffer pointer.
     * @param[in] buff_size
     *      The size of the buffer.
     * @param[out] used
     *      Refers to a variable that will be updated after each write into the buffer.
     */
    Err write_open_memory(char * buffer, std::size_t buffer_size, std::size_t * used)
    {
        return __prefix("write_open_memory", archive_write_open_memory(_archive, buffer, buffer_size, used));
    }

    Err read_open_memory(char * buffer, std::size_t buffer_size)
    {
        return __prefix("read_open_memory", archive_read_open_memory(_archive, buffer, buffer_size));
    }

    Err write_init(std::string const & format = COMPRESS_FORMAT_PAXR,
                   CompressType compress = CompressType::CT_NONE)
    {
        int code = ARCHIVE_FATAL;
        switch (compress) {
        case CompressType::CT_NONE:
            code = archive_write_add_filter_none(_archive);
            break;
        case CompressType::CT_GZIP:
            code = archive_write_add_filter_gzip(_archive);
            break;
        case CompressType::CT_BZIP2:
            code = archive_write_add_filter_bzip2(_archive);
            break;
        case CompressType::CT_LZMA:
            code = archive_write_add_filter_lzma(_archive);
            break;
        case CompressType::CT_XZ:
            code = archive_write_add_filter_xz(_archive);
            break;
        default:
            tDLogE("BaseArchive::Impl::write_init() Unknown compress type: {}", (int)compress);
            return Err::E_ILLARGS;
        }

        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::write_init() filter error: {}", getErrorString());
            return getErrFromArchiveCode(code);
        }

        if (compress == CompressType::CT_GZIP) {
            // We're not able to specify an arbitrary timestamp for gzip.
            // The next best thing is to omit the timestamp entirely.
            code = archive_write_set_filter_option(_archive, "gzip", "timestamp", nullptr);
            if (code != ARCHIVE_OK) {
                tDLogE("BaseArchive::Impl::write_init() filter option(gzip/timestamp) error: {}", getErrorString());
                return getErrFromArchiveCode(code);
            }
        }

        code = archive_write_set_format_by_name(_archive, format.c_str());
        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::write_init() {} format error: {}", format, getErrorString());
            return getErrFromArchiveCode(code);
        }

        // do not pad the last block!!
        code = archive_write_set_bytes_in_last_block(_archive, 1);
        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::write_init() bytes in last block error: {}", getErrorString());
            return getErrFromArchiveCode(code);
        }

        return Err::E_SUCCESS;
    }

    Err read_init()
    {
        int code = archive_read_support_filter_all(_archive);
        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::read_init() filter error: {}", getErrorString());
            return getErrFromArchiveCode(code);
        }

        code = archive_read_support_format_all(_archive);
        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::read_init() format error: {}", getErrorString());
            return getErrFromArchiveCode(code);
        }

        code = archive_read_support_compression_all(_archive);
        if (code != ARCHIVE_OK) {
            tDLogE("BaseArchive::Impl::read_init() compression error: {}", getErrorString());
            return getErrFromArchiveCode(code);
        }

        return Err::E_SUCCESS;
    }

    static void entryInternal2external(struct archive_entry /*const*/ * in, ArchiveEntry * out) TBAG_NOEXCEPT
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
        out->uid = archive_entry_uid(in);
        out->gid = archive_entry_gid(in);
        //archive_entry_uname(in);
        //archive_entry_gname(in);
        out->permission = archive_entry_perm(in);
    }

    static void entryExternal2internal(ArchiveEntry const * in, struct archive_entry * out) TBAG_NOEXCEPT
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
    Err write(ArchiveEntry const & info, std::string const & format, ReaderCb reader)
    {
        auto const PAGE_SIZE = static_cast<std::size_t>(libtbag::system::getPageSize());
        libtbag::util::Buffer buffer(PAGE_SIZE);

        struct archive_entry * entry = archive_entry_new();
        assert(entry != nullptr);

        entryExternal2internal(&info, entry);

        archive_entry_acl_clear(entry);
        archive_entry_xattr_clear(entry);
        archive_entry_set_fflags(entry, 0, 0);

        if (format == COMPRESS_FORMAT_PAX || format == COMPRESS_FORMAT_PAXR) {
            // Sparse files are a GNU tar extension.
            // Do not use them in standard tar files.
            archive_entry_sparse_clear(entry);
        }

        Err last_err;
        int code = archive_write_header(_archive, entry);
        if (code == ARCHIVE_OK) {
            auto left = info.size;

            while (left > 0) {
                auto next = (left > PAGE_SIZE ? PAGE_SIZE : left);
                auto read = reader(buffer.data(), next);
                if (read != next) {
                    tDLogE("BaseArchive::Impl::write() read size mismatch: {}/{}", read, next);
                    last_err = Err::E_RDERR;
                    break;
                }

                auto written = archive_write_data(_archive, buffer.data(), next);
                if (written != next) {
                    tDLogE("BaseArchive::Impl::write() written size mismatch: {}/{}", written, next);
                    last_err = Err::E_WRERR;
                    break;
                }
                left -= next;
            }

            if (left == 0) {
                last_err = Err::E_SUCCESS;
            }
        } else {
            tDLogE("BaseArchive::Impl::write() header write error: {}", getErrorString());
            last_err = getErrFromArchiveCode(code);
        }

        archive_entry_free(entry);
        return last_err;
    }

    template <typename Predicated>
    void read_for_each(Predicated predicated)
    {
        struct archive_entry * entry = nullptr;
        int code = 0;

        while (true) {
            code = archive_read_next_header(_archive, &entry);
            if (code == ARCHIVE_EOF) {
                tDLogI("BaseArchive::Impl::read_for_each() End Of File");
                break;
            }
            if (code < ARCHIVE_WARN) {
                tDLogI("BaseArchive::Impl::read_for_each() next header error: {}", getErrorString());
                break;
            }
            assert(code >= ARCHIVE_WARN);
            if (code < ARCHIVE_OK) {
                tDLogW("BaseArchive::Impl::read_for_each() next header warning: {}", getErrorString());
            }

            bool const CONSUMED_DATA = predicated(_archive, entry);
            if (!CONSUMED_DATA) {
                code = archive_read_data_skip(_archive);
                if (code != ARCHIVE_OK) {
                    tDLogE("BaseArchive::Impl::read_for_each() skip data error: {}", getErrorString());
                }
            }
        }
    }
};

// -----------
// BaseArchive
// -----------

BaseArchive::BaseArchive() : _impl(std::make_unique<Impl>())
{
    assert(static_cast<bool>(_impl));
}

BaseArchive::~BaseArchive()
{
    // EMPTY.
}

bool BaseArchive::exists() const
{
    assert(static_cast<bool>(_impl));
    return _impl->exists();
}

// -------------
// ArchiveWriter
// -------------

ArchiveWriter::ArchiveWriter(std::string const & format, CompressType compress)
        : BaseArchive(), _format(format), _compress(compress)
{
    _impl->write_new();
    assert(exists());
}

ArchiveWriter::~ArchiveWriter()
{
    _impl->write_free();
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

    return _impl->write(info, _format, [&](char * data, std::size_t size) -> std::size_t {
        return file.read(data, size);
    });
}

Err ArchiveWriter::writeFromMemory(ArchiveEntry const & enrty, char const * buffer, std::size_t buffer_size)
{
    std::size_t offset = 0;
    return _impl->write(enrty, _format, [&](char * data, std::size_t size) -> std::size_t {
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

// -------------
// ArchiveReader
// -------------

ArchiveReader::ArchiveReader() : BaseArchive()
{
    _impl->read_new();
    assert(exists());
}

ArchiveReader::~ArchiveReader()
{
    _impl->read_free();
}

int ArchiveReader::readToFile(std::string const & prefix)
{
    std::size_t success_count = 0;
    _impl->read_for_each([&](struct archive * a, struct archive_entry * entry) -> bool {
        assert(a != nullptr);
        assert(entry != nullptr);

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
                tDLogI("ArchiveReader::readToFile() current({}) data block EOF.", name);
                ++success_count;
                break;
            }
            if (code < ARCHIVE_WARN) {
                tDLogE("ArchiveReader::readToFile() current({}) data block error: {}",
                       name, _impl->getErrorString());
                break;
            }
            assert(code >= ARCHIVE_WARN);
            if (code < ARCHIVE_OK) {
                tDLogW("ArchiveReader::readToFile() current({}) data block warning: {}",
                       name, _impl->getErrorString());
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

int ArchiveReader::readToMemory(MemoryEntries & entries)
{
    entries.clear();
    _impl->read_for_each([&](struct archive * a, struct archive_entry * entry) -> bool {
        assert(a != nullptr);
        assert(entry != nullptr);

        char const * name = archive_entry_pathname(entry);
        if (name == nullptr) {
            tDLogW("ArchiveReader::readToMemory() empty path name.");
            return false;
        }

        ArchiveMemoryEntry info;
        Impl::entryInternal2external(entry, &info);
        if (info.size >= 1) {
            info.data.resize(info.size);

            void const * buffer;
            size_t size;
            la_int64_t offset;
            int code;

            while (true) {
                code = archive_read_data_block(a, &buffer, &size, &offset);
                if (code == ARCHIVE_EOF) {
                    tDLogI("ArchiveReader::readToMemory() current({}) data block EOF.", name);
                    break;
                }
                if (code < ARCHIVE_WARN) {
                    tDLogE("ArchiveReader::readToMemory() current({}) data block error, {}",
                           name, _impl->getErrorString());
                    break;
                }
                assert(code >= ARCHIVE_WARN);
                if (code < ARCHIVE_OK) {
                    tDLogW("ArchiveReader::readToMemory() current({}) data block warning: {}",
                           name, _impl->getErrorString());
                }
                memcpy(info.data.data() + offset, buffer, size);
            }
        }

        entries.push_back(info);
        return true;
    });
    return entries.size();
}

int ArchiveReader::readHeader(Entries & entries)
{
    entries.clear();
    _impl->read_for_each([&](struct archive * a, struct archive_entry * entry) -> bool {
        assert(a != nullptr);
        assert(entry != nullptr);

        ArchiveEntry info;
        Impl::entryInternal2external(entry, &info);
        entries.push_back(info);
        return false;
    });
    return entries.size();
}

int ArchiveReader::readName(std::vector<std::string> & names)
{
    names.clear();
    _impl->read_for_each([&](struct archive * a, struct archive_entry * entry) -> bool {
        assert(a != nullptr);
        assert(entry != nullptr);

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
    assert(exists());
    auto code = _impl->write_open_filename(path.c_str());
    if (isFailure(code)) {
        throw ErrException(code);
    }

    code = _impl->write_init(format, compress);
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

FileArchiveWriter::~FileArchiveWriter()
{
    _impl->write_close();
}

// -------------------
// MemoryArchiveWriter
// -------------------

MemoryArchiveWriter::MemoryArchiveWriter(std::size_t max_memory_size, std::string const & format, CompressType compress)
        : ArchiveWriter(format, compress), _archive_memory(max_memory_size)
{
    assert(exists());
    auto code = _impl->write_open_memory(_archive_memory.data(), _archive_memory.size(), &_used);
    if (isFailure(code)) {
        throw ErrException(code);
    }

    code = _impl->write_init(format, compress);
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

MemoryArchiveWriter::~MemoryArchiveWriter()
{
    _impl->write_close();
}

// -----------------
// FileArchiveReader
// -----------------

FileArchiveReader::FileArchiveReader(std::string const & path) : ArchiveReader()
{
    assert(_impl->exists());
    auto code = _impl->read_open_filename(path.c_str());
    if (isFailure(code)) {
        throw ErrException(code);
    }

    code = _impl->read_init();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

FileArchiveReader::~FileArchiveReader()
{
    _impl->read_close();
}

// -------------------
// MemoryArchiveReader
// -------------------

MemoryArchiveReader::MemoryArchiveReader(char const * buffer, std::size_t buffer_size)
        : ArchiveReader(), _archive_memory(buffer, buffer + buffer_size)
{
    assert(_impl->exists());
    auto code = _impl->read_open_memory(_archive_memory.data(), _archive_memory.size());
    if (isFailure(code)) {
        throw ErrException(code);
    }

    code = _impl->read_init();
    if (isFailure(code)) {
        throw ErrException(code);
    }
}

MemoryArchiveReader::~MemoryArchiveReader()
{
    _impl->read_close();
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

