DIR=$(DESTDIR)/opt/iot
LIBDIR=$(DESTDIR)/usr/lib
CFGDIR=$(DESTDIR)/etc/iotsample-odroid

LIBMQTT_PATH=/usr/local/lib
LIBMQTT_NAME=paho-mqtt3as

all: iot

.PHONY: all install clean distclean

iot: iotmain.c cpustat.c mac.c mqttPublisher.c jsonator.c iot.h jsonReader.c
	export LIBRARY_PATH=$(LIBMQTT_PATH):$(LIBRARY_PATH)
	$(CC) iotmain.c cpustat.c mac.c mqttPublisher.c jsonator.c cJSON.c jsonReader.c -o $@ -l$(LIBMQTT_NAME) -lpthread -lssl -lm -I.
	strip $@

install: iot
	mkdir -p $(LIBDIR)
	mkdir -p $(DIR)
	mkdir -p $(CFGDIR)
	install iot $(DIR)/iot
	install iotGetDeviceID.sh $(DIR)/iotGetDeviceID.sh
	cp README.md $(DIR)/README
	cp IoTFoundation.pem $(DIR)/IoTFoundation.pem
	cp etc/QuickStart.html.template $(CFGDIR)/QuickStart.html.template
	cp etc/iotsample-odroid /etc/init.d/iotsample-odroid
	update-rc.d iotsample-odroid defaults 90

clean:
	rm -f iot
