#ifndef FIANLCLASS_H

/*************************************************************************************
 Implementing Final Class,
 Suppose MyClass is ur Class and if u dont want others to inherit their class from ur 
 MyClass class, implement it this way
 *************************************************************************************/
class MyClass;

class LockClass {
	friend class MyClass;

private:
	LockClass() {}
	LockClass(const LockClass& aLockClass) {}
	};

class MyClass : public virtual LockClass {
public:
	MyClass() {}
	MyClass(const MyClass& aMyClass) { }
	/*  All ur implementation of MyClass class Comes here*/
	};


class ExtendMyClass: public MyClass {// Its a compiler Error, if u try to inherit from MyClass!!!
public:
	ExtendMyClass() { }
};

class A {
public:
	A() {}
	A(const A& a){}
};

class B : private A {
};

class C : public B {
};

#endif //FIANLCLASS_H
