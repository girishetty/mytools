// To compile this, run this command ==> gcc main.cpp -g -std=c++1y -lstdc++
#include <iostream>
#include <vector>

// Union that initializes the float ==> wrong usage as std::vector is in invalid state
union Sample1 {
  int a;
  float b;
  std::vector<char> c;
  Sample1() : b(0.0) {}
  Sample1(const Sample1& s) : b(s.b) {}
  ~Sample1() {}
};

// Union that initializes the std::vector ==> Good usage, but again its a bad design to use unions with members that needs constructors
union Sample2 {
  int a;
  float b;
  std::vector<char> c;
  Sample2() : c() {}
  Sample2(const Sample2& s) : c(s.c) {}
  ~Sample2() {}
};

void UnionTest() {
  Sample1 s1;
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

void UnionTest1(int val) {
  Sample1 s1;
  s1.a = val;
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

void UnionTest2(float val) {
  Sample1 s1;
  s1.b = val;
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

void UnionTest3(size_t val) {
  Sample1 s1;
  for (size_t index = 0; index < val; index++) {
    s1.c.push_back(val + index); // ==> This will crash, for the same reason why your code crashed.
  }
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

void UnionTest4(size_t val) {
  Sample2 s1;
  for (size_t index = 0; index < val; index++) {
    s1.c.push_back(val + index); // ==> This will crash, for the same reason why your code crashed.
  }
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

void UnionTest5() {
  Sample2 s1;
  std::cout << "int: " << s1.a << " float: " << s1.b << " vector size: " << s1.c.size() << std::endl;
}

// To see Union crashing (your case), enable the macro below
// #define USE_UNION_WITHOUT_VECTOR_INIT

int main() {
  // uninitalized union test
  UnionTest();
  // assign int value and check all memver test
  UnionTest1(10);
  // assign float value and check all memver test
  UnionTest2(10.0);
#ifdef USE_UNION_WITHOUT_VECTOR_INIT
  // assign std::vector value and check all memver test
  // This will crash as std::vector inside the union is not initialized
  UnionTest3(10); // ==> This will crash as std::vector inside the union is not initialized
#endif

  // assign std::vector value and check all memver test
  // This will work as we are initalizing std::vector inside the union.
  // But check the value on int and float values, they aren't 0 anymore!
  UnionTest4(10);
  // uninitalized union test
  UnionTest5();
  return 0;
}

