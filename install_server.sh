echo "[Install] evsr-root"
cd ./evsr-web/
npm install

echo "[Install] evsr-server"
cd ./server
npm install

echo "[Install] evsr-client"
cd ../client
npm install

echo "[Build] evsr-client"
npm run build

echo "[Install] evsr-presentation"
cd ../presentation
npm install

echo "[Build] evsr-presentation"
npm run build

cd ../../

echo "[Copy] evsr-binary"

cp ./bin/evsr_run ./evsr-web/server/evsr/bin/