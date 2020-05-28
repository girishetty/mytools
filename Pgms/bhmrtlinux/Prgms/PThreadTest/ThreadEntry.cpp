#include <pthread.h>

class Sample {
public:
    Sample(int aVal = 0) : iVal(aVal) { }
    ~Sample() { }
    void ThreadEntryFunctionOne() {}
private:
        int iVal;
};

class OtherClass {
public:
    OtherClass(int aVal = 0) : iVal(aVal) { }
    ~OtherClass() { }
    void ThreadEntryFunctionTwo() {}
private:
        int iVal;
};

template<class T, void (T::*EntryFunction)()> void* MyEntryFun(void* aPtr) {
     T* p = static_cast<T*>(aPtr);
     (p->*EntryFunction)();
     return 0;
}

template<typename ClassName> void* EntryFunction(void* aPtr) {
        ClassName* sam = static_cast<ClassName*>(aPtr);
        sam->ThreadEntryFunction();
        return 0;
}


int Foo() {
        Sample sam;
        OtherClass obj;
        pthread_t thrd;
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

        pthread_create(&thrd, &attr, MyEntryFun<Sample, Sample::ThreadEntryFunctionOne>,  &sam);
        pthread_create(&thrd, &attr, MyEntryFun<OtherClass, OtherClass::ThreadEntryFunctionTwo>,  &obj);
        /*
        pthread_create(&thrd, &attr, EntryFunction<Sample>,  &sam);
        pthread_create(&thrd, &attr, EntryFunction<OtherClass>,  &obj);*/
        pthread_attr_destroy(&attr);
        return 0;
}

