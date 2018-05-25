#!/bin/bash

echo "[Downloading] GloVe"
curl https://nlp.stanford.edu/data/glove.6B.zip > glove.6B.zip

unzip ./glove.6B.zip -d ./glove.6B

echo "> [Copy] glove.6B.300d.txt"
mkdir ./data/w2v
cp ./glove.6B/glove.6B.300d.txt ./data/w2v

mkdir ./evsr-web/server/evsr/data
cp ./glove.6B/glove.6B.300d.txt ./evsr-web/server/evsr/data

rm -rf ./glove.6B
rm -rf ./glove.6B.zip