#!/bin/bash
cd bng-graph
g++ -c BNGcore/*.cpp -I./BNGcore -I./nauty -I./nauty/nauty24
