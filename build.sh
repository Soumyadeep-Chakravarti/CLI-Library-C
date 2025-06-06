#!/bin/sh

set -e

BUILD_DIR="build"

if [ "$1" = "clean" ]; then
    echo "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    exit 0
fi

echo "Generating build files with CMake..."
cmake -B "$BUILD_DIR" -G Ninja

echo "Building project..."
cmake --build "$BUILD_DIR"

echo "Build successful. Running binary..."
exec "./$BUILD_DIR/hello_tui"
