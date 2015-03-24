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


def getFiles(directory,extension):
    """
    Gets a list of bngl files that could be correctly translated in a given 'directory'
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append([os.path.join(root, filename),os.path.getsize(os.path.join(root, filename))])

    #sort by size
    matches.sort(key=lambda filename: filename[1], reverse=False)
    
    matches = [x[0] for x in matches]

    return matches

import progressbar

def start_queue(fileNameSet,outputdirectory):
    print len(fileNameSet)
    progress = progressbar.ProgressBar()
    for idx in progress(range(len(fileNameSet))):
	fileName = fileNameSet[idx]
        # Open a pipe to the qsub command.
        output, input = popen2('qsub')
        
        # Customize your options here
        job_name = "jjtv_atomizer_{0}".format(fileName.split('/')[-1])
        walltime = "1:00:00"
        processors = "nodes=1:ppn=1"
        command = ['python -m sbmlTranslator','-i',
            #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
            fileName,
            '-o',os.path.join(outputdirectory, str(fileName.split('/')[-1])) + '.bngl',
            '-c','config/reactionDefinitions.json',
            '-n','config/namingConventions.json',
            '-a']
        command = ' '.join(command)
        job_string = """#!/bin/bash
        #PBS -N %s
        #PBS -l walltime=%s
        #PBS -l %s
	#$ -cwd
	#PBS -q serial_queue
	# #PBS -M jjtapia@gmail.com
	# #PBS -m abe  # (a = abort, b = begin, e = end)
	PATH=/usr/local/anaconda/bin:$PATH
        cd /home/mscbio/jjtapia/workspace/bionetgen/parsers/SBMLparser/SBMLparser
	
        %s""" % (job_name, walltime, processors, 
	 command)
        
        # Send job_string to qsub
        input.write(job_string)
        input.close()
        
        # Print your job and the system response to the screen as it's submitted
        #print(job_string)
        #print(output.read())
        
        time.sleep(0.05)

if __name__ == "__main__":
    xmlfiles = getFiles("../XMLExamples/biomodels/non_metabolic","xml")
    start_queue(xmlfiles,"../non_metabolic")


