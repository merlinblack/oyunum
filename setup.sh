#! /bin/bash

git submodule update --init

./getdata.sh

mkdir build
cd build

cmake ..

make

