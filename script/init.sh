#!/bin/bash
# init.sh: iniitalize scripts for development and testing
export PATH_SCRIPT="$(realpath ./script)"
export PATH_BUILD_GUI="$(realpath ./build_gui)"
export PATH_BUILD_NOGUI="$(realpath ./build_nogui)"
export PATH_ROOT="$(pwd)"


tg () { make gui -C $PATH_ROOT && $PATH_BUILD_GUI/traffic_sim; }
tng () { make nogui -C $PATH_ROOT && $PATH_BUILD_NOGUI/traffic_sim; }
