TARGETS=xwin-wrapper xwin-button-wrapper xwin-double-window send-button-event set-xprop xwininfo
LDLIBS:=$(shell pkg-config --libs gtk+-2.0) $(shell pkg-config --cflags --libs x11)
CFLAGS:=-g $(shell pkg-config --cflags gtk+-2.0)

all: $(TARGETS)

clean:
	rm -f *~ *.o

distclean: clean
	rm -f $(TARGETS)
