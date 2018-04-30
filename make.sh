#!/bin/sh
cd build/
if [ "$1" = "clean" ]; then
echo "> [Clean]"
make clean
else
echo "> [Build]"
make
fi
