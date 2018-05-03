#!/bin/bash
for i in $(seq 12)
do
	perl ../../BNG2.pl --sbmlmulti "Model${i}.bngl"
done
