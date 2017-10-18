#! /bin/bash
# Grab, build and localally install luasocket
# for remote debugging.

DESTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"/build/lualib

git clone https://github.com/diegonehab/luasocket.git

cd luasocket

mkdir -p results

make LUAV=5.2 &&
make LUAV=5.2 prefix=$(realpath results) install

mkdir -vp $DESTDIR

cp -vr results/lib/lua/5.2/* $DESTDIR
cp -vr results/share/lua/5.2/* $DESTDIR
