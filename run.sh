#!/bin/sh

#== colors ==#

RCol='\033[0m'    # Text Reset
# Regular
Blu='\033[0;34m';
Red='\033[0;31m'; 
Gre='\033[0;32m'; 


echo -e "${Blu}> [Build] ${RCol}"
cd build/
make
echo
cd ..
if [ "$1" = "-t" ] || [ "$1" = "--test" ]; then
  echo -e "${Gre}> [Unit_Tests_run] ${RCol}"
  ./bin/Unit_Tests_run
else
  echo -e "${Gre}> [evsr_run] ${RCol} with pre-defined parameters: --dimensions 5000 --tiers 100 --collection-path ./data/d-collection.docs --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --trace --seed 87693 "
  ./bin/evsr_run --dimensions 5000 --tiers 100 --collection-path ./data/d-collection.docs --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --trace --seed 87693
fi
