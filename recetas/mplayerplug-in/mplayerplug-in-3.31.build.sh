#!/bin/sh 

## UNPACK
tar xvf mplayerplug-in-3.31.tar.gz
cd mplayerplug-in

## CONFIG
./configure --prefix=/usr || exit -1

## BUILD
make || exit -1

## PKG
mkdir dest
make DESTDIR=$(pwd)/dest install
cd dest
tar cjvfp ../../mplayerplug-in-3.31.tar.bz2 *
cd ..

## CLEANUP
cd ..
rm -rf mplayerplug-in
