BUILD ?= $(abspath build)

default: all test

$(BUILD):
	mkdir -p $(BUILD)

all: $(BUILD)
	cd $(BUILD) && cmake ../
	$(MAKE) -C $(BUILD)

test: all
	env GTEST_COLOR=1 $(MAKE) -C $(BUILD) testcommon test ARGS=-VV

doc: all
	make -C $(BUILD) doc

clean:
	rm -rf $(BUILD)

veryclean: clean
	rm -rf gitdeps

.PHONY: doc
