# -*- coding: utf-8 -*-
"""
Created on Fri Feb 22 14:37:28 2013

@author: proto
"""

import libsbml
import libsbml2bngl
from subprocess import call        
import numpy as np  
import re   
import pickle
#import matplotlib.pyplot as plt
        
def xml2cps():
    with open('dummy.tmp','w') as d:
        for element in range(1,463):
            call(['/home/proto/Downloads/copasi/bin/CopasiSE','-i','XMLExamples/curated/BIOMD%010i.xml' % element],stdout=d)
            print element
#    inputFile = open('fceri_ji_{0}b.cps'.format(index),'r')
    
    
def correctCPS():
    for element in range(1,463):
        #inputFile = open('XMLExamples/curated/BIOMD%010i.xml' % element,'r')
        cinputFile = open('XMLExamples/curated/BIOMD%010i.cps' % element,'r')
        content = cinputFile.readlines()
        newContent = []     
        flag = False
        counter = 0
        reader = libsbml.SBMLReader()
        document = reader.readSBMLFromFile('XMLExamples/curated/BIOMD%010i.xml' % element)
        parser =libsbml2bngl.SBML2BNGL(document.getModel(),True)
        speciesList = []
        speciesDict = {}
        compartmentList = {}        
        for compartment in parser.model.getListOfCompartments():
            compartmentList[compartment.getId()] = compartment.getName() if compartment.getName() != '' else compartment.getId()
        for species in parser.model.getListOfSpecies():
            speciesList.append((parser.getRawSpecies(species)['name'],compartmentList[parser.getRawSpecies(species)['compartment']],parser.getRawSpecies(species)['returnID']))
            speciesDict[speciesList[-1][0]] = speciesList[-1][2]
            
        param,zparam = parser.getParameters()
        molecules,_,_,_ = parser.getSpecies({})
        _,_,_,_,removeParams,_ = parser.getAssignmentRules(zparam,param,molecules)  
        
        
        
        f = open('copasiBenchmark/speciesDict{0}.dump'.format(element),'w')
        pickle.dump(speciesDict,f)
        for idx,line in enumerate(content):
            if line.rfind('name="Time-Course" type="timeCourse"') != -1:
                newContent.append(line.replace('false', 'true'))
                newContent.append('<Report reference="Report_90" target="output_{0}.txt" append="0"/>\n'.format(element))
                flag = True
            elif flag == True:
                if counter == 2:
                    newContent.append(line.replace('0.01','1'))
                elif counter == 3:
                    newContent.append(line.replace('1','100'))
                    flag = False
                else:
                    newContent.append(content[idx])
                counter += 1
            
            
            elif line.rfind('<ListOfReports>') != -1:
                newContent.append(line)
                newContent.append('<Report key="Report_90" name="Time, Concentrations, Volumes, and Global Quantity Values" taskType="timeCourse" separator="&#x09;" precision="6"> \n\
          <Table printTitle="1"> \n\
            <Object cn="CN=Root,Model={0},Reference=Time"/> \n'.format(parser.model.getName()))
                for species in speciesList:
                    newContent.append('\t<Object cn="CN=Root,Model={1},Vector=Compartments[{2}],Vector=Metabolites[{0}],Reference=Concentration"/> \n'.format(species[0],parser.model.getName(),species[1]))
                
                for artificialSpecies in removeParams:
                    newContent.append('\t<Object cn="CN=Root,Model={1},Vector=Values[{0}],Reference=Value"/> \n'.format(artificialSpecies.split(' ')[0],parser.model.getName()))
                newContent.append('</Table> \n</Report>\n')
            else:
                newContent.append(content[idx])
                    
        outputFile = open('copasiBenchmark/mBIOMD%010i.cps' % (element),'w')
        outputFile.write(''.join(newContent))
        
 
def generateCopasiOutput():
    with open('dummy.tmp','w') as d:
        for element in range(1,463):
            call(['/home/proto/Downloads/copasi/bin/CopasiSE','copasiBenchmark/mBIOMD%010i.cps' % element],stdout=d)
            print element


def loadResults(fileName,split):
    print fileName,
    try:
        with open(fileName) as dataInput:
            timeCourse = []
            #remove spaces
            line = dataInput.readline().strip()
            headers = re.sub('\s+',' ',line).split(split)
            
            for line in dataInput:
                 nline = re.sub('\s+',' ',line.strip()).split(' ')
                 try:
                     timeCourse.append([float(x) for x in nline])     
                 except:
                     print '++++',nline
        print 'loaded'
        return headers,np.array(timeCourse)
    except IOError:
        print 'no file'
        return [],[]

'''        
def plotResults(fileResults1,fileResults2):
    plt.figure(1)
    plt.subplot(211)
    plt.plot(fileResults1[:,1:])
    plt.subplot(212)
    plt.plot(fileResults2[:,1:])
    plt.show()
'''
  
def evaluate(fileNumber):
    copheaders,copasi = loadResults('copasiBenchmark/output_{0}.txt'.format(fileNumber),'[')
    copheaders = [x.replace(']','').strip() for x in copheaders]
    copheaders = [x.replace('-','_').strip() for x in copheaders]
    bngheaders,bng = loadResults('output{0}.gdat'.format(fileNumber),' ')
    with open('copasiBenchmark/speciesDict{0}.dump'.format(fileNumber)) as f:
        translator = pickle.load(f)
    newCopHeaders = []
    newBngHeaders = []
    if len(copasi) < 2:
        raise
    elif len(bng) < 2:
        raise
    elif np.size(copasi,0) != np.size(bng,0):
        raise
    for idx,element in enumerate(copheaders):
        if element == 'Time':
            continue
            #newCopHeaders.append('time')
        elif element in bngheaders:
            if not (bng[-1,bngheaders.index(element)-1] ==0 and copasi[-1,idx] > 1e-5):
                newCopHeaders.append(idx)
                newBngHeaders.append(bngheaders.index(element)-1)
        elif element not in translator:
            
            nelement = element.split(' ')[0]  
            if nelement in bngheaders and nelement != '#':
                if not (bng[-1,bngheaders.index(element.split(' ')[0])-1] ==0 and copasi[-1,idx] != 0):
                        newCopHeaders.append(idx)
                        newBngHeaders.append(bngheaders.index(element.split(' ')[0])-1)
                #else:
                #    print 'herp derp',element
            continue
        elif translator[element] in bngheaders:
            if not (bng[-1,bngheaders.index(translator[element])-1] ==0 and copasi[-1,idx] > 1e-5):
                newCopHeaders.append(idx)
                newBngHeaders.append(bngheaders.index(translator[element])-1)
    #newBngHeaders = [x for x in bngheaders if x in newCopHeaders]
    #bNGIndexes = [idx for idx,x in enumerate(bngheaders) if x in newCopHeaders]
    #copasiIndexes = range(1,len(bNGIndexes))
    #print 
    #print bNGIndexes,copasiIndexes
    #print np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0)
    atol = copasi[:,newCopHeaders] - bng[:,newBngHeaders]
    rtol = atol/copasi[:,newCopHeaders]
    rtol = rtol[np.logical_not(np.isnan(rtol))]
    score =  np.average(pow(np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0),0.5))
    print '---',score,fileNumber    
    return score
    
def compareResults():
    good= 0
    tested = 0
    for fileNumber in [51]:
        print fileNumber
        copheaders,copasi = loadResults('copasiBenchmark/output_{0}.txt'.format(fileNumber),'[')
        copheaders = [x.replace(']','').strip() for x in copheaders]
        copheaders = [x.replace('-','_').strip() for x in copheaders]
        bngheaders,bng = loadResults('output{0}.gdat'.format(fileNumber),' ')
        with open('copasiBenchmark/speciesDict{0}.dump'.format(fileNumber)) as f:
            translator = pickle.load(f)
        newCopHeaders = []
        newBngHeaders = []
        
        if len(copasi) < 2:
            print 'copasi pass'
            continue
        elif len(bng) < 2:
            print 'bng pass'
            continue
        elif np.size(copasi,0) != np.size(bng,0):
            print 'different times'
            continue
        #print translator
        for idx,element in enumerate(copheaders):
            if element == 'Time':
                continue
                #newCopHeaders.append('time')
            elif element in bngheaders:
                if not (bng[-1,bngheaders.index(element)-1] ==0 and copasi[-1,idx] > 1e-5):
                    newCopHeaders.append(idx)
                    newBngHeaders.append(bngheaders.index(element)-1)
            elif element not in translator:
                nelement = element.split(' ')[0]  
                if nelement in bngheaders and nelement != '#':
                    if not (bng[-1,bngheaders.index(element.split(' ')[0])-1] ==0 and copasi[-1,idx] != 0):
                            newCopHeaders.append(idx)
                            newBngHeaders.append(bngheaders.index(element.split(' ')[0])-1)
                    else:
                        print 'herp derp',element
                continue
            elif translator[element] in bngheaders:
                if not (bng[-1,bngheaders.index(translator[element])-1] ==0 and copasi[-1,idx] > 1e-5):
                    newCopHeaders.append(idx)
                    newBngHeaders.append(bngheaders.index(translator[element])-1)
            
        #newBngHeaders = [x for x in bngheaders if x in newCopHeaders]
        #bNGIndexes = [idx for idx,x in enumerate(bngheaders) if x in newCopHeaders]
        #copasiIndexes = range(1,len(bNGIndexes))
        #print 
        #print bNGIndexes,copasiIndexes
        #print np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0)
        #atol = copasi[:,newCopHeaders] - bng[:,newBngHeaders]
        #rtol = atol/copasi[:,newCopHeaders]
        #rtol = rtol[np.logical_not(np.isnan(rtol))]
        score =  np.average(pow(np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0),0.5))
        
        #mini = np.min(np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0))
        #if score>1e-3 and mini<1e-5:
        #    print np.sum(pow(copasi[:,newCopHeaders] - bng[:,newBngHeaders],2),axis=0)/np.size(copasi,0)       
        tested += 1        
        if score < 0.001:
            good += 1
        else:
            print score,np.size(bng,1)
        #print newCopHeaders
    #print bngheaders,copheaders,copasiIndexes
    #print bng[0:3,newBngHeaders]
    #print copasi[0:3,newCopHeaders]
#    print bng[:,newBngHeaders]
#    print copasi[:,newCopHeaders]
    #print copheaders[1:]
    print tested,good 
    plotResults(bng[:,newBngHeaders],copasi[:,newCopHeaders])           

        
def main():
    #xml2cps()
    #correctCPS()
    #generateCopasiOutput()
    #loadCopasiResults()
    #loadBNGResults(1)
    compareResults()

if __name__ == "__main__":
    main()
    
#16
#311,248: Using reaction ID's as parameters
