#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..
if [ "$1" = "-t" ] || [ "$1" = "--test" ]; then
  echo "> [Unit_Tests_run]"
  ./bin/Unit_Tests_run
else
  echo "> [Evsr_run]"
  ./bin/evsr_run
fi
