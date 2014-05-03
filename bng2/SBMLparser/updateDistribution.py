# -*- coding: utf-8 -*-
"""
Created on Sat May  3 16:06:41 2014

@author: proto
"""

from os import listdir
from os.path import isfile, join

import shutil
def main():
    directory = '.'
    onlyfiles = [ f for f in listdir('./' + directory) if isfile(join('./' + directory,f)) ]
    configFiles =  [ f for f in listdir('./config') if isfile(join('./config',f)) ]   
    #onlyNotFiles =[ f for f in listdir('./' + directory) if not isfile(join('./' + directory,f)) ]
    originalFiles = listdir('../../parsers/SBMLparser')         
    for element in onlyfiles:
        if element in originalFiles:
            try:
                shutil.copy('../../parsers/SBMLparser/' + element, '.')
            except:
                continue
    for element in configFiles:
        ('../../parsers/SBMLparser/config' + element, './config')
    
if __name__ == "__main__":
    main()