#include <iostream>
using namespace std;

//forward declaration of class which encapsulates single instance (static) of Singleton object
class CreateSingleton;

class Singleton {
	friend class CreateSingleton;
public:
	// Returns pointer to the singleton Object
	static Singleton* GetInstance();
	/* expose all public methods needed here */

private:
	// Hide constructors
	Singleton(int aMem=0) :iMem(aMem) {cout<<"Constructed with "<<iMem<<endl;}
	Singleton(const Singleton& s) { }
	~Singleton() { }
	// Its not mandatory to make assignment private
	Singleton& operator=(const Singleton& s) { }

private:
	/* encapsulate the data/resources here */
	int iMem;

};

class CreateSingleton {
	friend class Singleton;
private:
	static Singleton instance;
};

Singleton CreateSingleton::instance(1);

Singleton* Singleton::GetInstance() {
	return &CreateSingleton::instance;
}

int main() {
	//Singleton s;
	//Singleton* pS = new Singleton;
	//CreateSingleton cs;
	//CreateSingleton* pCS = new CreateSingleton;
	Singleton* p = Singleton::GetInstance();
	//delete pCS;
	return 0;
}

#if 0

class Singleton {
public:
	// Returns pointer to the singleton Object
	static Singleton* GetInstance() {
		//Lock the mutex
		mutex_lock();
		if(!pInstance) {
			pInstance = new Singleton();
		}
		//Unlock the mutex
		mutex_unlock();
		//if reference counting is used to keep track of the reference, increment the reference count
		return pInstance;
	}
	static void DestroyInstance() {
		//If reference counting is used, then delete it only if count is 0
		//if(--refCount == 0)
		delete pInstance;
	}
	/* expose all public methods needed here */

private:
	// Hide constructors
	Singleton() {}
	Singleton(const Singleton& s) { }
	~Singleton() { }
	// Its not mandatory to make assignment private
	Singleton& operator=(const Singleton& s) { }

private:
	static Singleton* pInstance;
	/* encapsulate the data/resources here */
};
#endif
