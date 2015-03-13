# -*- coding: utf-8 -*-
"""
Created on Sat May  3 16:06:41 2014

@author: proto
"""

from os import listdir
import os.path
import shutil
import json
import argparse
import os

def defineConsole():
    parser = argparse.ArgumentParser(description='SBMLparser file copy script')
    parser.add_argument('-c','--copy',action='store_true',help='copy files from the main distro')
    parser.add_argument('-r','--remove',action='store_true',help='remove files from this directory')
    return parser    


fileList = []
def resource_path(relative_path):
    base_path = os.path.abspath(".")
    convertedPath =  os.path.normpath(os.path.join(base_path, relative_path))
    return convertedPath


def clean_directory():
    with open('fileList.json','r') as f:
        onlyfiles = json.load(f)
    for fileName in onlyfiles:
        try:
            os.remove(fileName)
        except OSError:
            continue


def update_directory():
    srcDirectory = resource_path(os.path.join('..','..','parsers','SBMLparser'))  
    with open('fileList.json','r') as f:
        onlyfiles = json.load(f)
    #onlyfiles = [ f for f in listdir('./' + directory) if os.path.isfile(os.path.join('./' + directory,f)) ]
    configFiles =  [ f for f in listdir(os.path.join('.','config')) if os.path.isfile(os.path.join('.','config',f)) ]   
    #onlyNotFiles =[ f for f in listdir('./' + directory) if not isfile(join('./' + directory,f)) ]
    originalFiles = listdir(srcDirectory)
    for element in onlyfiles:
        if element in originalFiles:
            try:
                shutil.copy(os.path.join(srcDirectory,  element), '.')
            except:

                continue
    for element in configFiles:
        shutil.copy(os.path.join(srcDirectory,'config',element), os.path.join('.','config'))
    
if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    if namespace.copy:
        update_directory()
    elif namespace.remove:
        clean_directory()
