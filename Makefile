#
#
#
BUILD ?= $(abspath build)
SHELL := /bin/bash
OFFLINE := /bin/false
BUILD_NUMBER ?= none

default: setup all

all: samd51 samd09 test

checks: amd64
	valgrind $(BUILD)/amd64/tests/hosted/testall

ci: veryclean setup samd51 samd09 package

gitdeps:
	echo noop

setup: .python-setup fk/secrets.h fk/secrets.cpp fk/data/animals.h fk/data/adjectives.h dependencies

.python-setup:
	pip3 install -U wheel
	pip3 install -U sphinx pyelftools pyblake2
	pip3 install --index-url https://lief.quarkslab.com/packages lief==0.12.0.dev0
	touch .python-setup

cmake: $(BUILD)/samd51 $(BUILD)/samd09 $(BUILD)/amd64

$(BUILD)/samd51: setup
	mkdir -p $(BUILD)/samd51
	cd $(BUILD)/samd51      && cmake -DTARGET_ARCH=samd51 ../../

$(BUILD)/amd64: setup
	mkdir -p $(BUILD)/amd64
	cd $(BUILD)/amd64 && cmake -DTARGET_ARCH=amd64 ../../

$(BUILD)/samd09: setup
	mkdir -p $(BUILD)/samd09
	cd $(BUILD)/samd09 && cmake -DTARGET_ARCH=samd09 ../../

samd51: $(BUILD)/samd51
	cd $(BUILD)/samd51 && $(MAKE)
	true || tools/stack-usage.py $(BUILD)/samd51 > doc/stack-usage.txt

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

package: samd51 samd09
	cd build/samd51/fk && $(MAKE) package

package-old: fw
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
	touch $(BUILD)/$(PACKAGE)/fk.cfg-disabled
	cd $(BUILD) && zip -r $(PACKAGE).zip $(PACKAGE)
	cp $(BUILD)/$(PACKAGE).zip $(BUILD)/fk-firmware.zip

veryclean: clean

cppcheck:
	rm -rf $(BUILD)/cpp-check
	mkdir $(BUILD)/cpp-check
	cd fk && cppcheck -q --enable=all --inline-suppr --force --output-file=$(BUILD)/cppcheck.xml --xml --xml-version=2 .
	cd fk && cppcheck-htmlreport --source-dir=. --title=fk --file=$(BUILD)/cppcheck.xml --report-dir=$(BUILD)/cpp-check

info:
	+@for m in build/samd51/fk/*.map; do                                                       \
    echo $$m.cpp;                                                                              \
    cat $$m | c++filt > $$m.cpp;                                                               \
	done

debug-tests: amd64
	gdb build/amd64/tests/hosted/testall

module-test:
	cd build/samd51/modules/dynamic/main && $(MAKE) clean > /dev/null
	cd build/samd51/modules/dynamic/main && $(MAKE)
	readelf --sections --relocs build/samd51/modules/dynamic/main/fkdynamic-fkb.elf
	arm-none-eabi-objdump -dr build/samd51/modules/dynamic/main/fkdynamic-fkb.elf
	ls -alh build/samd51/modules/dynamic/main/*.bin
	xxd -i build/samd51/modules/dynamic/main/fkdynamic-fkb.bin > fk/modules/dyn/compiled.h
	sed -i 's/\[\] = /\[\] __attribute__((aligned(4))) =/g' fk/modules/dyn/compiled.h
	cd build/samd51/fk && $(MAKE)

options:
	cd $(BUILD)/samd51 && cmake ../../
	cd $(BUILD)/samd51 && cmake -LA ../../

enable-phylum:
	cd $(BUILD)/samd51 && cmake -DFK_PHYLUM=ON ../../

enable-phylum-amplify:
	cd $(BUILD)/samd51 && cmake -DFK_PHYLUM=ON -DFK_PHYLUM_AMPLIFICATION=10 ../../

clean:
	rm -rf $(BUILD)

.PHONY: dependencies doc cmake
