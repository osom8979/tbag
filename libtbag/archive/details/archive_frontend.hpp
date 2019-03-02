/**
 * @file   archive_frontend.hpp
 * @brief  archive_frontend class prototype.
 * @author zer0
 * @date   2019-03-02
 */

#ifndef __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_DETAILS_ARCHIVE_FRONTEND_HPP__
#define __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_DETAILS_ARCHIVE_FRONTEND_HPP__

// MS compatible compilers support #pragma once
#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#include <cstdint>
#include <cstddef>

unsigned int const _archive_entry_ifmt   = 0170000;
unsigned int const _archive_entry_ifreg  = 0100000;
unsigned int const _archive_entry_iflnk  = 0120000;
unsigned int const _archive_entry_ifsock = 0140000;
unsigned int const _archive_entry_ifchr  = 0020000;
unsigned int const _archive_entry_ifblk  = 0060000;
unsigned int const _archive_entry_ifdir  = 0040000;
unsigned int const _archive_entry_ififo  = 0010000;

int const _archive_eof    =   1;
int const _archive_ok     =   0;
int const _archive_retry  = -10;
int const _archive_warn   = -20;
int const _archive_failed = -25;
int const _archive_fatal  = -30;

void * _archive_write_new();
void * _archive_read_new ();
void * _archive_entry_new();

int  _archive_write_free(void * a);
int  _archive_read_free (void * a);
void _archive_entry_free(void * a);

char const * _archive_error_string(void * a);

int    _archive_write_add_filter_none        (void * a);
int    _archive_write_add_filter_gzip        (void * a);
int    _archive_write_add_filter_bzip2       (void * a);
int    _archive_write_add_filter_lzma        (void * a);
int    _archive_write_add_filter_xz          (void * a);
int    _archive_write_header                 (void * a, void * e);
size_t _archive_write_data                   (void * a, void const * buffer, size_t size);
int    _archive_write_finish_entry           (void * a);
int    _archive_write_set_filter_option      (void * a, char const * m, char const * o, char const * v);
int    _archive_write_set_format_by_name     (void * a, char const * name);
int    _archive_write_set_bytes_in_last_block(void * a, int bytes_in_last_block);
int    _archive_write_open_filename          (void * a, char const * filename);
int    _archive_write_open_memory            (void * a, void * buffer, size_t size, size_t * used);
int    _archive_write_close                  (void * a);

int _archive_read_support_filter_all     (void * a);
int _archive_read_support_format_all     (void * a);
int _archive_read_support_compression_all(void * a);
int _archive_read_open_filename          (void * a, char const * filename, size_t block_size);
int _archive_read_open_memory            (void * a, void const * buffer, size_t size);
int _archive_read_close                  (void * a);
int _archive_read_next_header            (void * a, void ** e);
int _archive_read_data_skip              (void * a);
int _archive_read_data_block             (void * a, void const ** buffer, size_t * size, int64_t * offset);

char const * _archive_entry_sourcepath(void * e);
char const * _archive_entry_pathname  (void * e);
unsigned int _archive_entry_filetype  (void * e);
int64_t      _archive_entry_size      (void * e);
int64_t      _archive_entry_uid       (void * e);
int64_t      _archive_entry_gid       (void * e);
int64_t      _archive_entry_perm      (void * e);

void _archive_entry_copy_sourcepath(void * e, char const * str);
void _archive_entry_copy_pathname  (void * e, char const * str);
void _archive_entry_set_filetype   (void * e, unsigned int type);
void _archive_entry_set_size       (void * e, int64_t size);
void _archive_entry_set_uid        (void * e, int64_t uid);
void _archive_entry_set_gid        (void * e, int64_t gid);
void _archive_entry_set_perm       (void * e, int64_t perm);
void _archive_entry_acl_clear      (void * e);
void _archive_entry_xattr_clear    (void * e);
void _archive_entry_set_fflags     (void * e, unsigned long set, unsigned long clear);
void _archive_entry_sparse_clear   (void * e);

#endif // __INCLUDE_LIBTBAG__LIBTBAG_ARCHIVE_DETAILS_ARCHIVE_FRONTEND_HPP__

