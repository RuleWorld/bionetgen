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

queue_list= {'noc_64_core':64,'serial_queue':1}
import progressbar
import tempfile
import yaml
def start_queue(fileNameSet,outputdirectory,queue,batchSize):
    print len(fileNameSet)
    progress = progressbar.ProgressBar()
    settings = 'settings.yaml'
    for idx in progress(range(0,len(fileNameSet),batchSize)):
    	fileNameSubset = fileNameSet[idx:min(idx+batchSize,len(fileNameSet))]
        settings = {}
        settings['inputfiles'] = fileNameSubset
        settings['outputdirectory'] = outputdirectory

        pointer = tempfile.mkstemp(suffix='.yml',text=True)
        with open(pointer[1],'w') as f:
            f.write(yaml.dump(settings))
        # Open a pipe to the qsub command.
        output, input = popen2('qsub')
        
        # Customize your options here
        job_name = "jjtv_atomizer_{1}".format(outputdirectory,idx)
        walltime = "1:00:00"
        processors = "nodes=1:ppn={0}".format(queue_list[queue])
        command = ['python stats/analyzeModelSet','-s',pointer[1],
            #'XMLExamples/curated/BIOMD%010i.xml' % self.param,
        ]
        command = ' '.join(command)
        job_string = """#!/bin/bash
        #PBS -N %s
        #PBS -l walltime=%s
        #PBS -l %s
	#$ -cwd
	#PBS -q %s

    echo Running on `hostname`
    echo workdir $PBS_O_WORKDIR

	# #PBS -M jjtapia@gmail.com
	# #PBS -m abe  # (a = abort, b = begin, e = end)
    PYTHONPATH=$PYTHONPATH:./:./SBMLparser
	PATH=/usr/local/anaconda/bin:$PATH
        cd /home/mscbio/jjtapia/workspace/bionetgen/parsers/SBMLparser/
	
        %s""" % (job_name, walltime, processors, queue,command)
        
        # Send job_string to qsub
        input.write(job_string)
        input.close()
        
        # Print your job and the system response to the screen as it's submitted
        #print(job_string)
        #print(output.read())
        
        time.sleep(0.05)

if __name__ == "__main__":
    xmlfiles = getFiles("XMLExamples/curated","xml")
    start_queue(xmlfiles[0:100],"complex3",'serial_queue',10)


