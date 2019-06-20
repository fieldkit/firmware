#
#
#
BUILD ?= $(abspath build)
SHELL := /bin/bash

default: all

all: test samd51

samd51: dependencies
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 ../../
	cd $(BUILD)/samd51 && $(MAKE)

amd64: dependencies
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../
	cd $(BUILD)/amd64 && $(MAKE)

test: amd64
	cd $(BUILD)/amd64 && $(MAKE) test

dependencies: gitdeps

gitdeps:
	simple-deps --config libraries/dependencies.sd

deps-initialize:
	git subtree add --prefix libraries/phylum https://github.com/conservify/phylum.git master --squash
	git subtree add --prefix libraries/lwstreams https://github.com/conservify/lwstreams.git master --squash
	git subtree add --prefix libraries/lwcron https://github.com/conservify/lwcron.git master --squash
	git subtree add --prefix libraries/nanopb https://github.com/nanopb/nanopb.git master --squash
	git subtree add --prefix libraries/arduino-logging https://github.com/Conservify/arduino-logging.git master --squash
	git subtree add --prefix libraries/WiFi101 https://github.com/conservify/WiFi101.git master --squash
	git subtree add --prefix libraries/Adafruit_SPIFlash https://github.com/conservify/Adafruit_SPIFlash.git master --squash
	git subtree add --prefix libraries/Adafruit_QSPI https://github.com/conservify/Adafruit_QSPI.git master --squash
	git subtree add --prefix libraries/u8g2 https://github.com/olikraus/u8g2.git master --squash
	git subtree add --prefix libraries/TinyGPS https://github.com/mikalhart/TinyGPS.git master --squash

deps-update:
	git subtree pull --prefix libraries/phylum https://github.com/conservify/phylum.git master --squash
	git subtree pull --prefix libraries/lwstreams https://github.com/conservify/lwstreams.git master --squash
	git subtree pull --prefix libraries/lwcron https://github.com/conservify/lwcron.git master --squash
	git subtree pull --prefix libraries/nanopb https://github.com/nanopb/nanopb.git master --squash
	git subtree pull --prefix libraries/arduino-logging https://github.com/Conservify/arduino-logging.git master --squash
	git subtree pull --prefix libraries/WiFi101 https://github.com/conservify/WiFi101.git master --squash
	git subtree pull --prefix libraries/Adafruit_SPIFlash https://github.com/conservify/Adafruit_SPIFlash.git master --squash
	git subtree pull --prefix libraries/Adafruit_QSPI https://github.com/conservify/Adafruit_QSPI.git master --squash
	git subtree pull --prefix libraries/u8g2 https://github.com/olikraus/u8g2.git master --squash
	git subtree pull --prefix libraries/TinyGPS https://github.com/mikalhart/TinyGPS.git master --squash

clean:
	rm -rf $(BUILD)
