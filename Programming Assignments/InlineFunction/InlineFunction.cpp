# include <iostream.h>


inline void girish (int x) {
	x = 5;
}

enum Girish {
		Giri1,
		Giri2,
		Giri3,
		Giri4,
		Giri5
	};

int main () {
	int a = 10;
	cout<<"a is : "<<a<<endl; 
	girish (a);
	/*
	int a = 5;
	*/
	cout<<"a is : "<<a<<endl;
	//cout<<"x is : "<<x<<endl;

	int b = Giri1;

	return 0;
}