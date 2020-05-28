#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>

using namespace std;

int main()
{
	errno = 0;	
	
	fstream outfile;
	outfile.open("test.txt",fstream::out| fstream::in);


	if (outfile.is_open())
  {
  	outfile<<"test line\n";
    cout << "File successfully open";
    outfile.close();
   }
  else
  {
    cout << "Error opening file";
		cout <<"\nErro = "<<errno;
    
  }
  getchar();
  outfile.close();
  return 0;

}

