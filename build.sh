#!/bin/bash

MODE=$1
VERSION=$2

if [ -z "$MODE" ]; then
    echo "Building in DEFAULT mode..."
    gcc -std=c2x -Wall -Wextra -D$VERSION -o main platform/main.c $(pkg-config --libs --cflags raylib)
else
    echo "Building in $MODE mode..."
    gcc -std=c2x -Wall -Wextra -D$VERSION -D$MODE -o main platform/main.c $(pkg-config --libs --cflags raylib)
fi

echo "Build complete."
