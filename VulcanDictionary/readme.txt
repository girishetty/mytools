I have copied the code into a zip file

- VulcanDictionary.7z

The code structure is as below:

Headers:
TypeDefines.h             ==> has some common data type defines
AlphabetTable.h           ==> has the defination of Alphabet Table
DictionaryReader.h        ==> has the defination of Dictionary Reader

Sources:

AlphabetTable.cpp         ==> implementaion of Alphabet Table
DictionaryReader.cpp      ==> implementaion of Dictionary Reader
main.cpp                  ==> Execution entry point

Build:
Makefile                  ==> makefile for building the binaries

Test:
dictionary_sample.txt     ==> sample dictionary input file


I have tried this on both Windows (with MinGW http://www.mingw.org/) and Linux Machine.

How to build and run:

On windows:
- extract the zip file (7z) onto a folder.
- If you have MinGW, open the MinGW shell, go to that extracted fodler and type "make"
- That will create the binary. And now type "VulcanDictionary.exe"
- That should display the output

- If you want to add more entries you can edit the file dictionary_sample.txt
- Of if you have your own input file, over-write that on to dictionary_sample.txt

NOTE#1: I didn't add the code to prompt the user for the input file (matter of cin/scanf, but I didn't do that!)
NOTE#2: I have tried this with Microsoft Visual Studio as well. If needed you can create a project and add the source/headers


On Linux/UNIX:
- extract the zip file (7z) onto a folder.
- Open the shell/terminal, go to that extracted fodler and type "make"
- That will create the binary. And now type "./VulcanDictionary"
- That should display the output

- If you want to add more entries you can edit the file dictionary_sample.txt
- Of if you have your own input file, over-write that on to dictionary_sample.txt

