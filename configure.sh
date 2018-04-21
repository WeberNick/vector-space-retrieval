#!/bin/bash

#================================================================
# HEADER
#================================================================
#% SYNOPSIS
#+    ${SCRIPT_NAME} [-h] [-c[compiler_path]]  [-cxx[compiler_path]] [-b[boost_path]] [-a]
#%
#% DESCRIPTION
#%    Setting up CMake, directory structure and compile everything to start the main evsr executable
#%
#% OPTIONS
#%    -c [path], --c [path]         Set a custom C compiler path
#%    -cxx [path], --cxx [path]     Set a custom C++ compiler path
#%    -b, --boost                   Set path to your boost library
#%    -a, --all                     Also delete external lib folders and clone again
#%
#% EXAMPLES
#%    ./${SCRIPT_NAME} -c /usr/local/Cellar/gcc/7.3.0_1/bin/gcc-7 -cxx /usr/local/Cellar/gcc/7.3.0_1/bin/g++-7 -b /usr/local/Cellar/boost/1.66.0/ -a
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
    -b|--boost)
    BOOST="$2"
    shift # past argument
    shift # past value
    ;;
     -a|--all)
    ALL="$2"
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

# Show help message
if [ ! -z ${HELP+x} ]
then
  usagefull
  exit 1
fi

# Delete directories
echo "> [Deleting]"
if [ -d ./build ];
then
  echo "Deleting ./build directory"
  rm -rf ./build
fi

if [ -d ./src/lib ];
then
  if [ ! -z ${ALL+x} ]
  then
    echo "Deleting./src/lib directory"
    rm -rf ./src/lib
  else
    echo "Not deleting ./src/lib directory - external libs are not cloned again!"
  fi

fi

if [ -d ./bin ];
then
  echo "Deleting ./bin directory"
  rm -rf ./bin
fi

if [ -d ./tests/lib ];
then
 if [ ! -z ${ALL+x} ]
  then
  echo "Deleting ./tests/lib directory"
  rm -rf ./tests/lib
  else
     echo "Not deleting ./tests/lib  directory - googletest is not cloned again!"
  fi
fi

# Create necessary directories
echo
echo "> [Creating] ./build directory"
mkdir ./build
cd ./build


# Build cmake command string out of the options
command="cmake "

if [ ! -z "$C" ]
then
  command="$command -DCMAKE_C_COMPILER=$C"
fi

if [ ! -z "$CXX" ]
then
  command="$command -DCMAKE_CXX_COMPILER=$CXX"
fi

if [ ! -z "$BOOST" ]
then
  command="BOOST=$BOOST $command"
fi

command="$command ../"

echo $command
echo

# Execute CMake
echo "> [Executing] CMake"
eval $command

cmake --build ./ --target all --config Debug
cd ../