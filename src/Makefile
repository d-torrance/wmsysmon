CC = gcc

# Added for Debian GNU/Linux
DESTDIR =
prefix = /usr/local
bindir = ${prefix}/bin
mandir = ${prefix}/share/man/man1

INSTALL = install -c
LIBDIR += -L/usr/X11R6/lib
LDFLAGS += -lXpm -lXext -lX11 -lm -s
#CFLAGS = -g #-DMONDEBUG
CFLAGS += -O3 -m486 -fomit-frame-pointer -I/usr/X11R6/include #-DHI_INTS #undefine HI_INTS if your x86 SMP or alpha

BIN = wmsysmon
MANPAGE = wmsysmon.1
OBJS =	wmgeneral.o \
	wmsysmon.o

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LIBDIR) $(LDFLAGS)

all: wmsysmon

clean:
	rm -f $(OBJS) $(BIN)

install:
	$(INSTALL) -g root -o root $(BIN) $(DESTDIR)$(bindir)
	$(INSTALL) -g root -o root $(MANPAGE) $(DESTDIR)$(mandir)
