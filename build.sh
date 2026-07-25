#!/bin/bash

MODE=$1

if [ -z "$MODE" ]; then
    echo "Building in DEFAULT mode..."
    gcc -std=c2x -Wall -Wextra -o main src/platform/main.c $(pkg-config --libs --cflags raylib)
else
    echo "Building in $MODE mode..."
    gcc -std=c2x -Wall -Wextra -D$MODE -o main src/platform/main.c $(pkg-config --libs --cflags raylib)
fi

echo "Build complete."
