#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..
if [ "$1" = "-t" ] || [ "$1" = "--test" ]; then
echo "> [Evsr_Run]"
./bin/Unit_Tests_run
else
echo "> [Unit_Tests_run]"
./bin/evsr_run
fi