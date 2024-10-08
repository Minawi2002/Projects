#!/bin/bash

#download stuff we need
sudo apt-get update
sudo apt-get install build-essential
sudo apt install cmake
sudo apt-get install libwxgtk3.0-gtk3-dev
sudo apt install osspd

mkdir cmake-build-debug
cd cmake-build-debug
cmake ..
make

cd ..

echo everything is built yay