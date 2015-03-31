#!/usr/bin/python
# Example PBS cluster job submission in Python

from popen2 import popen2
import time
    
# If you want to be emailed by the system, include these in job_string:
#PBS -M your_email@address
#PBS -m abe  # (a = abort, b = begin, e = end)

# Loop over your jobs
import os.path
import fnmatch
import argparse


def getFiles(directory,extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'

    Keyword arguments:
    directory -- The directory we will recurseviley get files from
    extension -- A file extension filter
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            filepath = os.path.abspath(os.path.join(root, filename))
            matches.append([filepath,os.path.getsize(os.path.join(root, filename))])

    #sort by size
    #matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches

queue_list= {'noc_64_core':64,'serial_queue':1,'dept_24_core':24,'dmz_core36':36,'bahar_64_core':64}
import progressbar
import tempfile
import yaml
def start_queue(fileNameSet,outputdirectory,queue,batchSize,outputtype):
    """
    sends a batch job with the qsub queue and executes a command over it.
    using PBS commands
 
    Keyword arguments:
    fileNameSet -- A list of files containg the files to be sent to the queue
    outputdirectory -- The directory we will finally place our results in
    queue -- The queue we will send the job to. It has to make reference to an entry in queue_list
    batchsize -- The total number of jobs per batch job. (in other words, the number of nodes we will be using is <fileNameSet>/<batchSize>. Typically you want this to be a multiple of the number of cores per node.)
    """
    print len(fileNameSet)
    progress = progressbar.ProgressBar()
    settings = 'settings.yaml'
    for idx in progress(range(0,len(fileNameSet),batchSize)):
        fileNameSubset = fileNameSet[idx:min(idx+batchSize,len(fileNameSet))]
        settings = {}
        settings['inputfiles'] = fileNameSubset

        pointer = os.path.abspath(tempfile.mkstemp(suffix='.yml',text=True,dir='./tmp')[1])
        with open(pointer,'w') as f:
            f.write(yaml.dump(settings))
        # Open a pipe to the qsub command.
        output, input = popen2('qsub')
        
        # Customize your options here
        job_name = "jjtv_atomizer_{1}".format(outputdirectory,idx)
        walltime = "24:00:00"
        processors = "nodes=1:ppn={0}".format(queue_list[queue])
        analyzer = os.path.abspath('stats/analyzeModelSet.py')
        command = ['python {0}'.format(analyzer),'-s',pointer,'-o','${SCRDIR}/',
        '-t',outputtype
            #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        ]
        command = ' '.join(command)

        if outputtype == "atomize":
            tail_job_string = """%s
            cd ${SCRDIR}
            """ % (command)
        else:
            tail_job_string = """cd ${SCRDIR}
            %s
            """ % (command)

        job_string = """#!/bin/bash
        #PBS -N %s
        #PBS -l walltime=%s
        #PBS -l %s
        #$ -cwd
        #PBS -q %s

        echo Running on `hostname`
        echo workdir $PBS_O_WORKDIR

        #PBS -M jjtapia@gmail.com
        #PBS -m abe  # (a = abort, b = begin, e = end)
        PYTHONPATH=$PYTHONPATH:./:./SBMLparser
        PATH=/usr/local/anaconda/bin:$PATH
        SCRDIR=/scr/$PBS_JOBID

        #if the scratch drive doesn't exist (it shouldn't) make it.
        if [[ ! -e $SCRDIR ]]; then
                mkdir $SCRDIR
        fi

        cd $PBS_O_WORKDIR
        echo scratch drive ${SCRDIR}

        trap "cd ${SCRDIR};mv * $PBS_O_WORKDIR/%s" EXIT
	    %s
        
        """ % (job_name, walltime, processors, queue,outputdirectory,tail_job_string)
        
        # Send job_string to qsub
        input.write(job_string)
        input.close()
        
        # Print your job and the system response to the screen as it's submitted
        #print(job_string)
        #print(output.read())
        
        time.sleep(0.05)

def restart(targetset,comparisonset,targetExtension):
    """
    This function is used to restart from a previouly incomplete run
    """
    targetFolder = '/'.join(targetset[0].split('/')[:-1]) + '/'

    existingset = [targetFolder + '.'.join(x.split('/')[-1].split('.')[:-1]) +  targetExtension for x in comparisonset]
    filenameset = [x for x in targetset if x not in existingset]
    return filenameset

def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-r','--resume',action='store_true',help='Resume previous run')
    parser.add_argument('-t','--type',type=str,help='atomize or convert to bng-xml')
    parser.add_argument('-q','--queue',type=str,help='queue to run in')
    parser.add_argument('-b','--batch',type=int,help='batch size')
    parser.add_argument('-i','--input',type=str)
    parser.add_argument('-o','--output',type=str)

    return parser    


if __name__ == "__main__":
    parser = defineConsole()
    namespace = parser.parse_args()
    outputfolder = namespace.output
    inputfolder = namespace.input
    queue = namespace.queue
    batchsize = namespace.batch
    if namespace.type == 'atomize':
        finalfiles = getFiles(inputfolder,"xml")
        if namespace.resume:
            bnglfiles = getFiles(outputfolder,"bngl")
            finalfiles = restart(finalfiles,bnglfiles,'')
            
    elif namespace.type =='bngxml':
        finalfiles = getFiles(inputfolder,"bngl")
        if namespace.resume:
            bngxmlfiles = getFiles(inputfolder,"xml")
            finalfiles = restart(finalfiles,bngxmlfiles,'.bngl')
    #print len(finalfiles)
    
    start_queue(finalfiles,outputfolder,queue,batchsize,namespace.type)

