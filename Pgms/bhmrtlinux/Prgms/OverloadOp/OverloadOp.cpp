#include <iostream>
using namespace std;

template<class T> 
T min(const T &a, const T &b) { 
	cout<<"1"<<endl;
   return (a < b) ? a : b; 
} 

template<> 
int min<int>(const int &a, const int &b) { 
	cout<<"2"<<endl;
   return (a < b) ? a : b; 
} 

int min(int a, int b) { 
	cout<<"3"<<endl;
   return (a < b) ? a : b; 
} 

class Base{
	int iBase;
public:
	Base(int val = 0 ) : iBase(val) { }

	const Base& operator=(const Base& ob) {
		iBase = ob.iBase;
	}
};

class Derived : public Base {
	int iDerive;
public:
	Derived (int val = 0 ) : iDerive(val) { }

	const Derived& operator=(const Derived& ob) {
		Base::operator=(ob);
		iDerive = ob.iDerive;
	}
};

class Point {
	int x;
	int y;
public:
	Point(int a=0, int b=0) : x(a), y(b) {
		cout<<"Constructing Point with : "<<x<<" "<<y<<endl;
	}

	Point(const Point& p) : x(p.x), y(p.y) {
		cout<<"Copy Constructing Point with : "<<x<<" "<<y<<endl;
	}

	const Point& operator=(const Point& p) {
		cout<<"assignment Operator"<<endl;
		x = p.x;
		y = p.y;
		return (*this);
	}

	Point& operator+=(const Point& p) {
		cout<<"Member Operator +"<<endl;
		x += p.x;
		y += p.y;
		return (*this);
	}

	Point& operator-=(const Point& p) {
		cout<<"Member Operator -"<<endl;
		x -= p.x;
		y -= p.y;
		return (*this);
	}

	Point& operator*=(const Point& p) {
		cout<<"Member Operator *"<<endl;
		x *= p.x;
		y *= p.y;
		return (*this);
	}

	ostream& operator<<(ostream& os) const {
		std::cout << "Calling Point::<<"<<endl;
		os << "X is : "<<x<<" Y is : "<<y<<endl;
		return os;
	}

	void Helper() const {
		cout<<"Point::Helper"<<endl;
	}
};

Point operator+(const Point& lhs, const Point& rhs) {
	cout<<"Global Operator +"<<endl;
	Point temp(lhs);
	temp += rhs;
	return temp;
}

Point operator-(const Point& lhs, const Point& rhs) {
	cout<<"Global Operator -"<<endl;
	Point temp(lhs);
	temp -= rhs;
	return temp;
}

Point operator*(const Point& lhs, const Point& rhs) {
	cout<<"Global Operator *"<<endl;
	Point temp(lhs);
	temp *= rhs;
	return temp;
}

ostream& operator<<(ostream &os, const Point& p) { 
	os << "Calling Global <<"<<endl;
	p<<os;
	return os; 
}

class SimpleString { 
	char strBody[256]; 
public: 
	SimpleString(const char *str) { 
		std::cout << "Calling SimpleString Constructor"<<endl;
		strcpy(strBody, str); 
	} 

	void* operator new (size_t size) {
		std::cout << "Calling SimpleString::new "<<endl;
		return malloc(size);
	}
	std::ostream& operator<<(std::ostream& os)const {
		std::cout << "Calling SimpleString::<<"<<endl;
		os << strBody <<endl;
		return os;
	}
	const char* GetStrBody() const {
		std::cout << "Calling SimpleString::GetStrBody"<<endl;
		return strBody;
	}
}; 

void* operator new (size_t size) {
	std::cout << "Calling Global new "<<endl;
	return malloc(size);
}

std::ostream& operator<<(std::ostream &os, const SimpleString &s) { 
	os << "Calling Global <<"<<endl;
	os << s.GetStrBody()<<endl; 
	return os; 
}

void IOOperatorTest() {
	SimpleString* pSam = new SimpleString("Girish");
	std::cout<<*pSam;
	pSam->operator<<(cout);
	*pSam<<cout;
	delete pSam;
}

void AirthmaticOperatorTest() {
	Point p1(10, 20);
	Point p2;
	Point p3;
	cout<<p1<<p2;
	p3 = p2 += p1;
	cout<<p1<<p2<<p3;
	//This cant invoke non-const functions, as assignment returns const reference
	(p1 = p2 = p3).Helper();
	cout<<p1<<p2<<p3;
	p3 = p1 * p2;
	cout<<p1<<p2<<p3;
}

int main(){
	//IOOperatorTest();
	AirthmaticOperatorTest();
	return 0;
}

