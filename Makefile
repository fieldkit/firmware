BUILD ?= $(abspath build)

default: m4 linux

m0:
	mkdir -p $(BUILD)/m0
	cd $(BUILD)/m0 && cmake -D TARGET_M0=ON ../../
	$(MAKE) -C $(BUILD)/m0

m4:
	mkdir -p $(BUILD)/m4
	cd $(BUILD)/m4 && cmake -D TARGET_M4=ON ../../
	$(MAKE) -C $(BUILD)/m4

linux:
	mkdir -p $(BUILD)/linux
	cd $(BUILD)/linux && cmake -D TARGET_LINUX=ON -DCMAKE_BUILD_TYPE=Debug ../../
	$(MAKE) -C $(BUILD)/linux

test: linux
	env GTEST_COLOR=1 $(MAKE) -C $(BUILD)/linux test ARGS=-VV

clean:
	rm -rf $(BUILD)
