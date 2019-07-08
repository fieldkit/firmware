#
#
#
BUILD ?= $(abspath build)
SHELL := /bin/bash
OFFLINE := /bin/false

LIBRARY_REPOSITORIES := jlewallen/arduino-osh jlewallen/loading conservify/segger \
	conservify/phylum conservify/lwstreams conservify/lwcron conservify/arduino-logging \
	conservify/WiFi101 conservify/Adafruit_SPIFlash conservify/Adafruit_QSPI nanopb/nanopb \
	olikraus/u8g2 mikalhart/TinyGPS nodejs/http-parser arduino-libraries/ArduinoMDNS \
	fieldkit/app-protocol fieldkit/data-protocol conservify/asf4
LOCAL_LIBRARY_PATHS := $(patsubst %, libraries/%, $(LIBRARY_REPOSITORIES))

default: setup all

all: samd51 test

ci: all doc

setup: .python-setup fk/secrets.h

.python-setup:
	pip3 install -U lief sphinx
	touch .python-setup

samd51: dependencies
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 ../../
	cd $(BUILD)/samd51 && $(MAKE)

amd64: dependencies
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../
	cd $(BUILD)/amd64 && $(MAKE)

test: amd64
	cd $(BUILD)/amd64 && env GTEST_COLOR=1 $(MAKE) test ARGS=-VV

doc:
	cd $(BUILD)/amd64 && $(MAKE) doc

fk/secrets.h: fk/secrets.h.template
	cp $^ $@

dependencies: $(LOCAL_LIBRARY_PATHS)
	@for l in $(LOCAL_LIBRARY_PATHS); do                                                       \
		if [[ ! -d $$l ]]; then                                                                  \
			echo $$l;                                                                              \
	  fi;                                                                                      \
	done

$(LOCAL_LIBRARY_PATHS): simple-deps

simple-deps: bootloader/dependencies.cmake libraries/dependencies.cmake

bootloader/dependencies.cmake:
	$(OFFLINE) || simple-deps --nested --config bootloader/dependencies.sd --dir libraries

libraries/dependencies.cmake:
	$(OFFLINE) || simple-deps --nested --config libraries/dependencies.sd --dir libraries

deps-initialize:
	+@for l in $(LIBRARY_REPOSITORIES); do                                                     \
		git subtree add --prefix libraries/$(l) https://github.com/$(l).git master --squash;     \
	done

deps-update:
	+@for l in $(LIBRARY_REPOSITORIES); do                                                     \
		git subtree pull --prefix libraries/$(l) https://github.com/$(l).git master --squash;    \
	done

veryclean: clean
	@for l in $(LOCAL_LIBRARY_PATHS); do                                                       \
		echo rm -rf $$l; rm -rf $$l;                                                             \
	done

clean:
	rm -rf bootloader/dependencies.cmake libraries/dependencies.cmake
	rm -rf $(BUILD)

.PHONY: dependencies doc
