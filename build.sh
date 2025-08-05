#!/bin/bash

# build.sh

# Exit on any error
set -e

# Create a build directory if it doesn't exist
mkdir -p build

# Configure the project using CMake and Ninja
cmake -S . -B build -G "Ninja"

# Build the project
ninja -C build