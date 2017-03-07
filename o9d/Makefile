SRCS=o9d.c info.c utils.c bitmap.c tree.c dump.c colour.c

CFLAGS=
LDFLAGS=

LIBS=

CC=gcc
LD=gcc

HDRS=$(wildcard *.h)
OBJS=$(subst .c,.o,$(SRCS))
EXE=o9d

.c.o: $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:	all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS) *.r *~

