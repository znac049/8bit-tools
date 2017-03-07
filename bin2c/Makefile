SRCS=bin2c.c

CFLAGS=
LDFLAGS=

LIBS=

CC=gcc
LD=gcc

HDRS=$(wildcard *.h)
OBJS=$(subst .c,.o,$(SRCS))
EXE=bin2c

.c.o: $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY:	all clean

all: $(EXE)

$(EXE): $(OBJS)
	$(LD) -o $@ $(OBJS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS) *.r *~

