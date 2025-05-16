#!/bin/bash

test_dir=test
type=Release

cd $test_dir

export GTEST_COLOR=1

if [ ! -d build/$type ]; then
	cmake -B build/$type -DCMAKE_BUILD_TYPE=$type
fi

cmake --build build/$type

ctest --test-dir build/$type -V
