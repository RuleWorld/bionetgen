import namingDatabase


def createNamingMatrix(fileList):
    pass
    

if __name__ == "__main__":
    db = namingDatabase.NamingDatabase('namingConventions.db')
    fileList = ['BIOMD0000000205.xml','BIOMD0000000048.xml','BIOMD0000000019.xml']
    createNamingMatrix(fileList)