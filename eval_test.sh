#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..

##### Evaluate different random projections dimensions ##### 
echo "> [Eval] Random Projections with dimension 500"
./bin/evsr_run --dimensions 500 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --rand

echo "> [Eval] Random Projections with dimension 1000"
./bin/evsr_run --dimensions 1000 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --rand

echo "> [Eval] Random Projections with dimension 2500"
./bin/evsr_run --dimensions 2500 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --rand

echo "> [Eval] Random Projections with dimension 5000"
./bin/evsr_run --dimensions 5000 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --rand

##### Evaluate different random projections dimensions ##### 
echo "> [Eval] Tiered Index with tiers 5"
./bin/evsr_run --tiers 5 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --tiered

echo "> [Eval] Tiered Index with tiers 10"
./bin/evsr_run --tiers 10 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --tiered

echo "> [Eval] Tiered Index with tiers 25"
./bin/evsr_run --tiers 25 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --tiered

echo "> [Eval] Tiered Index with tiers 50"
./bin/evsr_run --tiers 50 --collection-path ./testdata/d-collection.docs --query-path ./testdata/ --scores-path ./testdata/s-3.qrel --stopword-path ./testdata/stopwords.large --word-embeddings-path ./testdata/w2v/glove.6B.300d.txt --tiered

