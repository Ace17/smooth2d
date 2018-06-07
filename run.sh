#!/usr/bin/env bash
set -euo pipefail

g++ main.cpp `pkg-config sdl2 --cflags --libs` -o smooth
./smooth
