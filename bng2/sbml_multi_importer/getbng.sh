#!/bin/bash
for i in $(seq 12)
do
	python sbml_multi_importer.py "bng_tests/Model${i}_sbml_sbmlmulti.xml" 
done
