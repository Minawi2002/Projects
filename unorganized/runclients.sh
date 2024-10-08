#!/bin/bash
#if no number is given, default to 4 or if number is bigger than 4, default to 4 or if number is smaller than 1, default to 4
if [ -z "$1" ] || [ $1 -gt 4 ] || [ $1 -lt 1 ]
then
echo "No argument given, defaulting to 4 players"
for i in $(seq 1 4)
do
    ./cmake-build-debug/Uno-client &
done
#if number is given, run that many clients
else
for i in $(seq 1 $1)
do
    ./cmake-build-debug/Uno-client &
done
fi