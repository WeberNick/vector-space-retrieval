#!/bin/bash
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
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters


if [ -d ./build ]; 
then
  echo "Deleting ./build directory"
  rm -rf ./build 
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