#!/bin/bash
# init.sh: iniitalize scripts for development and testing
export PATH_SCRIPT="$(realpath ./script)"
export PATH_BUILD="$(realpath ./build)"
export PATH_ROOT="$(pwd)"

e () { $PATH_SCRIPT/e.sh; }
t () { $PATH_SCRIPT/t.sh; }
v () { $PATH_SCRIPT/v.sh; }
