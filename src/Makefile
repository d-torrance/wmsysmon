CC = gcc
PREFIX = /usr/local

LIBDIR += -L/usr/X11R6/lib
LIBS = -lXpm -lXext -lX11 -lm
#CFLAGS = -g #-DMONDEBUG
CFLAGS += -Wall -O3 -fomit-frame-pointer -I/usr/X11R6/include #-DHI_INTS #undefine HI_INTS if your x86 SMP or alpha

BIN = wmsysmon
OBJS =	wmgeneral.o \
	wmsysmon.o

$(BIN): $(OBJS)
	$(CC) $(LDFLAGS) -o $(BIN) $(OBJS) $(LIBDIR) $(LIBS)

all: wmsysmon

clean:
	rm -f $(OBJS) $(BIN)

install:
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	install -g root -o root $(BIN) $(DESTDIR)$(PREFIX)/bin
