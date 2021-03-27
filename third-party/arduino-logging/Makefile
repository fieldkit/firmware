BUILD ?= $(abspath build)

all: build test

cmake:
	mkdir -p $(BUILD)
	cd $(BUILD) && cmake ../

build: cmake
	$(MAKE) -C $(BUILD)

test: build
	env GTEST_COLOR=1 $(MAKE) -C $(BUILD) testcommon test ARGS=-VV

clean:
	rm -rf $(BUILD)

veryclean:
	rm -rf $(BUILD) gitdeps

.PHONY: build cmake
