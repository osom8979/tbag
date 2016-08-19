# Copyright (c) 2013, Ben Noordhuis <info@bnoordhuis.nl>
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir)/include \
              -I$(top_srcdir)/src

include_HEADERS=include/uv.h include/uv-errno.h include/uv-threadpool.h include/uv-version.h

CLEANFILES =

lib_LTLIBRARIES = libuv.la
libuv_la_CFLAGS = @CFLAGS@
libuv_la_LDFLAGS = -no-undefined -version-info 1:0:0
libuv_la_SOURCES = src/fs-poll.c \
                   src/heap-inl.h \
                   src/inet.c \
                   src/queue.h \
                   src/threadpool.c \
                   src/uv-common.c \
                   src/uv-common.h \
                   src/version.c

if SUNOS
# Can't be turned into a CC_CHECK_CFLAGS in configure.ac, it makes compilers
# on other platforms complain that the argument is unused during compilation.
libuv_la_CFLAGS += -pthread
endif

if WINNT

include_HEADERS += include/uv-win.h include/tree.h
AM_CPPFLAGS += -I$(top_srcdir)/src/win \
               -DWIN32_LEAN_AND_MEAN \
               -D_WIN32_WINNT=0x0600
libuv_la_SOURCES += src/win/async.c \
                    src/win/atomicops-inl.h \
                    src/win/core.c \
                    src/win/dl.c \
                    src/win/error.c \
                    src/win/fs-event.c \
                    src/win/fs.c \
                    src/win/getaddrinfo.c \
                    src/win/getnameinfo.c \
                    src/win/handle.c \
                    src/win/handle-inl.h \
                    src/win/internal.h \
                    src/win/loop-watcher.c \
                    src/win/pipe.c \
                    src/win/poll.c \
                    src/win/process-stdio.c \
                    src/win/process.c \
                    src/win/req.c \
                    src/win/req-inl.h \
                    src/win/signal.c \
                    src/win/stream.c \
                    src/win/stream-inl.h \
                    src/win/tcp.c \
                    src/win/thread.c \
                    src/win/timer.c \
                    src/win/tty.c \
                    src/win/udp.c \
                    src/win/util.c \
                    src/win/winapi.c \
                    src/win/winapi.h \
                    src/win/winsock.c \
                    src/win/winsock.h

else  # WINNT

include_HEADERS += include/uv-unix.h
AM_CPPFLAGS += -I$(top_srcdir)/src/unix
libuv_la_SOURCES += src/unix/async.c \
                   src/unix/atomic-ops.h \
                   src/unix/core.c \
                   src/unix/dl.c \
                   src/unix/fs.c \
                   src/unix/getaddrinfo.c \
                   src/unix/getnameinfo.c \
                   src/unix/internal.h \
                   src/unix/loop-watcher.c \
                   src/unix/loop.c \
                   src/unix/pipe.c \
                   src/unix/poll.c \
                   src/unix/process.c \
                   src/unix/signal.c \
                   src/unix/spinlock.h \
                   src/unix/stream.c \
                   src/unix/tcp.c \
                   src/unix/thread.c \
                   src/unix/timer.c \
                   src/unix/tty.c \
                   src/unix/udp.c

endif  # WINNT

EXTRA_DIST = test/fixtures/empty_file \
             test/fixtures/load_error.node \
             include \
             test \
             docs \
             img \
             samples \
             android-configure \
             CONTRIBUTING.md \
             LICENSE \
             README.md \
             checksparse.sh \
             vcbuild.bat \
             Makefile.mingw \
             common.gypi \
             gyp_uv.py \
             uv.gyp



TESTS = test/run-tests
check_PROGRAMS = test/run-tests
test_run_tests_CFLAGS =
test_run_tests_LDFLAGS =
test_run_tests_SOURCES =
test_run_tests_LDADD = libuv.la

if WINNT
test_run_tests_SOURCES +=
else
test_run_tests_SOURCES +=
endif

if AIX
test_run_tests_CFLAGS += -D_ALL_SOURCE -D_XOPEN_SOURCE=500 -D_LINUX_SOURCE_COMPAT
endif

if SUNOS
test_run_tests_CFLAGS += -D__EXTENSIONS__ -D_XOPEN_SOURCE=500
endif


if AIX
libuv_la_CFLAGS += -D_ALL_SOURCE -D_XOPEN_SOURCE=500 -D_LINUX_SOURCE_COMPAT -D_THREAD_SAFE
include_HEADERS += include/uv-aix.h
libuv_la_SOURCES += src/unix/aix.c
endif

if ANDROID
include_HEADERS += include/android-ifaddrs.h \
                   include/pthread-barrier.h
libuv_la_SOURCES += src/unix/android-ifaddrs.c \
                    src/unix/pthread-fixes.c \
                    src/unix/pthread-barrier.c
endif

if DARWIN
include_HEADERS += include/uv-darwin.h \
                   include/pthread-barrier.h
libuv_la_CFLAGS += -D_DARWIN_USE_64_BIT_INODE=1
libuv_la_CFLAGS += -D_DARWIN_UNLIMITED_SELECT=1
libuv_la_SOURCES += src/unix/darwin.c \
                    src/unix/darwin-proctitle.c \
                    src/unix/fsevents.c \
                    src/unix/kqueue.c \
                    src/unix/proctitle.c \
                    src/unix/pthread-barrier.c
test_run_tests_LDFLAGS += -lutil
endif

if DRAGONFLY
include_HEADERS += include/uv-bsd.h
test_run_tests_LDFLAGS += -lutil
endif

if FREEBSD
include_HEADERS += include/uv-bsd.h
libuv_la_SOURCES += src/unix/freebsd.c src/unix/kqueue.c
test_run_tests_LDFLAGS += -lutil
endif

if LINUX
include_HEADERS += include/uv-linux.h
libuv_la_CFLAGS += -D_GNU_SOURCE
libuv_la_SOURCES += src/unix/linux-core.c \
                    src/unix/linux-inotify.c \
                    src/unix/linux-syscalls.c \
                    src/unix/linux-syscalls.h \
                    src/unix/proctitle.c
test_run_tests_LDFLAGS += -lutil
endif

if NETBSD
include_HEADERS += include/uv-bsd.h
libuv_la_SOURCES += src/unix/kqueue.c src/unix/netbsd.c
test_run_tests_LDFLAGS += -lutil
endif

if OPENBSD
include_HEADERS += include/uv-bsd.h
libuv_la_SOURCES += src/unix/kqueue.c src/unix/openbsd.c
test_run_tests_LDFLAGS += -lutil
endif

if SUNOS
include_HEADERS += include/uv-sunos.h
libuv_la_CFLAGS += -D__EXTENSIONS__ -D_XOPEN_SOURCE=500
libuv_la_SOURCES += src/unix/sunos.c
endif

if HAVE_PKG_CONFIG
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = @PACKAGE_NAME@.pc
endif
