#! /bin/bash

git submodule update --init

mkdir build
cd build

cmake ..

make

