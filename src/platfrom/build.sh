#!bin/bash

gcc -Wall -Wextra -o main main.c $(pkg-config --cflags --libs raylib)
echo "BUILT!"
