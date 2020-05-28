#include <pthread.h>
using namespace std;

class Sample { 
public: 
    Sample(int aVal = 0) : iVal(aVal) { }
    ~Sample() { }
    void ThreadEntryFunction() {}
private:
	int iVal;
}; 

class OtherClass { 
public: 
    OtherClass(int aVal = 0) : iVal(aVal) { }
    ~OtherClass() { }
    void ThreadEntryFunction() {}
private:
	int iVal;
}; 

template<class T, void (T::*EntryFunction)()> void* MyEntryFun(void* aPtr) {
     T* p = static_cast<T*>(aPtr);
     (p->*EntryFunction)();
     return 0;
}

template<typename ClassName, typename FunName> void* EntryFun(void* aPtr) { 
     ClassName* pClass = static_cast<ClassName*>(aPtr);
     pClass->FunName(); 
     return 0; 
} 

template<typename ClassName> void* EntryFunction(void* aPtr) {
	ClassName* sam = static_cast<ClassName*>(aPtr);
	sam->ThreadEntryFunction();
	return 0;
}

int main() {
	Sample sam; 
	OtherClass obj; 
	pthread_t thrd; 
	pthread_attr_t attr; 
	pthread_attr_init(&attr); 
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED); 

	//pthread_create(&thrd, &attr, &thunk<Sample, &Sample::ThreadEntryFunction>,  &sam); 
	//pthread_create(&thrd, &attr, EntryFunction,  &sam); 
	//pthread_create(&thrd, &attr, EntryFun<Sample, Sample::ThreadEntryFunction>,  &sam); 
	//pthread_create(&thrd, &attr, EntryFun<Sample, Sample::ThreadEntryFunction>,  &sam); 
	//pthread_create(&thrd, &attr, EntryFun<Sample, Sample::ThreadEntryFunction>,  &sam); 
	pthread_create(&thrd, &attr, MyEntryFun<OtherClass, OtherClass::ThreadEntryFunction>,  &obj); 
	pthread_create(&thrd, &attr, MyEntryFun<OtherClass, OtherClass::ThreadEntryFunction>,  &obj); 
	pthread_create(&thrd, &attr, EntryFunction<Sample>,  &sam); 
	pthread_create(&thrd, &attr, EntryFunction<OtherClass>,  &obj); 
	pthread_attr_destroy(&attr); 
	return 0;
}

