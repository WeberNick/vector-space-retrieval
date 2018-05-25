#!/bin/sh

if [ "$1" = "-rm" ] || [ "$1" = "--rm" ]; then
  echo "> [Removing all previous node_modules]"
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

echo "> [Install] evsr-root"
cd ./evsr-web/
npm install

echo "> [Install] evsr-server"
cd ./server
npm install

echo "> [Install] evsr-client"
cd ../client
npm install

echo "> [Build] evsr-client"
npm run build

echo "> [Install] evsr-presentation"
cd ../presentation
npm install

echo "> [Build] evsr-presentation"
npm run build

cd ../

echo "> [Generate] esvr-web documentation"
npm run doc

cd ../

echo "> [Copy] evsr-binary"
cp ./bin/evsr_run ./evsr-web/server/evsr/bin/

echo "> [Finished] installing and setting up evsr-server"
echo "> Start by running ./start_server.sh"