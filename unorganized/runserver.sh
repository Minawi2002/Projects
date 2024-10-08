#!/bin/bash
killall Uno-server
killall Uno-client
cd cmake-build-debug/
cmake ..
make
cd ..
./cmake-build-debug/Uno-server &
