#!/bin/bash
for i in $(seq 13)
do
	python sbml_multi_importer.py "testSuite/Model${i}_sbml_sbmlmulti.xml" 
done
