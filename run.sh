#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..
if [ "$1" = "-t" ] || [ "$1" = "--test" ]; then
  echo "> [Unit_Tests_run]"
  ./bin/Unit_Tests_run
else
  echo "> [Evsr_run]"
  ./bin/evsr_run --dimensions 1000 --tiers 10 --collection-path ./data/d-collection.docs --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --trace --server --seed 123
fi
