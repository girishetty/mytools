import os
from os import listdir
from os.path import isfile, join

mypath = os.getcwd()
print "Current Working Dir: " + mypath
contents = listdir(mypath)
print "Contents of " + mypath + "are: "
print contents
onlyfiles = [f for f in contents if isfile(join(mypath, f))]
print "Listing all the files under " + mypath
print onlyfiles
