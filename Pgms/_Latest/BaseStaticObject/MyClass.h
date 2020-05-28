#ifndef MY_CLASS_H
#define MY_CLASS_H

class Base {
protected:
	static int* p;

public:
	Base() {};
	~Base() {};

	void Display();
};

class DerivedA : public Base {
public:
	DerivedA() {
		if(!p)
			p = new int (10);
	}

	~DerivedA() {
		delete p;
	}
};

class DerivedB : public Base {
public:
	DerivedB() {
		if(!p)
			p = new int (20);
	}

	~DerivedB() {
		delete p;
	}
};

class DerivedC : public Base {
public:
	DerivedC() {
		if(!p)
			p = new int (30);
	}

	~DerivedC() {
		delete p;
	}
};

#endif //MY_CLASS_H
