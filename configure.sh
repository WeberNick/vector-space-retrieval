#!/bin/bash

#================================================================
# HEADER
#================================================================
#% SYNOPSIS
#+    ${SCRIPT_NAME} [-h] [-c[compiler_path]]  [-cxx[compiler_path]]
#%
#% DESCRIPTION
#%    Setting up CMake, directory structure and compile everything to start the main evsr executable
#%
#% OPTIONS
#%    -c [path], --c [path]         Set a custom C compiler path
#%    -cxx [path], --cxx [path]     Set a custom C++ compiler path
#%
#% EXAMPLES
#%    ${SCRIPT_NAME} -c "/usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7 -cxx /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7
#%
#================================================================


#== needed variables ==#
SCRIPT_HEADSIZE=$(head -200 ${0} |grep -n "^# END_OF_HEADER" | cut -f1 -d:)
SCRIPT_NAME="$(basename ${0})"

  #== usage functions ==#
usage() { printf "Usage: "; head -${SCRIPT_HEADSIZE:-99} ${0} | grep -e "^#+" | sed -e "s/^#+[ ]*//g" -e "s/\${SCRIPT_NAME}/${SCRIPT_NAME}/g" ; }
usagefull() { head -${SCRIPT_HEADSIZE:-99} ${0} | grep -e "^#[%+-]" | sed -e "s/^#[%+-]//g" -e "s/\${SCRIPT_NAME}/${SCRIPT_NAME}/g" ; }

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -c|--c)
    C="$2"
    shift # past argument
    shift # past value
    ;;
    -cxx|--cxx)
    CXX="$2"
    shift # past argument
    shift # past value
    ;;
    -h|--help)
    HELP="$2"
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters


if [ ! -z ${HELP+x} ]
then 
  usagefull
  exit 1
fi


if [ -d ./build ]; 
then
  echo "Deleting ./build directory"
  rm -rf ./build 
fi

if [ -d ./src/lib ];
then
  echo "Deleting ./src/lib directory"
  rm -rf ./src/lib
fi

if [ -d ./bin ]; 
then
  echo "Deleting ./bin directory"
  rm -rf ./bin 
fi 

if [ -d ./tests/lib ];
then
  echo "Deleting ./tests/lib directory"
  rm -rf ./tests/lib
fi

echo "Creating ./build directory"
mkdir ./build
cd ./build

echo "Executing CMake"

if [ -z ${C+x} ] && [ -z ${CXX+x} ]
then
  echo "No compiler flags set, use standard or the ones declared in ./CMakeLists.txt"
  cmake ../
else 
  if [ ! -z ${C+x} ] && [ ! -z ${CXX+x} ]
  then
    cmake -DCMAKE_CXX_COMPILER=$CXX -DCMAKE_C_COMPILER=$C ../
  else 
    if [ ! -z "$C" ]
    then 
      cmake -DCMAKE_C_COMPILER=$C ../
    else
      if [ ! -z "$CXX" ]
      then
        cmake -DCMAKE_CXX_COMPILER=$CXX ../
      fi
    fi
  fi  
fi

cmake --build ./ --target all --config Debug 
cd ../