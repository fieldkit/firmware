default:
	mkdir -p build
	cd build && cmake ..
	cd build && $(MAKE)

test: default
	env GTEST_COLOR=1 valgrind build/tests/testall -VV

example: default
	env GTEST_COLOR=1 valgrind build/example/example

clean:
	rm -rf build
