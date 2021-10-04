#!/usr/bin/python
import sys
import MySQLdb

def main():
    # Open database connection
    #db = MySQLdb.connect("localhost", "testuser", "test123", "TESTDB" )
    # prepare a cursor object using cursor() method
    #cursor = db.cursor()
    # execute SQL query using execute() method.
    #cursor.execute("SELECT VERSION()")
    # Fetch a single row using fetchone() method.
    #data = cursor.fetchone()
    data = "Girish"
    print "Database version : %s " % data
    # disconnect from server
    #db.close()

main()
