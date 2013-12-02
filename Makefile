V	= dyn-1.0.4
CC	= gcc
CFLAGS	= -O2
BIN	= /usr/local/bin
TARFILES= $(V)/Makefile $(V)/INSTALL $(V)/README* $(V)/dyn $(V)/dyn.*
# Uncomment -lnsl -lsocket in LIBS to compile on some UNIXs (SunOS, etc.)
LIBS	= # -lnsl -lsocket

all: dyn

dyn:
	$(CC) $(CFLAGS) -o dyn dyn.c $(LIBS)
	strip dyn

install:
	install -m 0700 -o root -s ./dyn $(BIN)/dyn
	
archive:
	cd .. ; \
	tar czvf $(V).tar.gz $(TARFILES)
