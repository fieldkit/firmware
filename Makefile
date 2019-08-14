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

all: samd51 samd09 test

ci: all doc

setup: .python-setup fk/secrets.h libraries/done

.python-setup:
	pip3 install -U lief sphinx
	touch .python-setup

cmake: $(BUILD)/samd51 $(BUILD)/samd09 $(BUILD)/amd64

$(BUILD)/samd51: setup
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 ../../

$(BUILD)/amd64: setup
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../

$(BUILD)/samd09: setup
	mkdir -p $(BUILD)/samd09
	cd $(BUILD)/samd09 && cmake -DTARGET_ARCH=samd09 ../../

samd51: $(BUILD)/samd51
	cd $(BUILD)/samd51 && $(MAKE)

samd09: $(BUILD)/samd09
	cd $(BUILD)/samd09 && $(MAKE)

amd64: $(BUILD)/amd64
	cd $(BUILD)/amd64 && $(MAKE)

fw: samd51 samd09

test: amd64
	cd $(BUILD)/amd64 && env GTEST_COLOR=1 $(MAKE) test ARGS=-VV

testf: amd64
	cd $(BUILD)/amd64 && env GTEST_COLOR=1 GTEST_FILTER="-Slow*" $(MAKE) test ARGS=-VV

fake: amd64

run-fake: fake
	build/amd64/tests/fake/fake

doc:
	cd $(BUILD)/amd64 && $(MAKE) doc

fk/secrets.h: fk/secrets.h.template
	cp $^ $@

dependencies: libraries/done

gitdeps: dependencies

libraries/done:
	$(OFFLINE) || simple-deps --nested --config bootloader/dependencies.sd --dir libraries
	$(OFFLINE) || simple-deps --nested --config libraries/dependencies.sd --dir libraries
	$(OFFLINE) || simple-deps --nested --config modules/weather/sidecar/dependencies.sd --dir libraries
	$(OFFLINE) || simple-deps --nested --config modules/weather/main/dependencies.sd --dir libraries
	touch libraries/done

deps-initialize:
	+@for l in $(LIBRARY_REPOSITORIES); do                                                     \
		git subtree add --prefix libraries/$(l) https://github.com/$(l).git master --squash;     \
	done

deps-update:
	+@for l in $(LIBRARY_REPOSITORIES); do                                                     \
		git subtree pull --prefix libraries/$(l) https://github.com/$(l).git master --squash;    \
	done

veryclean: clean
	rm -rf bootloader/dependencies.cmake libraries/dependencies.cmake libraries/done
	rm -rf modules/weather/sidecar/dependencies.cmake modules/weather/main/dependencies.cmake
	@for l in $(LOCAL_LIBRARY_PATHS); do                                                       \
		echo rm -rf $$l; rm -rf $$l;                                                             \
	done

info:
	+@for m in build/samd51/fk/*.map; do                                                       \
    echo $$m.cpp;                                                                            \
    cat $$m | c++filt > $$m.cpp;                                                             \
	done

clean:
	rm -rf $(BUILD)

.PHONY: dependencies doc cmake
