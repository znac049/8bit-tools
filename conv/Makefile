.PHONY:	all dist-clean clean disks install

DESTDIR=/home/bob/8bit

SRC = conv.cc \
	Converter.cc \
	Dragon.cc \
	IntelHex.cc \
	Memory.cc \
	MemoryCell.cc \
	MotoSREC.cc \
	Raw.cc \
	Utils.cc

OBJ=$(subst .cc,.o,$(SRC))
HDRS=$(wildcard *.h)

EXE = conv

CC = g++
LD = g++

CFLAGS = -O2 -UH6309 -Wno-format
LDFLAGS =

BINDIR = $(DESTDIR)/bin
MANDIR = $(DESTDIR)/share/man/man1
DOCDIR = $(DESTDIR)/share/doc

.cc.o: $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(EXE)

install: all
	install conv $(BINDIR)

$(EXE): ${OBJ}
	${LD} -o $(EXE) ${OBJ} ${LDFLAGS}

proto: clean
	cproto *.c >proto.h

clean:
	rm -f *.o *~ \#* $(EXE)

dist-clean: clean
	rm -f *.img proto.h
