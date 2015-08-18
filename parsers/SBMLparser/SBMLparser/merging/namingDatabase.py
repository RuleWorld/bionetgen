import sqlite3
import utils.annotationExtractor
import utils.annotationResolver
import argparse
import os
from copy import copy
import pprint
import fnmatch
import progressbar


def getFiles(directory, extension):
    """
    Gets a list of <*.extension> files. include subdirectories and return the absolute
    path. also sorts by size.
    """
    matches = []
    for root, dirnames, filenames in os.walk(directory):
        for filename in fnmatch.filter(filenames, '*.{0}'.format(extension)):
            matches.append([os.path.join(root, filename), os.path.getsize(os.path.join(root, filename))])

    # sort by size
    matches.sort(key=lambda filename: filename[1], reverse=False)
    matches = [x[0] for x in matches]

    return matches


class NamingDatabase:
    def __init__(self, databaseName):
        self.databaseName = databaseName

    def getAnnotationsFromSpecies(self, speciesName):
        connection = sqlite3.connect(self.databaseName)
        cursor = connection.cursor()
        queryStatement = 'SELECT annotationURI,annotationName from moleculeNames as M join identifier as I ON M.ROWID == I.speciesID join annotation as A on A.ROWID == I.annotationID and M.name == "{0}"'.format(speciesName)
        queryResult = [x[0] for x in cursor.execute(queryStatement)]
        connection.close()
        return queryResult

    def getFileNameFromSpecies(self, speciesName):
        connection = sqlite3.connect(self.databaseName)
        cursor = connection.cursor()
        queryStatement = 'SELECT DISTINCT file from moleculeNames WHERE name == "{0}"'.format(speciesName)
        queryResult = [x[0] for x in cursor.execute(queryStatement)]
        connection.close()
        return queryResult

    def getSpeciesFromAnnotations(self, annotation):
        connection = sqlite3.connect(self.databaseName)
        cursor = connection.cursor()
        queryStatement = 'SELECT name,A.annotationURI from moleculeNames as M join identifier as I ON M.ROWID == I.speciesID join annotation as A on A.ROWID == I.annotationID and A.annotationURI == "{0}"'.format(annotation)
        queryResult = [x[0] for x in cursor.execute(queryStatement)]
        connection.close()
        return queryResult

    def getFilesInDatabase(self):
        connection = sqlite3.connect(self.databaseName)
        cursor = connection.cursor()
        queryStatement = 'SELECT DISTINCT file from moleculeNames'
        queryResult = [x[0] for x in cursor.execute(queryStatement)]
        connection.close()
        return queryResult

    def getSpeciesFromFileName(self, fileName):
        connection = sqlite3.connect(self.databaseName)
        cursor = connection.cursor()
        queryStatement = 'SELECT file,name,A.annotationURI,A.annotationName,qualifier from moleculeNames as M join identifier as I ON M.ROWID == I.speciesID \
                            join annotation as A on A.ROWID == I.annotationID and M.file == "{0}" and I.qualifier != "BQB_HAS_PART" and \
                            I.qualifier != "BQB_HAS_VERSION" AND I.qualifier != "BQB_HAS_PROPERTY"'.format(fileName)

        speciesList = [x[1:] for x in cursor.execute(queryStatement)]
        tmp = {x[0]: set([]) for x in speciesList}
        tmp2 = {x[0]: set([]) for x in speciesList}
        tmp3 = {x[0]: set([]) for x in speciesList}
        for x in speciesList:
            tmp[x[0]].add(x[1])
            if x[2] != '':
                tmp2[x[0]].add(x[2])
            tmp3[x[0]].add(x[3])
        tmp = [{'name': set([x]), 'annotation': set(tmp[x]), 'annotationName': set(tmp2[x]), 'fileName': set([fileName]), 'qualifier': tmp3[x]} for x in tmp]
        return tmp

    def findOverlappingNamespace(self, fileList):
        fileSpecies = []
        if len(fileList) == 0:
            fileList = self.getFilesInDatabase()

        progress = progressbar.ProgressBar(maxval=len(fileList)).start()

        for idx in progress(range(len(fileList))):
            fileSpecies.extend(self.getSpeciesFromFileName(fileList[idx]))


        changeFlag = True
        fileSpeciesCopy = copy(fileSpecies)
        print 'finished processing files, obtained {0} groups'.format(len(fileSpecies))
        while changeFlag:
            try:
                changeFlag = False
                for idx in range(0, len(fileSpecies) - 1):
                    for idx2 in range(idx+1, len(fileSpecies)):
                        if (len(fileSpecies[idx]['name']) > 2 and fileSpecies[idx]['name'].intersection(fileSpecies[idx2]['name'])) \
                            or fileSpecies[idx]['annotation'].intersection(fileSpecies[idx2]['annotation']) \
                                or fileSpecies[idx]['annotationName'].intersection(fileSpecies[idx2]['annotationName']):

                            #print 'hello',fileSpecies[idx]['annotationName'],fileSpecies[idx2]['annotationName']
                            fileSpeciesCopy[idx]['name'] = fileSpeciesCopy[idx]['name'].union(fileSpeciesCopy[idx2]['name'])
                            fileSpeciesCopy[idx]['annotation'] = fileSpeciesCopy[idx]['annotation'].union(fileSpeciesCopy[idx2]['annotation'])
                            fileSpeciesCopy[idx]['annotationName'] = fileSpeciesCopy[idx]['annotationName'].union(fileSpeciesCopy[idx2]['annotationName'])
                            fileSpeciesCopy[idx]['fileName'] = fileSpeciesCopy[idx]['fileName'].union(fileSpeciesCopy[idx2]['fileName'])
                            fileSpeciesCopy[idx]['qualifier'] = fileSpeciesCopy[idx]['qualifier'].union(fileSpeciesCopy[idx2]['qualifier'])
                            del fileSpeciesCopy[idx2]
                            fileSpecies = fileSpeciesCopy
                            raise IOError
            except IOError:
                changeFlag = True
                continue
        #fileSpecies = [[x['name'], len(x['fileName'])] for x in fileSpecies]
        fileSpecies.sort(key=lambda x: len(x['fileName']), reverse=True)

        import pickle

        with open('results.dump','wb') as f:
            pickle.dump(fileSpecies,f)

        return fileSpecies

    def isFileInDatabase(self, fileName):
        return isFileInDatabase(self.databaseName, fileName)




def isFileInDatabase(databaseName, fileName):
    connection = sqlite3.connect(databaseName)
    cursor = connection.cursor()
    queryStatement = 'select file from biomodels WHERE file == "{0}"'.format(fileName)
    matchingFileNames = [x[0] for x in cursor.execute(queryStatement)]
    connection.close()
    return len(matchingFileNames) > 0


def setupDatabase(databaseName):
    connection = sqlite3.connect(databaseName)
    cursor = connection.cursor()
    cursor.execute('''CREATE TABLE biomodels(file UNIQUE, organismID INT, FOREIGN KEY(organismID) REFERENCES annotation(ROWID))''')
    cursor.execute('''CREATE TABLE moleculeNames(fileId INT,name, FOREIGN KEY(fileID) REFERENCES biomodels(file))''')
    cursor.execute('''CREATE TABLE annotation(annotationURI UNIQUE ON CONFLICT IGNORE,annotationName)''')
    cursor.execute('''CREATE TABLE identifier(annotationID INT, qualifier, speciesID INT, FOREIGN KEY(speciesID) \
                   REFERENCES moleculeName(ROWID), FOREIGN KEY(annotationID) references annotation(ROWID))''')
    connection.commit()
    connection.close()


def extractBasicAnnotations(fileName, userDefinitions=None):
    annotations = utils.annotationExtractor.AnnotationExtractor(fileName, userDefinitions)
    elementalMolecules = [x for x in annotations.sct if annotations.sct[x] == []]
    speciesAnnotations = {x: annotations.getAnnotationSystem()[x] for x in elementalMolecules}
    modelAnnotations = annotations.getModelAnnotations()
    return speciesAnnotations, modelAnnotations


def populateDatabaseFromFile(fileName, databaseName, userDefinitions=None):
    """
    Insert annotations from file <fileName>
    """

    fileName2 = fileName.split(os.sep)[-1]
    if isFileInDatabase(databaseName, fileName2):
        print('Database already contains annotation from file {0}'.format(fileName2))
        return -1

    connection = sqlite3.connect(databaseName)
    cursor = connection.cursor()

    basicModelAnnotations, generalModelAnnotations = extractBasicAnnotations(fileName, userDefinitions)

    moleculeNames = []
    annotationNames = []
    moleculeAnnotations = []


    modelSpecies = ''
    # insert model description
    for annotation in generalModelAnnotations:
        if 'taxonomy' in annotation[1]:
            modelSpecies = annotation
            break


    annotationNames.append(utils.annotationResolver.resolveAnnotation(annotation[1]))

    cursor.executemany("INSERT into annotation(annotationURI,annotationName) values (?, ?)", annotationNames)

    connection.commit()
    annotationID = [x for x in cursor.execute('select ROWID from annotation WHERE annotationURI == "{0}"'.format(annotationNames[-1][0]))][0][0]
    annotationNames = []
    cursor.executemany("INSERT into biomodels(file,organismID) values (?,?)", [[fileName2, annotationID]])
    connection.commit()

    modelID = [x for x in cursor.execute('select ROWID from biomodels WHERE file == "{0}"'.format(fileName2))][0][0]

    # insert moleculeNames
    for molecule in basicModelAnnotations:
        moleculeNames.append([modelID, molecule])

    annotationIDs = {x[1]: x[0] for x in cursor.execute("select ROWID,annotationURI from annotation")}

    #insert annotations
    for molecule in basicModelAnnotations:
        for annotationType in basicModelAnnotations[molecule]:
            for annotation in basicModelAnnotations[molecule][annotationType]:
                if annotation not in annotationIDs:
                    annotationName = utils.annotationResolver.resolveAnnotation(annotation)
                    annotationNames.append([annotation, annotationName[1]])

    cursor.executemany("insert into annotation(annotationURI,annotationName) values (?, ?)", annotationNames)
    connection.commit()
    cursor.executemany("insert into moleculeNames(fileId,name) values (?, ?)", moleculeNames)
    connection.commit()





    moleculeIDs = {x[1]: x[0] for x in cursor.execute("select ROWID,name from moleculeNames WHERE moleculeNames.fileId == '{0}'".format(modelID))}
    annotationIDs = {x[1]: x[0] for x in cursor.execute("select ROWID,annotationURI from annotation")}

    for molecule in basicModelAnnotations:
        for annotationType in basicModelAnnotations[molecule]:
            for annotation in basicModelAnnotations[molecule][annotationType]:
                moleculeAnnotations.append([annotationIDs[annotation], moleculeIDs[molecule], annotationType])
    
    cursor.executemany("insert into identifier(annotationID,speciesID, qualifier) values (?, ?, ?)", moleculeAnnotations)
    connection.commit()
    connection.close()


def defineConsole():
    parser = argparse.ArgumentParser(description='SBML to BNGL translator')
    parser.add_argument('-c', '--create',action='store_true',help='Create database tables')
    parser.add_argument('-d', '--database',type=str,help='database to modify',required=True)
    parser.add_argument('-i', '--input-file',type=str,help='input SBML file')
    parser.add_argument('-u', '--user_conventions',type=str,help='Use user convention definitions for SCT calculation')
    parser.add_argument('-q', '--query',action='store_true',help='Query a database for its common namespace')
    parser.add_argument('-s', '--specific_query', type=str, help='search for models with a given molecule')
    parser.add_argument('-r','--directory',type=str,help='Add SBML models in directory "directory" to database')
    #parser.add_argument('-o','--output-file',type=str,help='output SBML file',required=True)
    return parser    


def query(moleculeQuery):
    db = NamingDatabase('namingConventions.db')
    #db.getAnnotationsFromSpecies('EGFR')
    #db.getSpeciesFromAnnotations('http://identifiers.org/uniprot/P00533')
    #print db.getSpeciesFromFileName('BIOMD0000000048.xml')
    if moleculeQuery:
        selectedFiles = db.getFileNameFromSpecies(moleculeQuery)
        result = db.findOverlappingNamespace(selectedFiles)
    else:
        result = db.findOverlappingNamespace([])

    pprint.pprint([[x['name'], len(x['fileName'])] for x in result])

if __name__ == "__main__":
    
    
    parser = defineConsole()
    namespace = parser.parse_args()
    if namespace.query:
        query(namespace.specific_query)
    else:
        if namespace.create:
            setupDatabase(namespace.database)

        if namespace.input_file:
            userDefinitions = namespace.user_conventions
            populateDatabaseFromFile(namespace.input_file, namespace.database, userDefinitions)
        if namespace.directory:
            sbmlFiles = getFiles(namespace.directory, 'xml')
            progress = progressbar.ProgressBar(maxval=len(sbmlFiles)).start()
            for idx in progress(range(len(sbmlFiles))):
                populateDatabaseFromFile(sbmlFiles[idx], namespace.database, None)

