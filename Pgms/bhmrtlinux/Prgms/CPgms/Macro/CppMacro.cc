//#define name2(a, b) _rwname2(a,b)
//#define _rwname2(a, b) a##b
#define name2(a, b) a##b
#define RWGQueue(type) name2(type,RWGQueue)
#define declare(Class, type) name2(Class,declare)(type)
#define RWGQueuedeclare(type) class RWGQueue(type);

int main() {
	declare(RWGQueue, RWCollectable)
	return 0;
	}

