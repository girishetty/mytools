//#include <iostream>
using namespace std;

class Sample {
public:
	template<typename Type> friend void WorkOnSample(Type& aType);
	Sample(int aVal = 0) : iVal(aVal) { }
	~Sample() { }
	void Display() const {} 
private:
	int iVal;
};

template<typename Type> void WorkOnSample(Type& aType) {
	aType.iVal = 200;
}

int main() {
	Sample s1;
	Sample s2(10);

	s1.Display();
	s2.Display();
	WorkOnSample(s1);
	s1.Display();
}

