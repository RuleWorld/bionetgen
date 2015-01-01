# -*- coding: utf-8 -*-
"""
Created on Sat May  3 16:06:41 2014

@author: proto
"""

from os import listdir
import os.path
import shutil

def resource_path(relative_path):
    base_path = os.path.abspath(".")
    convertedPath =  os.path.normpath(os.path.join(base_path, relative_path))
    return convertedPath

def main():
    directory = '.'
    srcDirectory = resource_path('../../parsers/SBMLparser')  
    onlyfiles = [ f for f in listdir('./' + directory) if os.path.isfile(os.path.join('./' + directory,f)) ]
    configFiles =  [ f for f in listdir('./config') if os.path.isfile(os.path.join('./config',f)) ]   
    #onlyNotFiles =[ f for f in listdir('./' + directory) if not isfile(join('./' + directory,f)) ]
    originalFiles = listdir(srcDirectory)
    for element in onlyfiles:
        if element in originalFiles:
            try:
                shutil.copy(os.path.join(srcDirectory + element), '.')
            except:
                continue
    for element in configFiles:
        shutil.copy(os.path.join(srcDirectory,'config',element), './config')
    
if __name__ == "__main__":
    main()