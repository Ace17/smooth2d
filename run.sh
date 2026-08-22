#!/usr/bin/env bash
set -euo pipefail

g++ main.cpp `pkg-config sdl2 SDL2_gfx --cflags --libs` -o smooth
./smooth | tee stats.csv
