DIR=$(DESTDIR)/opt/iot
LIBDIR=$(DESTDIR)/usr/lib
COMPILELIBDIR=./lib
LIBNAME=paho-mqtt3a

all: iot

.PHONY: all install clean distclean

iot:
	export LIBRARY_PATH=$(COMPILELIBDIR):$(LIBRARY_PATH)
	$(CC) iotmain.c cpustat.c mac.c mqttPublisher.c jsonator.c -o $@ -l$(LIBNAME) -lpthread -lm -L $(COMPILELIBDIR) -I .
	strip $@

install: iot
	mkdir -p $(LIBDIR)
	mkdir -p $(DIR)
	chmod 777 $(DIR)
	install iot $(DIR)/iot
	cp $(COMPILELIBDIR)/libpaho-mqtt3a.so $(LIBDIR)/libpaho-mqtt3a.so
	cp $(COMPILELIBDIR)/libpaho-mqtt3a.so.1 $(LIBDIR)/libpaho-mqtt3a.so.1
	cp $(COMPILELIBDIR)/libpaho-mqtt3a.so.1.0 $(LIBDIR)/libpaho-mqtt3a.so.1.0
	cp iot.properties $(DIR)/iot.properties
	install start-iot.sh $(DIR)/start-iot.sh
	install stop-iot.sh $(DIR)/stop-iot.sh
	cp README $(DIR)/README
	cp iot.log $(DIR)/iot.log

clean:
	rm -f iot
