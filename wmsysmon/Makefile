LIBDIR = -L/usr/X11R6/lib
LIBS   = -lXpm -lXext -lX11 -lm
FLAGS = -O2
OBJS =	wmsysmon.o \
		../wmgeneral/wmgeneral.o \
		../wmgeneral/misc.o \
		../wmgeneral/list.o

.c.o:
	cc -I/usr/X11R6/share/include $(FLAGS) -c -Wall $< -o $*.o

wmsysmon: $(OBJS)
	cc $(FLAGS) -o wmsysmon $^ -lXext $(LIBDIR) $(LIBS)

all:: wmsysmon

clean::
	for i in $(OBJS) ; do \
		rm -f $$i ; \
	done
	rm -f wmsysmon
	rm -f *~

install:: wmsysmon
	cp -f wmsysmon $(DESTDIR)/usr/X11R6/bin/
	chmod 755 $(DESTDIR)/usr/X11R6/bin/wmsysmon
	chown root:root $(DESTDIR)/usr/X11R6/bin/wmsysmon
	@echo "wmsysmon Installation finished..."
