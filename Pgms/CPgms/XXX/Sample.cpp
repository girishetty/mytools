struct MyStruct {
	int a;
	int b;
	void foo();
};

int main() {
	int MyStruct::*m = &MyStruct::a;
	int r = sizeof(m);
	return;
}
