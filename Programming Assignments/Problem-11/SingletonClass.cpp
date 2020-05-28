#include "SingletonClass.h"

/*
 * This is an internal class (not exposed to the user) which encapsulates single 
 * instance of Singleton object as static data member.
 * So keep the defination/declaration in .cpp file
 * This class has all the access "rights" to construct and destruct the 
 * singleton class as this class is "friend" to Singleton class
 */

class CreateSingleton {
	friend class Singleton;
private:
	/* 
	 * Make Singleton "friend" to this class, so that Singleton can acess this 
	 * self object
	 */
	static Singleton instance;
};

Singleton CreateSingleton::instance;


Singleton::Singleton(/*Init Values*/) /* : initialization_list */ {
	/* Do other initialization now */
}
Singleton::Singleton(const Singleton& s) /* : initialization_list */ {
	/* Do other initialization now */
}

Singleton()::~Singleton() {
	/* Release all the resources held by this class */
}

//return the pointer to the singleton object
Singleton* Singleton::GetInstance() {
	return &CreateSingleton::instance;
}
