DIR=$(DESTDIR)/opt/iot
BINDIR=$(DIR)/bin
LIBDIR=$(DIR)/lib
COMPILELIBDIR=./lib
LIBNAME=paho-mqtt3a

all: iot

.PHONY: all install clean distclean

iot:
	export LIBRARY_PATH=$(COMPILELIBDIR):$(LIBRARY_PATH)
	$(CC) iotmain.c cpustat.c mac.c mqttPublisher.c -o $@ -l$(LIBNAME) -lpthread -lm -L $(COMPILELIBDIR) -I .
	strip $@

install: iot
	mkdir -p $(BINDIR)
	mkdir -p $(LIBDIR)
	install iot $(BINDIR)/iot
	install $(COMPILELIBDIR)/libpaho-mqtt3a.so $(LIBDIR)/libpaho-mqtt3a.so
	install $(COMPILELIBDIR)/libpaho-mqtt3a.so.1 $(LIBDIR)/libpaho-mqtt3a.so.1
	install $(COMPILELIBDIR)/libpaho-mqtt3a.so.1.0 $(LIBDIR)/libpaho-mqtt3a.so.1.0
	cp iot.properties $(BINDIR)/iot.properties

clean:
	rm -f iot
