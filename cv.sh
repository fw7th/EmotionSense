#!/bin/bash

# Check if a file was passed
if [ -z "$1" ]; then
    echo "❌ Usage: ./cvbuild.sh yourfile.cpp"
    exit 1
fi

# Compile and run
g++ -std=c++17 "$1" -o "${1%.cpp}" $(pkg-config --cflags --libs opencv4) && ./"${1%.cpp}"
