#ifndef _THREAD_H
#define _THREAD_H

#include <pthread.h>
#include <exception>
using namespace std;

//Thread related typedef
typedef int (*threadEntry)( void* anArg);
typedef void* threadArg;

//Using one exception for the thread creation to safegaurd the operations
class ThreadException : public exception {
public:
    ThreadException();
    ThreadException(const char* const& what) : exception(what) {}
    ThreadException(const char* const& what, int do_free) : exception(what, do_free) {}
    ThreadException(const ThreadException&) {}

    //Add any other exception specific functionality if needed
};

/* Structure used for passing user defined arguments to the thread */
typedef struct {
    //Owner of the Thread
    void* iOwner;
    /*
     * I am no FAN of variable arugment list, SO
     * For variable arguments, I am using void pointer, which can point to any user defined datatype (structure/class)
     * including primitive datatypes.
     * user will have to cast it back to what they have "Actually" passed in the original place
     * OR
     * another approach is to use <vector> STL
     */
    void* iArgs;
    /*
     * Scope for improvement:
     * we can have another boolean here that can indicate who owns this argument, so that it can be
     * freed up by the thread itself once after completion of thread_entry_function
     */
} ThreadParam;

class Thread {
public:
    //Construct that creats the thread and waits for it to start execution
    Thread(threadEntry entry, threadArg arg);
    ~Thread();

    //Way to wait for the parent/owner until the completion of this thread
    int join();  /* optional: implement only if you have time */

private:

    //A static private function that takes care of ThreadEntry Function for this class/thread
    static void* ThreadEntryPoint( void* anArg);

    //Members to do the house-keeping
    pthread_t   iThreadID;
    threadEntry iEntryFun;
    threadArg   iArg;
};

#endif
