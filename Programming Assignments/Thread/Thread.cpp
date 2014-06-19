#include "Thread.h"


void* Thread::ThreadEntryPoint( void* anArg) {
    //If we want to do any house-keeping activity, insert the code here
    /* house_keeping_code */

    Thread* pSelf = (Thread*)anArg;

    //Now call the thread entry function that user of this class wants to
    int ret = pSelf->iEntryFun(pSelf->iArg);

    //Add code here if we want to do any house-keeping, cleanup operation based on ret code of the thread function
    /* house_keeping_and_clean_up_code */

    return (void*) ret;
}

Thread::Thread(threadEntry entry, threadArg arg) : iEntryFun(entry), iArg(arg) {
    /*
     * Set up the thread attributes
     * One improvement can be to have a mechanism for the user of this class to have control over these attributes
     * I am creating one with default Joinable
     */
    pthread_attr_t threadAttr;
	pthread_attr_init( &threadAttr );
	pthread_attr_setdetachstate( &threadAttr, PTHREAD_CREATE_JOINABLE );
	
	// Create the thread now, and pass "this" pointer as it holds all the information needed 
    // for the execution of the thread
	int ret = pthread_create( &iThreadID, &threadAttr, ThreadEntryPoint, (void*)this);

	if(ret != 0) {
        // Thread creation failed!, do necessary cleanup of data members, as this is constructor and
        // then throw an exception
        throw ThreadException("Thread Creation Failed");
	}
}

Thread::~Thread() {
    //Do cleanup if needed
}

int Thread::join() {
    int exitReason = 0;
	// Wait for the completion of the thread
	int ret = pthread_join(iThreadID, (void**)&exitReason );
    // If needed do something with the exit reason, log or display based on users requirement
    // here, I have changed the function prototype of join, so that exitReason can be passed back to the user
    return exitReason;
}
