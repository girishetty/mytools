#include <iostream>
using namespace std;

template<typename T> struct can_use_f {
	typedef void type;
};
template<> struct can_use_f<int> {
};
template<typename T> typename can_use_f<T>::type f(T const& val){
	cout<<"Value is : "<<val<<endl;
}

int main() {
	//f(1);
	f(1.67);

	return 0;
}

