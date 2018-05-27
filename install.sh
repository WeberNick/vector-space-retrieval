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


#== colors ==#

 RCol='\033[0m'    # Text Reset
# Regular             Bold                  Underline             High Intensity        BoldHigh Intens       Background            High Intensity Backgrounds
Bla='\033[0;30m';     BBla='\033[1;30m';    UBla='\033[4;30m';    IBla='\033[0;90m';    BIBla='\033[1;90m';   On_Bla='\033[40m';    On_IBla='\033[0;100m';
Red='\033[0;31m';     BRed='\033[1;31m';    URed='\033[4;31m';    IRed='\033[0;91m';    BIRed='\033[1;91m';   On_Red='\033[41m';    On_IRed='\033[0;101m';
Gre='\033[0;32m';     BGre='\033[1;32m';    UGre='\033[4;32m';    IGre='\033[0;92m';    BIGre='\033[1;92m';   On_Gre='\033[42m';    On_IGre='\033[0;102m';
Yel='\033[0;33m';     BYel='\033[1;33m';    UYel='\033[4;33m';    IYel='\033[0;93m';    BIYel='\033[1;93m';   On_Yel='\033[43m';    On_IYel='\033[0;103m';
Blu='\033[0;34m';     BBlu='\033[1;34m';    UBlu='\033[4;34m';    IBlu='\033[0;94m';    BIBlu='\033[1;94m';   On_Blu='\033[44m';    On_IBlu='\033[0;104m';
Pur='\033[0;35m';     BPur='\033[1;35m';    UPur='\033[4;35m';    IPur='\033[0;95m';    BIPur='\033[1;95m';   On_Pur='\033[45m';    On_IPur='\033[0;105m';
Cya='\033[0;36m';     BCya='\033[1;36m';    UCya='\033[4;36m';    ICya='\033[0;96m';    BICya='\033[1;96m';   On_Cya='\033[46m';    On_ICya='\033[0;106m';
Whi='\033[0;37m';     BWhi='\033[1;37m';    UWhi='\033[4;37m';    IWhi='\033[0;97m';    BIWhi='\033[1;97m';   On_Whi='\033[47m';    On_IWhi='\033[0;107m';


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
if [ -d ./build ];
then
  echo -e "${Blu}> [Deleting] ${RCol} ./build"
  rm -rf ./build
fi

if [ -d ./src/lib ];
then
  if [ ! -z ${ALL+x} ]
  then
    echo -e "${Blu}> [Deleting] ${RCol} ./src/lib"
    rm -rf ./src/lib
    mkdir ./src/lib
  else
    echo -e "${Yel}> [NOT Deleting] ${RCol} ./src/lib - external libs are not cloned again!"
  fi

fi

if [ -d ./bin ];
then
  echo -e "${Blu}> [Deleting] ${RCol} ./bin"
  rm -rf ./bin
fi

if [ -d ./tests/lib ];
then
 if [ ! -z ${ALL+x} ];
  then
  echo -e "${Blu}> [Deleting] ${RCol} ./tests/lib"
  rm -rf ./tests/lib
  else
    echo -e "${Yel}> [NOT Deleting] ${RCol} ./tests/lib - googletest is not cloned again!"
  fi
fi

# Create necessary directories
echo
echo -e "${Blu}> [Creating] ${RCol} ./build"
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

# Execute CMake
echo -e "${Blu}> [Executing] ${RCol} cmake"
echo
echo $command
echo
eval $command

cmake --build ./ --target all --config Debug
cd ../

cd ./build/

echo -e "${Blu}> [Generating] ${RCol} evsr-binary documentation"
make doc 
cd ../


if ! [ -x "$(command -v node)" ]; then
  echo -e "${Red}> [Error] ${RCol} Node seems not be installed - not installing the web-interface"
  echo -e "${Red}> [Error] ${RCol} If you installed Node or want to try to only install the server interface - run ./server_install.sh"
else
    ./server_install.sh
fi


if ! [ -x "$(command -v curl)" ] && ! [ -x "$(command -v unzip)" ] && ! [ -f ./data/w2v/glove.6B.300d.txt ]; then
  echo -e "${Red}> [Error] ${RCol} curl and unzip need to be installed when you want to download the GloVe data"
else
    if ! [ -f ./data/w2v/glove.6B.300d.txt ] || ! [ -f ./evsr-web/server/evsr/data/glove.6B.300d.txt ]; then
      ./download_glove.sh
    else
      echo -e "${Yel}> [Info] ${RCol} w2v files are already existing - not downloading them again!"
    fi
fi

echo -e "${Gre}> [Finish] ${RCol} everything should be installed now"