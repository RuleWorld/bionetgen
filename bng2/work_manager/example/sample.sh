#!/bin/bash 

# Example model 
model='fceri_ji.net' 

# Echo command 
echo ../bin/run -m $model -t 100 -n 100 -q 25 -p ssa

# Execute 100 parallel SSA runs for 1,000 simulation time and 100 output steps   
../bin/run -m $model -t 1000 -n 100 -p ssa -q 100

#../bin/run -m $model -t 1000 -n 100 -p cvode -q 100
#../bin/run -m $model -t 1000 -n 100 -p ssa -q 100 -log kp1 1e-8 1e-6
#../bin/run -m $model -t 1000 -n 100 -p cvode -q 100 -log kp1 1e-8 1e-6
#../bin/run -m $model -t 1000 -n 100 -p ssa -q 100 -lin kp1 1e-8 1e-6
#../bin/run -m $model -t 1000 -n 100 -p cvode -q 100 -lin kp1 1e-8 1e-6
#../bin/run -m $model -t 1000 -n 100 -p ssa -q 100 -lgauss kp1 -7 2
#../bin/run -m $model -t 1000 -n 100 -p cvode -q 100 -lin kp1 -7 2
