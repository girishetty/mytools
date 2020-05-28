#include <iostream>
using namespace std;

typedef struct  {
	int* _array[10];
	int* _val;
	int _data;
}WSD;

WSD* pWSD = NULL;

WSD* GetWSD() {
	if(!pWSD) {
		pWSD = new WSD;
		for(int i=0; i<10;i++)
			pWSD->_array[i] = 0;
		pWSD->_val = 0;
		pWSD->_data = 10;
	}
	return pWSD;
}

int** get_array() {
	return (int**)&(GetWSD()->_array);
}


int** get_val() {
	return &(GetWSD()->_val);
}

int* get_data() {
	return &(GetWSD()->_data);
}


#define array (get_array())
#define val (*get_val())
#define data (*get_data())

int main() {
	val = (int*)0x40;
	cout<<"val is : "<<val<<endl;
	data = 1040;
	cout<<"data is : "<<data<<endl;
	int** p = pWSD->_array;
	p[0] = (int*)0x20;
	int** p1 = array;
	p1[0] = (int*)0x20;
	for(int i=0; i<10;i++) {
		array[i] = (int*)(0x200+i*10);
		cout<<"array["<<i<<"] : " <<array[i]<<endl;
	}
	return 0;
}
