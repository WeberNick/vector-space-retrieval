#!/bin/bash

#== colors ==#

RCol='\033[0m'    # Text Reset
# Regular
Blu='\033[0;34m';
Red='\033[0;31m'; 
Gre='\033[0;32m'; 



echo -e "${Blu}> [Installing] ${RCol} Server interface "
if [ "$1" = "-rm" ] || [ "$1" = "--rm" ]; then
  echo -e "${Blu}> [Deleting] ${RCol} all previous node_modules"
  cd ./evsr-web/
  rm -rf node_modules

  cd ./server
  rm -rf node_modules

  cd ../client
  rm -rf node_modules

  cd ../presentation
  rm -rf node_modules

  cd ../../
fi

echo -e "${Blu}> [Install] ${RCol} evsr-web root"
cd ./evsr-web/
npm install

echo -e "${Blu}> [Install] ${RCol} evsr-server"
cd ./server
npm install

echo -e "${Blu}> [Install] ${RCol} evsr-client"
cd ../client
npm install

echo -e "${Blu}> [Build] ${RCol} evsr-client"
npm run build

echo -e "${Blu}> [Install] ${RCol} evsr-presentation"
cd ../presentation
npm install

echo -e "${Blu}> [Build] ${RCol} evsr-presentation"
npm run build

cd ../

echo -e "${Blu}> [Generate] ${RCol} evsr-web documentation"
npm run doc

cd ../

echo -e "${Blu}> [Copy] ${RCol} evsr-binary"
mkdir ./evsr-web/server/evsr/bin
cp ./bin/evsr_run ./evsr-web/server/evsr/bin/

echo -e "${Blu}> [Info] ${RCol} start server by running ./server_start.sh"
echo -e "${Gre}> [Finish] ${RCol} installing and setting up evsr-server"
