#!/usr/bin/env bash

antlr4_Cpp_runtime_dir="./antlr4/runtime/Cpp"
build_dir="build"
run_dir="run"

cd "$antlr4_Cpp_runtime_dir"

if [ ! -d "$build_dir" ]; then
    mkdir "$build_dir"
fi

if [ ! -d "$run_dir" ]; then
    mkdir "$run_dir"
fi

cd "$build_dir"

cmake .. -DCMAKE_POSITION_INDEPENDENT_CODE=ON
make
DESTDIR=../"$run_dir" make install
