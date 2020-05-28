# include "Header.h"

extern int globVar;

int main () {
	
	statVar = 40;
	globVar = 50;
	Sample s;
	
	cout<<"statVar : "<<statVar<<endl;
	cout<<"globVar : "<<globVar<<endl;
	cout<<"Sample::iStatVal : "<<s.iStatVal<<endl;

	function ();

	cout<<"statVar : "<<statVar<<endl;
	cout<<"globVar : "<<globVar<<endl;
	cout<<"Sample::iStatVal : "<<s.iStatVal<<endl;

	return 0;
}