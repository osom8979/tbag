#.SUFFIXES: .cpp .o

CXX=g++
TARGET=libtinyxml2.a
OBJS=tinyxml2.o
SRCS=${OBJS:.o=.cpp}
CXXFLAGS=-w -O2 -fPIC -DNDEBUG

ifeq ($(OS),Windows_NT)
    CXXFLAGS+= -D_CRT_SECURE_NO_WARNINGS
endif


.PONEY=all clean

all: ${TARGET}

clean:
	rm -f ${OBJS} ${TARGET}

${TARGET}: ${OBJS}
	ar cru $@ $^
	ranlib $@

