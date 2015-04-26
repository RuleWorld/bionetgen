import pandas
import os
import fnmatch
import numpy as np
import math
import matplotlib.pyplot as plt 
import seaborn as sns
from scipy import stats

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


def divideBySize(weight):
    if weight < 20:
        return 'small'
    return 'large'
def assembleSingleColumnDataFrame(directory):
    entropyFiles = getFiles(directory,'h5')
    entropyFrames = [pandas.read_hdf(x,'entropy') for x in entropyFiles]
    finalFrame = pandas.concat(entropyFrames)
    return finalFrame


def loadDataFrames(directory,subdirectories):
    """
    unify a partitioned data frame into a single object
    """
    frames = []
    for subdirectory in subdirectories:
        frames.append(assembleSingleColumnDataFrame(os.path.join(directory,subdirectory)))
    finalFrames = pandas.concat(frames,axis=1)
    return finalFrames

def create2DdensityPlot(dataframe,columns,outputfilename):
    """
    creates a 2d density plot given a dataframe and two columns.
    saves the image in <outputfilename>
    """
    plt.clf()
    f, (ax1) = plt.subplots(1, 1, sharex=True, figsize=(8, 6))
    #with sns.axes_style("white"):
    #sns.jointplot("compression", "wiener index",atomizationInfo, kind="kde");
    g = sns.JointGrid(columns[0],columns[1],dataframe)
    g.plot_marginals(sns.distplot)
    g.plot_joint(sns.kdeplot, shade=True, cmap="PuBu");
    g.annotate(stats.pearsonr)
    #sns.kdeplot(atomizationInfo.compression, shade=True,ax=ax1);
    plt.savefig(outputfilename)

def create1Ddensityplot(data,outputfilename):
    plt.clf()
    f, (ax1) = plt.subplots(1, 1, sharex=True, figsize=(8, 6))
    #with sns.axes_style("white"):
    #sns.jointplot("compression", "wiener index",atomizationInfo, kind="kde");
    sns.kdeplot(data, shade=True,ax=ax1,clip=(0,1),bw=0.5);
    plt.savefig(outputfilename)




def analyzeDataSet(folder):
    
    finalFrames = loadDataFrames(folder,['raw','atom'])
    finalFrames.to_hdf('full_reg_comp.h5','entropy')
    

    atomizationInfo = pandas.read_hdf('atomizationResults.h5','atomization')
    atomizationInfo.rename(index=lambda x: x.replace('_regulatory','_collapsed'),inplace=True)

    finalFrames = pandas.read_hdf('full_reg_comp.h5','entropy')
    substract = finalFrames['atomized_wiener']/finalFrames['raw_wiener']
    substract2 = np.array([x for x in substract.values if x >=-10])
    print np.mean(substract),np.std(substract)
    colors= sns.color_palette("Set1", 3)
    f, (ax1) = plt.subplots(3, 1, sharex=True, figsize=(8, 6))
    #print (finalFrames['raw']-finalFrames['atomized'])['BIOMD0000000005.xml_regulatory.gml']
    sns.kdeplot(np.array([x for x in finalFrames['raw_wiener'].values if x>=0]), shade=True,color=colors[0],label='Raw translation',ax=ax1[0],bw=0.5)
    sns.kdeplot(np.array([x for x in finalFrames['atomized_wiener'].values if x>=0]), shade=True,color=colors[1],label='Atomized translation',ax=ax1[1],bw=0.5)
    sns.kdeplot(substract2, shade=True,color=colors[2],label='Atomized translation',ax=ax1[2],clip=(-0.0001, 1.0001),bw=0.5)
    plt.xlabel('Entropy difference between raw translation and atomized model')
    print finalFrames.columns

    #print len(substract)
    #ax1[0].set_xlim([0,1])
    #plt.show()
    plt.savefig('{0}/entropyDifference.png'.format(folder))
    #plt.clf()
    
    #print len(substract)
    atomizationInfo2 =  pandas.concat([atomizationInfo,finalFrames],axis=1)
    atomizationInfo2['wiener ration'] = substract
    atomizationInfo2['compression'] = 1 - atomizationInfo2['compression']
    atomizationInfo2['centropy_ratio'] = atomizationInfo2['raw_ccentropy']/atomizationInfo2['atomized_ccentropy']
    atomizationInfo2['compression2'] = atomizationInfo2['atomized_nprocess']*1.0/atomizationInfo2['weight']
    atomizationInfo2['size'] = atomizationInfo2['weight'].map(divideBySize )


    '''
    atomizationInfo['wiener ratio'] = substract
    atomizationInfo['compression'] = 1 - atomizationInfo['compression']
    atomizationInfo['wiener index'] = finalFrames['atomized_wiener']
    atomizationInfo['rwiener index'] = finalFrames['raw_wiener']
    atomizationInfo['wiener entropy'] = finalFrames['atomized_entropy']
    atomizationInfo['raw_centropy'] = finalFrames['raw_ccentropy']
    atomizationInfo['centropy'] = finalFrames['atomized_ccentropy']
    atomizationInfo['centropy_ratio'] = atomizationInfo['raw_centropy']/atomizationInfo['centropy']
    atomizationInfo['size'] = atomizationInfo['weight'].map(divideBySize )
    #atomizationInfo = atomizationInfo.dropna()
    atomizationInfo['atomized_nconn'] = finalFrames['atomized_nconn']
    atomizationInfo['raw_nconn'] = finalFrames['raw_nconn']
    atomizationInfo['compression2'] = finalFrames['atomized_nprocess']*1.0/atomizationInfo['weight']
    '''

    #print atomizationInfo.to_string()

    #atomizationInfo.dropna()
    create1Ddensityplot(atomizationInfo2['compression2'],'{0}/compression2density.png'.format(folder))

    create2DdensityPlot(atomizationInfo2,["compression","raw_wiener"],'{0}/compressionvswiener.png'.format(folder))
    create2DdensityPlot(atomizationInfo2,["compression","atomized_ccentropy"],'{0}/compressionvscentropy.png'.format(folder))
    create2DdensityPlot(atomizationInfo2,["compression","raw_ccentropy"],'{0}/compressionvsrcentropy.png'.format(folder))
    create2DdensityPlot(atomizationInfo2,["raw_ccentropy","atomized_ccentropy"],'{0}/rcentropyvsccentropy.png'.format(folder))


    '''
    print 'pairs'
    plt.clf()
    g = sns.PairGrid(atomizationInfo[atomizationInfo.weight > 20],dropna=True,vars=['centropy','raw_centropy','centropy_ratio','atomization','rwiener index','wiener index','wiener ratio','compression'])
    g.map_upper(sns.kdeplot, cmap="Blues_d")
    g.map_lower(sns.kdeplot, cmap="Blues_d")
    g.map_diag(sns.kdeplot, lw=3, legend=False)
    #sns.pairplot(atomizationInfo,dropna=True);
    plt.savefig('atomizationgrid.png')
    '''

    
    plt.clf()
    f,ax1 = plt.subplots(1,1)
    cmap = sns.diverging_palette(220, 10, as_cmap=True)
    sns.corrplot(atomizationInfo2, annot=True, sig_stars=False,
             diag_names=False, cmap=cmap, ax=ax1)
    f.tight_layout()
    plt.savefig('{0}/corrmatrix.png'.format(folder))

    plt.clf()
    f,ax1 = plt.subplots(1,1)
    cmap = sns.diverging_palette(220, 10, as_cmap=True)
    sns.corrplot(atomizationInfo2[["compression","atomized_ccentropy","raw_ccentropy","atomized_wiener"]], annot=True, sig_stars=False,
             diag_names=False, cmap=cmap, ax=ax1)
    f.tight_layout()
    plt.savefig('{0}/corrmatrix_subset.png'.format(folder))    

if __name__ == "__main__":
    analyzeDataSet('ccollapsed') 
