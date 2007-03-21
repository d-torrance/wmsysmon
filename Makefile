CC = gcc

LIBDIR += -L/usr/X11R6/lib
LDFLAGS += -lXpm -lXext -lX11 -lm -s
#CFLAGS = -g #-DMONDEBUG
CFLAGS += -Wall -O3 -fomit-frame-pointer -I/usr/X11R6/include #-DHI_INTS #undefine HI_INTS if your x86 SMP or alpha

BIN = src/wmsysmon
OBJS =	src/wmgeneral.o \
	src/wmsysmon.o

$(BIN): $(OBJS)
	$(CC) -o $(BIN) $(OBJS) $(LIBDIR) $(LDFLAGS)

all: wmsysmon

clean:
	rm -f $(OBJS) $(BIN)

install: $(BIN)
	mkdir -p $(DESTDIR)/usr/bin
	install -g root -o root $(BIN) $(DESTDIR)/usr/bin
