#!/bin/bash

mkdir build
cd build
cmake ..
make

cp py/gamo/core/_core/gamo_core_py.cpython-* ../py/gamo/core/gamo_core_py.so
