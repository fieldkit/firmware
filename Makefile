BUILD ?= $(abspath build)
SHELL := /bin/bash

default: setup all

setup:
	python3 --version || true
	python --version || true

$(BUILD):
	mkdir -p $(BUILD)

all: m0-fk m4-gc m4-fk

m4-gc: gitdeps
	mkdir -p build/m4-gc
	cd build/m4-gc && cmake -D TARGET_M4=ON -D TARGET_BOARD=grand_central_m4 ../../
	cd build/m4-gc && $(MAKE)

m4-fk: gitdeps
	mkdir -p build/m4-fk
	cd build/m4-fk && cmake -D TARGET_M4=ON -D TARGET_BOARD=fieldkit_core ../../
	cd build/m4-fk && $(MAKE)

m0-fk: gitdeps
	mkdir -p build/m0-fk
	cd build/m0-fk && cmake -D TARGET_M0=ON ../../
	cd build/m0-fk && $(MAKE)

gitdeps:
	simple-deps --config dependencies.sd

clean:
	rm -rf $(BUILD)
