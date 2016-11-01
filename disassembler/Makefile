OBJ = address_stack.o bda.o engine-6502.o engine-6809.c labels.o machines.o memory.o utils.c

CC = gcc
LD = gcc

CFLAGS = -O2
LDFLAGS = -lc

DESTDIR = /usr/local/6502

BINDIR = $(DESTDIR)/bin
MANDIR = $(DESTDIR)/share/man/man1
DOCDIR = $(DESTDIR)/share/doc

MKDIR = mkdir -p
INSTALL = install

all: bda

bda: ${OBJ}
	${LD} -o bda ${OBJ} ${LDFLAGS}

proto: clean
	cproto *.c >proto.h

clean:
	rm -f *.o *~ \#* bda
