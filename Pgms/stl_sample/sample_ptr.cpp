#include <iostream>
#include <memory>

class Base {
public:
  Base() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  virtual ~Base() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  virtual void Display() {
    std::cout << "Base::Display(): " << this << std::endl;
  }
};

class DerivedA : public Base {
public:
  DerivedA() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  ~DerivedA() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  void Display() override {
    std::cout << "DerivedA::Display: " << this << std::endl;
  }
};

class DerivedB : public Base {
public:
  DerivedB() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  ~DerivedB() {
    std::cout << __func__ << ": " << this << std::endl;
  }
  void Display() override {
    std::cout << "DerivedB::Display: " << this << std::endl;
  }
};

void foo(std::shared_ptr<Base> p) {
  std::cout << "foo begin" << std::endl;
  p->Display();
  p = std::make_shared<DerivedB>();
  p->Display();
  std::cout << "foo end" << std::endl;
}

void bar(std::shared_ptr<Base>& p) {
  std::cout << "bar begin" << std::endl;
  p->Display();
  p = std::make_shared<DerivedB>();
  p->Display();
  std::cout << "bar end" << std::endl;
}

int main() {
  std::cout << "main begin" << std::endl;
  std::shared_ptr<Base> p = std::make_shared<DerivedA>();
  p->Display();
#if 1
  foo(p);
#else
  bar(p);
#endif
  p->Display();
  std::cout << "main end" << std::endl;
}

