#!/usr/bin/env bash
g++ -std=c++17 -O2 "$1" -o main && ./main < in.txt > out.txt
