#include <assert.h>
#include <iostream>
#include "Thread.h"


class ThreadTest {
public:
    ThreadTest() : testCount(0) { }

    void run(void* args) {

#if defined( _WIN32 ) || defined( _WINDOWS )
        cout << "Parent thread ID: " << pthread_self().p << endl;
#else
        cout << "Parent thread ID: " << pthread_self() << endl;
#endif

        // Create the thread passing thread entry function and the arguments
        // I am making this as a stack variable, and passing the pointer to the thread.
        // Its safe "only" because I am waiting till the completion of the thread, by calling join in the same function
        // If we are not callig join in here, we need to make sure that param is created on the heap (and not on stack)
        // and Freed up later, once thread executioin is done
        // This can be done either using auto/smart pointer
        // Or, giving the owenership of the param to the thread and thread has to delete the param as part of the cleanup 
        // activity
        ThreadParam param;
        param.iOwner = this;
        param.iArgs = args;
        try {
            Thread child(ThreadTest::testEntry, &param);
            child.join();
        } catch(ThreadException tx) {
            cout<<"Error!! : "<<tx.what()<<endl;
            return;
        }

        assert(testCount == 1);
    }

private:
    int testCount;

    static int testEntry(void* v) {
        ThreadTest* pSelf = (ThreadTest*) ((ThreadParam*)v)->iOwner;
        //use the thread arguments if needed, that are stored in ((ThreadParam*)v)->iArgs;
        pSelf->testRun();
        return 0;
    }

    void testRun() {
#if defined( _WIN32 ) || defined( _WINDOWS )
        cout << "Child thread ID: " << pthread_self().p << endl;
#else
        cout << "Child thread ID: " << pthread_self() << endl;
#endif
        ++testCount;
    }
};

int main() {
    cout << "entered main "<< endl;
    ThreadTest test;
    test.run( NULL );

    cout << "returning from main" << endl;
    return 0;
}
