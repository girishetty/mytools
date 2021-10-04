#include <iostream>
using namespace std;

typedef void(*FunPtr)(void *);

class Sample { 
public: 
    Sample(int aVal = 0) : iVal(aVal) { }
    ~Sample() { }
    void ThreadEntryFunction() {
		cout<<"Vale is : "<<iVal<<endl;
	}

	void SetFunction(FunPtr aFunPtr) {
		iFunPtr = aFunPtr;
	}

	void CallFunction() {
		iFunPtr(NULL);
	}

private:
	int iVal;
	FunPtr iFunPtr;
}; 

template<class T, void(T::*mem_fn)(void*)> 
void thunk(void* p) { 
     (static_cast<T*>(p)->*mem_fn)(NULL); 
     return 0; 


} 

int main() {
	Sample sam;
	sam.SetFunction(thunk<Sample, &Sample::ThreadEntryFunction>);
	sam.CallFunction();
	return 0;
}