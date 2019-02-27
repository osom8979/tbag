/**
 * @file   Archive.cpp
 * @brief  Archive class implementation.
 * @author zer0
 * @date   2019-02-25
 */

#include <libtbag/archive/Archive.hpp>
#include <libtbag/log/Log.hpp>
#include <libtbag/system/SysInfo.hpp>
#include <libtbag/filesystem/Path.hpp>
#include <libtbag/filesystem/File.hpp>
#include <libtbag/util/BufferInfo.hpp>

#include <archive.h>
#include <archive_entry.h>

#include <cassert>

// -------------------
NAMESPACE_LIBTBAG_OPEN
// -------------------

namespace archive {

static char const * __get_archive_error_string(struct archive * a) TBAG_NOEXCEPT
{
    auto const * e = archive_error_string(a);
    return e ? e : "Unknown error";
}

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
                    tDLogE("compressArchive() read size mismatch: {}/{}", item, next);
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

// ----------------------
// Archive implementation
// ----------------------

Archive::Archive()
{
    // EMPTY.
}

Archive::~Archive()
{
    // EMPTY.
}

} // namespace archive

// --------------------
NAMESPACE_LIBTBAG_CLOSE
// --------------------

