#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..

###### Evaluate different random projections dimensions ##### 
#echo "> [Eval] Random Projections with dimension 500"
#./bin/evsr_run --dimensions 500 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --rand

#echo "> [Eval] Random Projections with dimension 1000"
#./bin/evsr_run --dimensions 1000 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --rand

#echo "> [Eval] Random Projections with dimension 2500"
#./bin/evsr_run --dimensions 2500 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --rand

#echo "> [Eval] Random Projections with dimension 5000"
#./bin/evsr_run --dimensions 5000 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --rand

###### Evaluate different random projections dimensions ##### 
#echo "> [Eval] Tiered Index with tiers 5"
#./bin/evsr_run --tiers 5 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --tiered

#echo "> [Eval] Tiered Index with tiers 10"
#./bin/evsr_run --tiers 10 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --tiered

#echo "> [Eval] Tiered Index with tiers 25"
#./bin/evsr_run --tiers 25 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --tiered

#echo "> [Eval] Tiered Index with tiers 50"
#./bin/evsr_run --tiers 50 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --tiered

#echo "> [Eval] Tiered Index with tiers 50"
./bin/evsr_run --tiers 100 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt --tiered

##### Evaluate different random projections dimensions ##### 
echo "> [Eval] Tier 50, Dim 1000"
./bin/evsr_run --tiers 50 --dimensions 1000 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt

echo "> [Eval] Tier 50, Dim 1000"
./bin/evsr_run --tiers 50 --dimensions 2500 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt

echo "> [Eval] Tier 100, Dim 1000"
./bin/evsr_run --tiers 100 --dimensions 1000 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt

echo "> [Eval] Tier 100, Dim 2500"
./bin/evsr_run --tiers 1000 --dimensions 2500 --collection-path ./data/d-collection.docs --query-path ./data/ --scores-path ./data/s-3.qrel --stopword-path ./data/stopwords.large --word-embeddings-path ./data/w2v/glove.6B.300d.txt

