from random import random
myfile = open('/Users/gshetty/work/codebase/repo/mytools/python_script/girish_test.txt', 'a')
myfile.write("Girish:" + str(random()) + "\n")
myfile.close();
