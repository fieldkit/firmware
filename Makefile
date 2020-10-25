#
#
#
BUILD ?= $(abspath build)
SHELL := /bin/bash
OFFLINE := /bin/false
BUILD_NUMBER ?= none
PACKAGE = fk-firmware-$(BUILD_NUMBER)

LIBRARY_REPOSITORIES := jlewallen/arduino-osh jlewallen/loading conservify/segger \
	conservify/phylum conservify/lwstreams conservify/lwcron conservify/arduino-logging \
	conservify/WiFi101 conservify/Adafruit_SPIFlash conservify/Adafruit_QSPI nanopb/nanopb \
	olikraus/u8g2 mikalhart/TinyGPS nodejs/http-parser arduino-libraries/ArduinoMDNS \
	fieldkit/app-protocol fieldkit/data-protocol conservify/asf4 conservify/SdFat
LOCAL_LIBRARY_PATHS := $(patsubst %, libraries/%, $(LIBRARY_REPOSITORIES))

default: setup all

all: samd51 samd09 test

ci: setup all doc package

setup: .python-setup fk/secrets.h fk/secrets.cpp fk/data/animals.h fk/data/adjectives.h libraries/done

.python-setup:
	pip3 install -U sphinx pyelftools pyblake2
	touch .python-setup

cmake: $(BUILD)/samd51 $(BUILD)/samd51-pic $(BUILD)/samd09 $(BUILD)/amd64

$(BUILD)/samd51: setup
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 ../../

$(BUILD)/samd51-pic: setup
	mkdir -p $(BUILD)/samd51-pic
	cd $(BUILD)/samd51-pic && cmake -DTARGET_ARCH=samd51 -DFK_TARGET_PIC=ON ../../

oversized:
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51 && cmake -DTARGET_ARCH=samd51 -DFK_OVERSIZED=ON ../../
	mkdir -p $(BUILD)/samd51-pic
	cd $(BUILD)/samd51-pic && cmake -DTARGET_ARCH=samd51 -DFK_OVERSIZED=ON -DFK_TARGET_PIC=ON ../../

$(BUILD)/amd64: setup
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../

$(BUILD)/samd09: setup
	mkdir -p $(BUILD)/samd09
	cd $(BUILD)/samd09 && cmake -DTARGET_ARCH=samd09 ../../

samd51: $(BUILD)/samd51 $(BUILD)/samd51-pic
	cd $(BUILD)/samd51 && $(MAKE)

samd51-pic: $(BUILD)/samd51 $(BUILD)/samd51-pic
	cd $(BUILD)/samd51-pic && $(MAKE)

samd09: $(BUILD)/samd09
	cd $(BUILD)/samd09 && $(MAKE)

amd64: $(BUILD)/amd64 tests/hosted/dns_packets.h
	cd $(BUILD)/amd64 && $(MAKE)

fw: samd51 samd09

tests/hosted/dns_packets.h: tools/write_test_packets.py
	tools/write_test_packets.py > tests/hosted/dns_packets.h

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

fk/secrets.cpp: fk/secrets.cpp.template
	cp $^ $@

fk/data/adjectives.h: fk/data/adjectives.txt
	cd fk/data && ./generate.py

fk/data/animals.h: fk/data/animals.txt
	cd fk/data && ./generate.py

package: fw
	mkdir -p $(BUILD)/$(PACKAGE)
	cp tools/flash-* $(BUILD)/$(PACKAGE)
	cp tools/jlink-* $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd51/bootloader/fkbl.elf $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd51/bootloader/fkbl-fkb.bin $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd51/fk/fk-bundled-fkb.elf $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd51/fk/fk-bundled-fkb.bin $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd09/modules/weather/sidecar/fk-weather-sidecar*.elf $(BUILD)/$(PACKAGE)
	cp $(BUILD)/samd09/modules/weather/sidecar/fk-weather-sidecar*.bin $(BUILD)/$(PACKAGE)
	chmod 644 $(BUILD)/$(PACKAGE)/*
	chmod 755 $(BUILD)/$(PACKAGE)/flash-*
	chmod 755 $(BUILD)/$(PACKAGE)/jlink-*
	cd $(BUILD) && zip -r $(PACKAGE).zip $(PACKAGE)
	cp $(BUILD)/$(PACKAGE).zip $(BUILD)/fk-firmware.zip

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
		git subtree add --prefix libraries/$(l) https://github.com/$(l).git master --squash;   \
	done

deps-update:
	+@for l in $(LIBRARY_REPOSITORIES); do                                                     \
		git subtree pull --prefix libraries/$(l) https://github.com/$(l).git master --squash;  \
	done

veryclean: clean
	rm -rf bootloader/dependencies.cmake libraries/dependencies.cmake libraries/done
	rm -rf modules/weather/sidecar/dependencies.cmake modules/weather/main/dependencies.cmake
	@for l in $(LOCAL_LIBRARY_PATHS); do                                                       \
		echo rm -rf $$l; rm -rf $$l;                                                           \
	done

cppcheck:
	rm -rf $(BUILD)/cpp-check
	mkdir $(BUILD)/cpp-check
	cd fk && cppcheck -q -j8 --enable=all --inline-suppr --force --output-file=$(BUILD)/cppcheck.xml --xml --xml-version=2 .
	cd fk && cppcheck-htmlreport --source-dir=. --title=fk --file=$(BUILD)/cppcheck.xml --report-dir=$(BUILD)/cpp-check

info:
	+@for m in build/samd51/fk/*.map; do                                                       \
    echo $$m.cpp;                                                                              \
    cat $$m | c++filt > $$m.cpp;                                                               \
	done
	+@for m in build/samd51-pic/fk/*.map; do                                                   \
    echo $$m.cpp;                                                                              \
    cat $$m | c++filt > $$m.cpp;                                                               \
	done

debug-tests: amd64
	gdb build/amd64/tests/hosted/testall

clean:
	rm -rf $(BUILD)

.PHONY: dependencies doc cmake
