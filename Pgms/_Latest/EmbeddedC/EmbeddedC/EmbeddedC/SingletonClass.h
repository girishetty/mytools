/*
 * Problem:
 * 11. Implement a Singleton.  It's the most over used and mis-used design pattern.  
 * Everyone knows the "textbook" implementation of the singleton but think of all the different ways you can implement it and
 * what are their pros/cons?  Make a thread-safe version.
 * Solution:
 * Try this new design, which does not need a lock, still this would be thread safe
 * Encapsulate all the resources which needs to be singleton in to a class, make all the constructors private
 * so that nobody should be able to construct this class on heap or stack.
 * Hide the destructor as well, so that user cant free/release it
 * Expose one static GetInstance() function (along with other public functions) to get the pointer to singleton object
 * Implement an internal class [CreateSingleton], which is hidden from the user, which has all the access "rights" to
 * construct and destruct the above mentioned singleton class
 * Advantage of this approach is:
 * 1) This does not need a lock/mutex, by design its thread safe
 * 2) Even before executing Main, this singleton object is constructed while loading data segment.
 * Disadvantage of this approach is:
 * 1) Process loading might take a while, if this singleton class has many resources encapsulated in this
 * 2) Singleton object resides in the memory till the process gets unloaded
 */

#ifndef _SINGLETON_CLASS_H
#define _SINGLETON_CLASS_H

//forward declaration
class CreateSingleton;

class Singleton {
	//Give access "right" to CreateSingleton class
	friend class CreateSingleton;
public:
	// Returns pointer to the singleton Object
	static Singleton* GetInstance();
	/* expose all public methods needed here */

private:
	// Hide constructors
	Singleton(/*Init Values*/);
	Singleton(const Singleton& s);
	//Hide destructor as well
	~Singleton();
	// Its not mandatory to make assignment private
	Singleton& operator=(const Singleton& s);

private:
	/* encapsulate the data/resources needed for the Singleton class here */
};

#endif //_SINGLETON_CLASS_H
