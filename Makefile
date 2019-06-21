#
#
#
BUILD ?= $(abspath build)
SHELL := /bin/bash
OFFLINE := /bin/false

default: setup all

all: test samd51

setup: .python-setup

.python-setup:
	pip install lief
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
	cd $(BUILD)/amd64 && $(MAKE) test

dependencies: libraries

gitdeps: libraries

LIBRARY_REPOSITORIES := jlewallen/arduino-osh jlewallen/loading conservify/segger \
	conservify/phylum conservify/lwstreams conservify/lwcron conservify/arduino-logging \
	conservify/WiFi101 conservify/Adafruit_SPIFlash conservify/Adafruit_QSPI nanopb/nanopb \
	olikraus/u8g2 mikalhart/TinyGPS
LOCAL_LIBRARY_PATHS := $(patsubst %, libraries/%, $(LIBRARY_REPOSITORIES))

libraries: $(LOCAL_LIBRARY_PATHS)

$(LOCAL_LIBRARY_PATHS):
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
	rm -rf gitdeps
	@for l in $(LOCAL_LIBRARY_PATHS); do                                                       \
		rm -rf $$l                                                                               \
	done

clean:
	rm -rf $(BUILD)

.PHONY: libraries
