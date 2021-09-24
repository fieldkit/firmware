#!/bin/bash

set -xe

function try() {
	valgrind build/tests/testall --gtest_filter="ReadFixture/2.ReadDataChain_TwoBlocks_WithAttributes" 2>&1 --verbose
	#valgrind build/tests/testall --gtest_filter="*Indexed*0*Large*Records*End" 2>&1 --verbose
}

function strip() {
	sed -r 's/\x1B\[([0-9]{1,3}(;[0-9]{1,2})?)?[mGK]//g'
}

make -j4

try | strip > broken

git stash

make -j4

try | strip > working

git stash pop

make -j4
