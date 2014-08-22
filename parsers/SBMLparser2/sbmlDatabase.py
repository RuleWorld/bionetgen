# -*- coding: utf-8 -*-
"""
Created on Fri May 18 18:47:25 2012

@author: proto
"""

import sqlite3 as lite

def connectToDatabase():
    con = None
    try:
        con = lite.connect('XMLExamples/data.db')
    except lite.Error,e :
        print "Error %s:" % e.args[0]
    return con

def createTables(con):
    with con:
        cur = con.cursor()
        #cur.execute('CREATE TABLE COMPOUNDS(Name text)')
        cur.execute('CREATE TABLE REFERENCES(Id INT, Name TEXT, File TEXT)')
if __name__ == "__main__":
    con = connectToDatabase()
    createTables(con)