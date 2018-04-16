#!/bin/sh
echo "> [Build]"
cd build/
make
echo
echo "> [Evsr_Run]"
cd ..
./bin/evsr_run
