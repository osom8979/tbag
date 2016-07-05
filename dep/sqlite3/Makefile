#.SUFFIXES: .cpp .o

CC=gcc
TARGET=libsqlite3.a
OBJS=sqlite3.o
SRCS=${OBJS:.o=.c}
CFLAGS=-w -O2 -fPIC -DNDEBUG -DSQLITE_THREADSAFE=1 -DSQLITE_ENABLE_FTS3 -DSQLITE_ENABLE_RTREE

.PONEY=all clean

all: ${TARGET}

clean:
	rm -f ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	ar cru $@ $^
	ranlib $@

