#!/usr/bin/env bash
set -e
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)
echo "Build complete. Executables in build/:"
ls -1
