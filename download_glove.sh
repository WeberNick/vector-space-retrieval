#!/bin/bash

#== colors ==#

RCol='\033[0m'    # Text Reset
# Regular
Blu='\033[0;34m';
Red='\033[0;31m'; 
Gre='\033[0;32m'; 

echo -e "${Blu}> [Downloading] ${RCol} GloVe"
curl https://nlp.stanford.edu/data/glove.6B.zip > glove.6B.zip

unzip ./glove.6B.zip -d ./glove.6B

echo -e "${Blu}> [Copy] ${RCol} glove.6B.300d.txt"
mkdir ./data/w2v
cp ./glove.6B/glove.6B.300d.txt ./data/w2v

mkdir ./evsr-web/server/evsr/data
cp ./glove.6B/glove.6B.300d.txt ./evsr-web/server/evsr/data

echo -e "${Blu}> [Deleting] ${RCol} .zip and unziped directory"
rm -rf ./glove.6B
rm -rf ./glove.6B.zip

echo -e "${Gre}> [Finish] ${RCol} downloading and copying GloVe"