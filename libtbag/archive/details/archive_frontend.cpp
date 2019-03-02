/**
 * @file   archive_frontend.cpp
 * @brief  archive_frontend class implementation.
 * @author zer0
 * @date   2019-03-02
 */

#include <libtbag/archive/details/archive_frontend.hpp>

#include <archive.h>
#include <archive_entry.h>

static_assert(AE_IFMT   == _archive_entry_ifmt  , "");
static_assert(AE_IFREG  == _archive_entry_ifreg , "");
static_assert(AE_IFLNK  == _archive_entry_iflnk , "");
static_assert(AE_IFSOCK == _archive_entry_ifsock, "");
static_assert(AE_IFCHR  == _archive_entry_ifchr , "");
static_assert(AE_IFBLK  == _archive_entry_ifblk , "");
static_assert(AE_IFDIR  == _archive_entry_ifdir , "");
static_assert(AE_IFIFO  == _archive_entry_ififo , "");

static_assert(ARCHIVE_EOF    == _archive_eof   , "");
static_assert(ARCHIVE_OK     == _archive_ok    , "");
static_assert(ARCHIVE_RETRY  == _archive_retry , "");
static_assert(ARCHIVE_WARN   == _archive_warn  , "");
static_assert(ARCHIVE_FAILED == _archive_failed, "");
static_assert(ARCHIVE_FATAL  == _archive_fatal , "");

inline static struct archive * cast_archive(void * a)
{
    return (struct archive *)a;
}

inline static struct archive_entry * cast_entry(void * e)
{
    return (struct archive_entry *)e;
}

void * _archive_write_new()
{
    return archive_write_new();
}

void * _archive_read_new()
{
    return archive_read_new();
}

void * _archive_entry_new()
{
    return archive_entry_new();
}

int _archive_write_free(void * a)
{
    return archive_write_free(cast_archive(a));
}

int _archive_read_free(void * a)
{
    return archive_read_free(cast_archive(a));
}

void _archive_entry_free(void * e)
{
    archive_entry_free(cast_entry(e));
}

char const * _archive_error_string(void * a)
{
    char const * e = archive_error_string(cast_archive(a));
    return e ? e : "Empty error message";
}

int _archive_write_add_filter_none(void * a)
{
    return archive_write_add_filter_none(cast_archive(a));
}

int _archive_write_add_filter_gzip(void * a)
{
    return archive_write_add_filter_gzip(cast_archive(a));
}

int _archive_write_add_filter_bzip2(void * a)
{
    return archive_write_add_filter_bzip2(cast_archive(a));
}

int _archive_write_add_filter_lzma(void * a)
{
    return archive_write_add_filter_lzma(cast_archive(a));
}

int _archive_write_add_filter_xz(void * a)
{
    return archive_write_add_filter_xz(cast_archive(a));
}

int _archive_write_header(void * a, void * e)
{
    return archive_write_header(cast_archive(a), cast_entry(e));
}

size_t _archive_write_data(void * a, void const * buffer, size_t size)
{
    return (size_t)archive_write_data(cast_archive(a), buffer, size);
}

int _archive_write_finish_entry(void * a)
{
    return archive_write_finish_entry(cast_archive(a));
}

int _archive_write_set_filter_option(void * a, char const * m, char const * o, char const * v)
{
    return archive_write_set_filter_option(cast_archive(a), m, o, v);
}

int _archive_write_set_format_by_name(void * a, char const * name)
{
    return archive_write_set_format_by_name(cast_archive(a), name);
}

int _archive_write_set_bytes_in_last_block(void * a, int bytes_in_last_block)
{
    return archive_write_set_bytes_in_last_block(cast_archive(a), bytes_in_last_block);
}

int _archive_write_open_filename(void * a, char const * filename)
{
    return archive_write_open_filename(cast_archive(a), filename);
}

int _archive_write_open_memory(void * a, void * buffer, size_t size, size_t * used)
{
    return archive_write_open_memory(cast_archive(a), buffer, size, used);
}

int _archive_write_close(void * a)
{
    return archive_write_close(cast_archive(a));
}

int _archive_read_support_filter_all(void * a)
{
    return archive_read_support_filter_all(cast_archive(a));
}

int _archive_read_support_format_all(void * a)
{
    return archive_read_support_format_all(cast_archive(a));
}

int _archive_read_support_compression_all(void * a)
{
    return archive_read_support_compression_all(cast_archive(a));
}

int _archive_read_open_filename(void * a, char const * filename, size_t block_size)
{
    return archive_read_open_filename(cast_archive(a), filename, block_size);
}

int _archive_read_open_memory(void * a, void const * buffer, size_t size)
{
    return archive_read_open_memory(cast_archive(a), buffer, size);
}

int _archive_read_close(void * a)
{
    return archive_read_close(cast_archive(a));
}

int _archive_read_next_header(void * a, void ** e)
{
    return archive_read_next_header(cast_archive(a), (struct archive_entry **)e);
}

int _archive_read_data_skip(void * a)
{
    return archive_read_data_skip(cast_archive(a));
}

int _archive_read_data_block(void * a, void const ** buffer, size_t * size, int64_t * offset)
{
    return archive_read_data_block(cast_archive(a), buffer, size, offset);
}

char const * _archive_entry_sourcepath(void * e)
{
    return archive_entry_sourcepath(cast_entry(e));
}

char const * _archive_entry_pathname(void * e)
{
    return archive_entry_pathname(cast_entry(e));
}

unsigned int _archive_entry_filetype(void * e)
{
    return archive_entry_filetype(cast_entry(e));
}

int64_t _archive_entry_size(void * e)
{
    return (int64_t)archive_entry_size(cast_entry(e));
}

int64_t _archive_entry_uid(void * e)
{
    return (int64_t)archive_entry_uid(cast_entry(e));
}

int64_t _archive_entry_gid(void * e)
{
    return (int64_t)archive_entry_gid(cast_entry(e));
}

int64_t _archive_entry_perm(void * e)
{
    return (int64_t)archive_entry_perm(cast_entry(e));
}

void _archive_entry_copy_sourcepath(void * e, char const * str)
{
    archive_entry_copy_sourcepath(cast_entry(e), str);
}

void _archive_entry_copy_pathname(void * e, char const * str)
{
    archive_entry_copy_pathname(cast_entry(e), str);
}

void _archive_entry_set_filetype(void * e, unsigned int type)
{
    archive_entry_set_filetype(cast_entry(e), type);
}

void _archive_entry_set_size(void * e, int64_t size)
{
    archive_entry_set_size(cast_entry(e), size);
}

void _archive_entry_set_uid(void * e, int64_t uid)
{
    archive_entry_set_uid(cast_entry(e), uid);
}

void _archive_entry_set_gid(void * e, int64_t gid)
{
    archive_entry_set_gid(cast_entry(e), gid);
}

void _archive_entry_set_perm(void * e, int64_t perm)
{
    archive_entry_set_perm(cast_entry(e), (__LA_MODE_T)perm);
}

void _archive_entry_acl_clear(void * e)
{
    archive_entry_acl_clear(cast_entry(e));
}

void _archive_entry_xattr_clear(void * e)
{
    archive_entry_xattr_clear(cast_entry(e));
}

void _archive_entry_set_fflags(void * e, unsigned long set, unsigned long clear)
{
    archive_entry_set_fflags(cast_entry(e), set, clear);
}

void _archive_entry_sparse_clear(void * e)
{
    archive_entry_sparse_clear(cast_entry(e));
}

