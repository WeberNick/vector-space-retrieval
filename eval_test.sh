#!/bin/sh
echo "> [Build]"
cd build/
make
echo
cd ..

#echo "> [Eval] Random Projections with dimension 2500"
#./bin/evsr_run --rand --dimensions 2500 

#echo "> [Eval] Random Projections with dimension 5000"
#./bin/evsr_run --rand --dimensions 5000 

#echo "> [Eval] Random Projections with dimension 10000"
#./bin/evsr_run --rand --dimensions 10000 

####### Evaluate different tiers for tiered index ##### 
#echo "> [Eval] Tiered Index with Tiers 100"
#./bin/evsr_run --tiered --tiers 100 

#echo "> [Eval] Tiered Index with Tiers 250"
#./bin/evsr_run --tiered --tiers 250

####### Evaluate Combination ##### 
#echo "> [Eval] Tiers: 100, Dimensions: 5000"
#./bin/evsr_run --tiers 100 --dimensions 5000

#echo "> [Eval] Tiers: 250, Dimensions: 5000"
#./bin/evsr_run --tiers 250 --dimensions 5000

#echo "> [Eval] Tiers: 100, Dimensions: 10000"
#./bin/evsr_run --tiers 100 --dimensions 10000

#echo "> [Eval] Tiers: 250, Dimensions: 10000"
#./bin/evsr_run --tiers 250 --dimensions 10000


echo "> [Eval] Tiers: 250, Dimensions: 5000, Seed: 87693"
./bin/evsr_run --tiers 250 --dimensions 5000 --seed 87693
