#!/bin/bash

wget https://sourceforge.net/projects/boost/files/boost/1.61.0/boost_1_61_0.tar.bz2
tar xjf boost_1_61_0.tar.bz2
cd boost_1_61_0/tools/build
./bootstrap.sh
./b2 install --prefix=$HOME
cd ../..
b2 --build-dir=/tmp toolset=gcc stage  #gcc for gcc toolchain


#Now we tell Cmake about the location of the boost libraries
