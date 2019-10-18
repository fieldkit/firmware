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
	fieldkit/app-protocol fieldkit/data-protocol conservify/asf4 conservify/SdFat
LOCAL_LIBRARY_PATHS := $(patsubst %, libraries/%, $(LIBRARY_REPOSITORIES))

default: setup all

all: samd51 samd09 test

ci: setup all doc package

setup: .python-setup fk/secrets.h libraries/done

.python-setup:
	pip3 install -U sphinx pyelftools
	touch .python-setup

cmake: $(BUILD)/samd51 $(BUILD)/samd51-pic $(BUILD)/samd09 $(BUILD)/amd64

$(BUILD)/samd51: setup
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 ../../

$(BUILD)/samd51-pic: setup
	mkdir -p $(BUILD)/samd51-pic
	cd $(BUILD)/samd51-pic && cmake -DTARGET_ARCH=samd51 -DTARGET_PIC=ON ../../

$(BUILD)/amd64: setup
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../

$(BUILD)/samd09: setup
	mkdir -p $(BUILD)/samd09
	cd $(BUILD)/samd09 && cmake -DTARGET_ARCH=samd09 ../../

samd51: $(BUILD)/samd51 $(BUILD)/samd51-pic
	cd $(BUILD)/samd51 && $(MAKE)
	cd $(BUILD)/samd51-pic && $(MAKE)

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

package:
	mkdir -p $(BUILD)/fk-firmware
	cp tools/flash-core $(BUILD)/fk-firmware
	cp tools/flash-weather $(BUILD)/fk-firmware
	cp $(BUILD)/samd51/bootloader/fkbl.elf  $(BUILD)/fk-firmware
	cp $(BUILD)/samd51/bootloader/fkbl.bin  $(BUILD)/fk-firmware
	cp $(BUILD)/samd51/fk/fk-bundled-fkb.elf  $(BUILD)/fk-firmware
	cp $(BUILD)/samd51/fk/fk-bundled-fkb.bin  $(BUILD)/fk-firmware
	cp $(BUILD)/samd09/modules/weather/sidecar/fk-weather-sidecar.elf  $(BUILD)/fk-firmware
	cp $(BUILD)/samd09/modules/weather/sidecar/fk-weather-sidecar.bin  $(BUILD)/fk-firmware
	chmod 644 $(BUILD)/fk-firmware/*
	chmod 755 $(BUILD)/fk-firmware/flash-*
	if [ -z $(BUILD_NUMBER) ]; then                                    \
	cd $(BUILD) && zip -r fk-firmware.zip fk-firmware;                 \
	else                                                               \
	cd $(BUILD) && zip -r fk-firmware-$(BUILD_NUMBER).zip fk-firmware; \
	fi

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
	+@for m in build/samd51-pic/fk/*.map; do                                                   \
    echo $$m.cpp;                                                                            \
    cat $$m | c++filt > $$m.cpp;                                                             \
	done

clean:
	rm -rf $(BUILD)

.PHONY: dependencies doc cmake
