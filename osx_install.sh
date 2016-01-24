#!/bin/bash
if [ "${TRAVIS_OS_NAME}" = "linux" ]; then
    echo "This should say linux"
    echo $TRAVIS_OS_NAME
    uname -a
    
else
    echo  "This should say osx"
    echo $TRAVIS_OS_NAME
    uname -a
    
    brew update
    brew install python
    brew tap homebrew/science
    brew install libxml2
    brew install libzip
    brew install libsbml
    brew link --overwrite python
    /usr/local/bin/pip install numpy
fi
